#include "device.hpp"

#include <runtime/common/assert.hpp>
#include <string>
#include <iostream>
#include <cstdlib>

namespace runtime::graphics_engine::webgpu {

namespace {

WGPUDevice requestDeviceSync(Adapter const& adapter, WGPUDeviceDescriptor const * descriptor) {
    struct UserData {
        WGPUDevice device = nullptr;
        bool requestEnded = false;
        std::string errorMsg;
    };
    UserData userData;

    auto onDeviceRequestEnded = [](WGPURequestDeviceStatus status, WGPUDevice device, char const * message, void * pUserData) {
        UserData& userData = *reinterpret_cast<UserData*>(pUserData);
        if (status == WGPURequestDeviceStatus_Success) {
            userData.device = device;
        } else {
            userData.errorMsg = "Could not get WebGPU device: " + std::string(message);
        }
        userData.requestEnded = true;
    };

    wgpuAdapterRequestDevice(adapter.adapter(), descriptor, onDeviceRequestEnded, &userData);

    ASSERT(userData.requestEnded);

    REQUIRE(userData.device, userData.errorMsg);

    return userData.device;
}

void onDeviceError(WGPUErrorType type, char const* message, void* /* pUserData */) {
    std::cout << "Uncaptured device error: type " << type;
    if (message) std::cout << " (" << message << ")";
    std::cout << std::endl;
    std::abort();
}

WGPULimits defaultLimits() {
    return WGPULimits {
        .maxTextureDimension1D = WGPU_LIMIT_U32_UNDEFINED,
        .maxTextureDimension2D = WGPU_LIMIT_U32_UNDEFINED,
        .maxTextureDimension3D = WGPU_LIMIT_U32_UNDEFINED,
        .maxTextureArrayLayers = WGPU_LIMIT_U32_UNDEFINED,
        .maxBindGroups = WGPU_LIMIT_U32_UNDEFINED,
        .maxBindGroupsPlusVertexBuffers = WGPU_LIMIT_U32_UNDEFINED,
        .maxBindingsPerBindGroup = WGPU_LIMIT_U32_UNDEFINED,
        .maxDynamicUniformBuffersPerPipelineLayout = WGPU_LIMIT_U32_UNDEFINED,
        .maxDynamicStorageBuffersPerPipelineLayout = WGPU_LIMIT_U32_UNDEFINED,
        .maxSampledTexturesPerShaderStage = WGPU_LIMIT_U32_UNDEFINED,
        .maxSamplersPerShaderStage = WGPU_LIMIT_U32_UNDEFINED,
        .maxStorageBuffersPerShaderStage = WGPU_LIMIT_U32_UNDEFINED,
        .maxStorageTexturesPerShaderStage = WGPU_LIMIT_U32_UNDEFINED,
        .maxUniformBuffersPerShaderStage = WGPU_LIMIT_U32_UNDEFINED,
        .maxUniformBufferBindingSize = WGPU_LIMIT_U64_UNDEFINED,
        .maxStorageBufferBindingSize = WGPU_LIMIT_U64_UNDEFINED,
        .minUniformBufferOffsetAlignment = WGPU_LIMIT_U32_UNDEFINED,
        .minStorageBufferOffsetAlignment = WGPU_LIMIT_U32_UNDEFINED,
        .maxVertexBuffers = WGPU_LIMIT_U32_UNDEFINED,
        .maxBufferSize = WGPU_LIMIT_U64_UNDEFINED,
        .maxVertexAttributes = WGPU_LIMIT_U32_UNDEFINED,
        .maxVertexBufferArrayStride = WGPU_LIMIT_U32_UNDEFINED,
        .maxInterStageShaderComponents = WGPU_LIMIT_U32_UNDEFINED,
        .maxInterStageShaderVariables = WGPU_LIMIT_U32_UNDEFINED,
        .maxColorAttachments = WGPU_LIMIT_U32_UNDEFINED,
        .maxColorAttachmentBytesPerSample = WGPU_LIMIT_U32_UNDEFINED,
        .maxComputeWorkgroupStorageSize = WGPU_LIMIT_U32_UNDEFINED,
        .maxComputeInvocationsPerWorkgroup = WGPU_LIMIT_U32_UNDEFINED,
        .maxComputeWorkgroupSizeX = WGPU_LIMIT_U32_UNDEFINED,
        .maxComputeWorkgroupSizeY = WGPU_LIMIT_U32_UNDEFINED,
        .maxComputeWorkgroupSizeZ = WGPU_LIMIT_U32_UNDEFINED,
        .maxComputeWorkgroupsPerDimension = WGPU_LIMIT_U32_UNDEFINED,
    };
}

WGPURequiredLimits requiredLimits(Adapter const& adapter) {
    WGPUSupportedLimits supportedLimits;
    supportedLimits.nextInChain = nullptr;
    wgpuAdapterGetLimits(adapter.adapter(), &supportedLimits);

    WGPURequiredLimits requiredLimits = {};
    requiredLimits.limits = defaultLimits();

    requiredLimits.limits.maxVertexAttributes = supportedLimits.limits.maxVertexAttributes;
    requiredLimits.limits.maxVertexBuffers = supportedLimits.limits.maxVertexBuffers;
    requiredLimits.limits.maxBufferSize = supportedLimits.limits.maxBufferSize;
    requiredLimits.limits.maxVertexBufferArrayStride = supportedLimits.limits.maxVertexBufferArrayStride;
    requiredLimits.limits.minUniformBufferOffsetAlignment = supportedLimits.limits.minUniformBufferOffsetAlignment;
    requiredLimits.limits.minStorageBufferOffsetAlignment = supportedLimits.limits.minStorageBufferOffsetAlignment;
    return requiredLimits;
}

} // namespace

Device::Device(Adapter const& adapter) {
    auto limits = requiredLimits(adapter);
    WGPUDeviceDescriptor deviceDesc = {};
    deviceDesc.nextInChain = nullptr;
    deviceDesc.label = "My Device"; // anything works here, that's your call
    deviceDesc.requiredFeatureCount = 0; // we do not require any specific feature
    deviceDesc.requiredLimits = &limits;
    deviceDesc.defaultQueue.nextInChain = nullptr;
    deviceDesc.defaultQueue.label = "The default queue";
    deviceDesc.deviceLostCallback = nullptr;
    _device = requestDeviceSync(adapter, &deviceDesc);

    wgpuDeviceSetUncapturedErrorCallback(_device, onDeviceError, nullptr /* pUserData */);
}

} // namespace runtime::graphics_engine::webgpu

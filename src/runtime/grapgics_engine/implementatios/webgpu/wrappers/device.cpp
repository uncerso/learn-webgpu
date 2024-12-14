#include "device.hpp"

#include <runtime/common/assert.hpp>

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

} // namespace

Device::Device(Adapter const& adapter) {
    WGPUDeviceDescriptor deviceDesc = {};
    deviceDesc.nextInChain = nullptr;
    deviceDesc.label = "My Device"; // anything works here, that's your call
    deviceDesc.requiredFeatureCount = 0; // we do not require any specific feature
    deviceDesc.requiredLimits = nullptr; // we do not require any specific limit
    deviceDesc.defaultQueue.nextInChain = nullptr;
    deviceDesc.defaultQueue.label = "The default queue";
    deviceDesc.deviceLostCallback = nullptr;
    _device = requestDeviceSync(adapter, &deviceDesc);

    wgpuDeviceSetUncapturedErrorCallback(_device, onDeviceError, nullptr /* pUserData */);
}

} // runtime::graphics_engine::webgpu

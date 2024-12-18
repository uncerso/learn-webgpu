#include "adapter.hpp"

#include <runtime/common/assert.hpp>
#include <string>

namespace runtime::graphics_engine::webgpu {

namespace {

WGPUAdapter requestAdapterSync(Instance const& instance, WGPURequestAdapterOptions const * options) {
    struct UserData {
        WGPUAdapter adapter = nullptr;
        bool requestEnded = false;
        std::string errorMsg;
    } userData;

    auto onAdapterRequestEnded = [](WGPURequestAdapterStatus status, WGPUAdapter adapter, char const * message, void * pUserData) {
        auto& userData = *static_cast<UserData*>(pUserData);
        if (status == WGPURequestAdapterStatus_Success) {
            userData.adapter = adapter;
        } else {
            userData.errorMsg = "Could not get WebGPU adapter: " + std::string(message);
        }
        userData.requestEnded = true;
    };

    wgpuInstanceRequestAdapter(instance.instance(), options, onAdapterRequestEnded, &userData);

    ASSERT(userData.requestEnded);

    REQUIRE(userData.adapter, userData.errorMsg);

    return userData.adapter;
}

} // namespace

Adapter::Adapter(Instance const& instance) {
    WGPURequestAdapterOptions adapterOpts = {};
    adapterOpts.nextInChain = nullptr;
    _adapter = requestAdapterSync(instance, &adapterOpts);
}

} // namespace runtime::graphics_engine::webgpu

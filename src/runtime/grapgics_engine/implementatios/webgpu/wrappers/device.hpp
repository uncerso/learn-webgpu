#pragma once

#include "adapter.hpp"
#include "resource_holder.hpp"

#include <webgpu/webgpu.hpp>

namespace runtime::graphics_engine::webgpu {

RESOURCE_HOLDER(Device, WGPUDevice, device, wgpuDeviceRelease);

class Device final : public DeviceResourceHolder {
public:
    Device(Adapter const& adapter);
};

} // runtime::graphics_engine::webgpu

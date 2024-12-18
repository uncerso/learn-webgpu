#pragma once

#include "adapter.hpp"
#include "resource_holder.hpp"

#include <webgpu/webgpu.h>

namespace runtime::graphics_engine::webgpu {

RESOURCE_HOLDER(Device, WGPUDevice, device, wgpuDeviceRelease);

class Device final : public DeviceResourceHolder {
public:
    Device(Adapter const& adapter);
};

} // namespace runtime::graphics_engine::webgpu

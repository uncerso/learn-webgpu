#include "shader_module.hpp"

#include <runtime/common/assert.hpp>

namespace runtime::graphics_engine::webgpu {

ShaderModule::ShaderModule(Device const& device, char const* shaderSource) {
    // Load the shader module
	WGPUShaderModuleDescriptor shaderDesc{};
#ifdef WEBGPU_BACKEND_WGPU
	shaderDesc.hintCount = 0;
	shaderDesc.hints = nullptr;
#endif

	// We use the extension mechanism to specify the WGSL part of the shader module descriptor
	WGPUShaderModuleWGSLDescriptor shaderCodeDesc{};
	// Set the chained struct's header
	shaderCodeDesc.chain.next = nullptr;
	shaderCodeDesc.chain.sType = WGPUSType_ShaderModuleWGSLDescriptor;
	// Connect the chain
	shaderDesc.nextInChain = &shaderCodeDesc.chain;
	shaderCodeDesc.code = shaderSource;
	_module = wgpuDeviceCreateShaderModule(device.device(), &shaderDesc);

    REQUIRE(_module, "Could not create WebGPU shader module");
}

} // runtime::graphics_engine::webgpu

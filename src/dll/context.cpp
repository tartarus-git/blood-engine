#include "context.h"

#include <CL/cl.h>

cl_context create_optimal_opencl_context() noexcept {
	// TODO: implement
	return nullptr;
}

bld_context_t bldCreateContext_inner() noexcept {
	bld_context_t result = new bld_context_t();

	result->platform_type = bld_context_platform_type_t::OPENCL;

	result->platform_context = create_optimal_opencl_context();

	return result;
}

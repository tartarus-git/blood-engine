#include "context.h"

bld_opencl_environment create_optimal_opencl_environment(bld_error_t *err) noexcept {
	*err = bld_error_t::SUCCESS;

	// TODO: implement
	return { };
}

bld_context_t bldCreateContext_inner(bld_error_t *err) noexcept {
	*err = bld_error_t::SUCCESS;

	bld_context_t result = new bld_context_inner_t();

	result->platform_type = bld_context_platform_type_t::OPENCL;

	result->opencl_environment = create_optimal_opencl_environment(err);
	if (*err != bld_error_t::SUCCESS) { return result; }

	return result;
}

bld_error_t bldReleaseContext_inner(bld_context_t context) noexcept {
	switch (clReleaseCommandQueue(context->opencl_environment.command_queue)) {
		case CL_SUCCESS: break;
		// TODO: Do other errors.
	}

	switch (clReleaseContext(context->opencl_environment.context)) {
		case CL_SUCCESS: break;
		// TODO: Do other errors.
	}

	delete context;

	return bld_error_t::SUCCESS;
}

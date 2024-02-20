#include "context.h"

cl_context create_optimal_opencl_context(bld_error_t *err) noexcept {
	*err = bld_error_t::SUCCESS;

	// TODO: implement
	return nullptr;
}

bld_context_t bldCreateContext_inner(bld_error_t *err) noexcept {
	*err = bld_error_t::SUCCESS;

	bld_context_t result = new bld_context_inner_t();

	result->platform_type = bld_context_platform_type_t::OPENCL;

	result->platform_context = create_optimal_opencl_context(err);
	if (*err != bld_error_t::SUCCESS) { return result; }

	return result;
}

bld_error_t bldReleaseContext_inner(bld_context_t context) noexcept {
	switch (clReleaseContext(context->platform_context)) {
		case CL_SUCCESS: break;
		// TODO: Do other errors.
	}

	delete context;

	return bld_error_t::SUCCESS;
}

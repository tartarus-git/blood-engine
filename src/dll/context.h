#pragma once

#include <CL/cl.h>

#include "error.h"

enum class bld_context_platform_type_t {
	OPENCL
};

struct bld_context_inner_t {
	bld_context_platform_type_t platform_type;
	union {
		cl_context platform_context;
	};
};

using bld_context_t = bld_context_inner_t*;

bld_context_t bldCreateContext_inner(bld_error_t *err) noexcept;

bld_error_t bldReleaseContext_inner(bld_context_t context) noexcept;

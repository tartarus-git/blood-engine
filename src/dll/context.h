#pragma once

#include <CL/cl.h>

#include "error.h"

enum class bld_context_platform_type_t {
	OPENCL
};

struct bld_opencl_environment {
	cl_context context;
	cl_command_queue command_queue;
};

struct bld_context_inner_t {
	bld_context_platform_type_t platform_type;
	union {
		bld_opencl_environment opencl_environment;
	};
};

using bld_context_t = bld_context_inner_t*;

bld_context_t bldCreateContext_inner(bld_error_t *err) noexcept;

bld_error_t bldReleaseContext_inner(bld_context_t context) noexcept;

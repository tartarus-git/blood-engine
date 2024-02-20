#pragma once

#include <CL/cl.h>

enum class bld_context_platform_type_t {
	OPENCL
};

using bld_context_t = struct {
	bld_context_platform_type_t platform_type;
	union {
		cl_context platform_context;
	}
}*;

bld_context_t bldCreateContext_inner() noexcept;

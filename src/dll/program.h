#pragma once

#include "common.h"
#include "context.h"

enum class bld_program_compilation_state_t {
	SOURCE,
	PLATFORM,
	DEVICE
};

struct bld_program_inner_t {
	bld_platform_type_t platform_type;

	bld_program_compilation_state_t compilation_state;
	const char *source_code;
	const char *platform_code;

	union {
		cl_program opencl_program;
	};
};

using bld_program_t = bld_program_inner_t*;

bld_program_t bldCreateProgram(bld_context_t context, const char *source_code) noexcept;

bld_error_t bldCompileProgramToPlatform(bld_context_t context, bld_program_t program) noexcept;

bld_error_t bldCompileProgram(bld_context_t context, bld_program_t program) noexcept;

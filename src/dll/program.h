#pragma once

#include "common.h"
#include "context.h"

enum class bld_program_compilation_state_t {
	SOURCE,
	PLATFORM,
	DEVICE
};

struct bld_program_parameter_t {
	bld_data_type_t type;
	union {
		bld_float_t float_value;
		bld_double_t double_value;
	};
};

struct bld_program_inner_t {
	bld_program_compilation_state_t compilation_state;
	const char *source_code;
	const char *platform_code;

	union {
		cl_program opencl_program;
	};

	bld_program_parameter_t *parameters;
	size_t parameter_count;
};

using bld_program_t = bld_program_inner_t*;

bld_program_t bldCreateProgram_inner(bld_context_t context, const char *source_code, bld_error_t *err) noexcept;

bld_error_t bldCompileProgramToPlatform_inner(bld_context_t context, bld_program_t program) noexcept;

bld_error_t bldCompileProgram_inner(bld_context_t context, bld_program_t program) noexcept;

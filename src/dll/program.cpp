#include "program.h"

#include <cstdlib>
#include <cstring>

bld_program_t bldCreateProgram(bld_context_t context, const char *source_code, bld_error_t *err) noexcept {
	*err = bld_error_t::SUCCESS;

	bld_program_t result = (bld_program_t)std::malloc(sizeof(bld_program_inner_t));

	if (result == nullptr) { *err = bld_error_t::OUT_OF_HOST_MEM; return nullptr; }

	result->compilation_state = bld_program_compilation_state_t::SOURCE;

	size_t source_code_size = (std::strlen(source_code) + 1) * sizeof(char);
	result->source_code = (const char*)std::malloc(source_code_size);
	std::memcpy((char*)result->source_code, source_code, source_code_size);

	result->platform_code = nullptr;

	cl_int opencl_err = CL_SUCCESS;
	result->opencl_program = clCreateProgramWithSource(context->opencl_environment.context,
							   1,
							   &(result->source_code),
							   nullptr,
							   &opencl_err);
	switch (opencl_err) {
	case CL_SUCCESS: break;
	case CL_INVALID_CONTEXT:
		*err = bld_error_t::INVALID_CONTEXT;
		goto free_and_return_null;
	case CL_INVALID_VALUE:
		*err = bld_error_t::BUG;
		goto free_and_return_null;
	case CL_OUT_OF_RESOURCES:
		*err = bld_error_t::OUT_OF_DEVICE_MEM;
		goto free_and_return_null;
	case CL_OUT_OF_HOST_MEMORY:
		*err = bld_error_t::OUT_OF_HOST_MEM;
		goto free_and_return_null;
	default:
		*err = bld_error_t::UNKNOWN_ERROR;
		goto free_and_return_null;
	}

	result->parameters = nullptr;
	result->parameter_count = 0;

	return result;

free_and_return_null:
	std::free((char*)result->source_code);
	std::free(result);
	return nullptr;
}

bld_error_t bldCompileProgramToPlatform(bld_context_t context, bld_program_t program) noexcept {
	// TODO: implement
	return bld_error_t::BUG;
}

bld_error_t bldCompileProgram(bld_context_t context, bld_program_t program) noexcept {
	switch (program->compilation_state) {

	case bld_program_compilation_state_t::SOURCE:
		return bldCompileProgramToPlatform(context, program);

	case bld_program_compilation_state_t::PLATFORM:
		break;

	case bld_program_compilation_state_t::DEVICE:
		return bld_error_t::ALREADY_COMPILED;

	}

	cl_int opencl_err = clBuildProgram(program->opencl_program,
					   0,
					   nullptr,
					   nullptr,
					   nullptr,
					   nullptr);
	switch (opencl_err) {

	case CL_SUCCESS: break;

	default: return bld_error_t::UNKNOWN_ERROR;

	}

	return bld_error_t::SUCCESS;
}

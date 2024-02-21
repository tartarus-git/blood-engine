#include "pipeline.h"

#include <cstdlib>

cl_image_format opencl_image_format_from_bld_pixel_format(bld_pixel_format_t pixel_format, size_t *pixel_size, bld_error_t *err) noexcept {
	*pixel_size = 3;

	switch (pixel_format) {

	case bld_pixel_format_t::R8G8B8:
		pixel_size = 3;
		return { CL_RGB, CL_UNSIGNED_INT8 };

	default:
		*err = bld_error_t::PIXEL_FORMAT_UNSUPPORTED;
		return { CL_RGB, CL_UNSIGNED_INT8 };

	}
}

bld_pipeline_t bldCreatePipeline_inner(bld_context_t context,
				       size_t width,
				       size_t height,
				       bld_pixel_format_t pixel_format,
				       bld_error_t *err) noexcept {
	*err = bld_error_t::SUCCESS;

	bld_pipeline_t result = (bld_pipeline_t)std::malloc(sizeof(bld_pipeline_inner_t));
	if (result == nullptr) {
		*err = bld_error_t::OUT_OF_HOST_MEM;
		return nullptr;
	}

	size_t opencl_pixel_size;
	cl_image_format opencl_image_format = opencl_image_format_from_bld_pixel_format(pixel_format, &pixel_size, err);
	if (*err != bld_error_t::SUCCESS) {
		std::free(result);
		return nullptr;
	}

	cl_image_desc opencl_image_description {
		CL_MEM_OBJECT_IMAGE2D,
		width,
		height,
		1,
		0,
		opencl_pixel_size * width,
		0,
		0,
		0,
		nullptr
	};

	cl_int opencl_err = CL_SUCCESS;

	result->opencl_image_a = clCreateImage(context->opencl_environment.context,
					       0,
					       opencl_image_format,
					       opencl_image_description,
					       nullptr,
					       &opencl_err);
	switch (opencl_err) {

	case CL_SUCCESS: break;

	default:
		*err = bld_error_t::UNKNOWN_ERROR;
		std::free(result);
		return nullptr;
	}

	result->opencl_image_b = clCreateImage(context->opencl_environment.context,
					       0,
					       opencl_image_format,
					       opencl_image_description,
					       nullptr,
					       &opencl_err);
	switch (opencl_err) {

	case CL_SUCCESS: break;

	default:
		*err = bld_error_t::UNKNOWN_ERROR;
		goto error_release_image_a_and_free_and_return_nullptr;

	}

	result->programs = nullptr;
	result->program_count = 0;

	return result;

error_release_image_a_and_free_and_return_nullptr:
	opencl_err = clReleaseMemObject(result->opencl_image_a);
	switch (opencl_err) {
	default:
		*err = bld_error_t::CORRUPTED_STATE;
		std::free(result);
		return nullptr;
	}

	std::free(result);
	return nullptr;
}

bld_error_t bldReleasePipeline_inner(bld_context_t context, bld_pipeline_t pipeline) noexcept {
	// TODO: Release the other resources that need releasing.

	std::free(pipeline->programs);

	std::free(pipeline);

	return bld_error_t::SUCCESS;
}

bld_error_t bldPushProgramToPipeline_inner(bld_context_t context, bld_pipeline_t pipeline, bld_program_t program) noexcept {
	bld_program_t *new_array = (bld_program_t*)std::realloc(pipeline->programs,
								(pipeline->program_count + 1) * sizeof(bld_program_inner_t));

	if (new_array == nullptr) { return bld_error_t::OUT_OF_HOST_MEM; }

	pipeline->programs = new_array;
	pipeline->program_count++;

	pipeline->programs[pipeline->program_count - 1] = program;

	return bld_error_t::SUCCESS;
}

bld_program_t bldPopProgramFromPipeline_inner(bld_context_t context, bld_pipeline_t pipeline, bld_error_t *err) noexcept {
	*err = bld_error_t::SUCCESS;

	bld_program_t result = pipeline->programs[pipeline->program_count - 1];

	bld_program_t *new_array = (bld_program_t*)std::realloc(pipeline->programs,
								(pipeline->program_count - 1) * sizeof(bld_program_inner_t));

	if (new_array == nullptr) { *err = bld_error_t::OUT_OF_HOST_MEM; return nullptr; }

	pipeline->programs = new_array;
	pipeline->program_count--;

	return result;
}

bld_error_t bldInsertProgramInPipeline_inner(bld_context_t context,
					     bld_pipeline_t pipeline,
					     size_t index,
					     bld_program_t program) noexcept {

	bld_program_t *new_array = (bld_program_t*)std::realloc(pipeline->programs,
								(pipeline->program_count + 1) * sizeof(bld_program_inner_t));

	if (new_array == nullptr) { return bld_error_t::OUT_OF_HOST_MEM; }

	pipeline->programs = new_array;
	pipeline->program_count++;

	for (size_t i = pipeline->program_count - 2; i >= index; i--) {
		pipeline->programs[i + 1] = pipeline->programs[i];
	}

	pipeline->programs[index] = program;

	return bld_error_t::SUCCESS;
}

bld_program_t bldRemoveProgramFromPipeline_inner(bld_context_t context,
						 bld_pipeline_t pipeline,
						 size_t index,
						 bld_error_t *err) noexcept {

	*err = bld_error_t::SUCCESS;

	bld_program_t *new_array = (bld_program_t*)std::malloc((pipeline->program_count - 1) * sizeof(bld_program_inner_t));

	if (new_array == nullptr) { *err = bld_error_t::OUT_OF_HOST_MEM; return nullptr; }

	for (size_t i = 0; i < index; i++) {
		new_array[i] = pipeline->programs[i];
	}

	bld_program_t result = pipeline->programs[index];

	for (size_t i = index + 1; i < pipeline->program_count; i++) {
		new_array[i - 1] = pipeline->programs[i];
	}

	std::free(pipeline->programs);

	pipeline->programs = new_array;
	pipeline->program_count--;

	return result;
}

bld_error_t bldSetPipelineDimensions_inner(bld_context_t context,
					   bld_pipeline_t pipeline,
					   size_t width,
					   size_t height) noexcept {

	// TODO: implement
	return bld_error_t::BUG;
}

#include "pipeline.h"

#include <cstdlib>
#include <cstdint>
#include <thread>
#include <functional>

#define OPENCL_KERNEL_IMAGE_SOURCE_ARGUMENT_INDEX 0
#define OPENCL_KERNEL_IMAGE_DESTINATION_ARGUMENT_INDEX 1

cl_image_format opencl_image_format_from_bld_pixel_format(bld_pixel_format_t pixel_format, size_t &pixel_size, bld_error_t &err) noexcept {
	pixel_size = 3;

	switch (pixel_format) {

	case bld_pixel_format_t::R8G8B8:
		pixel_size = 3;
		return { CL_RGB, CL_UNSIGNED_INT8 };

	default:
		err = bld_error_t::PIXEL_FORMAT_UNSUPPORTED;
		return { CL_RGB, CL_UNSIGNED_INT8 };

	}
}

bld_error_t opencl_fill_pattern_from_opencl_image_format(cl_image_format opencl_image_format,
							 const void * &pattern) noexcept {

	pattern = nullptr;

	switch (opencl_image_format.image_channel_order) {

	case CL_RGB:
		switch (opencl_image_format.image_channel_data_type) {
		case CL_UNSIGNED_INT8:
			pattern = std::malloc(3);
			for (unsigned char i = 0; i < 3; i++) { ((int8_t*)pattern)[i] = 0; }
			return bld_error_t::SUCCESS;

		default: return bld_error_t::INVALID_IMAGE_FORMAT;
		}
	
	default: return bld_error_t::INVALID_IMAGE_FORMAT;

	}
}

bld_error_t regenerate_opencl_images(bld_context_t context, bld_pipeline_t pipeline) noexcept {
	bld_error_t err = bld_error_t::SUCCESS;

	size_t opencl_pixel_size;
	const cl_image_format opencl_image_format = opencl_image_format_from_bld_pixel_format(pipeline->pixel_format,
											      opencl_pixel_size,
											      err);
	if (err != bld_error_t::SUCCESS) { return err; }

	const void *opencl_fill_pattern = nullptr;
	err = opencl_fill_pattern_from_opencl_image_format(opencl_image_format, opencl_fill_pattern);
	if (err != bld_error_t::SUCCESS) { return err; }

	const cl_image_desc opencl_image_description {
		CL_MEM_OBJECT_IMAGE2D,
		pipeline->width,
		pipeline->height,
		1,
		0,
		opencl_pixel_size * pipeline->width,
		0,
		0,
		0,
		nullptr
	};

	cl_int opencl_err = CL_SUCCESS;
	cl_mem new_opencl_image_a = clCreateImage(context->opencl_environment.context,
						  0,
						  &opencl_image_format,
						  &opencl_image_description,
						  nullptr,
						  &opencl_err);
	switch (opencl_err) {

	case CL_SUCCESS: break;

	// TODO: Put in other cases here.
	default:
		return bld_error_t::UNKNOWN_ERROR;

	}

	cl_mem new_opencl_image_b = clCreateImage(context->opencl_environment.context,
						  0,
						  &opencl_image_format,
						  &opencl_image_description,
						  nullptr,
						  &opencl_err);
	switch (opencl_err) {

	case CL_SUCCESS: break;

	// TODO: Other cases.
	default:
		return bld_error_t::UNKNOWN_ERROR;

	}

	opencl_err = clReleaseMemObject(pipeline->opencl_image_a);
	switch (opencl_err) {
	case CL_SUCCESS: break;

	default:
			 // TODO: Remove CORRUPTED_STATE from everywhere else where it's no good. Not everywhere!
		return bld_error_t::UNKNOWN_ERROR;
	}

	pipeline->opencl_image_a = new_opencl_image_a;

	opencl_err = clReleaseMemObject(pipeline->opencl_image_b);
	switch (opencl_err) {
	case CL_SUCCESS: break;
	default:
		pipeline->opencl_image_b = new_opencl_image_b;
		return bld_error_t::UNKNOWN_ERROR;
	}

	pipeline->opencl_image_b = new_opencl_image_b;

	pipeline->opencl_cache.image_format = opencl_image_format;

	std::free((void*)pipeline->opencl_cache.fill_pattern);
	pipeline->opencl_cache.fill_pattern = opencl_fill_pattern;

	pipeline->opencl_cache.pixel_size = opencl_pixel_size;
	pipeline->opencl_cache.image_size = pipeline->width * pipeline->height * pipeline->opencl_cache.pixel_size;

	return bld_error_t::SUCCESS;
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

	// TODO: Somehow track whether or not an object has been initialized or not, or what state it's currently in.
	// Enum is probably the cleanest way I suppose.
	// Then every function should first check if the object is in a valid state.

	result->width = width;
	result->height = height;
	result->pixel_format = pixel_format;
	result->programs = nullptr;
	result->program_count = 0;

	result->opencl_cache.fill_pattern = nullptr; // SUPER IMPORTANT, DON'T REMOVE

	*err = regenerate_opencl_images(context, result);
	if (*err != bld_error_t::SUCCESS) {
		std::free(result);
		return nullptr;
	}

	return result;
}

bld_error_t bldReleasePipeline_inner(bld_context_t context, bld_pipeline_t pipeline) noexcept {
	cl_int opencl_err = clReleaseMemObject(pipeline->opencl_image_a);
	switch (opencl_err) {
	case CL_SUCCESS: break;
	// TODO: Handle other errors.
	default:
		clReleaseMemObject(pipeline->opencl_image_b);
		std::free(pipeline->programs);
		std::free(pipeline);
		return bld_error_t::RELEASE_FAILED;
	}

	opencl_err = clReleaseMemObject(pipeline->opencl_image_b);
	switch (opencl_err) {
	case CL_SUCCESS: break;
	// TODO: Handle other errors.
	// TODO: Consider handling the tracking of the releases in the context struct. I think you should,
	// not totally sure though.
	default:
		std::free(pipeline->programs);
		std::free(pipeline);
		return bld_error_t::CORRUPTED_STATE;
	}

	std::free(pipeline->programs);

	// TODO: Consider simply having an oversized fill_pattern container array. That way you don't have to
	// manage it separately, and it'll just dissappear with the free pipeline. Seems simpler and better that way.
	std::free((void*)pipeline->opencl_cache.fill_pattern);

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

	pipeline->width = width;
	pipeline->height = height;

	bld_error_t err = regenerate_opencl_images(context, pipeline);
	if (err != bld_error_t::SUCCESS) { return err; }

	return bld_error_t::SUCCESS;
}

bld_error_t bldGetPipelineDimensions_inner(bld_context_t context,
					   bld_pipeline_t pipeline,
					   size_t *width,
					   size_t *height) noexcept {

	*width = pipeline->width;
	*height = pipeline->height;

	return bld_error_t::SUCCESS;
}

bld_error_t bldSetPipelinePixelFormat_inner(bld_context_t context,
					    bld_pipeline_t pipeline,
					    bld_pixel_format_t pixel_format) noexcept {

	pipeline->pixel_format = pixel_format;

	bld_error_t err = regenerate_opencl_images(context, pipeline);
	if (err != bld_error_t::SUCCESS) { return err; }

	return bld_error_t::SUCCESS;
}

bld_error_t bldGetPipelinePixelFormat_inner(bld_context_t context,
					    bld_pipeline_t pipeline,
					    bld_pixel_format_t *pixel_format) noexcept {

	*pixel_format = pipeline->pixel_format;

	return bld_error_t::SUCCESS;
}

// TODO: Put these two things inside of the pipeline struct so that multiple pipelines can be rendered at once.
// Should they use the same command queue? If they do, were gonna have to fuck around with events and orderings dependencies and whatever.
// That sounds like a bad idea.
// SOLUTION: Have every pipeline have it's own command queue. Then they'll be freed from each other.
// Same context, different command queue, that'll make it simple.
std::thread render_thread;
bld_error_t render_thread_return_value = bld_error_t::SUCCESS;

bld_error_t bldExecutePipeline_inner(bld_context_t context, bld_pipeline_t pipeline, void *frame_destination) noexcept {

	render_thread = std::thread([](bld_context_t &context, bld_pipeline_t &pipeline, void *frame_destination) static {

		cl_int opencl_err = clEnqueueFillBuffer(context->opencl_environment.command_queue,
							pipeline->opencl_image_a,
							pipeline->opencl_cache.fill_pattern,
							pipeline->opencl_cache.pixel_size,
							0,
							pipeline->opencl_cache.image_size,
							0,
							nullptr,
							nullptr);
		switch (opencl_err) {
		case CL_SUCCESS: break;
		default:
			return render_thread_return_value = bld_error_t::UNKNOWN_ERROR;
		}

		opencl_err = clFinish(context->opencl_environment.command_queue);
		switch (opencl_err) {
		case CL_SUCCESS: break;
		default:
			return render_thread_return_value = bld_error_t::UNKNOWN_ERROR;
		}

		const size_t global_work_offset[] { 0, 0, 0 };

		for (size_t i = 0; i < pipeline->program_count; i++) {
			cl_mem opencl_source_image = (pipeline->image_order == bld_pipeline_image_order_t::A_NEWER) ?
							pipeline->opencl_image_b : pipeline->opencl_image_a;

			cl_mem opencl_destination_image = (pipeline->image_order == bld_pipeline_image_order_t::A_NEWER) ?
								pipeline->opencl_image_a : pipeline->opencl_image_b;

			opencl_err = clSetKernelArg(pipeline->programs[i]->opencl_data.kernel,
						    OPENCL_KERNEL_IMAGE_SOURCE_ARGUMENT_INDEX,
						    sizeof(cl_mem),
						    &opencl_source_image);
			switch (opencl_err) {
			case CL_SUCCESS: break;
			default:
				return render_thread_return_value = bld_error_t::UNKNOWN_ERROR;
			}

			opencl_err = clSetKernelArg(pipeline->programs[i]->opencl_data.kernel,
						    OPENCL_KERNEL_IMAGE_DESTINATION_ARGUMENT_INDEX,
						    sizeof(cl_mem),
						    &opencl_destination_image);
			switch (opencl_err) {
			case CL_SUCCESS: break;
			default:
				return render_thread_return_value = bld_error_t::UNKNOWN_ERROR;
			}

			const size_t *local_work_size = pipeline->programs[i]->opencl_data.local_work_size;

			const size_t global_work_size[] {
				pipeline->width + pipeline->width % local_work_size[0],
				pipeline->height + pipeline->height % local_work_size[1],
				1
			};

			opencl_err = clEnqueueNDRangeKernel(context->opencl_environment.command_queue,
							    pipeline->programs[i]->opencl_data.kernel,
							    2,
							    global_work_offset,
							    global_work_size,
							    local_work_size,
							    0,
							    nullptr,
							    nullptr);


			opencl_err = clFinish(context->opencl_environment.command_queue);
			switch (opencl_err) {
			case CL_SUCCESS: break;
			default:
				return render_thread_return_value = bld_error_t::UNKNOWN_ERROR;
			}

			pipeline->image_order = (pipeline->image_order == bld_pipeline_image_order_t::A_NEWER) ?
							bld_pipeline_image_order_t::B_NEWER :
							bld_pipeline_image_order_t::A_NEWER;
		}

		cl_mem opencl_output_image = (pipeline->image_order == bld_pipeline_image_order_t::A_NEWER) ?
						pipeline->opencl_image_a : pipeline->opencl_image_b;

		const size_t global_work_size[] { pipeline->width, pipeline->height, 1 };

		opencl_err = clEnqueueReadImage(context->opencl_environment.command_queue,
						opencl_output_image,
						true,
						global_work_offset,
						global_work_size,
						pipeline->width * pipeline->opencl_cache.pixel_size,
						0,
						frame_destination,
						0,
						nullptr,
						nullptr);
		switch (opencl_err) {
		case CL_SUCCESS: break;
		default:
			return render_thread_return_value = bld_error_t::UNKNOWN_ERROR;
		}

		return render_thread_return_value = bld_error_t::SUCCESS;

	}, std::ref(context), std::ref(pipeline), frame_destination);

	return bld_error_t::SUCCESS;

}

bld_error_t bldFinishPipelineExecution_inner(bld_context_t context, bld_pipeline_t pipeline) noexcept {
	render_thread.join();
	return render_thread_return_value;
}

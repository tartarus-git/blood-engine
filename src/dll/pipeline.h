#pragma once

#include "error.h"
#include "context.h"
#include "program.h"

enum class bld_pipeline_image_order_t : bool {
	A_NEWER,
	B_NEWER
};

enum class bld_pixel_format_t {
	R8G8B8
};

struct bld_pipeline_inner_t {
	union {
		cl_mem opencl_image_a;
	};
	union {
		cl_mem opencl_image_b;
	};

	bld_pipeline_image_order_t image_order;

	size_t width;
	size_t height;
	bld_pixel_format_t pixel_format;

	union {
		struct {
			cl_image_format image_format;
			const void *fill_pattern;
			size_t pixel_size;
			size_t image_size;
		} opencl_cache;
	};

	bld_program_t *programs;
	size_t program_count;
};

using bld_pipeline_t = bld_pipeline_inner_t*;

// TODO: Have releasing the context release all the stuff inside the context.
// That means you're going to have to track the stuff inside the context from the context struct.
// TODO: Is having the context worth it? Consider just doing away with it.

bld_pipeline_t bldCreatePipeline_inner(bld_context_t context,
				       size_t width,
				       size_t height,
				       bld_pixel_format_t pixel_format,
				       bld_error_t *err) noexcept;

bld_error_t bldReleasePipeline_inner(bld_context_t context, bld_pipeline_t pipeline) noexcept;

bld_error_t bldPushProgramToPipeline_inner(bld_context_t context, bld_pipeline_t pipeline, bld_program_t program) noexcept;

bld_program_t bldPopProgramFromPipeline_inner(bld_context_t context, bld_pipeline_t pipeline, bld_error_t *err) noexcept;

bld_error_t bldInsertProgramInPipeline_inner(bld_context_t context,
					     bld_pipeline_t pipeline,
					     size_t index,
					     bld_program_t program) noexcept;

bld_program_t bldRemoveProgramFromPipeline_inner(bld_context_t context,
						 bld_pipeline_t pipeline,
						 size_t index,
						 bld_error_t *err) noexcept;

bld_error_t bldSetPipelineDimensions_inner(bld_context_t context,
					   bld_pipeline_t pipeline,
					   size_t width,
					   size_t height) noexcept;

bld_error_t bldGetPipelineDimensions_inner(bld_context_t context,
					   bld_pipeline_t pipeline,
					   size_t *width,
					   size_t *height) noexcept;

bld_error_t bldSetPipelinePixelFormat_inner(bld_context_t context,
					    bld_pipeline_t pipeline,
					    bld_pixel_format_t pixel_format) noexcept;

bld_error_t bldGetPipelinePixelFormat_inner(bld_context_t context,
					    bld_pipeline_t pipeline,
					    bld_pixel_format_t *pixel_format) noexcept;

bld_error_t bldExecutePipeline_inner(bld_context_t context,
				     bld_pipeline_t pipeline) noexcept;

bld_error_t bldFinishPipelineExecution_inner(bld_context_t context,
					     bld_pipeline_t pipeline) noexcept;

bld_error_t bldGetPipelineOutput_inner(bld_context_t context,
					 bld_pipeline_t pipeline,
					 void *destination,
					 size_t x,
					 size_t y,
					 size_t width,
					 size_t height) noexcept;

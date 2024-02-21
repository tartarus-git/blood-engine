#define DLL_EXPORT __attribute__((visibility("default")))

#include "error.h"
#include "context.h"
#include "pipeline.h"
#include "program.h"

extern "C" {
	
	DLL_EXPORT bld_context_t bldCreateContext(bld_error_t *err) noexcept {
		return bldCreateContext_inner(err);
	}

	DLL_EXPORT bld_error_t bldReleaseContext(bld_context_t context) noexcept {
		return bldReleaseContext_inner(context);
	}

	DLL_EXPORT bld_pipeline_t bldCreatePipeline(bld_context_t context,
						    size_t width,
						    size_t height,
						    bld_pixel_format_t pixel_format,
						    bld_error_t *err) noexcept {

		return bldCreatePipeline_inner(context, width, height, pixel_format, err);
	}

	DLL_EXPORT bld_error_t bldReleasePipeline(bld_context_t context, bld_pipeline_t pipeline) noexcept {
		return bldReleasePipeline_inner(context, pipeline);
	}

	DLL_EXPORT bld_error_t bldPushProgramToPipeline(bld_context_t context,
							bld_pipeline_t pipeline,
							bld_program_t program) noexcept {

		return bldPushProgramToPipeline_inner(context, pipeline, program);
	}

	DLL_EXPORT bld_program_t bldPopProgramFromPipeline(bld_context_t context,
							   bld_pipeline_t pipeline,
							   bld_error_t *err) noexcept {

		return bldPopProgramFromPipeline_inner(context, pipeline, err);
	}

	DLL_EXPORT bld_error_t bldInsertProgramInPipeline(bld_context_t context,
							  bld_pipeline_t pipeline,
							  size_t index,
							  bld_program_t program) noexcept {

		return bldInsertProgramInPipeline_inner(context, pipeline, index, program);
	}

	DLL_EXPORT bld_program_t bldRemoveProgramFromPipeline(bld_context_t context,
							      bld_pipeline_t pipeline,
							      size_t index,
							      bld_error_t *err) noexcept {

		return bldRemoveProgramFromPipeline_inner(context, pipeline, index, err);
	}

	DLL_EXPORT bld_error_t bldSetPipelineDimensions(bld_context_t context,
							bld_pipeline_t pipeline,
							size_t width,
							size_t height) noexcept {

		return bldSetPipelineDimensions_inner(context, pipeline, width, height);
	}

	DLL_EXPORT bld_error_t bldGetPipelineDimensions(bld_context_t context,
							bld_pipeline_t pipeline,
							size_t *width,
							size_t *height) noexcept {

		return bldGetPipelineDimensions_inner(context, pipeline, width, height);
	}

	DLL_EXPORT bld_error_t bldSetPipelinePixelFormat(bld_context_t context,
							 bld_pipeline_t pipeline,
							 bld_pixel_format_t pixel_format) noexcept {

		return bldSetPipelinePixelFormat_inner(context, pipeline, pixel_format);
	}

	DLL_EXPORT bld_error_t bldGetPipelinePixelFormat(bld_context_t context,
							 bld_pipeline_t pipeline,
							 bld_pixel_format_t *pixel_format) noexcept {

		return bldGetPipelinePixelFormat_inner(context, pipeline, pixel_format);
	}

	DLL_EXPORT bld_error_t bldExecutePipeline(bld_context_t context,
						  bld_pipeline_t pipeline) noexcept {

		return bldExecutePipeline_inner(context, pipeline);
	}

	DLL_EXPORT bld_error_t bldFinishPipelineExecution(bld_context_t context,
							  bld_pipeline_t pipeline) noexcept {

		return bldFinishPipelineExecution_inner(context, pipeline);
	}

	DLL_EXPORT bld_error_t bldGetPipelineOutput1D(bld_context_t context,
						      bld_pipeline_t pipeline,
						      void *destination,
						      size_t offset,
						      size_t length) noexcept {

		return bldGetPipelineOutput1D_inner(context, pipeline, destination, offset, length);
	}

	DLL_EXPORT bld_error_t bldGetPipelineOutput2D(bld_context_t context,
						      bld_pipeline_t pipeline,
						      void *destination,
						      size_t x,
						      size_t y,
						      size_t width,
						      size_t height) noexcept {

		return bldGetPipelineOutput2D_inner(context, pipeline, destination, x, y, width, height);
	}

	DLL_EXPORT bld_program_t bldCreateProgram(bld_context_t context,
						  const char *source_code,
						  bld_error_t *err) noexcept {

		return bldCreateProgram_inner(context, source_code, err);
	}

	DLL_EXPORT bld_error_t bldCompileProgramToPlatform(bld_context_t context, bld_program_t program) noexcept {

		return bldCompileProgramToPlatform_inner(context, program);
	}

	DLL_EXPORT bld_error_t bldCompileProgram(bld_context_t context, bld_program_t program) noexcept {

		return bldCompileProgram_inner(context, program);
	}
}

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

	DLL_EXPORT bld_pipeline_t bldCreatePipeline(bld_context_t context, bld_error_t *err) noexcept {
		return bldCreatePipeline_inner(context, err);
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

}

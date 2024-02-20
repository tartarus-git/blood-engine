#define DLL_EXPORT __attribute__((visibility("default")))

#include "error.h"
#include "context.h"
#include "pipeline.h"

extern "C" {
	
	DLL_EXPORT bld_context_t bldCreateContext(bld_error_t *err) noexcept {
		return bldCreateContext_inner(err);
	}

	DLL_EXPORT bld_error_t bldReleaseContext(bld_context_t context) noexcept {
		return bldReleaseContext_inner(context);
	}

	DLL_EXPORT bld_pipeline_t bldCreatePipeline(bld_error_t *err) noexcept {
		return bldCreatePipeline_inner(err);
	}

	DLL_EXPORT bld_error_t bldReleasePipeline(bld_pipeline_t pipeline) noexcept {
		return bldReleasePipeline_inner(pipeline);
	}

}

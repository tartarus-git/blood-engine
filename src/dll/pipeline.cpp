#include "pipeline.h"

bld_pipeline_t bldCreatePipeline_inner(bld_error_t *err) noexcept {
	*err = bld_error_t::SUCCESS;

	bld_pipeline_t result = new bld_pipeline_inner_t();

	return result;
}

bld_error_t bldReleasePipeline_inner(bld_pipeline_t pipeline) noexcept {
	delete pipeline;

	return bld_error_t::SUCCESS;
}

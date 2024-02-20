#pragma once

#include "error.h"

struct bld_pipeline_inner_t {
};

using bld_pipeline_t = bld_pipeline_inner_t*;

bld_pipeline_t bldCreatePipeline_inner(bld_error_t *err) noexcept;

bld_error_t bldReleasePipeline_inner(bld_pipeline_t pipeline) noexcept;

#pragma once

#include "error.h"
#include "context.h"
#include "program.h"

struct bld_pipeline_inner_t {
	bld_program_t *programs;
	size_t program_count;
};

using bld_pipeline_t = bld_pipeline_inner_t*;

// TODO: Have releasing the context release all the stuff inside the context.
// That means you're going to have to track the stuff inside the context from the context struct.
// TODO: Is having the context worth it? Consider just doing away with it.

bld_pipeline_t bldCreatePipeline_inner(bld_context_t context, bld_error_t *err) noexcept;

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

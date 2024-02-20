#include "pipeline.h"

#include <cstdlib>

bld_pipeline_t bldCreatePipeline_inner(bld_context_t context, bld_error_t *err) noexcept {
	*err = bld_error_t::SUCCESS;

	bld_pipeline_t result = (bld_pipeline_t)std::malloc(sizeof(bld_pipeline_inner_t));

	if (result == nullptr) { *err = bld_error_t::OUT_OF_HOST_MEM; return nullptr; }

	return result;
}

bld_error_t bldReleasePipeline_inner(bld_context_t context, bld_pipeline_t pipeline) noexcept {
	std::free(pipeline->programs);

	delete pipeline;

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

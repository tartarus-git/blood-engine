#pragma once

enum class bld_error_t {
	SUCCESS,
	OUT_OF_HOST_MEM,
	INVALID_CONTEXT,
	BUG,
	OUT_OF_DEVICE_MEM,
	UNKNOWN_ERROR,
	ALREADY_COMPILED,
	PIXEL_FORMAT_UNSUPPORTED,
	INVALID_IMAGE_FORMAT,
	RELEASE_FAILED,
	CORRUPTED_STATE,	// TODO: Look over this one's use cases, make sure you use it sparingly if at all, avoid it when possible.
	BUILD_FAILURE
};

#define DLL_EXPORT __declspec(dllexport)

#include "context.h"

extern "C" {

	DLL_EXPORT bld_context_t bldCreateContext() noexcept {
		return bldCreateContext_inner();
	}

}

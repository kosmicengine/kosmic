#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#endif

// -- KOSMIC start --
#include <zlib.h> // Should come before including tinyexr.
// -- KOSMIC end --

#define TINYEXR_IMPLEMENTATION
#include "tinyexr.h"

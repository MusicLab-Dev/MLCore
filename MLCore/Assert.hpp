/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Assert helper
 */

#pragma once

#ifdef NDEBUG // Release mode

# define CORE_DEBUG_BUILD false

// Do nothing
# define coreAssert(expression, onError) static_cast<void>(0)
# define coreAssertFallback(expression, onFallback, onError) static_cast<void>(0)

#else // Debug mode

# define CORE_DEBUG_BUILD true

# define coreAssert(expression, onError) if (!(expression)) onError
# define coreAssertFallback(expression, onFallback, onError) if (!(expression)) { onFallback; onError; }
#endif

/** @brief Exception helper */
#define nothrow_ndebug !CORE_DEBUG_BUILD

/** @brief Helper to make a function noexcept only in release */
#define noexcept_ndebug noexcept(!CORE_DEBUG_BUILD)

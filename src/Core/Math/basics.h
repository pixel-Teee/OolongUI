#pragma once

#include <float.h>
#include <limits>
#include <cmath>
#include <cassert>

namespace Oolong
{
	typedef unsigned int uint;

	#define cassert(x) static_assert(x, #x)
}
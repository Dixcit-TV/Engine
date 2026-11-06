#include "MathHelpers.h"

namespace MathHelpers
{
	uint32_t RoundUpTo(uint32_t value, uint32_t multiple)
	{
		return (value + (multiple - 1)) & ~(multiple - 1);
	}

};
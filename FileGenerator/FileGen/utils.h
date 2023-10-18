#pragma once

#include <stdint.h>
#include <chrono>
#include <thread>

namespace utils
{
	inline void sleep(uint32_t ulMill)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(ulMill));
	}
};
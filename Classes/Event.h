#pragma once

#include <functional>

#include "CommonData.h"

class EventListener : public std::function<void()>
{
public:
	/*bool operator==(const std::function<void()> &func)
	{
		if (*this == func)
			return true;
			
		return false;
	}*/
};
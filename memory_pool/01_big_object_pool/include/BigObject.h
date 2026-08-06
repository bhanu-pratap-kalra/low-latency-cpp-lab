#pragma once

#include <algorithm>
#include <iterator>

class BigObject
{
public:
	void reset()
	{
		std::fill(std::begin(data), std::end(data), 0);
	}
private:
	int data[10000];
};

#pragma once

#include <iostream>

template<typename T, typename ...Types>
void print(T const& firstArg, Types const&... args)
{
	std::cout << firstArg << ' ';

	if constexpr (sizeof...(args) > 0) {
		print(args...);
	}
	else {
		std::cout << '\n';
	}
	
}
/*******************************************************************
** This code is part of ShootingRange game.
** Created Baleyv Nikolay nicmc9@gmail.com
**
** ShootingRange is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/

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


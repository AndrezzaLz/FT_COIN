/* ********************************************************************************
 * Prof. Dr. Andre F. de Angelis
 * School of Technology
 * University of Campinas (Unicamp)
 * 1st Semester - 2024
 * ********************************************************************************
 * This file is part of a C++ teaching project directed to undergraduate students
 * of Information System; and System Analyzes and Development courses of the School
 * of Technology of Unicamp.
 * The project is a simplified cash flow control intended to demonstrate C++
 * resources and capabilities. There are non-optimized parts and some unrealistic
 * pieces of code in it, as well as advanced function pointers and unconventional
 * use of data structures, according to the project objectives.
 * The cash flow program is not a real application. Use its code to learn C++.
 * ********************************************************************************
 * g++ (GCC) 13.3.1 20240522 (Red Hat 13.3.1-1)
 * Eclipse Version: 2023-12 (4.30.0) Build id: 20231201-2043
 * Fedora Linux 39 + KDE
 * ********************************************************************************
 * Copyright (C) 2024 Andre F. de Angelis
 * ********************************************************************************
 * Utils.cpp
 * ********************************************************************************
 */

#include <iostream>
#include "Utils.h"
#include "SysInfo.h"

void Utils::printMessage(string text)
	{
	cout << "* " << SysInfo::getSystemName() << " message: " << text << endl;
	}

void Utils::printFramedMessage(string text, string symbol, int length)
	{
	cout << replicate(symbol, length) << endl;
	;
	cout << text << endl;
	cout << replicate(symbol, length) << endl;
	;
	}

string Utils::replicate(string text, int times)
	{
	string buffer;
	for (int count = 0; count < times; count++)
		{
		buffer += text;
		}
	return (buffer);
	}

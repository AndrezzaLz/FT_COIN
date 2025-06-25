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
 * Utils.h
 * ********************************************************************************
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <string>

using namespace std;

class Utils final
{
public:
	static void printMessage(string text);
	static void printFramedMessage(string text, string symbol, int length);
	static string replicate(string text, int times);
	
	static const int AUTO_GENERATED_ID = 0;
	
	static const int TABLE_WIDTH_NORMAL = 80;
	static const int TABLE_WIDTH_WIDE = 100;
	static const string DECORATOR_EQUALS;
    static const string DECORATOR_DASH;

	static const char* ANSI_RED;
	static const char* ANSI_GREEN;
	static const char* ANSI_RESET;
};

#endif /* UTILS_H_ */

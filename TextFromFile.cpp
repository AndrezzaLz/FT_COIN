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
 * TextFromFile.cpp
 * ********************************************************************************
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

#include "TextFromFile.h"
#include "Utils.h"

TextFromFile::TextFromFile(string fileName) :
		fileName(fileName)
	{
	fileContent = "";
	try
		{
		ifstream inputFile(fileName);
		stringstream buffer;

		if (!inputFile.is_open())
			{
			throw runtime_error("Failed to open the file: " + fileName);
			}

		buffer << inputFile.rdbuf(); // Read entire file into the buffer
		inputFile.close();
		fileContent = buffer.str();
		}
	catch (const exception &myException)
		{
		Utils::printMessage("Unexpected problem: " + string(myException.what()));
		}
	}

string& TextFromFile::getFileContent()
	{
	return fileContent;
	}

string& TextFromFile::getFileName()
	{
	return fileName;
	}

TextFromFile::~TextFromFile()
	{
	}

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
 * SysInfo.h
 * ********************************************************************************
 */

#ifndef SYSINFO_H_
#define SYSINFO_H_

using namespace std;

#include <string>

class SysInfo final
	{
	private:
		static const string author;
		static const string date;
		static const string department;
		static const string institution;
		static const string systemName;
		static const string version;
		static const string helpFile;

	public:

		static const string& getAuthor();
		static const string& getDate();
		static const string& getDepartment();
		static const string& getInstitution();
		static const string& getSystemName();
		static const string& getVersion();
		static const string& getHelpFile();

		static const string getFullVersion();
	};

#endif /* SYSINFO_H_ */

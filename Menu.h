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
 * Menu.h
 * ********************************************************************************
 */

#ifndef MENU_H_
#define MENU_H_

#include <iostream>
#include <vector>

using namespace std;

class Menu final
	{
	private:
		vector<string> &itens;

		string title;
		string message;

		string symbol;
		int width;
		bool repeatList;
		bool zeroForLastOpt;

		const bool isNotAValidChoice(unsigned long choice) const;
		const string replicate(string text, int times) const;
		const string makeDecorator();

	public:
		Menu(vector<string> &itens, string title = "Menu", string message = "Option: ");
		virtual ~Menu();

		const int getChoice();
		void setDecorator(const string &decorator, int width);

		const string& getDecorator() const;
		const string& getMessage() const;
		const int getLength() const;
		void setMessage(const string &message);
		bool isRepeatList() const;
		void setRepeatList(bool repeatList = false);
		const string getSymbol() const;
		void setSymbol(const string &symbol = "-");
		const string& getTitle() const;
		void setTitle(const string &title);
		int getWidth() const;
		void setWidth(int width = 0);
		bool isZeroForLastOpt() const;
		void setZeroForLastOpt(bool zeroForLastOpt = true);
	};

#endif /* MENU_H_ */

#ifndef DATE_HPP
#define DATE_HPP

#include <iostream>
#include <ctime>

class Date
	{
	private:
		int day;
		int month;
		int year;

	public:
		Date();
		Date(int day, int month, int year);
		Date(std::string isoFormat);
		virtual ~Date();

		int getYear();
		int getMonth();
		int getDay();
		std::string getIsoFormat();

		bool operator==(const Date &other) const;
		bool operator!=(const Date &other) const;
		bool operator>(const Date &other) const;
		bool operator>=(const Date &other) const;
		bool operator<(const Date &other) const;
		bool operator<=(const Date &other) const;

		friend std::ostream& operator<<(std::ostream &os, const Date &date);
		friend std::istream& operator>>(std::istream &is, Date &date);
	};

#endif
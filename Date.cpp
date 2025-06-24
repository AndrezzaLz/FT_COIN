#include <iomanip>
#include <ctime>
#include <stdexcept>
#include <sstream>

#include "Date.hpp"

using namespace std;

Date::Date()
{
	time_t now = time(NULL);
	tm *currentTime = localtime(&now);
	day = currentTime->tm_mday;
	month = currentTime->tm_mon + 1;
	year = currentTime->tm_year + 1900;
}

Date::Date(string isoFormat)
{
	try 
	{
        if (isoFormat.length() < 10 || isoFormat.at(4) != '-' || isoFormat.at(7) != '-') 
		{
            throw invalid_argument("Formato ISO de data invalido: esperado YYYY-MM-DD.");
        }

		day = stoi(isoFormat.substr(8, 2));
		month = stoi(isoFormat.substr(5, 2));
		year = stoi(isoFormat.substr(0, 4));

		if (month < 1 || month > 12 || day < 1 || day > 31) 
		{ 
			throw invalid_argument("Data fora do intervalo valido (ISO).");
		}
	} 
	catch (const out_of_range& oor) 
	{
        throw invalid_argument("Valor numerico fora do range no formato ISO de data.");
    } 
	catch (const invalid_argument& ia) 
	{
        throw invalid_argument("Erro ao converter data ISO: " + string(ia.what()));
    }

}

Date::Date(int day, int month, int year) : day(day), month(month), year(year)
{
}

Date::~Date()
{
}

int Date::getYear()
{
	return year;
}

int Date::getMonth()
{
	return month;
}

int Date::getDay()
{
	return day;
}

string Date::getIsoFormat()
{
	stringstream ss;
	ss << setfill('0') << setw(4) << year << "-";
	ss << setfill('0') << setw(2) << month << "-";
	ss << setfill('0') << setw(2) << day;
	return ss.str(); 
}

bool Date::operator==(const Date &other) const
{
	return year == other.year && month == other.month && day == other.day;
}

bool Date::operator!=(const Date &other) const
{
	return !(*this == other);
}

bool Date::operator>(const Date &other) const
{
	if (year != other.year)
		return year > other.year;
	if (month != other.month)
		return month > other.month;
	return day > other.day;
}

bool Date::operator>=(const Date &other) const
{
	return (*this == other) || (*this > other);
}

bool Date::operator<(const Date &other) const
{
	return !(*this >= other);
}

bool Date::operator<=(const Date &other) const
{
	return !(*this > other);
}

ostream &operator<<(ostream &os, const Date &date)
{
	char original_fill = os.fill();

	os << setfill('0') << setw(4) << date.year << ".";
	os << setfill('0') << setw(2) << date.month << ".";
	os << setfill('0') << setw(2) << date.day;

	os.fill(original_fill);

	return os;
}

istream &operator>>(std::istream &is, Date &date)
{
	char separator;
	is >> date.day >> separator >> date.month >> separator >> date.year;
	return is;
}
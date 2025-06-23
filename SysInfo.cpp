#include "SysInfo.h"

const string SysInfo::author = "Andrezza Zozzoro, Bianca Oshiro, Sidinei Junior, Henrique Saito, Yasmin Inoue. Parte do trabalho foi reutilizado do projeto Agregador do professor Angelis.";
const string SysInfo::date = "2025, Junho";
const string SysInfo::department = "Faculdade de Tecnologia - Limeira/SP";
const string SysInfo::institution = "Universidade Estadual de Campinas (Unicamp)";
const string SysInfo::systemName = "FT COIN 2025 C++";
const string SysInfo::version = "1.0";
const string SysInfo::helpFile = "help.txt";

const string &SysInfo::getAuthor()
{
	return author;
}

const string &SysInfo::getDate()
{
	return date;
}

const string &SysInfo::getDepartment()
{
	return department;
}

const string &SysInfo::getInstitution()
{
	return institution;
}

const string &SysInfo::getSystemName()
{
	return systemName;
}

const string &SysInfo::getVersion()
{
	return version;
}

const string &SysInfo::getHelpFile()
{
	return helpFile;
}

const string SysInfo::getFullVersion()
{
	return systemName + " | Ver. " + version + " (" + date + ")";
}

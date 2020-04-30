#include "eltab.h"
#include <algorithm>
#include <regex>
#include <cctype>

void eltab::reset()
{
	for (auto iterator : tab) {
		*iterator.second = variant();
	}
}

eltab::eltab(const eltab& vars)
{
	for (auto &iterator : tab) {
		delete iterator.second;
	}
	tab.clear();
	for (const auto &iterator : vars.tab) {
		tab.emplace(iterator.first, new variant(*iterator.second));
	}
}

void eltab::copy(const eltab& vars)
{
	for (auto &iterator : tab)
	{
	 	const auto iter = vars.tab.find(iterator.first);
		if (iter != vars.tab.cend())
			*iterator.second = *iter->second;
	}
}

bool eltab::my_predicate(char c)
{
	std::regex reg("a-zA-Z0-9");
	std::string check(&c);
	bool logic = std::regex_match(check, reg);
	return logic;
}

bool eltab::add(char* s)
{	
	char* b = s;
	while ((*b >= 'a' && *b <= 'z') || (*b >= 'A' && *b <= 'Z')||(*b >= '0' && *b <= '9'))
		b++;
	char c = *b;
	*b = 0;
	std::string name(s);
	*b = c;
	variant* var = new variant;
	std::pair<std::unordered_map<std::string,variant*>::iterator,bool>ins = tab.emplace(name,var);
	if(!ins.second)
		delete var;
	return ins.second;
}
variant* eltab::get(char* s)
{
	char* b = s;
	while ((*b >= 'a' && *b <= 'z') || (*b >= 'A' && *b <= 'Z') || (*b >= '0' && *b <= '9'))
		b++;
	char c = *b;
	*b = 0;
	std::string name(s);
	*b = c;
	std::unordered_map<std::string,variant*>::iterator i=tab.find(name);
	if(i==tab.end())
		return nullptr;
	return i->second;
}
bool eltab::set(char*s,variant* var)
{
	char* b = s;
	while ((*b >= 'a' && *b <= 'z') || (*b >= 'A' && *b <= 'Z') || (*b >= '0' && *b <= '9'))
		b++;
	char c = *b;
	*b = 0;
	std::string name(s);
	*b = c;
	std::unordered_map<std::string,variant*>::iterator i=tab.find(name);
	if(i==tab.end())
		return false;
	delete i->second;
	i->second = var;
	return true;
}

void eltab::print()
{
	printf("\nVariables:\n");
	for (auto &iterator : tab) {
		printf("\n%s:\n", iterator.first.c_str());
		iterator.second->print();
	}
}

eltab::eltab(){}
eltab::~eltab()
{
	for (auto &iterator : tab) {
		delete iterator.second;
	}
}

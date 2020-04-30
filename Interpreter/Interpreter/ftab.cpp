#include "ftab.h"

function::function():stree(nullptr){}
function::~function(){delete stree;}

bool ftab::add(char* fname)
{	
	char* b = fname;
	while ((*b >= 'a' && *b <= 'z') || (*b >= 'A' && *b <= 'Z') || (*b >= '0' && *b <= '9'))
		b++;
	char c = *b;
	*b = 0;
	std::string name(fname);
	*b = c;
	std::pair< std::unordered_map<std::string,function*>::iterator,bool>ins=tab.emplace(name,new function);
	return ins.second;
}
variant* ftab::getdef(char* fname, char* var)
{
	char* b = fname;
	while ((*b >= 'a' && *b <= 'z') || (*b >= 'A' && *b <= 'Z') || (*b >= '0' && *b <= '9'))
		b++;
	char c = *b;
	*b = 0;
	std::string name(fname);
	*b = c;
	std::unordered_map<std::string,function*>::iterator i = tab.find(name);
	if(i == tab.end())
		return nullptr;
	return i->second->getdef(var);
}
bool ftab::adddef(char* fname, char* var)
{
	char* b = fname;
	while ((*b >= 'a' && *b <= 'z') || (*b >= 'A' && *b <= 'Z') || (*b >= '0' && *b <= '9'))
		b++;
	char c = *b;
	*b = 0;
	std::string name(fname);
	*b = c;
	std::unordered_map<std::string,function*>::iterator i = tab.find(name);
	if(i == tab.end())
		return false;
	return i->second->adddef(var);
}

bool function::adddef(char* var)
{
	return deftab.add(var);
}

function* ftab::getf(char* fname)
{
	char* b = fname;
	while ((*b >= 'a' && *b <= 'z') || (*b >= 'A' && *b <= 'Z') || (*b >= '0' && *b <= '9'))
		b++;
	char c = *b;
	*b = 0;
	std::string name(fname);
	*b = c;
	std::unordered_map<std::string,function*>::iterator i = tab.find(name);
	if(i == tab.end())
		return nullptr;
	return i->second;
}

void ftab::settree(char* fname, tree* func_tree)
{
	char* b = fname;
	while ((*b >= 'a' && *b <= 'z') || (*b >= 'A' && *b <= 'Z') || (*b >= '0' && *b <= '9'))
		b++;
	char c = *b;
	*b = 0;
	std::string name(fname);
	*b = c;
	std::unordered_map<std::string,function*>::iterator i = tab.find(name);
	if(i == tab.end())
		throw "Function not found";
	i->second->settree(func_tree);
}

void function::settree(tree*t)
{
	delete stree;
	stree=t;
}

variant* function::run(variant var,field& f)
{
	if(stree)
	{	
		variant bel(param);
		param.rfl(var);
		eltab btab(deftab);
		deftab.reset();
		variant* ret = stree->run(f);
		deftab.copy(btab);
		param = bel;
		return ret;
	}
	return nullptr;
}

ftab::ftab(){}

variant* function::getdef(char* fname)
{
	return deftab.get(fname);
}

ftab::~ftab()
{
	for (auto & iterator : tab) {
		delete iterator.second;
	}
}

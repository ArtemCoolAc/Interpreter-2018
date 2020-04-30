#include "variant.h"
#include <cstdio>

void variant::convert(int from,int to)
{
	for(std::vector<std::vector<element> >::iterator i = v.begin(); i != v.end(); i++)
		for(std::vector<element>::iterator j = i->begin();j != i->end(); j++)
			j->convert(from,to);
}

int variant::get1d() {return v.size();}
int variant::get2d (int x)
{
	if (v.size() <= x)
		return 0;
	return v[x].size();
}

const variant variant::operator + (const variant& s)
{
	variant res;
	int x = 0;
	std::vector<std::vector<element> >::const_iterator vi;
	std::vector<std::vector<element> >::const_iterator si;
	for (vi = v.cbegin(), si = s.v.cbegin(); vi != v.cend() && si != s.v.cend(); vi++, si++, x++)
	{
		res.v.push_back(std::vector<element>());
		std::vector<element>::const_iterator vj;
		std::vector<element>::const_iterator sj;
		for(vj = vi->cbegin(), sj = si->cbegin(); vj != vi->cend() && sj != si->cend(); vj++, sj++)
			res.v[x].push_back(*vj + *sj);
		if(vj != vi->cend() || sj != si->cend())
		{
			std::vector<element>::const_iterator j = (vj==vi->cend()?sj:vj);
			std::vector<element>::const_iterator ej = (vj==vi->cend()?si->cend():vi->cend());
			for(; j != ej; j++) res.v[x].push_back(*j);
		}
	}
	if (vi != v.cend() || si != s.v.cend())
	{
		res.v.push_back(std::vector<element>());
		std::vector<std::vector<element> >::const_iterator i = (vi == v.cend()?si:vi);
		std::vector<std::vector<element> >::const_iterator ei = (vi == v.cend()?s.v.cend():v.cend());
		for(; i != ei; i++, x++)
		{
			for(std::vector<element>::const_iterator j = i->cbegin(); j != i->cend(); j++)
				res.v[x].push_back(*j);
		}
	}
	return res;
}

void variant::addf()
{
	v.insert(v.begin(),std::vector<element>());
}

const variant operator-(const variant& s)
{
	variant res(s);
	for(std::vector<std::vector<element> >::iterator i = res.v.begin(); i != res.v.end(); i++)
		for(std::vector<element>::iterator j = i->begin(); j != i->end(); j++)
			*j = -*j;
	return res;
}

variant& variant::operator=(const variant& s)
{
	v.clear();
	for(std::vector<std::vector<element> >::const_iterator i = s.v.cbegin(); i != s.v.cend(); i++)
	{
		v.push_back(std::vector<element>());
		for(std::vector<element>::const_iterator j = i->cbegin(); j != i->cend(); j++)
			v[v.size() - 1].push_back(*j);
	}
	return *this;
}

variant::variant(const variant& s)
{
	for(std::vector<std::vector<element> >::const_iterator i=s.v.cbegin();i!=s.v.cend();i++)
	{
		v.push_back(std::vector<element>());
		for(std::vector<element>::const_iterator j=i->cbegin();j!=i->cend();j++)
			v[v.size()-1].push_back(*j);
	}
}

element& variant::get(int x, int y)
{
	if(v.size() < x + 1 || v[x].size() < y + 1)
		set(x, y, element());
	return v[x][y];
}
	
variant& variant::set(int x, int y, element s)
{
	while(v.size() < (x + 1)) {	
		v.push_back(std::vector<element>());}
	while(v[x].size() < y + 1)
		v[x].push_back(element());
	v[x][y] = s;
	return *this;
}

void variant::push(variant& s)
{
	for(std::vector<std::vector<element> >::iterator i = s.v.begin(); i != s.v.end(); i++)
	{
		std::vector<element> nv;
		for(std::vector<element>::iterator j = i->begin(); j != i->end(); j++)
			nv.push_back(*j);
		v.insert(v.begin(), nv);
	}
}	

variant::variant(){}
variant::~variant(){}

void variant::combiney(const variant& left, const variant& right)
{
	if(v.size() < 1)
		v.push_back(std::vector<element>());
	if(v.size() < 2)
		v.push_back(std::vector<element>());
	v[1].clear();
	for(std::vector<element>::const_iterator i = left.v[1].cbegin();i != left.v[1].cend(); i++)
		v[1].push_back(*i);
	for(std::vector<element>::const_iterator i = right.v[1].cbegin();i != right.v[1].cend(); i++)
		v[1].push_back(*i);
}

void variant::combinex(const variant& left, const variant& right)
{
	v.clear();
	v.push_back(std::vector<element>());
	if (left.v.size() > 1)
		for(std::vector<std::vector<element> >::const_iterator i = left.v.cbegin() + 1; i != left.v.cend(); i++)
			v.push_back(*i);
	if (right.v.size()>1)
		for(std::vector<std::vector<element> >::const_iterator i = right.v.cbegin() + 1; i != right.v.cend(); i++)
			v.push_back(*i);
}

void variant::rfl(variant s)
{
	v.clear();
	if (s.v.size()>1)
		for(std::vector<std::vector<element> >::const_iterator i = s.v.cbegin() + 1; i != s.v.cend(); i++)
			v.push_back(*i);
}

void variant::print()
{
	int x = 0, y = 0;
	for(std::vector<std::vector<element> >::iterator i = v.begin(); i != v.end(); i++, x++)
	{
		for(std::vector<element>::iterator j = i->begin(); j != i->end(); j++, y++)
		{
			printf("(%d,%d):\n", x, y);
			j->print();
		}
		y = 0;
	}
}

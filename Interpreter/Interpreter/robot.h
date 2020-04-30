#pragma once
#include <cstdio>
#include <vector>
#include "variant.h"

class field
{
	std::vector<std::vector<int> > v;
	int x;
	int y;
	public:
	field(FILE*);
	variant* act(variant&);
	void print();
	~field();
};

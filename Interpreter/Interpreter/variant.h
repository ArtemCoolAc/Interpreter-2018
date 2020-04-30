#include <vector>
#include "element.h"
class variant
{
	std::vector<std::vector<element> > v;
	public:
	variant& set(int x,int y,element s);
	element& get(int x,int y);
	int get1d();
	int get2d(int x);
	void combiney(const variant&left,const variant&right);
	void combinex(const variant&left,const variant&right);
	void push(variant&);
	void print();
	void rfl(variant);
	void convert(int from,int to);
	variant& operator=(const variant&);
	const variant operator +(const variant&);
	void addf();
	friend const variant operator-(const variant&);
	variant(const variant&);
	variant();
	~variant();
};

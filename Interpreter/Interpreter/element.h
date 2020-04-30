class element
{
	bool bval;
	int ival;
	char* strval;
	bool strtob(char*);
	char* itostr(int);
	public:
	friend const element operator-(const element&);
	const element operator+(const element&)const;
	const element& operator=(const element&);
	element& convert(int from,int to);
	element& digitize(int from);
	void set(bool b,int i,char* s);
	int geti();
	bool getb();
	char* gets();
	void seti(const int);
	void setb(const bool);
	void sets(char*);
	void print();
	element();
	element(const bool b,const int i,char* s);
	element(const element&);
	element(element&& s);
	~element();
};

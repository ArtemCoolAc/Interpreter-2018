#include "element.h"
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <string>

element::element()
{
	bval=0;
	ival=0;
	strval=new char[1];
	strval[0]=0;
}
	
element::element(const bool boolean,const int integer,char* string)
{
	bval = boolean;
	ival = integer;
	if(string)
	{
		int length = strlen(string) + 1;
		strval = new char[length];
		strval[0] = 0;
		strcpy(strval, string);
	}
	else
	{
		strval = new char[1];
		strval[0] = 0;
	}
}

element::element(const element& element)
{
	ival = element.ival;
	bval = element.bval;	
	int length = strlen(element.strval) + 1;
	strval = new char[length];
	strval[0] = 0;
	strcpy(strval,element.strval);
}
	
element::~element()
{
	if (strval)
	delete strval;
}

element::element(element&& element)
{
	ival = element.ival;
	bval = element.bval;
	strval = element.strval;
	element.strval = nullptr;
}

void element::set(bool boolean,int integer,char* string)
{
	bval = boolean;
	ival = integer;
	if(string && string != strval)
	{
		if(strval)
			delete strval;
		int len = strlen(string) + 1;
		strval = new char[len];
		strval[0]=0;
		strcpy(strval,string);
	}
	if(!string && strval)
	{
		delete strval;
		strval = string;
	}
}

const element operator-(const element& Element)
{
	char* nstr;
	if(!Element.strval)
		nstr=nullptr;
	else if(strcmp(Element.strval,"UP")==0)
	{
		nstr = new char[strlen("DOWN") + 1];
	 	nstr[0]=0;
		strcpy(nstr,"DOWN");
	}
	else if(strcmp(Element.strval,"DOWN")==0)
	{
		nstr = new char[strlen("UP") + 1];
		nstr[0]=0;
		strcpy(nstr,"UP");
	}
	else if(strcmp(Element.strval,"LEFT")==0)
	{
		nstr = new char[strlen("RIGHT") + 1];
		nstr[0]=0;
		strcpy(nstr,"RIGHT");
	}
	else if(strcmp(Element.strval,"RIGHT")==0)
	{
		nstr = new char[strlen("LEFT") + 1];
		nstr[0]=0;
		strcpy(nstr,"LEFT");
	} else if(strcmp(Element.strval,"LOOKUP")==0)
	{
		nstr = new char[strlen("LOOKDOWN") + 1];
		nstr[0]=0;
		strcpy(nstr,"LOOKDOWN");
	}
	else if(strcmp(Element.strval,"LOOKDOWN")==0)
	{
		nstr = new char[strlen("LOOKUP") + 1];
		nstr[0]=0;
		strcpy(nstr,"LOOKUP");
	}
	else if(strcmp(Element.strval,"LOOKLEFT")==0)
	{
		nstr = new char[strlen("LOOKRIGHT") + 1];
		nstr[0]=0;
		strcpy(nstr,"LOOKRIGHT");
	}
	else if(strcmp(Element.strval,"LOOKRIGHT")==0)
	{
		nstr = new char[strlen("LOOKLEFT") + 1];
		nstr[0]=0;
		strcpy(nstr,"LOOKLEFT");
	}
	else
	{
		int length = strlen(Element.strval) + 1;
		nstr = new char[length];
		nstr[0]=0;
		strcpy(nstr,Element.strval);
	}
	element res;
	res.set(!Element.bval,-Element.ival,nstr);
	delete nstr;
	return res;
}

element& element::digitize(int from)
{
	switch(from)
	{
	case 0:
		if (bval) { 
			ival = 1;
		}
		else ival = 0;
		break;	
	case 1:
		ival = atoi(strval);
		break;	
	default:
		break;
	}
	return *this;
}

const element& element::operator =(const element& element)
{
	set(element.bval,element.ival,element.strval);
	return *this;
}

const element element::operator+(const element& Element) const
{
	element res;
	char* new_string;
	int length = strlen(strval) + strlen(Element.strval) + 1;
	new_string = new char[length];
	new_string[0] = 0;
	strcat(new_string, strval);
	strcat(new_string, Element.strval);
	bool result = bval || Element.bval;
	int resulting = ival + Element.ival;
	res.set(result, resulting, new_string);
	delete new_string;
	return res;
}

element& element::convert(int from,int to)
{
	char* newstr;
	char* b;
	switch(from + to * 3)
	{
	case 1:
		if (!ival) {
			bval = false;
		}
		else bval = true;
		break;
	case 2:
		bval = strtob(strval);
		break;
	case 3:
		if (bval) {
			ival = 1;
		}
		else ival = 0;
		break;	
	case 5:
		ival = atoi(strval);
		break;
	case 6: {
		int length;
		char* tr = "TRUE"; char* fa = "FALSE"; char* choice;
		if (bval) { length = 5; choice = tr; }
		else { length = 6; choice = fa; }
		newstr = new char[length];
		newstr[0] = 0;
		strcpy(newstr, choice);
		b = strval;
		strval = newstr;
		delete b;
	}
		break;
	case 7:
		strval = itostr(ival);
		break;			
	default:
		break;
	}
	return *this;
}

bool element::strtob(char*s)
{
	while(*s!=0)
	{
		if (strncmp(s, "TRUE" ,4) == 0)
			return 1;
		else if (strncmp(s, "FALSE", 5) == 0)
			return 0;
		s++;
	}
	return 0;
}

char* element::itostr(int i)
{
	char b[12];
	sprintf(b,"%d",i);
	int length = strlen(b) + 1;
	char* result = new char[length];
	result[0] = 0;
	strcpy(result ,b);
	return result;
}

int element::geti(){return ival;}
bool element::getb(){return bval;}
char* element::gets()
{
	char* newstr = new char[strlen(strval)];
	newstr[0]=0;
	strcpy(newstr,strval);
	return newstr;
}

void element::print()
{
	printf("%s,%d,\"%s\"\n",bval?"TRUE":"FALSE",ival,strval);
}

void element::seti(const int integer) {ival = integer;}
void element::setb(const bool boolean) {bval = boolean;}
void element::sets(char* string)
{
	if(string && string != strval)
	{
		if(strval)
			delete strval;
		int length = strlen(string) + 1;
		strval = new char[length];
		strval[0]=0;
		strcpy(strval, string);
	}
	if(!string && strval)
	{
		delete strval;
		strval = string;
	}
}


#include "robot.h"
#include <cstring>

field::field(FILE* file)
{
	if(!file)
	{
		v.push_back(std::vector<int>());
		v[0].push_back(2);
		x = 0;
		y = 0;
	}
	char symbol;
	int i = 0, j = 0;
	x = -1;
	y = -1;
	v.push_back(std::vector<int>());
	while ((symbol = getc(file)) != EOF)
	{
		switch (symbol)
		{
			case '#':
				v[i].push_back(1);
				j++;
				break;
			case 'X':
				x=j;y=i;
				v[i].push_back(0);
				j++;
				break;
			case '!':
				v[i].push_back(2);
				j++;
				break;
			case '\n':
				v.push_back(std::vector<int>());
				i++;j=0;
				break;
			default:
				if (x == -1) {x = j; y = i;}
				v[i].push_back(0);
				j++;
				break;
		}
	}
	if(i == 0 && j == 0)
	{
		v[0].push_back(2);
		x = 0; y = 0;
	}
}

variant* field::act(variant& s)
{
	variant* ret = new variant();
	ret->addf();
	int com_num = 0;
	char walls[] = "WALL", exits[] = "EXIT";
	for (int i = 1; i < s.get1d(); i++)
		for (int j = 0; j < s.get2d(i); j++)
		{
			char* full_command = s.get(i,j).gets();
			char* cur_com = full_command;
			while (*cur_com != 0)
			{
				while (*cur_com == ' ') cur_com++;
				if ((strlen(cur_com) >= 2) && (strncmp(cur_com, "UP", 2) == 0) && (*(cur_com + 2) == 0 || *(cur_com + 2) == ' '))
				{
					if ((y == 0) || (v[y - 1].size() <= x) || (v[y - 1][x] == 1))
						ret->set(1, com_num++, element(false, 1, walls));
					else
					{
						--y;
						if (v[y][x] == 2)
							ret->set(1, com_num++, element(true, 0, exits));
						else
						{
							int away;
							for (away = y - 1; (away >= 0) && (v[away].size() > x) && (v[away][x] == 0); away--);
							if ((away<0) || (v[away].size() <= x) || (v[away][x] == 1))
								ret->set(1, com_num++, element(true, y - away, walls));
							else
								ret->set(1, com_num++, element(true, y - away, exits));
						}
					}
					print();
				}

				else if ((strlen(cur_com)) >= 2 && strncmp(cur_com, "JUMPUP", 6) == 0 && (*(cur_com + 6) == 0 || *(cur_com + 6) == ' ')) {
					if ((y == 0) || (v[y - 2].size() <= x) || (v[y - 2][x] == 1))
						ret->set(1, com_num++, element(false, 2, walls));
					else
					{
						if (v[y - 2][x] != 1)
						--y; --y;
						if (v[y][x] == 2)
							ret->set(1, com_num++, element(true, 0, exits));
						else
						{
							int away;
							for (away = y - 2; (away >= 0) && (v[away].size()>x) && (v[away][x] == 0); away--);
							if ((away < 0) || (v[away].size() <= x) || (v[away][x] == 1))
								ret->set(1, com_num++, element(true, y - away, walls));
							else
								ret->set(1, com_num++, element(true, y - away, exits));
						}
					}
					print();
				}

				else if (strlen(cur_com) >= 4 && strncmp(cur_com, "DOWN", 4) == 0 && (*(cur_com + 4) == 0 || *(cur_com + 4) == ' '))
				{
					if((y >= v.size() - 1) || (v[y + 1].size() <= x) || (v[y + 1][x] == 1))
						ret->set(1, com_num++, element(false, 1, walls));
					else
					{
						++y;
						if (v[y][x] == 2)
							ret->set(1, com_num++, element(true, 0, exits));
						else
						{
							int away;
							for (away = y + 1; (away<v.size()) && (v[away].size()>x) && (v[away][x] == 0); away++);
							if ((away == v.size()) || (v[away].size() <= x) || (v[away][x] == 1))
								ret->set(1, com_num++, element(true, away - y, walls));
							else
								ret->set(1, com_num++, element(true, away - y, exits));
						}
					}
					print();
				}

				else if ((strlen(cur_com)) >= 2 && strncmp(cur_com, "JUMPDOWN", 8) == 0 && (*(cur_com + 8) == 0 || *(cur_com + 8) == ' ')) {
					if ((y == 0) || (v[y + 2].size() <= x) || (v[y + 2][x] == 1))
						ret->set(1, com_num++, element(false, 2, walls));
					else
					{
						if (v[y + 2][x] != 1)
							++y; ++y;
						if (v[y][x] == 2)
							ret->set(1, com_num++, element(true, 0, exits));
						else
						{
							int away;
							for (away = y + 2; (away >= 0) && (v[away].size()>x) && (v[away][x] == 0); away++);
							if ((away < 0) || (v[away].size() <= x) || (v[away][x] == 1))
								ret->set(1, com_num++, element(true, y - away, walls));
							else
								ret->set(1, com_num++, element(true, y - away, exits));
						}
					}
					print();
				}

				else if (strlen(cur_com) >= 4 && strncmp(cur_com, "LEFT", 4) == 0 && (*(cur_com + 4) == 0 || *(cur_com + 4) == ' '))
				{
					if ((x==0) || (v[y][x - 1] == 1))
						ret->set(1, com_num++, element(false, 1, walls));
					else
					{
						--x;
						if (v[y][x] == 2)
							ret->set(1, com_num++, element(true, 0, exits));
						else
						{
							int away;
							for (away = x - 1; (away >= 0) && (v[y][away] == 0); away--);
							if ((away < 0) || (v[y][away] == 1))
								ret->set(1, com_num++, element(true, x - away, walls));
							else
								ret->set(1, com_num++, element(true, x - away, exits));
						}
					}
					print();
				}

				else if ((strlen(cur_com)) >= 2 && strncmp(cur_com, "JUMPLEFT", 8) == 0 && (*(cur_com + 8) == 0 || *(cur_com + 8) == ' ')) {
					if ((x == 0) || (v[y][x - 2] == 1))
						ret->set(1, com_num++, element(false, 2, walls));
					else
					{
						if (v[y][x - 2] != 1)
							--x; --x;
						if (v[y][x] == 2)
							ret->set(1, com_num++, element(true, 0, exits));
						else
						{
							int away;
							for (away = x - 2; (away >= 0) && (v[y][away] == 0); away--);
							if ((away < 0) || (v[y][away] == 1))
								ret->set(1, com_num++, element(true, x - away, walls));
							else
								ret->set(1, com_num++, element(true, x - away, exits));
						}
					}
					print();
				}

				else if (strlen(cur_com) >= 5 && strncmp(cur_com, "RIGHT", 5) == 0 && (*(cur_com + 5) == 0 || *(cur_com + 5) == ' '))
				{
					if ((x >= v[y].size() - 1) || (v[y][x + 1] == 1))
						ret->set(1, com_num++, element(false, 1, walls));
					else
					{
						++x;
						if (v[y][x] == 2)
							ret->set(1, com_num++, element(true, 0, exits));
						else
						{
							int away;
							for (away = x + 1; (away<v[y].size()) && (v[y][away] == 0); away++);
							if ((away == v[y].size()) || (v[y][away] == 1))
								ret->set(1, com_num++, element(true, away - x, walls));
							else
								ret->set(1, com_num++, element(true, away - x, exits));
						}
					}
					print();
				}

				else if ((strlen(cur_com)) >= 2 && strncmp(cur_com, "JUMPRIGHT", 9) == 0 && (*(cur_com + 9) == 0 || *(cur_com + 9) == ' ')) {
					if ((x == 0) || (v[y][x + 2] == 1))
						ret->set(1, com_num++, element(false, 2, walls));
					else
					{
						if (v[y][x + 2] != 1)
							++x; ++x;
						if (v[y][x] == 2)
							ret->set(1, com_num++, element(true, 0, exits));
						else
						{
							int away;
							for (away = x + 2; (away >= 0) && (v[y][away] == 0); away++);
							if ((away < 0) || (v[y][away] == 1))
								ret->set(1, com_num++, element(true, x - away, walls));
							else
								ret->set(1, com_num++, element(true, x - away, exits));
						}
					}
					print();
				}

				else if (strlen(cur_com) >= 6 && strncmp(cur_com, "LOOKUP", 6) == 0 && (*(cur_com + 6) == 0 || *(cur_com + 6) == ' '))
				{
					int away;
					for (away = y - 1; (away >= 0) && (v[away].size()>x) && (v[away][x] == 0); away--);
					if ((away<0) || (v[away].size() <= x) || (v[away][x] == 1))
						ret->set(1, com_num++, element(true, y - away, walls));
					else
						ret->set(1, com_num++, element(true, y - away, exits));
				}
				else if (strlen(cur_com) >= 8 && strncmp(cur_com, "LOOKDOWN", 8) == 0 && (*(cur_com + 8) == 0 || *(cur_com + 8) == ' '))
				{
					int away;
					for (away = y + 1; (away < v.size()) && (v[away].size()>x) && (v[away][x] == 0); away++);
					if ((away == v.size()) || (v[away].size() <= x) || (v[away][x] == 1))
						ret->set(1, com_num++, element(true, away - y, walls));
					else
						ret->set(1, com_num++, element(true, away - y, exits));
				}
				else if (strlen(cur_com) >= 8 && strncmp(cur_com, "LOOKLEFT", 8) == 0 && (*(cur_com + 8) == 0 || *(cur_com + 8) == ' '))
				{
					int away;
					for (away = x - 1; (away >= 0) && (v[y][away] == 0); away--);
					if ((away < 0) || (v[y][away] == 1))
						ret->set(1, com_num++, element(true, x - away, walls));
					else
						ret->set(1, com_num++, element(true, x - away, exits));
				}
				else if (strlen(cur_com) >= 9 && strncmp(cur_com, "LOOKRIGHT", 9) == 0 && (*(cur_com + 9) == 0 || *(cur_com + 9) == ' '))
				{
					int away;
					for (away = x + 1; (away < v[y].size()) && (v[y][away] == 0); away++);
					if ((away == v[y].size()) || (v[y][away] == 1))
						ret->set(1, com_num++, element(true, away - x, walls));
					else
						ret->set(1, com_num++, element(true, away - x, exits));
				}
				while (*cur_com != ' ' && *cur_com != 0) cur_com++;
			}
			//delete b;
		}
	return ret;
}

void field::print()
{
	int ic = 0, jc;
	for(std::vector<std::vector <int>>::const_iterator i = v.cbegin(); i != v.cend(); i++, ic++)
	{
		jc = 0;
		for(std::vector<int>::const_iterator j = i->cbegin();j!=i->cend();j++,jc++)
			if ((ic == y) && (jc == x))
				if(*j == 2)
					printf("Y");
				else
					printf("X");
			else if(*j == 1)
				printf("#");
			else if(*j == 2)
				printf("!");
			else
				printf(" ");
		printf("\n");
	}
}

field::~field(){}

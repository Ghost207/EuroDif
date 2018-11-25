#include <stdio.h>
#include <vector>
#include <conio.h>

using namespace std;

struct country
{
	bool is_finished;
	char name[255];
	int code;
	int xl;
	int yl;
	int xh;
	int yh;
	int time_of_finishing;
};

struct quantity
{
	int code;
	int value;
	int incoming;
};

struct country_pair
{
	int code1;
	int code2;
	int value;
};

class city
{
private:
	vector<quantity> quantities;
public:
	bool is_initialized;
	bool is_finished;
	int code_;
	city()
	{
		is_initialized = false;
		is_finished = false;
	}
	~city()
	{
		quantities.clear();
	}

	void init(int code, int number_of_countries)
	{
		code_ = code;
		quantity tmp_quantity;
		for(int i = 0; i < number_of_countries; i++)
		{
			tmp_quantity.code = i;
			tmp_quantity.value = 0;
			tmp_quantity.incoming = 0;
			if(code_ == i)
			{
				tmp_quantity.value = 1000000;
			}
			quantities.push_back(tmp_quantity);
		}
		is_initialized = true;
	}

	void send(city (&cities)[11][11], int y, int x)
	{
		int tmp_value = 0;
		unsigned int size = quantities.size();
		for (int i = 0; i < size; i++)
		{
			tmp_value = int(quantities[i].value / 1000);
			if(true == cities[y - 1][x].is_initialized)
			{
				cities[y - 1][x].receive(quantities[i].code, tmp_value);
				quantities[i].value -= tmp_value;
			}
			if(true == cities[y][x - 1].is_initialized)
			{
				cities[y][x - 1].receive(quantities[i].code, tmp_value);
				quantities[i].value -= tmp_value;
			}
			if(true == cities[y + 1][x].is_initialized)
			{
				cities[y + 1][x].receive(quantities[i].code, tmp_value);
				quantities[i].value -= tmp_value;
			}
			if(true == cities[y][x + 1].is_initialized)
			{
				cities[y][x + 1].receive(quantities[i].code, tmp_value);
				quantities[i].value -= tmp_value;
			}
		}
	}

	void receive(int code, int value)
	{
		unsigned int size = quantities.size();
		for (int i = 0; i < size; i++)
		{
			if(quantities[i].code == code)
			{
				quantities[i].incoming += value;
				return;
			}
		}
		printf("Wrong code");
		return;
	}

	void take()
	{
		unsigned int size = quantities.size();
		for (int i = 0; i < size; i++)
		{
			quantities[i].value += quantities[i].incoming;
			quantities[i].incoming = 0;
		}
	}

	void check()
	{
		unsigned int size = quantities.size();
		for (int i = 0; i < size; i++)
		{
			if(0 == quantities[i].value)
			{
				return;
			}
		}
		is_finished = true;
		return;
	}
};

void init_cities(country* countries, int number_of_countries, city (&cities)[11][11])
{
	for(int k = 0; k < number_of_countries; k++)
	{
		for(int i = countries[k].yl; i <= countries[k].yh; i++)
		{
			for(int j = countries[k].xl; j <= countries[k].xh; j++)
			{
				cities[i][j].init(countries[k].code, number_of_countries);
			}
		}
	}
	return;
}

int init_countries(FILE* input, country* countries, int number_of_countries)
{
	char buffer[254];
	int xl = 0;
	int xh = 0;
	int yl = 0;
	int yh = 0;
	for(int i = 0; i < number_of_countries; i++)
	{
		fscanf(input, "%s%d%d%d%d", buffer, &xl, &yl, &xh, &yh);
		if(xl < 1 || xl > xh || yl < 1 || yl > yh || yh > 10 || xh > 10)
		{
			return -1;
		}
		countries[i].code = i;
		countries[i].xl = xl;
		countries[i].yl = yl;
		countries[i].xh = xh;
		countries[i].yh = yh;
		countries[i].is_finished = false;
		strcpy(countries[i].name, buffer);
	}
	return 0;
}

void sort_countries(country* countries, int lenth)
{
	country tmp;
	for(int j = 0; j < lenth; j++)
	{
		for(int k = 0; k < lenth; k++)
		{
			if(strcmp(countries[j].name, countries[k].name)<0)
			{
				tmp = countries[j];
				countries[j] = countries[k];
				countries[k] = tmp;
			}
		}
	}
	for(int i = 1; i < lenth; i++)
	{
		for(int j = i; j > 0 && countries[j - 1].time_of_finishing > countries[j].time_of_finishing; j--)
		{
			country tmp = countries[j - 1];
			countries[j - 1] = countries[j];
			countries[j] = tmp;
		}
	}
	return;
}

void send_quantities(country* countries, int number_of_countries, city (&cities)[11][11])
{
	for(int k = 0; k < number_of_countries; k++)
	{
		for(int i = countries[k].yl; i <= countries[k].yh; i++)
		{
			for(int j = countries[k].xl; j <= countries[k].xh; j++)
			{
				cities[i][j].send(cities, i, j);
			}
		}
	}
	return;
}

void take_quantities(country* countries, int number_of_countries, city (&cities)[11][11])
{
	for(int k = 0; k < number_of_countries; k++)
	{
		for(int i = countries[k].yl; i <= countries[k].yh; i++)
		{
			for(int j = countries[k].xl; j <= countries[k].xh; j++)
			{
				cities[i][j].take();
			}
		}
	}
	return;
}

void do_case(country* countries, int number_of_countries)
{
	int timer_of_days = 0;	
	bool all_done = false;
	bool country_done = false;
	city cities[11][11];

	init_cities(countries, number_of_countries, cities);
	while(!all_done)
	{
		all_done = true;
		for(int k = 0; k < number_of_countries; k++)
		{
			country_done = true;
			for(int i = countries[k].yl; i <= countries[k].yh; i++)
			{
				for(int j = countries[k].xl; j <= countries[k].xh; j++)
				{
					if(false == cities[i][j].is_finished)
					{
						cities[i][j].check();
					}
					country_done &= cities[i][j].is_finished;
				}
			}
			if(true == country_done && false == countries[k].is_finished)
			{
				countries[k].time_of_finishing = timer_of_days;
				countries[k].is_finished = true;
			}
			all_done &= country_done;
		}
		if(true == all_done)
		{
			break;
		}
		send_quantities(countries, number_of_countries, cities);
		take_quantities(countries, number_of_countries, cities);
		timer_of_days++;
	}
	return;
}

void print_countries(country* countries, int number_of_countries, int case_number)
{
	printf("\nCase Number %d\n", case_number);
	for(int i = 0; i < number_of_countries; i++)
	{
		printf("%s %d\n", countries[i].name, countries[i].time_of_finishing);
	}
	return;
}

void do_pairs(vector<country_pair> &country_pairs, int index)
{
	int lenth = country_pairs.size();
	for(int i = 0; i < lenth; i++)
	{
		if(index != i)
		{
			if(country_pairs[i].value != 0)
			{
				if((country_pairs[index].code1 == country_pairs[i].code1 || 
					country_pairs[index].code2 == country_pairs[i].code2 ||
					country_pairs[index].code2 == country_pairs[i].code1 ||
					country_pairs[index].code1 == country_pairs[i].code2) && country_pairs[index].value == 0)
				{
					country_pairs[i].value = 0;
					do_pairs(country_pairs, 0);
				}
			}
		}
	}
	return;
}

bool all_countries_in_pairs(vector<country_pair> country_pairs, country* countries, int number_of_countries)
{
	bool in_pair = true;
	int lenth = country_pairs.size();
	for(int i = 0; i < number_of_countries; i++)
	{
		in_pair = false;
		for(int j = 0; j < lenth; j++)
		{
			if(country_pairs[j].code1 == countries[i].code || country_pairs[j].code2 == countries[i].code)
			{
				in_pair = true;
				break;
			}
		}
		if(!in_pair)
		{
			return false;
		}
	}
	return true;
}

bool countries_not_intersect(country* countries, int number_of_countries)
{
	if(1 == number_of_countries)
	{
		return true;
	}
	for(int i = 0; i < number_of_countries; i++)
	{
		for(int j = 0; j < number_of_countries; j++)
		{
			if(i != j)
			{
				if(countries[i].xl >= countries[j].xl && countries[i].yl >= countries[j].yl && 
					(countries[i].xl <= countries[j].xh && countries[i].yl <= countries[j].yh))
				{
					return false;
				}
			}
		}
	}
	return true;
}

void create_pairs(vector<country_pair> &country_pairs, country* countries, int number_of_countries)
{
	country_pair tmp;
	for(int i = 0; i < number_of_countries - 1; i++)
	{
		for(int j = i + 1; j < number_of_countries; j++)
		{

				if((countries[i].xl == (countries[j].xh + 1) && countries[i].yl <= countries[j].yh) ||
				(countries[i].xl <= countries[j].xh && countries[i].yl == (countries[j].yh + 1)) ||
				(countries[j].xl == (countries[i].xh + 1) && countries[j].yl <= countries[i].yh) ||
				(countries[j].xl <= countries[i].xh && countries[j].yl == (countries[i].yh + 1)))
				{
					tmp.code1 = countries[i].code;
					tmp.code2 = countries[j].code;
					tmp.value = -1;
					country_pairs.push_back(tmp);
				}
			
		}
	}
}

bool check_countries(country* countries, int number_of_countries)
{
	if(!countries_not_intersect(countries, number_of_countries))
	{
		return false;
	}
	vector<country_pair> country_pairs;
	create_pairs(country_pairs, countries, number_of_countries);
	if(country_pairs.empty())
	{
		return false;
	}
	if(!all_countries_in_pairs(country_pairs, countries, number_of_countries))
	{
		return false;
	}
	country_pairs[0].value = 0;
	do_pairs(country_pairs, 0);
	int lenth = country_pairs.size();
	for(int i = 0; i < lenth; i++)
	{
		if(country_pairs[i].value != 0)
		{
			country_pairs.clear();
			return false;
		}
	}
	country_pairs.clear();
	return true;
}

int main()
{
	int number_of_countries = 0;
	int case_counter = 0;

	FILE* input = fopen("input.txt", "r");
	if (input == NULL)
	{
		return -1;
	}
	while (true)
	{
		case_counter++;
		fscanf(input,  "%d", &number_of_countries);
		if(20 < number_of_countries)
		{
			printf("Too many countries");
			break;
		}
		if(0 == number_of_countries)
		{
			break;
		}
		country *countries = new country[number_of_countries]; 
		if(-1 == init_countries(input, countries, number_of_countries))
		{
			printf("This coordinates is not allowed");
			break;
		}
		if(!check_countries(countries, number_of_countries))
		{
			printf("Wrong coordinates");
			break;
		}
		do_case(countries, number_of_countries);
		sort_countries(countries, number_of_countries);
		print_countries(countries,number_of_countries, case_counter);
		delete[] countries;
	}
	fclose(input);	
	getch();
	return 0;
}

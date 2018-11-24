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

void init_countries(FILE* input, country* countries, int number_of_countries)
{
	char buffer[254];
	int xl = 0;
	int xh = 0;
	int yl = 0;
	int yh = 0;
	for(int i = 0; i < number_of_countries; i++)
		{
			fscanf(input, "%s%d%d%d%d", buffer, &xl, &yl, &xh, &yh);
			countries[i].code = i;
			countries[i].xl = xl;
			countries[i].yl = yl;
			countries[i].xh = xh;
			countries[i].yh = yh;
			countries[i].is_finished = false;
			strcpy(countries[i].name, buffer);
		}
}

void sort_countries(country* countries, int lenth)
{
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
		if(0 == number_of_countries)
		{
			break;
		}
		country *countries = new country[number_of_countries]; 
		init_countries(input, countries, number_of_countries);
		do_case(countries, number_of_countries);
		sort_countries(countries, number_of_countries);
		print_countries(countries,number_of_countries, case_counter);
		delete[] countries;
	}
	fclose(input);	
	getch();
	return 0;
}

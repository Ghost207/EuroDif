#include <stdio.h>
#include <vector>
#include <regex>
#include <cctype>

using namespace std;

const int start_value_of_motif = 1000000, representative_part = 1000;

struct country
{
	bool is_finished;
	char name[26];
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

class City
{
private:
	vector<quantity> quantities;

	void send_to_neighbor(City (&cities)[11][11], int y, int x, int value, int index_of_country, int dy, int dx)
	{
		if(cities[y + dy][x + dx].is_initialized)
		{
			cities[y + dy][x + dx].receive(quantities[index_of_country].code, value);
			quantities[index_of_country].value -= value;
		}
	}
public:
	bool is_initialized;
	bool is_finished;
	int code;
	City()
	{
		is_initialized = false;
		is_finished = false;
	}
	~City()
	{
		quantities.clear();
	}

	void init(int code_, int number_of_countries)
	{
		code = code_;
		quantity tmp_quantity;
		for(int i = 0; i < number_of_countries; ++i)
		{
			tmp_quantity.code = i;
			tmp_quantity.value = 0;
			tmp_quantity.incoming = 0;
			if(code == i)
			{
				tmp_quantity.value = start_value_of_motif;
			}
			quantities.push_back(tmp_quantity);
		}
		is_initialized = true;
	}

	void send(City (&cities)[11][11], int y, int x)
	{
		int tmp_value = 0;
		unsigned int size = quantities.size();
		for (int i = 0; i < size; ++i)
		{
			tmp_value = int(quantities[i].value / representative_part);
				send_to_neighbor(cities, y, x, tmp_value, i, -1, 0);
				send_to_neighbor(cities, y, x, tmp_value, i, 0, -1);
				send_to_neighbor(cities, y, x, tmp_value, i, 1, 0);
				send_to_neighbor(cities, y, x, tmp_value, i, 0, 1);
		}
	}

	void receive(int code, int value)
		for(auto& quantity : quantities)
		{
			if (quantity.code == code)
			{
				quantity.incoming += value;
				return;
			}
		}
		printf("Wrong code");
	}

	void take()
	{
		for(auto& quantity : quantities)
		{
			quantity.value += quantity.incoming;
			quantity.incoming = 0;
		}
	}

	void check()
	{
		for(auto& quantity : quantities)
		{
			if(0 == quantity.value)
			{
				return;
			}
		}
		is_finished = true;
	}
};

void init_cities(country* countries, int number_of_countries, City (&cities)[11][11])
{
	for(int k = 0; k < number_of_countries; ++k)
	{
		for(int i = countries[k].yl; i <= countries[k].yh; ++i)
		{
			for(int j = countries[k].xl; j <= countries[k].xh; ++j)
			{
				cities[i][j].init(countries[k].code, number_of_countries);
			}
		}
	}
}

bool init_countries(FILE* input, country* countries, int number_of_countries)
{
	char buffer[254];
	int xl = 0;
	int xh = 0;
	int yl = 0;
	int yh = 0;
	for(int i = 0; i < number_of_countries; ++i)
	{
		fscanf(input, "%s%d%d%d%d", buffer, &xl, &yl, &xh, &yh);
		if(xl < 1 || xl > xh || yl < 1 || yl > yh || yh > 10 || xh > 10)
		{
			return false;
		}
		countries[i].code = i;
		countries[i].xl = xl;
		countries[i].yl = yl;
		countries[i].xh = xh;
		countries[i].yh = yh;
		countries[i].is_finished = false;
		strcpy(countries[i].name, buffer);
	}
	return true;
}

void sort_countries(country* countries, int lenth)
{
	country tmp;
	for(int j = 0; j < lenth; ++j)
	{
		for(int k = 0; k < lenth; ++k)
		{
			if(strcmp(countries[j].name, countries[k].name) < 0)
			{
				tmp = countries[j];
				countries[j] = countries[k];
				countries[k] = tmp;
			}
		}
	}
	for(int i = 1; i < lenth; ++i)
	{
		for(int j = i; j > 0 && countries[j - 1].time_of_finishing > countries[j].time_of_finishing; --j)
		{
			country tmp = countries[j - 1];
			countries[j - 1] = countries[j];
			countries[j] = tmp;
		}
	}
}

void take_quantities(City (&cities)[11][11], int i, int j)
{
	cities[i][j].take();
}

void send_quantities(City (&cities)[11][11], int i, int j)
{
	cities[i][j].send(cities, i, j);
}

void actions_with_quantities(country* countries, int number_of_countries, City (&cities)[11][11], void (*func)(City (&)[11][11], int, int))
{
	for(int k = 0; k < number_of_countries; ++k)
	{
		for(int i = countries[k].yl; i <= countries[k].yh; ++i)
		{
			for(int j = countries[k].xl; j <= countries[k].xh; ++j)
			{
				(func)(cities, i, j);
			}
		}
	}
}

void do_case(country* countries, int number_of_countries)
{
	int timer_of_days = 0;	
	bool all_done = false;
	bool country_done = false;
	City cities[11][11];

	init_cities(countries, number_of_countries, cities);
	while(!all_done)
	{
		all_done = true;
		for(int k = 0; k < number_of_countries; ++k)
		{
			country_done = true;
			for(int i = countries[k].yl; i <= countries[k].yh; ++i)
			{
				for(int j = countries[k].xl; j <= countries[k].xh; ++j)
				{
					if(!cities[i][j].is_finished)
					{
						cities[i][j].check();
					}
					country_done &= cities[i][j].is_finished;
				}
			}
			if(country_done && !countries[k].is_finished)
			{
				countries[k].time_of_finishing = timer_of_days;
				countries[k].is_finished = true;
			}
			all_done &= country_done;
		}
		if(all_done)
		{
			break;
		}
		actions_with_quantities(countries, number_of_countries, cities, send_quantities);
		actions_with_quantities(countries, number_of_countries, cities, take_quantities);
		timer_of_days++;
	}
}

void print_countries(country* countries, int number_of_countries, int case_number)
{
	printf("\nCase Number %d\n", case_number);
	for(int i = 0; i < number_of_countries; ++i)
	{
		printf("%s %d\n", countries[i].name, countries[i].time_of_finishing);
	}
}

void do_pairs(vector<country_pair> &country_pairs, int index)
{
	int lenth = country_pairs.size();
	for(int i = 0; i < lenth; ++i)
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
					do_pairs(country_pairs, i);
				}
			}
		}
	}
}

bool all_countries_in_pairs(vector<country_pair> country_pairs, country* countries, int number_of_countries)
{
	bool in_pair = true;
	int lenth = country_pairs.size();
	for(int i = 0; i < number_of_countries; ++i)
	{
		in_pair = false;
		for(int j = 0; j < lenth; ++j)
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
	for(int i = 0; i < number_of_countries - 1; ++i)
	{
		for(int j = i + 1; j < number_of_countries; ++j)
		{
			if((countries[i].xl >= countries[j].xl && countries[i].yl >= countries[j].yl && 
					countries[i].xl <= countries[j].xh && countries[i].yl <= countries[j].yh) ||
					countries[j].xl >= countries[i].xl && countries[j].yl >= countries[i].yl && 
					countries[j].xl <= countries[i].xh && countries[j].yl <= countries[i].yh)
			{
				return false;
			}
		}
	}
	return true;
}

bool check_right_and_above_from(country* countries, int i, int j) // for user, above is bellow, because y inverse in matrix
{
	bool on_right = countries[i].xl == (countries[j].xh + 1) && countries[i].yl <= countries[j].yh && countries[j].yl <= countries[i].yh;
	bool on_above = countries[i].xl <= countries[j].xh && countries[i].yl == (countries[j].yh + 1) && countries[j].xl <= countries[i].xh;
	return (on_right || on_above);
}

void create_pairs(vector<country_pair> &country_pairs, country* countries, int number_of_countries)
{
	country_pair tmp;
	for(int i = 0; i < number_of_countries - 1; ++i)
	{
		for(int j = i + 1; j < number_of_countries; ++j)
		{
			if(check_right_and_above_from(countries, i, j) || check_right_and_above_from(countries, j, i))
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
	if(1 == number_of_countries)
	{
		return true;
	}
	if(!countries_not_intersect(countries, number_of_countries))
	{
		printf("\nIntersection of countries\n");
		return false;
	}
	vector<country_pair> country_pairs;
	create_pairs(country_pairs, countries, number_of_countries);
	if(country_pairs.empty())
	{
		printf("\nUnreachable countries exist\n");
		return false;
	}
	if(!all_countries_in_pairs(country_pairs, countries, number_of_countries))
	{
		printf("\nUnreachable countries exist\n");
		return false;
	}
	country_pairs[0].value = 0;
	do_pairs(country_pairs, 0);
	int lenth = country_pairs.size();
	for(int i = 0; i < lenth; ++i)
	{
		if(country_pairs[i].value != 0)
		{
			printf("\nUnreachable countries exist\n");
			country_pairs.clear();
			return false;
		}
	}
	country_pairs.clear();
	return true;
}

bool skip_empty_lines(char* buffer, FILE* input)
{
	while(buffer[0] == '\n')
	{
		if (NULL == fgets(buffer, 255, input))
		{
			return false;
		}
	}
	return true;
}

bool number_of_inputs_correct(char* buffer)
{
	int n = strlen(buffer);
	for(int i = 0; i < n; ++i)
	{
		if(!isdigit(buffer[i]))
		{
			if(buffer[i] != '\n' && buffer[i] != '\t' && buffer[i] != ' ')
			{
				return false;
			}
		}
	}
	return true;
}

bool counties_description_is_correct(FILE* input, char* buffer, int number_of_inputs)
{
	regex base_regex("^[ \f\n\r\t\v]{0,}[A-Z][a-z]{0,24}[ \f\n\r\t\v]{1,}[0-9]{1,2}[ \f\n\r\t\v]{1,}"
		"[0-9]{1,2}[ \f\n\r\t\v]{1,}[0-9]{1,2}[ \f\n\r\t\v]{1,}[0-9]{1,2}[ \f\n\r\t\v]{0,}$");
	smatch base_match;
	string string_buffer;
	for(int i = 0; i < number_of_inputs; ++i)
	{
		if (NULL == fgets(buffer, 999, input) || !skip_empty_lines(buffer, input))
		{
			fclose(input);
			return false;
		}
		string_buffer.clear();
		string_buffer.append(buffer);
		if (!regex_match(string_buffer, base_match, base_regex))
		{
			fclose(input);
			return false;
		}
	}
	return true;
}

bool input_is_correct(string name)
{
	int number_of_inputs = -1;
	char buffer[1000];
	FILE* input = fopen(name.c_str(), "r");
	if (input == NULL)
	{
		return -1;
	}
	while(0 != number_of_inputs)
	{
		if(NULL == fgets(buffer, 255, input) || !skip_empty_lines(buffer, input) || !number_of_inputs_correct(buffer))
		{
			printf("Wrong input pattern");
			fclose(input);
			return false;
		}
		number_of_inputs = atoi(buffer);
		if(!counties_description_is_correct(input, buffer, number_of_inputs))
		{
			printf("Wrong input pattern");
			fclose(input);
			return false;
		}
	}
	fclose(input);	
	return true;
}

bool countries_are_unique(country* countries, int number_of_countries)
{
	string tmp_name1, tmp_name2;
	for(int i = 0; i < number_of_countries; ++i)
	{
		for(int j = i + 1; j < number_of_countries - 1; ++j)
		{
			tmp_name1 = countries[i].name;
			tmp_name2 = countries[j].name;
			if(tmp_name1.compare(tmp_name2) == 0)
			{
				return false;
			}
		}
	}
	return true;
}

int main()
{
	int number_of_countries = 0;
	int case_counter = 0;
	if(false == input_is_correct("input.txt"))
	{
		printf("\n");
		system("pause");
		return -1;
	}
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
		if(number_of_countries > 20 || number_of_countries < 0)
		{
			printf("\nWrong number of countries");
		}
		else
		{
			country *countries = new country[number_of_countries];
			if(!init_countries(input, countries, number_of_countries))
			{
				printf("\nThis coordinates is not allowed");
			}
			else
			{
				if(!countries_are_unique(countries, number_of_countries))
				{
						printf("\nNames of countries must be unique");
				}
				else
				{
					if(!check_countries(countries, number_of_countries))
					{
						printf("\nWrong coordinates");
					}
					else
					{
						do_case(countries, number_of_countries);
						sort_countries(countries, number_of_countries);
						print_countries(countries,number_of_countries, case_counter);
					}
				}
			}
			delete[] countries;
		}		
	}
	fclose(input);
	printf("\n");
	system("pause");
	return 0;
}

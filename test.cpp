#include <iostream>
#include "train.h"

int main()
{
	timetable tt;
	std::fstream fin("train.txt");
	if (!fin.is_open())
	{
		std::cout << "This file cannot be opened";
	}
	else
	{
		while (fin.good())
		{
			fin >> tt;
		}
	}
	fin.close();
	std::cout << tt;
	system("pause");
	return(0);
}
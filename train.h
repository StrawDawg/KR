#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <algorithm>

struct train
{
	std::string departure_city;
	tm departure_date;
	std::string arrival_city;
	tm arrival_date;
	int tickets_left;
	int capacity;

	bool book_ticket();
};

class timetable
{
	std::vector<train> list_;
	int voyages_count;

public:

	timetable() = default;
	timetable(timetable const&) = delete;
	timetable(timetable&&) = delete;
	train& operator[](unsigned index);

	friend std::ostream& operator<<(std::ostream& out, timetable const& t);
	friend std::istream& operator>>(std::istream& in, timetable& t);

	void sort_by_date();
};
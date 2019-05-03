#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <immintrin.h>

struct event
{
	std::string name;
	unsigned int left;
	unsigned int total;
	tm time;
};

class agency
{
	std::vector<event> events;

public:
	agency() = default;
	agency(agency const&) = delete;
	agency(agency&&) = delete;

	event operator[](int) const;
	int num() const;
	void sort_by_date();
	void sort_by_name();
	void book(unsigned int, unsigned int);
	std::vector<event>::iterator begin();
	std::vector<event>::iterator end();
	friend std::ostream& operator<< (std::ostream& out, const agency& a);
	friend std::istream& operator>> (std::istream& in, agency& a);
};

std::ostream& operator<< (std::ostream& out, const agency& a);
std::istream& operator>> (std::istream& in, agency& a);
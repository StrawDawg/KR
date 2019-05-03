#include "agency.h"
#include <iostream>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <sstream>

event agency::operator[](int i) const
{
	return events[i];
}

int agency::num() const
{
	return events.size();
}

void agency::sort_by_date()
{
	std::sort(events.begin(), events.end(), [](event e1, event e2) -> bool
	{
		return mktime(&(e1.time)) < mktime(&(e2.time));
	});

}

void agency::sort_by_name()
{
	std::sort(events.begin(), events.end(), [](const event e1, const event e2) -> bool
	{
		return e1.name < e2.name;
	});
}

void agency::book(unsigned int const ind, unsigned int const num)
{
	try
	{
		if (events[ind - 1].left > num)
		{
			events[ind - 1].left--;
		}
		else
		{
			throw std::length_error("No tickets left!");
		}
	}
	catch (std::length_error &e)
	{
		std::cout << e.what() << std::endl;
	}
}

std::istream& operator>>(std::istream& in, agency& a)
{
	std::string line;
	std::getline(in, line);

	std::string subs[4];

	for (auto i = 0; i < 3; i++)
	{
		const auto pos = line.find(';');
		subs[i] = line.substr(0, pos);
		line.erase(0, pos + 1);
	}
	subs[3] = line;

	event e;
	e.name = subs[0];
	e.total = std::stoi(subs[1]);
	e.left = std::stoi(subs[2]);
	std::stringstream ss(subs[3]);
	ss >> std::get_time(&(e.time), "%Y-%m-%d %H:%M");

	a.events.push_back(e);

	return in;
}

std::ostream& operator<<(std::ostream& out, agency const& a)
{
	for (const event& e : a.events)
	{
		out << e.name << " " << e.left << "/" << e.total << " " << std::put_time(&(e.time), "%b %d %Y %H:%M") << "\n";
	}
	return out;
}

std::vector<event>::iterator agency::begin()
{
	return events.begin();
}

std::vector<event>::iterator agency::end()
{
	return events.end();
}
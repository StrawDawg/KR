#include "train.h"

bool train::book_ticket()
{
	if (tickets_left == 0)
	{
		return false;
	}
	else
	{
		tickets_left--;

		return true;
	}
}

train& timetable::operator[](unsigned index)
{
	if (index >= list_.size() || index < 0)
	{
		throw std::out_of_range("Invalid index");
	}
	return list_[index];
}

std::ostream& operator<<(std::ostream& out, timetable const& t)
{
	for (auto i = 0; i < t.voyages_count; i++)
	{
		out << t.list_[i].departure_city << ";" <<
			std::put_time(&(t.list_[i].departure_date), "%b %d %Y %H:%M") << ";"
			<< t.list_[i].arrival_city << ";" <<
			std::put_time(&(t.list_[i].arrival_date), "%b %d %Y %H:%M") << ";" <<
			t.list_[i].capacity << ";" <<
			t.list_[i].tickets_left << std::endl;
	}
	return out;
}

std::istream& operator>> (std::istream& in, timetable& t)
{
	std::string line;
	std::getline(in, line);
	size_t pos = 0;
	std::string parts[6];

	for (auto& part : parts)
	{
		pos = line.find(';');
		part = line.substr(0, pos);
		line.erase(0, pos + 1);
	}

	train tr;
	tr.departure_city = parts[0];
	tr.arrival_city = parts[2];
	tr.capacity = std::stoi(parts[4]);
	tr.tickets_left = std::stoi(parts[5]);
	std::stringstream s1(parts[1]);
	std::stringstream s2(parts[3]);
	s1 >> std::get_time(&(tr.departure_date), "%Y-%m-%d %H:%M");
	s2 >> std::get_time(&(tr.arrival_date), "%Y-%m-%d %H:%M");
	
	t.list_.push_back(tr);
	t.voyages_count++;

	return in;
}

void timetable::sort_by_date()
{
	std::sort(list_.begin(), list_.end(), [](train tr1, train tr2) -> bool
	{
		return mktime(&tr1.departure_date) < mktime(&tr2.departure_date);
	});
}
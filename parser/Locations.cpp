#include "Locations.hpp"
#include <stack>

Locations::Locations()
{
}

Locations::~Locations() {}

Locations::Locations(const Locations& loc)
{
	_v = loc._v;
	_blank = loc._blank;
}

void Locations::operator = (const Locations& loc)
{
	_v = loc._v;
	_blank = loc._blank;
}

unsigned long Locations::size()
{
	return (_v.size());
}

Location& Locations::operator [] (unsigned long i)
{
	return (_v[i]);
}

Location& Locations::get_loc(std::string name)
{
	for (unsigned long i = 0; i < _v.size(); i++)
	{
		if (name == _v[i].get_name())
			return (_v[i]);
	}
	return (_blank);
}

std::string Locations::get_path(std::string name)
{
	for (unsigned long i = 0; i < _v.size(); i++)
	{
		if (name == _v[i].get_name())
			return (_v[i].get_path());
	}
	return ("");
}

void Locations::push_back(Location loc)
{
	_v.push_back(loc);
}

void Locations::print()
{
	std::cout << "----------Locations:-----------\n";
	for (unsigned long i = 0; i < _v.size(); i++)
	{
		_v[i].print();
	}
	_blank.print();
}

static void stack_url(std::stack<std::string>& urls, std::string b)
{
	unsigned long i = b.find_first_of('/');

	while (1)
	{
		if (i != b.npos)
			urls.push(b.substr(0, i + 1));
		else
		{
			urls.push(b);
			break ;
		}
		i = b.find_first_of('/', i + 1);
	}
}

Location& Locations::get_loc_by_url(std::string url)
{
	std::stack<std::string> urls;

	stack_url(urls, url);

	std::string last = urls.top();

	while (urls.size())
	{
		for (unsigned long i = 0; i < _v.size(); i++)
		{
			if (urls.top()[urls.top().size() - 1] != '/')
				urls.top().push_back('/');
			if (urls.top() == _v[i].get_name())
				return (_v[i]);
		}
		urls.pop();
	}
	return (_blank);
}

void Locations::check_path_validity()
{
	for (unsigned long i = 0; i < size(); i++)
	{
		_v[i].check_path_validity();
	}
}

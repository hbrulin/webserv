#include "Locations.hpp"
#include <stack>

// mettre root

Locations::Locations()
{
}

Locations::~Locations() {}

Locations::Locations(const Locations& locs)
{
	_v = locs._v;
	_blank = locs._blank;
}

void Locations::operator = (const Locations& locs)
{
	_v = locs._v;
	_blank = locs._blank;
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
}

static void stack_url(std::stack<std::string>& urls, std::string b)
{
	unsigned long i = b.find_first_of('/');

	while (1)
	{
		//std::cout << i << std::endl;
		if (i != b.npos)
		{
			urls.push(b.substr(0, i + 1));
			//std::cout << urls.top() << std::endl;
		}
		else
		{
			urls.push(b);
			//std::cout << urls.top() << std::endl;
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

/*	if (last.find('.') != last.npos) // if true, we got a file
	{
		last = last.substr(last.find('.'));
		//std::cout << "llaaa: " << last << std::endl;
		for (unsigned long i = 0; i < _v.size(); i++)
		{
			if (last == _v[i].get_name())
				return (_v[i]);
		}
	}*/

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
	//std::cout << "blank" << std::endl;
	return (_blank);
}

#include "Data.hpp"
#include "listener.hpp"

int main (int ac, char **av) {
	std::string conf;

	//parse config
	try
	{
		Data data(av[1]);
		int size = data.getSize();
		int i = 0;
		std::vector<Listener> serverList(size);

		while (i < size)
		{
			serverList[i] = Listener(data.getConfigList()[i]);
			serverList[i].init();
			serverList[i].run();
			i++;
		}
	}
	catch (std::logic_error& e)
	{
		std::cout << e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Error while configuring, abort ...\n";
		return -1;
	}

	//server.clean(); //jamais utilisé vu que ctrl-C?

	return 0;
}
#include "Data.hpp"
#include "listener.hpp"

int main (int ac, char **av) {
	int pid;
	int status;
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
			//std::cout << "test" << i << std::endl;
			serverList[i].run();
			i++;
		}
		//server.clean(); //jamais utilisé vu que ctrl-C?
		//free
	}
	catch (std::logic_error& e)
	{
		std::cout << e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Error while configuring, abort ...\n"; //ou autres erreurs
		return -1;
	}

	

	return 0;
}
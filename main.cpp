#include "Data.hpp"
#include "listener.hpp"

int main (int ac, char **av) {
	try
	{
		Data data(av[1]);
		int size = data.getSize();
		Listener server(data.getConfigList(), size);
		server.init();
		server.run();
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
#include "Data.hpp"
#include "listener.hpp"

int main (int ac, char **av) {
	int pid;
	int status;
	try
	{
		Data data(av[1]);
		Listener server(data);

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
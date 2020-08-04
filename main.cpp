#include "parser/Data.hpp"
#include "listener/listener.hpp"

int main (int ac, char **av) {
	(void)ac;
	try
	{
		Data data(av[1]);
		int size = data.getSize();
		std::cout << "-1\n";
		Listener server(data.getConfigList(), size);
		std::cout << "0\n";
		server.init();
		std::cout << "1\n";
		server.run();
		std::cout << "2\n";
		//server.clean(); //jamais utilisé vu que ctrl-C?
		//free
	}
	catch (std::logic_error& e)
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}

	return 0;
}

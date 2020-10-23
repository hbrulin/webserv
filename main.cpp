#include "parser/Data.hpp"
#include "listener/listener.hpp"

int main (int ac, char **av) {
	(void)ac;
	try
	{
		Data data(av[1]);
		int size = data.getSize();
		Listener server(data.getConfigList(), size);
		//std::cout << "HERE\n" << data.getConfigList()[0]._locations[2].get_autoindex();
		server.init();
		server.run();
	}
	catch (std::logic_error& e)
	{
		std::cerr << e.what() << std::endl;
		return -1;
	}

	return 0;
}

#include "listener.hpp"

int main (void) {

	Listener server;

	//parse config for init();
	
	server.init();
	server.run();

	return 0;
}
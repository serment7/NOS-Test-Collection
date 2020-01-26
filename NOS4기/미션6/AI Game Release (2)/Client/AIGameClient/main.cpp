#include <string>
#include "Connector.h"

int main( int argc, char* argv[] ) {

	std::string teamName = "TestAI1";
	std::string ip = "127.0.0.1";
	int port = 12719;
	Connector connector(teamName, ip, port);
	connector.Ready();

	while(true)
	{
		//Receive game information from server,,
		GameInfo gameInfo = connector.Update();

		//TODO: Implement HERE.
	

		//Send your commands.
		connector.SendCommands();
	}

	return 0;
}
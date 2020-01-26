#include "Connector.h"
#include <string>

Connector::Connector(std::string teamName, std::string ip, int port) 
{
	if(!Connect(teamName, ip, port))
	{
		std::cout << "Connector Construct Fail" << std::endl;
		exit(0);
	}
	std::cout << "Connection OK" << std::endl;
}
Connector::~Connector() 
{
	if(s != 0)
	{
		closesocket(s);
	}
	WSACleanup();
}

bool Connector::Connect(std::string teamName, std::string ip, int port)
{
	if ( WSAStartup( MAKEWORD( 2, 2 ), &wsadata) != 0 )
	{
		std::cout << "Failed to initialize WinSock : " << WSAGetLastError() << std::endl;
		return false;
	}

	s = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	
	if( s == SOCKET_ERROR )
	{
		std::cout << "Socket Error!" << std::endl;
		return false;
	}

	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_port = htons( 12719 );
	addr.sin_addr.s_addr = inet_addr(ip.c_str());


	if ( connect( s, (SOCKADDR*)&addr, sizeof(addr) ) == -1 )
	{
		std::cout << "Connect Error!" << std::endl;
		return false;
	}

	this->teamName = teamName;

	return true;
}

GameInfo Connector::Update()
{
	char* buffer = new char[1024];
	int recvNumber = recv(s, buffer, 1024, 0);
	GameInfo gameInfo;

	if(recvNumber == SOCKET_ERROR)
	{
		exit(0);
	}
	else if(recvNumber == 0)
	{
		exit(0);
	}
	else if(recvNumber > 0)
	{
		for(int i=0; i<20; i++)
		{
			for(int j=0; j<20; j++)
			{
				gameInfo.Map[i][j] = (Element)(buffer[i * 20 + j] - '0');
			}
		}
	}
	
	//맵 구성 후 리턴
	return gameInfo;
}

void Connector::Move(int fromX, int fromY, int toX, int toY)
{
	Command cmd;
	cmd._type = Move_;
	cmd.fromX = fromX;
	cmd.fromY = fromY;
	cmd.toX = toX;
	cmd.toY = toY;

	commandList.push_back(cmd);
}
void Connector::PutDown(int fromX, int fromY, int toX, int toY)
{
	Command cmd;
	cmd._type = PutDown_;
	cmd.fromX = fromX;
	cmd.fromY = fromY;
	cmd.toX = toX;
	cmd.toY = toY;

	commandList.push_back(cmd);
}

void Connector::SendCommands()
{
	std::string cmdString = std::string("");
	for(std::vector<Command>::iterator iter = commandList.begin();
		iter != commandList.end(); iter++)
	{
		char cmdBuffer[100];
		sprintf(cmdBuffer, "<CMD>%d %d %d %d %d\n", static_cast<int>((*iter)._type), 
			(*iter).fromX, (*iter).fromY, (*iter).toX, (*iter).toY);
		cmdString += cmdBuffer;
	}
	
	cmdString += "<EOF>";

	int strLength = cmdString.length();
	for(int i=0; i<=(strLength+1)/1024; i++)
	{
		std::string sendString = cmdString.substr(i * 1024, (strLength - i * 1024) > 1024? 1024 : strLength - i * 1024);
		send(s, sendString.c_str(), sendString.length(), 0);
	}
	commandList.clear();
}

bool Connector::Ready()
{
	char* buf = new char[1024];
	std::string msg = "<Ready>"+teamName+"<EOF>";
	send(s, msg.c_str(), msg.length(), 0);
	recv(s, buf, 1024, 0);

	return true;
}
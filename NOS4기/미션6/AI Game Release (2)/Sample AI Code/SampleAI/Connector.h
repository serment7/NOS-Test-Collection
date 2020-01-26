#ifndef AIGAMECLIENT_CONNECTOR_H
#define AIGAMECLIENT_CONNECTOR_H

#include <iostream>
#include <WinSock2.h>
#include <vector>

#pragma comment ( lib, "ws2_32.lib" )
#pragma pack( 1 )

enum Element
{
	None = 0,
	NeutralRes,
	AllyRes,
	EnemyRes,
	AllyUnit,
	EnemyUnit,
	AllyUnitRes,
	EnemyUnitRes
};
enum CmdType
{
	Move_ = 0,
	PutDown_
};

typedef struct GameInfo
{
	Element Map[20][20];
	GameInfo()
	{
		for(int i=0; i<20; i++)
		{
			for(int j=0; j<20; j++)
			{
				Map[i][j] = None;
			}
		}
	}
}GameInfo;
typedef struct Command
{
	CmdType _type;
	int fromX, fromY, toX, toY;
	
	Command() : _type(Move_), fromX(0), fromY(0), toX(0), toY(0) {}
}Command;

class Connector
{
private:
	WSADATA wsadata;
	SOCKET s;
	std::string teamName;
	std::vector<Command> commandList;

	std::string getMapString(char* buffer);
	
public:

	Connector(std::string teamName, std::string ip, int port);
	virtual ~Connector();
	
	bool Connect(std::string teamName, std::string ip, int port);
	bool Ready();

	GameInfo Update();
	void Move(int fromX, int fromY, int toX, int toY);
	void PutDown(int fromX, int fromY, int toX, int toY);	
	void SendCommands();
};

#endif
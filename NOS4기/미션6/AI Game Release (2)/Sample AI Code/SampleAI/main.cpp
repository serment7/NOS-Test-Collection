#include <string>
#include "Connector.h"
#include <vector>

typedef struct Point
{
	int x, y;
	Point() : x(0), y(0) {}
	Point(int _x, int _y) : x(_x), y(_y) {}
} Point;

Point findResource(std::vector<Point>& resources, const Point& unitPosition);

int main( int argc, char* argv[] ) {

	std::string teamName = "NexonNexon";
	std::string ip = "127.0.0.1";
	int port = 12719;
	Connector connector(teamName, ip, port);
	connector.Ready();

	while(true)
	{
		//Receive game information from server
		GameInfo gameInfo = connector.Update();

		//TODO: Implement HERE.

		std::vector<Point> resources;
		std::vector<Point> ourResources;
		std::vector<Point> units;
		std::vector<Point> unitsRes;

		for(int i=0; i<20; i++)
		{
			for(int j=0; j<20; j++)
			{
				switch(gameInfo.Map[i][j])
				{
				case NeutralRes:
				case EnemyRes:
					resources.push_back(Point(j, i));
					break;
				case AllyRes:
					ourResources.push_back(Point(j, i));
					break;
				case AllyUnit:
					units.push_back(Point(j, i));
					break;
				case AllyUnitRes:
					unitsRes.push_back(Point(j, i));
					break;
				default:
					break;
				}
			}
		}

		for(std::vector<Point>::iterator iter = units.begin(); iter != units.end(); iter++)
		{
			Point nearResource = findResource(resources, (*iter));
			int dx = nearResource.x - (*iter).x;
			int dy = nearResource.y - (*iter).y;	

			if(dx > 0)
			{
				connector.Move(iter->x, iter->y, (iter->x)+1, iter->y);
				
			}
			else if(dx < 0)
			{
				connector.Move(iter->x, iter->y, (iter->x)-1, iter->y);
				
			}

			else if(dy < 0)
			{
				connector.Move(iter->x, iter->y, iter->x, (iter->y)-1);
				
			}

			else if(dy > 0)
			{
				connector.Move(iter->x, iter->y, iter->x, (iter->y)+1);
				
			}


		}

		for(std::vector<Point>::iterator iter = unitsRes.begin(); iter != unitsRes.end(); iter++)
		{
			Point nearHouse = findResource(ourResources, (*iter));
			bool flag = false;

			if(nearHouse.x == 0 && nearHouse.y == 0)
			{
				flag = true;
			}

			int dx = nearHouse.x - (*iter).x;
			int dy = nearHouse.y - (*iter).y;	

			if(dx >= 1 && std::abs(dx) + std::abs(dy) > 1 && !flag)
			{
				dx = dx > 0? 1 : -1;
				connector.Move(iter->x, iter->y, (iter->x)+dx, iter->y);
			}
			else if(dx <= -1 && std::abs(dx) + std::abs(dy) > 1 && !flag)
			{
				dx = dx > 0? 1 : -1;
				connector.Move(iter->x, iter->y, (iter->x)+dx, iter->y);
			}
			else if(dy >= 1 && std::abs(dx) + std::abs(dy) > 1 && !flag)
			{
				dy = dy > 0? 1 : -1;
				connector.Move(iter->x, iter->y, iter->x, (iter->y)+dy);
			}
			else if(dy <= 1 && std::abs(dx) + std::abs(dy) > 1 && !flag)
			{
				dy = dy > 0? 1 : -1;
				connector.Move(iter->x, iter->y, iter->x, (iter->y)+dy);
			}

			else if( std::abs(dx) + std::abs(dy) == 1 || flag)

			{
				//빈 공간을 찾자.
				if ((iter->x)+1 < 20 && gameInfo.Map[(iter->y)][(iter->x)+1] == None)
				{
					connector.PutDown(iter->x, iter->y, (iter->x)+1, iter->y);
				}
				else if((iter->x)-1 >= 0 && gameInfo.Map[iter->y][(iter->x)-1] == None)
				{
					connector.PutDown(iter->x, iter->y, (iter->x)-1, iter->y);
				}
				else if((iter->y)+1 < 20 && gameInfo.Map[(iter->y)-1][(iter->x)] == None)
				{
					connector.PutDown(iter->x, iter->y, iter->x, (iter->y)+1);
				}
				else
				{
					connector.PutDown(iter->x, iter->y, iter->x, (iter->y)-1);
				}



			}

		}

		//Send your commands.
		connector.SendCommands();
	}

	return 0;
}

Point findResource(std::vector<Point>& resources, const Point& unitPosition)
{
	int minDistance = 10000;
	Point minPosition;

	for(std::vector<Point>::iterator iter = resources.begin(); iter != resources.end(); iter++)
	{
		if(std::abs((*iter).x - unitPosition.x) + std::abs((*iter).y - unitPosition.y) < minDistance)
		{
			minDistance = std::abs((*iter).x - unitPosition.x) + std::abs((*iter).y - unitPosition.y);
			minPosition = Point((*iter).x, (*iter).y);
		}
	}

	return minPosition;
}
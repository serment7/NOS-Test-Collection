#include <stdio.h>
#include <list>

using namespace std;

void GetWinner(const int& PlayerNum, const int& Interval, const int& StartNum)
{
	list<int> Players;

	for (int Index = 1; Index <= PlayerNum; ++Index)
	{
		Players.push_back(Index);
	}

	auto Iter = Players.begin();
	const int StartIndex = (StartNum + Interval - 1) % PlayerNum;

	advance(Iter, StartIndex);

	for (int DropPlayerCount = 1; DropPlayerCount < PlayerNum; ++DropPlayerCount)
	{
		printf("erase : %d\n", *Iter);
		Iter = Players.erase(Iter);

		if (Iter == Players.end())
		{
			Iter = Players.begin();
		}

		for (int IntervalCount = Interval; IntervalCount > 1; --IntervalCount)
		{
			++Iter;
			if (Iter == Players.end())
			{
				Iter = Players.begin();
			}
		}
	}

	if (Players.size() == 1)
	{
		printf("answer : %d\n", *(Players.begin()));
	}
}

int main()
{
	//N 파티에 모인 사람 수(1 <= N <= 2000)
	//K 사회자가 고른 숫자(1 <= K <= 2000)
	//S 사회자가 지목한 사람의 번호 (1 <= S <= N)
	int N = 0, K = 0, S = 0;

	scanf_s("%d", &N);
	scanf_s("%d", &K);
	scanf_s("%d", &S);

	GetWinner(N, K, S);

	system("pause");

	return 0;
}

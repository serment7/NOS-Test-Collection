#include <stdio.h>
#include <vector>
#include <queue>
#include <set>

using namespace std;

using NumberType = unsigned long long;

NumberType GetNumber(int TargetIndex)
{
	if (!(1 <= TargetIndex && TargetIndex <= 500))
	{
		printf("K is Invalid!\n");
		return 0ULL;
	}

	priority_queue<NumberType, vector<NumberType>, greater<NumberType>> ComfortableNumbers;
	vector<NumberType> PrimeFormatList = { 2,3,5 };
	set<NumberType> InsertedNum;
	
	ComfortableNumbers.push(1);

	while (--TargetIndex)
	{
		auto Top = ComfortableNumbers.top();
		ComfortableNumbers.pop();

		for (NumberType n : PrimeFormatList)
		{
			auto InsertValue = Top * n;
			if (InsertedNum.find(InsertValue) == InsertedNum.end())
			{
				InsertedNum.insert(InsertValue);
				ComfortableNumbers.push(InsertValue);
			}
		}
	}

	printf("큐 크기 : %d\n", ComfortableNumbers.size());

	return ComfortableNumbers.top();
}

int main()
{
	//K번째 편안한 숫자를 구하라.
	//편안한 숫자 = 소인수가 2,3,5로 이루어진 숫자.
	//1,2,3,4,5,6,8,9,10,12,15
	int K = 0;

	scanf_s("%d", &K);

	printf("result : %llu\n",GetNumber(K));

	system("pause");

	return 0;
}

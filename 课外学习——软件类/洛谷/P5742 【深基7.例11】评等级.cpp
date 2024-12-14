#include <bits/stdc++.h>

using namespace std;

struct stu
{
	int xuehao;
	int a, b;
	int zongchengji, zonghefenshu;
};

stu arr[10000];

int main()
{
	int n; 
	cin >> n;
	
	for (int i = 0; i < n; i++)
	{
		cin >> arr[i].xuehao >> arr[i].a >> arr[i].b;
		arr[i].zongchengji = arr[i].a + arr[i].b;
		arr[i].zonghefenshu = arr[i].a * 7 + arr[i].b * 3;
	}
	
	for (int i = 0; i < n; i++)
	{
		if (arr[i].zongchengji > 140 &&arr[i].zonghefenshu >= 800)
		{
			cout << "Excellent" << endl;
		}
		else
		{
			cout << "Not excellent" << endl;
		}
	}
	
	return 0;
}

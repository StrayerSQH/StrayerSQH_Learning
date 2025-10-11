#include <iostream>
#include <cmath>

using namespace std;

// 第二个方程的导数
double Function_Two_Derivative(double x)
{
	return sin(x) + cos(x);
}

// 第二个方程
double Function_Two(double x)
{
	return sin(x) - cos(x) + 0.5;
}

// 第一个方程的导数
double Function_One_Derivative(double x)
{
	return 3 * pow(x, 2) - 2 * x - 1;
}

// 第一个方程
double Function_One(double x)
{
	return pow(x, 3) - pow(x, 2) - x - 1;
}

void Question_Two()
{
	double down = 0.4, up = 0.5;

	for (int i = 0; i < 100; i++)
	{
		double up_tmp = up;
		up = up - Function_Two(up) / Function_Two_Derivative(up);

		if (abs(up_tmp - up) <= 1e-5)
		{
			cout << "根" << up << "   " << "f(x)在根为" << up << "时的值为：" << Function_One(up) << endl;
			break;
		}
	}

}

void Question_One()
{
	double down = 1, up = 2;

	for (int i = 0; i < 100; i++)
	{
		double up_tmp = up;
		up = up - Function_One(up) / Function_One_Derivative(up);
		
		if (abs(up_tmp - up) <= 1e-5)
		{
			cout << "根" << up << "   " << "f(x)在根为" << up << "时的值为：" << Function_One(up) << endl;
			break;
		}
	}
}


int main()
{
	Question_One(); // 计算第一个问题
	Question_Two(); // 计算第二个问题

	return 0;
}

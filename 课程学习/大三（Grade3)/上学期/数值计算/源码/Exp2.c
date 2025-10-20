#include <iostream>

using namespace std;

void SOR(int n, double omega, double x1, double x2, double x3)
{
	cout << "SOR(" << omega << ")" << endl;

	double x1_t = x1;
	double x2_t = x2;
	double x3_t = x3;

	for (int i = 0; i < n; i++)
	{
		x1 = x1_t + 0.25 * omega * (-4 * x1_t - 3 * x2_t + 24);
		x2 = x2_t + 0.25 * omega * (-3 * x1 - 4 * x2_t + x3_t + 30);
		x3 = x3_t + 0.25 * omega * (x2 - 4 * x3_t - 24);

		cout << "    第" << i + 1 << "次迭代结果：" << "x1: " << x1 << "     x2: " << x2 << "     x3: " << x3 << endl;
		x1_t = x1;
		x2_t = x2;
		x3_t = x3;
	}
}

void Guass_Seidel(int n, double x1, double x2, double x3)
{
	cout << "Guass-Seidel" << endl;

	double x1_t = x1;
	double x2_t = x2;
	double x3_t = x3;

	for (int i = 0; i < n; i++)
	{
		x1 = -0.75 * x2_t + 6;
		x2 = -0.75 * x1 + 0.25 * x3_t + 7.5;
		x3 = 0.25 * x2 - 6;

		cout << "    第" << i + 1 << "次迭代结果：" << "x1: " << x1 << "     x2: " << x2 << "     x3: " << x3 << endl;
		x1_t = x1;
		x2_t = x2;
		x3_t = x3;
	}
}

void Jacobian(int n, double x1, double x2, double x3)
{
	cout << "Jacobian" << endl;

	double x1_t = x1;
	double x2_t = x2;
	double x3_t = x3;

	for (int i = 0; i < n; i++)
	{
		x1 = -0.75 * x2_t + 6;
		x2 = -0.75 * x1_t + 0.25 * x3_t + 7.5;
		x3 = 0.25 * x2_t - 6;
		
		cout << "    第"<< i + 1<< "次迭代结果：" << "x1: " << x1 << "     x2: " << x2 << "     x3: " << x3 << endl;
		x1_t = x1;
		x2_t = x2;
		x3_t = x3;
	}
}

int main()
{
	Jacobian(100, 0, 0, 0);
	Guass_Seidel(100, 0, 0, 0);
	SOR(100, 1.8, 0, 0, 0);
	SOR(100, 1.22, 0, 0, 0);
	Jacobian(100, 100, 100, 100);
}

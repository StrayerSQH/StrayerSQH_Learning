#include <iostream>

using namespace std;

class diff_time
{
	private:
		int h1_a, t1_a;	
		int h2_a, t2_a;
		int diff_h, diff_t;
		int total_t;
		
	public:
		void get(int h1, int t1, int h2, int t2)
		{
			h1_a = h1;
			t1_a = t1;
			h2_a = h2;
			t2_a = t2;
		}
		
		void diff()
		{
			if (t2_a - t1_a < 0)
			{
				diff_h = h2_a - h1_a - 1;
				diff_t = t2_a - t1_a + 60;
				total_t = diff_h * 60 + diff_t;
			}
			else
			{
				diff_h = h2_a - h1_a;
				diff_t = t2_a - t1_a;
				total_t = diff_h * 60 + diff_t;
			}
		}
		
		void print()
		{
			cout << total_t << endl;
		}
};

int main()
{
	int a, b, c, d;
	cin >> a >> b >> c >> d;
	
	diff_time time;
	time.get(a, b, c, d); 
	time.diff();
	time.print();
	
	return 0;
}

#include <iostream>
#include <cstring>

using namespace std;

struct team_information
{
	int id;
	string name;	
};

struct total_game_information
{
	int tid;
	int pid;
	string s;
	int hour;
	int minute;
};

struct total_game_information_state_one
{
	int tid;
	int pid;
	string s;
	int hour;
	int minute;
	int time_difference;
};

struct search
{
	string time;
	string name;	
};

void sort(int hour, int minute, string name, total_game_information total_game[], team_information team[], int n, int m);
int time_Difference(int start_hour, int start_minute, int hour, int minute);

int main()
{
	int start_hour = 9;
	int start_minute = 0;
	int n, m, q;
	cin >> n >> m >> q;
	
	team_information team[m];
	total_game_information total_game[n];
	search search;
	
	//输入队伍的id和该队伍的名字
	for (int i = 0; i <m; i++)
	{
		cin >> team[i].id >> team[i].name;
	}
	
	// tid代表提交的队伍id, pid代表提交的题目id, s代表评测状态, t 代表提交时间
	for (int i = 0; i < n; i++)
	{
		string time;
		cin >> total_game[i].tid >> total_game[i].pid >> total_game[i].s >> time;
		
		if (time[0] == '0')
		{
			total_game[i].hour = (time[1] - '0');
		}
		else
		{
			total_game[i].hour = (time[0] - '0') * 10 + (time[1] - '0');
		}
		total_game[i].minute = (time[3] - '0') * 10 + (time[4] - '0');
	} 
	
	//查询部分 
	for (int i = 0; i < q; i++)
	{
		bool state;
		cin >> state;
		
		//按队伍名查询，输入的第三个字符串为要查询的队伍名字
		if (state)
		{
			int hour, minute;
			cin >> search.time >> search.name;
			
			hour = (search.time[0] - '0') * 10 + (search.time[1] - '0');
			minute = (search.time[3] - '0') * 10 + (search.time[4] - '0');
			
			//调用排序函数sort
			sort(hour, minute, search.name, total_game, team, n, m); 
			 
			
		}
	}
	
	return 0;
}

void sort(int hour, int minute, string name, total_game_information total_game[], team_information team[], int n, int m)
{
	int start_hour = 9;
	int start_minute = 00;
	int time_difference = time_Difference(start_hour, start_minute, hour, minute);
	
	total_game_information_state_one total_game_state_one[n];
	int all;
	
	//将查询的队伍名称转为相应的tid
	for (int i = 0; i < m; i++)
	{
		if (name == team[i].name)
		{
			//计算哪些在查询的时间内 
			for (int j = all; j < n; j++)
			{
				if (time_Difference(start_hour, start_minute, total_game[j].hour, total_game[j].minute) <= time_difference)
				{
	    			total_game_state_one[all].tid = total_game[j].tid;
    				total_game_state_one[all].pid = total_game[j].pid;
				    total_game_state_one[all].s = total_game[j].s;
    				total_game_state_one[all].hour = total_game[j].hour;
    				total_game_state_one[all].minute = total_game[j].minute;
    				total_game_state_one[all].time_difference = time_Difference(start_hour, start_minute, total_game[j].hour, total_game[j].minute);
	    			all++;
				}
			}
		}
	} 
	
	// 首先按照 tid 排序
	for (int i = 0; i < all - 1; i++)
	{
    	for (int j = 0; j < all - 1 - i; j++)
    	{
        	if (total_game_state_one[j].tid >= total_game_state_one[j + 1].tid)
        	{
            	// 交换total_game_state_one[j]和total_game_state_one[j + 1]
            	total_game_information_state_one temp = total_game_state_one[j];
            	total_game_state_one[j] = total_game_state_one[j + 1];
            	total_game_state_one[j + 1] = temp;
        	}
    	}
	}

	// 然后在每个 tid 内部按照 pid 排序
	for (int i = 0; i < all - 1; i++)
	{
    	for (int j = 0; j < all - 1 - i; j++)
    	{
        	if (total_game_state_one[j].pid > total_game_state_one[j + 1].pid)
        	{
            	// 交换total_game_state_one[j]和total_game_state_one[j + 1]
            	total_game_information_state_one temp = total_game_state_one[j];
            	total_game_state_one[j] = total_game_state_one[j + 1];
            	total_game_state_one[j + 1] = temp;
        	}
    	}
	}
	
	int init_team_tid = total_game_state_one[0].tid;
	int init_team_pid = total_game_state_one[0].pid;
	
	for (int i = 0; i < all; i++)
	{
		
		int punish_times = 0;
		int total_punish_time = 0;
		int pass = 0;
		if (init_team_tid == total_game_state_one[i].tid && init_team_pid = total_game_state_one[0].pid;)
		{
			int punishi_times = 0;
			if (total_game_state_one[i].s != "AC" && total_game_state_one[i].s != "CE")
			{
				punish_times++;
			}
			else if (total_game_state_one[i].s == "AC")
			{
				total_punish_time += time_Difference(start_hour, start_minute, total_game_state_one[i].hour, total_game_state_one[i].minute) + punish_times * 20;
				pass++;
			}
		}
	}
	for (int i = 0; i < all; i++)
	{
		cout << total_game_state_one[i].tid << endl;
	}
}

int time_Difference(int start_hour, int start_minute, int end_hour, int end_minute)
{
	int start_time_in_minutes = start_hour * 60 + start_minute;
	int end_time_in_minutes = end_hour * 60 + end_minute;

	int time_difference = end_time_in_minutes - start_time_in_minutes;
	
	return time_difference;
}
//5 3 1
//1 yyds
//2 yyds
//3 ysqd
//1 1 WA 09:54
//2 1 AC 10:13
//1 1 AC 10:14
//1 3 AC 11:11
//2 2 CE 12:00
//1 10:14 yyds

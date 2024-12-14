#include <iostream>  
#include <string>  
  
using namespace std;  
  
class SHR  
{  
private:  
    int n;
    int len = 0;
    char s[100];  
  
public:   
    void input() 
	{  
	    cin >> n;
	    char t = cin.get();
        for (int i = 0; i < 500; i++)
        {
        	cin.get(s[i]);
        	
        	if (s[i] == '\n')
        	{
        		break;
			}
			
        	len++;
		}
    }  
    
    void show() 
	{  
		for (int i = len - n; i < len; i++)
		{
			cout << s[i];
		} 
		
		for (int i = 0; i < len - n; i++)
		{
			cout << s[i];
		}
    }  
};  
  
int main()  
{  
    SHR shr;  
    shr.input();
    shr.show();  
  
    return 0;  
}

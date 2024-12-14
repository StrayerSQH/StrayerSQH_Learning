#include <iostream>  
#include <string>  
#include <cctype>  
#include <cmath>
#include <vector>
#include "Stack.h"
#include "Calculator.h"

using namespace std;

bool isRight(string s)
{
    Stack<char> opeJudge;
    int arr[10001];
    int j = 0;

    for (int i = 0; i < s.size() - 1; i++)
    {
        if (s[i] == '(')
        {
            opeJudge.push(s[i]);
            arr[j] = i;
            j++;
        }
        else if (s[i] == ')')
        {
            if (opeJudge.isEmpty())
            {
                for (int k = 0; k < i - 2; k++)
                {
                    cout << " ";
                }
                cout << "^�˴����Ų�ƥ�䣬�������룡" << endl;
                for (int i = 0; i < 2; i++)
                {
                    cout << endl;
                }
                return false;
            }
            else
            {
                char tmp;
                opeJudge.pop(tmp);
                arr[j] = 0;
                j--;
            }
        }
    }
    j--;
    if (!opeJudge.isEmpty())
    {
        for (int k = 0; k <= arr[j] - 1 - 2; k++)
        {
            cout << " ";
        }
        cout << "^�˴����Ų�ƥ�䣬�������룡" << endl;
        for (int i = 0; i < 2; i++)
        {
            cout << endl;
        }
        return false;
    }

    return true;
}

void Introduction()
{
    std::string courseInfo = "�Ĵ���ѧ�����ѧԺ 2024-2025 ѧ�����ݽṹ���㷨�����γ�ʵ�����ҵһ";
    std::string name = "SQH";
    std::string studentID = "2023141460***";

    std::cout << "+----------------------------------------------------------------------------+" << std::endl;
    std::cout << "| " << courseInfo;
    std::cout << std::string(74 - courseInfo.length(), ' ') << " |" << std::endl;
    std::cout << "| ������" << name;
    std::cout << std::string(68 - name.length(), ' ') << " |" << std::endl;
    std::cout << "| ѧ�ţ�" << studentID;
    std::cout << std::string(68 - studentID.length(), ' ') << " |" << std::endl;
    std::cout << "+----------------------------------------------------------------------------+" << std::endl;

    cout << "+----------------------------------------------------------------------------+" << endl;
    cout << "ʹ�÷�ʽ��1.��������������������������㣬����:1+(1-(2*3+1))=" << endl;
    cout << "          2.֧�ֳ˷����㣬���磺2^3=" << endl;
    cout << "          3.֧��С�����㣬���磺1.1+1.1=" << endl;
    cout << "          4.֧����Ȼ����e���㣬����e^2=" << endl;
    cout << "          5.֧�����Ǻ���sin,cos,tan,cot����,������Ƕ�ס�����sin(9)+cos(9)+tan(9)" << endl;
    cout << "          6.֧������ƥ�����ָ��������,���磺" << endl;
    cout << "                 10+(10*8+1))=" << endl;
    cout << "                            ^ �˴����Ų�ƥ�䣬�������룡" << endl;
    cout << "                 1+((2))3=" << endl;
    cout << "                       ^^�������岻�����ʽ�����������룡" << endl;
    cout << "                 1 + 2(2 + 1) =" << endl;
    cout << "                     ^^�������岻�����ʽ�����������룡" << endl;
    cout << "          7.�Ƿ���0�����⣬���磺" << endl;
    cout << "                 1+1/0=" << endl;
    cout << "                     ^ ��������Ϊ0�����������룡" << endl;
    cout << "                 1+2/(1-1)=" << endl;
    cout << "                         ^ ��������Ϊ0�����������룡" << endl;
    cout << "          8.֧��С���������⡣������һ��������Ĭ������" << endl;
    cout << "                 1.1.1*2+1=3.11" << endl;
    cout << "                 �����С���д���С���㡮.'��������1��" << endl;
    std::cout << "+----------------------------------------------------------------------------+" << std::endl;
    for (int i = 0; i < 1; i++)
    {
        cout << endl;
    }
}

int main()
{
    Introduction();

    char ifEnd = 'y';

    while (toupper(ifEnd) != 'N')
    {
        if (toupper(ifEnd) == 'Y')
        {
            cout << "��������ʽ:" << endl;
            string expression;
            cin >> expression;
            expression.insert(0, "0+");

            if (expression[expression.size() - 1] != '=')
            {
                for (int i = 0; i < expression.size() - 2; i++)
                {
                    cout << " ";
                }
                cout << "^δ���롰=�����޷��������㡣���������롣" << endl;
                for (int i = 0; i < 2; i++)
                {
                    cout << endl;
                }
            }
            else if (isRight(expression) && expression[expression.size() - 1] == '=')
            {
                Calculator<char> Cal(expression);
                double result = Cal.expRun(1);
            }
        }
        else
        {
            cout << "����ȷ�����Ƿ��������y/n),��л��⣡" << endl;
            for (int i = 0; i < 2; i++)
            {
                cout << endl;
            }
        }


        cout << "�Ƿ�����������㣨y/n��:";
        cin >> ifEnd;
    }

    cout << "��лʹ�ã�";

    return 0;
}
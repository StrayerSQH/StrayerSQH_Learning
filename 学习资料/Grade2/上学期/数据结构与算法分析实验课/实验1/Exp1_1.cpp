#include <iostream>  
#include <string>  
#include <cctype>  
#include <cmath>

using namespace std;

template <class T>
class Stack
{
private:

    struct Node {
        T data;
        Node* next;
    };

    Node* head;

public:
    Stack() : head(NULL) {}

    void push(T d)
    {
        Node* q = new Node;
        q->data = d;
        q->next = head;
        head = q;
    }

    bool pop(T& d)
    {
        if (head == NULL) return false;
        Node* q = head;
        d = q->data;
        head = head->next;
        delete q;
        return true;
    }

    bool top(T& d)
    {
        if (head == NULL) return false;
        Node* q = head;
        d = q->data;
    }


    bool isEmpty() const
    {
        return head == NULL;
    }
};

template <class T>
class Calculator {
private:
    Stack<T> NumStack;
    Stack<char> OpeStack;
    string expression;

public:
    Calculator() {}
    Calculator(string exp) : expression(exp) {}

    ~Calculator() {}

    int getValue()
    {
        int i = 0, a = 0, judge = 1;
        char ch;
        NumStack.pop(ch);

        while (ch != 'A')
        {
            judge = 0;
            a += pow(10, i) * ch;
            i++;
            NumStack.pop(ch);

            if (ch == 'A')
            {
                judge = 0;
            }
        }

        return a;
    }

    int opeGrade(char ope)
    {
        switch (ope)
        {
        case '=': return 1;
        case '(': return 2;
        case ')': return 2;
        case '+': return 3;
        case '-': return 3;
        case '*': return 4;
        case '/': return 4;
        case '%': return 4;
        case '^': return 5;
        default: return 0;
        }
    }

    int MyCalculator(int a, char ope, int b)
    {
        int result;
        switch (ope)
        {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        case '%': return a % b;
        case '^': return pow(b, a);
        }

    }

    void expRun()
    {
        //将操作符栈中压入字符=
        OpeStack.push('=');
        NumStack.push('A');
        //定义相关变量
        char beforCh; //前一个字符
        char opeCh = '='; //操作符并将其赋值为操作符栈顶中的=
        char opeBefore = '=';
        char opeTop = 'T';

        int i = 0;
        int a, b, c;
        while (i < expression.size())
        {
            if (isdigit(expression[i]) || expression[i] == '.')
            {
                char t = expression[i];
                NumStack.push(t - '0');
                beforCh = 'N'; //将前一个字符标记为不代表运算符的N
            }
            else
            {
                opeCh = expression[i];
                if (opeGrade(opeBefore) < opeGrade(opeCh))
                {
                    OpeStack.push(opeBefore);
                    opeBefore = opeCh;
                    NumStack.push('A');
                }
                else if (opeGrade(opeCh) <= opeGrade(opeBefore))
                {
                    while (opeGrade(opeCh) <= opeGrade(opeBefore))
                    {

                        a = getValue();
                        b = getValue();
                        NumStack.push('A');
                        //OpeStack.pop(opeCh);
                        c = MyCalculator(b, opeBefore, a);
                        NumStack.push(c);
                        OpeStack.pop(opeBefore);
                        if (OpeStack.isEmpty())
                        {
                            break;
                        }
                    }

                    NumStack.push('A');
                    opeBefore = opeCh;
                }
            }
            i++;
        }
        cout << "表达式的计算结果是：" << c << endl;
    }

};

bool isOperator(char ch)
{
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' || ch == ')' || ch == '=')
    {
        return true;
    }
    return false;
}

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
                for (int k = 0; k < i; k++)
                {
                    cout << " ";
                }
                cout << "^此处括号不匹配，请检查输入" << endl;
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

        for (int k = 0; k <= arr[j] - 1; k++)
        {
            cout << " ";
        }
        cout << "^此处括号不匹配，请检查输入" << endl;
        return false;
    }

    return true;
}


int main()
{
    char ifEnd = 'y';

    while (toupper(ifEnd) == 'Y')
    {
        cout << "请输入表达式:" << endl;
        string expression;
        cin >> expression;

        if (isRight(expression))
        {
            Calculator<char> Cal(expression);
            Cal.expRun();
        }

        cout << "是否继续进行运算（y/n）:";
        cin >> ifEnd;
    }

    return 0;
}

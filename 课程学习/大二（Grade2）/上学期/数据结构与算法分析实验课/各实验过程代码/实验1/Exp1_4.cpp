#include <iostream>  
#include <string>  
#include <cctype>  
#include <cmath>
#include <vector>

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

    double getValue()
    {
        int i = 0, judge = 0, pointNum = 0;
        double a = 0;
        char ch;
        NumStack.pop(ch);

        while (ch != 'A')
        {
            a += pow(10, i) * ch;
            i++;
            NumStack.pop(ch);
            if (ch == '.')
            {
                pointNum++;
                if (pointNum > 1)
                {
                    cout << "输入的小数有错误，小数点‘.'个数超过1个，自动处理为将第一个小数点作为小数点。（若不符合需求，请忽略运算结果）" << endl;
                }
                a /= pow(10, i);
                i = -1;
                ch = 0;
            }

            if (ch == '-')
            {
                judge = 1;
                NumStack.pop(ch);
                break;
            }
        }

        if (judge)
        {
            a *= -1;
        }

        return a;
    }

    bool isOperator(char ch)
    {
        if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^')
        {
            return true;
        }
        return false;
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

    double MyCalculator(double a, char ope, double b)
    {
        switch (ope)
        {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
            //case '%': return a % b;
        case '^': return pow(a, b);
        }

    }

    double expRun()
    {
        //将操作符栈中压入字符=
        OpeStack.push('=');
        NumStack.push('A');
        OpeStack.push('+');
        //定义相关变量
        char beforCh; //前一个字符
        char opeCh = '='; //操作符并将其赋值为操作符栈顶中的=
        char opeBefore = '=';
        char opeTop = 'T';

        int i = 0;
        double a, b, c;
        while (i < expression.size())
        {
            if (expression[i] == 's' || expression[i] == 'c' || expression[i] == 't')
            {
                string b;
                vector<string> trigFunctions = { "sin", "cos", "tan", "cot"};

                size_t startIndex = string::npos;
                size_t endIndex = string::npos;

                for (const auto& func : trigFunctions) 
                {
                    size_t startIndex = 0;
                    while ((startIndex = expression.find(func + "(", startIndex)) != std::string::npos) 
                    {
                        size_t endIndex = expression.find(")", startIndex);
                        if (endIndex != std::string::npos) 
                        {
                            b = expression.substr(startIndex + func.size() + 1, endIndex - startIndex - func.size() - 1);
                            startIndex = endIndex;
                        }
                    }
                }

                b.insert(b.size(), 1, '=');
                b.insert(0, "0+(");
                Calculator<char> CalTri(b);

                double resultTri;
                if (expression[i] == 's')
                {
                    resultTri = sin(CalTri.expRun());
                }
                else if (expression[i] == 'c' && expression[i + 2] == 's')
                {
                    resultTri = cos(CalTri.expRun());
                }
                else if (expression[i] == 't')
                {
                    resultTri = tan(CalTri.expRun());
                }
                else if (expression[i] == 'c' && expression[i + 2] == 't')
                {
             
                    resultTri = 1 / tan(CalTri.expRun());
                }

                string numTri = to_string(resultTri);
                for (int i = 0; i < numTri.size(); i++)
                {
                    if (isdigit(numTri[i]))
                    {
                        NumStack.push(numTri[i] - '0');
                    }
                    else
                    {
                        NumStack.push(numTri[i]);
                    }
                }
                beforCh = 'N';
                i += b.size();
            }
            else if (expression[i] == '(')
            {
                OpeStack.push(opeBefore);
                OpeStack.push(expression[i]);
                opeBefore = expression[i];
            }
            else if (isdigit(expression[i]))
            {
                if (isdigit(expression[i]) && expression[i + 1] == '(')
                {
                    for (int k = 0; k < i - 2; k++)
                    {
                        cout << ' ';
                    }
                    cout << "^^出现意义不明表达式，请重新输入！" << endl;
                    for (int i = 0; i < 2; i++)
                    {
                        cout << endl;
                    }
                    return i;
                }
                char t = expression[i];
                NumStack.push(t - '0');
                beforCh = 'N';
            }
            else if (expression[i] == 'e')
            {
                string numE = to_string(2.718281828459045);
                for (int i = 0; i < numE.size(); i++)
                {
                    if (isdigit(numE[i]))
                    {
                        NumStack.push(numE[i] - '0');
                    }
                    else
                    {
                        NumStack.push(numE[i]);
                    }
                }
            }
            else if (expression[i] == '.')
            {
                NumStack.push(expression[i]);
            }
            else if ((!isdigit(expression[i - 1]) && expression[i - 1] != ')' && expression[i] == ')') || 
                     (expression[i] == ')' && isdigit(expression[i + 1])) || 
                     (isOperator(expression[i]) && isOperator(expression[i + 1])))
            {
                for (int k = 0; k < i - 2; k++)
                {
                    cout << ' ';
                }
                cout << "^^出现意义不明表达式，请重新输入！" << endl;
                for (int i = 0; i < 2; i++)
                {
                    cout << endl;
                }
                return i;
            }
            else
            {
                opeCh = expression[i];
                if (opeGrade(opeBefore) < opeGrade(opeCh))
                {
                    if (opeBefore != '(')
                    {
                        OpeStack.push(opeBefore);
                    }
                    opeBefore = opeCh;
                    if (opeCh != '(')
                    {
                        NumStack.push('A');
                    }
                }
                else if (opeGrade(opeCh) <= opeGrade(opeBefore))
                {
                    while (opeGrade(opeCh) <= opeGrade(opeBefore) && !(opeBefore == '=' && opeBefore == opeCh) && !(opeBefore == '(' && opeCh == ')'))
                    {

                        a = getValue();
                        b = getValue();
                        NumStack.push('A');
                        if (a == 0 && opeBefore == '/')
                        {
                            for (int k = 0; k < i - 1 - 2; k++)
                            {
                                cout << ' ';
                            }
                            cout << "^除数不能为0，请重新输入！" << endl;
                            for (int i = 0; i < 2; i++)
                            {
                                cout << endl;
                            }
                            return i;
                        }
                        else
                        {
                            c = MyCalculator(b, opeBefore, a);
                        }
                        string numTmp = to_string(c);
                        for (int i = 0; i < numTmp.size(); i++)
                        {
                            if (isdigit(numTmp[i]))
                            {
                                NumStack.push(numTmp[i] - '0');
                            }
                            else
                            {
                                NumStack.push(numTmp[i]);
                            }
                        }
                        OpeStack.pop(opeBefore);
                        if (OpeStack.isEmpty() || (opeBefore == '('))
                        {
                            OpeStack.push(opeBefore);
                            if (opeCh != ')')
                            {
                                opeBefore = opeCh;
                            }
                            break;
                        }
                    }

                    NumStack.push('A');
                    if (opeBefore != '(')
                    {
                        if (opeGrade(opeBefore) < opeGrade(opeCh))
                        {
                            OpeStack.push(opeBefore);
                        }
                        opeBefore = opeCh;
                    }
                    else if (opeCh == ')' && opeBefore == '(')
                    {
                        char deletCh1;
                        NumStack.pop(deletCh1);
                        OpeStack.pop(opeBefore);
                        OpeStack.pop(opeBefore);
                    }
                }
            }
            i++;
        }

        return c;
    }
};

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
                cout << "^此处括号不匹配，请检查输入！" << endl;
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
        cout << "^此处括号不匹配，请检查输入！" << endl;
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
    std::string courseInfo = "四川大学计算机学院 2024-2025 学年数据结构与算法分析课程实验课作业一";
    std::string name = "SQH";
    std::string studentID = "2023141460***";

    std::cout << "+----------------------------------------------------------------------------+" << std::endl;
    std::cout << "| " << courseInfo;
    std::cout << std::string(74 - courseInfo.length(), ' ') << " |" << std::endl;
    std::cout << "| 姓名：" << name;
    std::cout << std::string(68 - name.length(), ' ') << " |" << std::endl;
    std::cout << "| 学号：" << studentID;
    std::cout << std::string(68 - studentID.length(), ' ') << " |" << std::endl;
    std::cout << "+----------------------------------------------------------------------------+" << std::endl;

    for (int i = 0; i < 2; i++)
    {
        cout << endl;
    }
    std::cout << "+----------------------------------------------------------------------------+" << std::endl;
    cout << "使用方式：" << endl;
    cout << "          1.可以完成正常的四则运算混合运算，例如:1+(1-(2*3+1))=" << endl;
    cout << "          2.支持乘方运算，例如：2^3=" << endl;
    cout << "          3.支持小数运算，例如：1.1+1.1=" << endl;
    cout << "          4.支持自然对数e运算，例如e^2=" << endl;
    cout << "          5.支持三角函数sin,cos,tan,cot运算,但不能嵌套。例如sin(9)+cos(9)+tan(9)" << endl;
    cout << "          6.支持括号匹配检测和指出错误功能,例如：" << endl;
    cout << "                 10+(10*8+1))=" << endl;
    cout << "                            ^ 此处括号不匹配，请检查输入！" << endl;
    cout << "                 1+((2))3=" << endl;
    cout << "                       ^^出现意义不明表达式，请重新输入！" << endl;
    cout << "                 1 + 2(2 + 1) =" << endl;
    cout << "                     ^^出现意义不明表达式，请重新输入！" << endl;
    cout << "          7.非法除0运算检测，例如：" << endl;
    cout << "                 1+1/0=" << endl;
    cout << "                     ^ 除数不能为0，请重新输入！" << endl;
    cout << "                 1+2/(1-1)=" << endl;
    cout << "                         ^ 除数不能为0，请重新输入！" << endl;
    cout << "          8.支持小数点运算检测。若超过一个则启动默认运算" << endl;
    cout << "                 1.1.1*2+1=3.11" << endl;
    cout << "                 输入的小数有错误，小数点‘.'个数超过1个" << endl;
    std::cout << "+----------------------------------------------------------------------------+" << std::endl;
    for (int i = 0; i < 2; i++)
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
            cout << "请输入表达式:" << endl;
            string expression;
            cin >> expression;
            expression.insert(0, "0+");

            if (expression[expression.size() - 1] != '=')
            {
                for (int i = 0; i < expression.size() - 2; i++)
                {
                    cout << " ";
                }
                cout << "^未输入“=”，无法进行运算。请重新输入。" << endl;
                for (int i = 0; i < 2; i++)
                {
                    cout << endl;
                }
            }
            else if (isRight(expression) && expression[expression.size() - 1] == '=')
            {
                Calculator<char> Cal(expression);
                double result = Cal.expRun();

                cout << "表达式的计算结果是：" << result << endl;
                for (int i = 0; i < 2; i++)
                {
                    cout << endl;
                }
            }
        }
        else
        {
            cout << "请正确输入是否继续程序（y/n),感谢理解！" << endl;
            for (int i = 0; i < 2; i++)
            {
                cout << endl;
            }
        }


        cout << "是否继续进行运算（y/n）:";
        cin >> ifEnd;
    }

    cout << "感谢使用！";

    return 0;
}

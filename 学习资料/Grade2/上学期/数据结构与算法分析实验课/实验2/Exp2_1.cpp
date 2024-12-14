#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

void Introduction();
void TxtI();

class SearchAndReplacement {
private:
    string matchTxt;
    string originalTxt;
    int next[100000];
    
public:
    SearchAndReplacement() {};

    SearchAndReplacement(string m, string o) : matchTxt(m), originalTxt(o){}

    void GetNext()
    {
        int j = 0;
        next[0] = 0;
        for (int i = 0; i < matchTxt.size(); i++)
        {
            while (j > 0 && matchTxt[i] != matchTxt[j])
            {
                j = next[j - 1];
            }

            if (matchTxt[i] == matchTxt[j])
            {
                j++;
            }
            next[i] = j;
        }
    }

    int Search()
    {
        GetNext();

        for (int i = 0, j = 0; i < originalTxt.size(); i++)
        {
            while (j > 0 && originalTxt[i] != matchTxt[j])
            {
                j = next[j - 1];
            }

            if (originalTxt[i] == matchTxt[j])
            {
                j++;
            }

            if (j == matchTxt.size())
            {
                j = next[j - 1];
                return i - matchTxt.size() + 1;
            }
        }

        return -1;
    }
};

class OperationHistory {
private:
    int opeTimes = 0;

    struct Node {
        int opeLine;
        char opeCommand;
        string opeTxt;

        Node* next;
    };
    
    Node* head;

public:
    OperationHistory() : head(NULL) {};

    void push(int l, char c, string t)
    {
        Node* q = new Node;
        q->opeLine = l;
        q->opeCommand = c;
        q->opeTxt = t;
        q->next = head;
        head = q;
    }

    bool pop(int& l, char& c, string& t)
    {
        if (head == NULL)
        {
            return false;
        }
        
        Node* q = head;
        l = q->opeLine;
        c = q->opeCommand;
        t = q->opeTxt;
        head = head->next;
        delete q;

        return true;
    }
};

class TxtEditor {
private:
    string fileName;
    string usedFileName;
    string tmpFileName;
    string insertLineString;
    string line;

    char command;

    int opeTimes = -1;

    int currentLineNo;
    int maxLineNo = 0;

    int insertLineNumber;
    int deleteLineNumber;
    int getLineNumber;

    bool firstReadFile = true;

    ifstream inFile;
    ofstream outFile;

    vector<string> lines;

public:
    OperationHistory history;

    TxtEditor() {};
    
    int ReadTxt()
    {
        cout << "输入文件名：";
        cin >> tmpFileName;
        tmpFileName += ".txt";

        if (firstReadFile)
        {
            usedFileName = tmpFileName;
            fileName = tmpFileName;
            firstReadFile = false;
        }
        else
        {
            if (fileName != tmpFileName)
            {
                cout << "读取" << tmpFileName << "将保存并删除缓冲区中" << fileName << "中的内容，是否要继续操作（Y/N）？";
                char judge = ' ';
                cin >> judge;
                if (toupper(judge) == 'N' && toupper(judge) != 'Y')
                {
                    cout << "放弃读取" << tmpFileName << endl;
                    return 1;
                }
                else
                {
                    usedFileName = fileName;
                    fileName = tmpFileName;
                    cout << "自动保存了" << usedFileName << "在缓冲区中的内容并";
                    WriteTxt(usedFileName);
                }
            }
        }

        inFile.open(fileName);
        if (!inFile.is_open())
        {
            cout << fileName << "不存在，自动创建相应文件" << endl;
            SaveTxt();
            inFile.open(fileName);
        }
        else
        {
            cout << "成功读取文件：" << fileName << endl;
            lines.clear();
            while (getline(inFile, line)) 
            {
                lines.push_back(line);
                maxLineNo++;
            }
            inFile.close();
        }
        usedFileName = fileName;
    }

    void WriteTxt(string file)
    {
        outFile.open(file);
        if (outFile.is_open())
        {
            for (const string& line : lines)
            {
                outFile << line << endl;
            }
            outFile.close();
        }
    }

    void SaveTxt()
    {
        outFile.open(fileName);
        outFile.close();
    }

    void InsertTxt(string newLine, int insertLineNo)
    {
        insertLineNo--;
        currentLineNo = insertLineNo + 1;

        if (insertLineNo == maxLineNo + 1)
        {
            lines.push_back(newLine);
        }
        else
        {
            lines.push_back(" ");
            for (int i = maxLineNo; i > insertLineNo; i--)
            {
                lines[i] = lines[i - 1];
            }

            lines[insertLineNo] = newLine;
            maxLineNo++;
        }
    }

    string DeleteTxt(int deleteLineNo)
    {
        string deleteTxt = lines[deleteLineNo - 1];
        for (int i = maxLineNo - 1; i >= deleteLineNo; i--)
        {
            lines[i - 1] = lines[i];
        }
        maxLineNo--;
        currentLineNo = deleteLineNo;
        lines.pop_back();
        return deleteTxt;
    }

    bool FindTxt()
    {
        string findTxt;
        cout << "输入需要查找的内容：";
        cin >> findTxt;

        if (findTxt.size() == 0)
        {
            cout << "输入内容不能为空！" << endl;
            return false;
        }
        else
        {
            for (int currentLineNo = 1; currentLineNo <= maxLineNo; currentLineNo++)
            {
                SearchAndReplacement findTxtinFile(findTxt, lines[currentLineNo - 1]);
                findTxtinFile.GetNext();
                int k = findTxtinFile.Search();
                if (k != -1)
                {
                    cout << lines[currentLineNo - 1] << endl;
                    for (int j = 0; j < k; j++)
                    {
                        cout << " ";
                    }
                    cout << "^当前第" << currentLineNo<< "行，需要查找的字符“" << findTxt << "”首次出现在此处。" << endl;

                    return true;
                }
            }
        }

        cout << "未在文件中找到匹配字符。" << endl;
        return false;
        
    }

    bool ChageTxt()
    {
        string changedTxt;
        string changingTxt;
        cout << "当前为第"<< currentLineNo << "行 ,内容为：" << endl << lines[currentLineNo - 1] << endl << "输入需要替换的内容：";
        cin >> changedTxt;
        cout << "替换为：";
        cin >> changingTxt;

        if (changedTxt.size() == 0)
        {
            cout << "替换内容不能为空！" << endl;
            return false;
        }
        else
        {
            SearchAndReplacement changeTxtinFile(changedTxt, lines[currentLineNo - 1]);
            changeTxtinFile.GetNext();
            int k = changeTxtinFile.Search();
            if (k != -1)
            {
                if (changedTxt.size() >= changingTxt.size())
                {
                    int j, t;
                    for (j = k, t = 0; j < k + changingTxt.size(); j++, t++)
                    {
                        lines[currentLineNo - 1][j] = changingTxt[t];
                    }

                    for (j; j < lines[currentLineNo - 1].size(); j++)
                    {
                        lines[currentLineNo - 1][j] = lines[currentLineNo - 1][j + 1];
                    }
                    lines[currentLineNo - 1].substr(0, lines[currentLineNo - 1].length() - 1);
                }
                else
                {
                    int sizeofChangedTxt, sizeofChagingTxt, cha;
                    sizeofChangedTxt = changedTxt.size();
                    sizeofChagingTxt = changingTxt.size();
                    cha = sizeofChagingTxt - sizeofChangedTxt;

                    string tmp;
                    for (int j = 0; j < cha; j++)
                    {
                        tmp.insert(tmp.size(), "*");
                    }
                    lines[currentLineNo - 1] += tmp;

                    int j, t;
                    for (j = lines[currentLineNo - 1].size() - 1, t = cha; t >= 0; t--, j--)
                    {
                        lines[currentLineNo - 1][j] = lines[currentLineNo - 1][j - cha];
                    }

                    for (j = k, t = 0; t < changingTxt.size(); j++, t++)
                    {
                        lines[currentLineNo - 1][j] = changingTxt[t];
                    }
                }
            }
        }
        return false;
    }

    string GetTxt(int getLineNo)
    {
        currentLineNo = getLineNo;
        return lines[getLineNo - 1];
    }

    void ViewTxt()
    {
        for (const string& line : lines)
        {
            cout << line << endl;
        }
    }

    int Run()
    {
        char command = 'H';
        OperationHistory opeHistory;

        while (command != ' ')
        {
            switch (command) 
            {
            case 'R':
                ReadTxt();
                break;
            case 'W':
            case 'S':
                cout << "将缓冲区的内容写入文本文件..." << endl;
                WriteTxt(fileName);
                break;
            case 'I':
                cout << "当前有" << maxLineNo << "行。请输入需要在哪行前插入或在最后一行后插入？";
                cin >> insertLineNumber;
                if (insertLineNumber > maxLineNo + 1 || insertLineNumber < 0)
                {
                    cout << "可插入的行数为1至" << maxLineNo << "之间或在最后一行后面进行插入！" << endl;
                }
                else
                {
                    currentLineNo = insertLineNumber;
                    cout << "输入需要插入的内容：" << endl;
                    cin >> insertLineString;
                    InsertTxt(insertLineString, insertLineNumber);
                    opeHistory.push(currentLineNo, 'D', "");
                }
                break;
            case 'D':
                cout << "当前有" << maxLineNo << "行。请输入需要删除那一行？";
                cin >> deleteLineNumber;
                if (deleteLineNumber > maxLineNo + 1 || deleteLineNumber < 0)
                {
                    cout << "可删除的行数为1至" << maxLineNo << "之间！" << endl;
                }
                else
                {
                    string deleteTmp = DeleteTxt(deleteLineNumber);
                    opeHistory.push(currentLineNo, 'I', deleteTmp);
                    cout << "删除的内容为：" << deleteTmp << endl;
                }
                break;
            case 'F':
                FindTxt();
                break;
            case 'C':
                ChageTxt();
                break;
            case 'Q':
                WriteTxt(fileName);
                cout << "已自动保存" << fileName << "。程序结束，感谢使用！" << endl;
                exit(0);
                break;
            case 'H':
            case '?':
                cout << "指令集为：" << endl;
                TxtI();
                break;
            case 'N':
                if (currentLineNo == maxLineNo)
                {
                    cout << "当前已在最后一行，无法跳转至下一行！" << endl;
                }
                else
                {
                    cout << "当前为第" << currentLineNo << "行，现跳转到第" << currentLineNo + 1 << "行" << endl;
                    cout << "第" << currentLineNo << "行的文本为：" << GetTxt(currentLineNo + 1) << endl;
                }
                break;
            case 'P':
                if (currentLineNo == 1)
                {
                    cout << "当前已在第一行，无法跳转至上一行！" << endl;
                }
                else
                {
                    cout << "当前为第" << currentLineNo << "行，现跳转到第" << currentLineNo - 1 << "行" << endl;
                    cout << "第" << currentLineNo << "行的文本为：" << GetTxt(currentLineNo - 1) << endl;
                }
                break;
            case 'B':
                cout << "跳转到第1行" << endl;
                cout << "第1行的文本为：" << GetTxt(1) << endl;
                break;
            case 'E':
                cout << "跳转到第" << maxLineNo << "行" << endl;
                cout << "第" << maxLineNo << "行的文本为：" << GetTxt(maxLineNo) << endl;
                break;
            case 'G':
                cout << "当前有" << maxLineNo << "行。请输入需要需要查看的行数：";
                cin >> getLineNumber;
                cout << "第" << getLineNumber << "行的文本为：" << GetTxt(getLineNumber) << endl;
                break;
            case 'V':
                cout << "文件" << fileName << "中的全部内容为：" << endl;
                ViewTxt();
                break;
            case 'U':
                break;
            case 'Z':
                break;
            default:
                cout << "未知指令。请正确输入如下指令集中的一个：" << endl;
                TxtI();
                break;
            }

            cout << "------------------------------------------------------------" << endl;
            cout << endl;
            cout << "请输入指令：";
            cin >> command;
            command = toupper(command);

        }
        
        return 0;
    }
};


int main()
{
    Introduction();

    TxtEditor txtEditor;

    txtEditor.Run();

    return 0;
}

void TxtI()
{
    cout << "          R：读取文本文件到缓冲区中，缓冲区中以前的任何内容将丢失，当前行是文件的第一行。" << endl;
    cout << "          W 或 S：将缓冲区的内容写入文本文件，当前行或缓冲区均不改变。" << endl;
    cout << "          I：插入单个新行，用户必须在恰当的提示符的响应中键入新行并提供其行号。" << endl;
    cout << "          D：删除当前行并移到下一行。" << endl;
    cout << "          F：从当前行开始，查找包含有用户请求的目标串的第一行。" << endl;
    cout << "          C：将用户请求的字符串修改成用户请求的替换字符串，仅在当前行中有效。" << endl;
    cout << "          Q：退出编辑器。" << endl;
    cout << "          H 或 ?：显示解释所有命令的帮助消息。" << endl;
    cout << "          N：下一行，在缓冲区中进一行。" << endl;
    cout << "          P：上一行，在缓冲区中退一行。" << endl;
    cout << "          B：开始，到缓冲区的第一行。" << endl;
    cout << "          E：结束，到缓冲区的最后一行。" << endl;
    cout << "          G：转到缓冲区中用户指定的行号。" << endl;
    cout << "          V：查看缓冲区的全部内容，显示到终端上。" << endl;
    cout << "          U: 撤销，回到上一步操作之前的状态。" << endl;
    cout << "          Z: 重做，撤销后可以恢复撤销前的状态。" << endl;
}

void Introduction()
{
    std::string courseInfo = "四川大学计算机学院 2024-2025 学年数据结构与算法分析课程实验课作业二";
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
}

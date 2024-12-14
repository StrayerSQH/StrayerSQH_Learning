#ifndef TXTEDITOR
#define TXTEDITOR

#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdlib>

using namespace std;

void TxtI()
{
    cout << "ָ�Ϊ��" << endl;
    cout << "          R����ȡ�ı��ļ����������У�����������ǰ���κ����ݽ���ʧ����ǰ�����ļ��ĵ�һ�С�" << endl;
    cout << "          W �� S����������������д���ı��ļ�����ǰ�л򻺳��������ı䡣" << endl;
    cout << "          I�����뵥�����У��û�������ǡ������ʾ������Ӧ�м������в��ṩ���кš�" << endl;
    cout << "          D��ɾ����ǰ�в��Ƶ���һ�С�" << endl;
    cout << "          F���ӵ�ǰ�п�ʼ�����Ұ������û������Ŀ�괮�ĵ�һ�С�" << endl;
    cout << "          C�����û�������ַ����޸ĳ��û�������滻�ַ��������ڵ�ǰ������Ч��" << endl;
    cout << "          Q���˳��༭����" << endl;
    cout << "          H �� ?����ʾ������������İ�����Ϣ��" << endl;
    cout << "          N����һ�У��ڻ������н�һ�С�" << endl;
    cout << "          P����һ�У��ڻ���������һ�С�" << endl;
    cout << "          B����ʼ�����������ĵ�һ�С�" << endl;
    cout << "          E���������������������һ�С�" << endl;
    cout << "          G��ת�����������û�ָ�����кš�" << endl;
    cout << "          V���鿴��������ȫ�����ݣ���ʾ���ն��ϡ�" << endl;
    cout << endl << endl << endl << endl << endl;
}

void Introduction()
{
    std::string courseInfo = "�Ĵ���ѧ�����ѧԺ 2024-2025 ѧ�����ݽṹ���㷨�����γ�ʵ�����ҵ��";
    std::string name = "��ǫ�";
    std::string studentID = "2023141460321";

    std::cout << "+----------------------------------------------------------------------------+" << std::endl;
    std::cout << "| " << courseInfo;
    std::cout << std::string(74 - courseInfo.length(), ' ') << " |" << std::endl;
    std::cout << "| ������" << name;
    std::cout << std::string(68 - name.length(), ' ') << " |" << std::endl;
    std::cout << "| ѧ�ţ�" << studentID;
    std::cout << std::string(68 - studentID.length(), ' ') << " |" << std::endl;
    std::cout << "+----------------------------------------------------------------------------+" << std::endl;
    cout << endl;
}

class TxtEditor {
private:
    string fileName;
    string usedFileName;
    string tmpFileName;
    string insertLineString;
    string line;

    char command;

    int opeTimes = -1;

    int currentLineNo = 1;
    int maxLineNo = 0;

    int pages;
    int total;

    int insertLineNumber;
    int deleteLineNumber;
    int getLineNumber;

    bool firstReadFile = true;

    ifstream inFile;
    ofstream outFile;

    vector<string> lines;

public:
    TxtEditor() {};

    void PageAndTotal()
    {
        if (currentLineNo % 20 == 0)
        {
            pages = currentLineNo / 20;
        }
        else
        {
            pages = currentLineNo / 20 + 1;
        }

        if (maxLineNo % 20 == 0)
        {
            total = maxLineNo / 20;
        }
        else
        {
            total = maxLineNo / 20 + 1;
        }
    }

    void Screen(string command)
    {
        cout << endl;
        if (command == "Right")
        {
            if (maxLineNo == 0)
            {
                for (int i = 1; i <= 20; i++)
                {
                    cout << setw(2) << i << " | " << endl;
                }
            }
            else
            {
                PageAndTotal();

                int base = pages * 20;
                if (maxLineNo >= base)
                {
                    for (int i = 0; i < 20; i++)
                    {
                        if (i == currentLineNo - 1)
                        {
                            cout << "\033[32m" << setw(2) << i + (pages - 1) * 20 + 1 << " | " << lines[i] << "\033[0m" << endl;
                        }
                        else
                        {
                            cout << setw(2) << i + (pages - 1) * 20 + 1 << " | " << lines[i + (pages - 1) * 20] << endl;
                        }
                    }
                }
                else
                {
                    int i;
                    for (i = 0; i < maxLineNo - (pages - 1) * 20; i++)
                    {
                        if (i + (pages - 1) * 20 == currentLineNo - 1)
                        {
                            cout << "\033[32m" << setw(2) << i + (pages - 1) * 20 + 1 << " | " << lines[i + (pages - 1) * 20] << "\033[0m" << endl;
                        }
                        else
                        {
                            cout << setw(2) << i + (pages - 1) * 20 + 1 << " | " << lines[i + (pages - 1) * 20] << endl;
                        }
                    }
                    i += (pages - 1) * 20;
                    for (i; i < base; i++)
                    {
                        cout << setw(2) << i + 1 << " | " << endl;
                    }
                }

                cout << endl << "Page:" << setw(2) << pages << " ,Total:" << setw(2) << total << endl;
            }
        }
        else
        {
            TxtI();
        }
        cout << endl;
        cout << "----------------------------------------------------" << endl;
    }

    int ReadTxt(string tmpFileName)
    {
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
                cout << "��ȡ" << tmpFileName << "�����沢ɾ����������" << fileName << "�е����ݣ��Ƿ�Ҫ����������Y/N����";
                char judge = ' ';
                cin >> judge;
                if (toupper(judge) == 'N' && toupper(judge) != 'Y')
                {
                    cout << "������ȡ" << tmpFileName << endl;
                    return 1;
                }
                else
                {
                    usedFileName = fileName;
                    fileName = tmpFileName;
                    cout << "�Զ�������" << usedFileName << "�ڻ������е����ݲ�";
                    WriteTxt(usedFileName);
                }
            }
        }

        inFile.open(fileName);
        if (!inFile.is_open())
        {
            cout << fileName << "�����ڣ��Զ�������Ӧ�ļ�" << endl;
            SaveTxt();
            inFile.open(fileName);
        }
        else
        {
            cout << "�ɹ���ȡ�ļ���" << fileName << endl;
            lines.clear();
            maxLineNo = 0;
            currentLineNo = 1;
            while (getline(inFile, line))
            {
                lines.push_back(line);
                maxLineNo++;
            }

            if (maxLineNo == 0)
            {
                lines.push_back("");
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

    bool InsertTxt()
    {
        string newLine;
        int insertLineNo;
        cout << "��ǰ��" << maxLineNo << "�С���������Ҫ������ǰ����������һ�к���룿";
        cin >> insertLineNumber;
        if (insertLineNumber > maxLineNo + 1 || insertLineNumber < 0)
        {
            cout << "�ɲ��������Ϊ1��" << maxLineNo << "֮��������һ�к�����в��룡" << endl;
            return false;
        }
        else
        {
            currentLineNo = insertLineNumber;
            insertLineNo = insertLineNumber;
            cout << "������Ҫ��������ݣ�";
            cin >> insertLineString;

            if (insertLineNo == maxLineNo + 1)
            {
                lines.push_back(insertLineString);
                maxLineNo++;
            }
            else
            {
                lines.push_back(" ");
                for (int i = maxLineNo; i >= insertLineNo; i--)
                {
                    lines[i] = lines[i - 1];
                }

                lines[insertLineNo - 1] = insertLineString;
                maxLineNo++;
            }

            return true;
        }

    }

    void DeleteTxt()
    {
        int deleteLineNo;
        cout << "��ǰ��" << maxLineNo << "�С���������Ҫɾ����һ�У�";
        cin >> deleteLineNo;
        if (deleteLineNo > maxLineNo + 1 || deleteLineNo < 0)
        {
            cout << "��ɾ��������Ϊ1��" << maxLineNo << "֮�䣡" << endl;
        }
        else
        {
            for (int i = deleteLineNo - 1; i <= maxLineNo - 2; i++)
            {
                lines[i] = lines[i + 1];
            }
            maxLineNo--;
            currentLineNo = deleteLineNo;
            lines.pop_back();
        }
    }

    bool FindTxt()
    {
        string findTxt;
        cout << "������Ҫ���ҵ����ݣ�";
        cin >> findTxt;

        if (findTxt.size() == 0)
        {
            cout << "�������ݲ���Ϊ�գ�" << endl;
            return false;
        }
        else
        {
            for (currentLineNo = 1; currentLineNo <= maxLineNo; currentLineNo++)
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
                    cout << "^��ǰ��" << currentLineNo << "�У���Ҫ���ҵ��ַ���" << findTxt << "���״γ����ڴ˴���" << endl;

                    return true;
                }
            }
        }

        cout << "δ���ļ����ҵ�ƥ���ַ���" << endl;
        return false;

    }

    void ChageTxt()
    {
        string changedTxt;
        string changingTxt;
        cout << "������Ҫ�滻�����ݣ�";
        cin >> changedTxt;
        cout << "�滻Ϊ��";
        cin >> changingTxt;
        if (changedTxt.size() == 0)
        {
            cout << "���滻���ݲ���Ϊ�գ�" << endl;
        }
        else
        {
            SearchAndReplacement changeTxtinFile(changedTxt, lines[currentLineNo - 1]);
            changeTxtinFile.GetNext();
            int k = changeTxtinFile.Search();
            int sizeofChangedTxt, sizeofChagingTxt, cha;
            if (k != -1)
            {
                sizeofChangedTxt = changedTxt.size();
                sizeofChagingTxt = changingTxt.size();
                lines[currentLineNo - 1].erase(k, sizeofChangedTxt);
                lines[currentLineNo - 1].insert(k, changingTxt);
            }
        }
    }

    string GetTxt(int getLineNo)
    {
        currentLineNo = getLineNo;
        return lines[getLineNo - 1];
    }

    void ViewTxt()
    {
        currentLineNo = 1;
        PageAndTotal();
        int getNextPage;
        cout << "����鿴��ҳ��Ϊ��";
        cin >> getNextPage;
        if (0 < getNextPage && getNextPage <= total)
        {
            currentLineNo += (getNextPage - 1) * 20;
        }
    }

    int Run()
    {
        char command = ' ';
        char tmpCommand = 'H';

        while (tmpCommand == 'H' || tmpCommand == '?')
        {
            system("cls");
            Introduction();
            Screen("Wrong");
            cout << "���ȡһ���ļ�(R)�����˳�(Q)��";
            cin >> tmpCommand;
            tmpCommand = toupper(tmpCommand);
            if (tmpCommand != 'R' && tmpCommand != 'Q')
            {
                tmpCommand = 'H';
            }
        }

        command = tmpCommand;

        system("cls");
        Introduction();
        Screen("Right");
        while (command != ' ')
        {
            switch (command)
            {
            case 'R':
                cout << "�����ļ�����";
                cin >> tmpFileName;
                tmpFileName += ".txt";
                ReadTxt(tmpFileName);
                break;
            case 'W':
            case 'S':
                cout << "��������������д���ı��ļ�..." << endl;
                WriteTxt(fileName);
                break;
            case 'I':
                InsertTxt();
                break;
            case 'D':
                DeleteTxt();
                break;
            case 'F':
                FindTxt();
                break;
            case 'C':
                ChageTxt();
                break;
            case 'Q':
                WriteTxt(fileName);
                exit(0);
                break;
            case 'H':
            case '?':
                system("cls");
                Introduction();
                Screen("Wrong");
                break;
            case 'N':
                if (currentLineNo != maxLineNo)
                {
                    GetTxt(currentLineNo + 1);
                }
                break;
            case 'P':
                if (currentLineNo != 1)
                {
                    GetTxt(currentLineNo - 1);
                }
                break;
            case 'B':
                GetTxt(1);
                break;
            case 'E':
                GetTxt(maxLineNo);
                break;
            case 'G':
                cout << "��ǰ��" << maxLineNo << "�С���������Ҫ��Ҫ�鿴��������";
                cin >> getLineNumber;
                if (1 <= getLineNumber && getLineNumber <= maxLineNo)
                {
                    GetTxt(getLineNumber);
                }
                break;
            case 'V':
                ViewTxt();
                break;
            case 'U':
                break;
            case 'Z':
                break;
            default:
                cout << "δָ֪�����ȷ��������ָ��е�һ����" << endl;

                Screen("Wrong");
                break;
            }

            if (command != 'H' && command != '?')
            {
                system("cls");
                Introduction();
                Screen("Right");
            }
            cout << "������ָ�";
            cin >> command;
            command = toupper(command);

        }

        return 0;
    }
};

#endif TXTEDITOR
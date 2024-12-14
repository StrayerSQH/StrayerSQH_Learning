#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>

using namespace std;

int MAX = 20;

class CourseInformation {
private:
public:
    //课程信息变量
    int courseTypeId = 0;
    string courseName = "";
    int courseId = 0;
    string teacherName = "";
    int credit;
    int termOnSchedule;
    int startTime;
    int endTime;
    int day;
    int ifReutrn = 0;
    bool study = false; // 用于存储该课程是否有被学习过
    string dependenceTxt;
    vector<CourseInformation*> prerequisites; // 先修课程列表
    vector<CourseInformation*> successors; // 后继课程列表

    //广义表变量
    int atomType = 0;//0表示为子表，1表示为原子，通过后面程序进行定义
    CourseInformation* next = NULL;
    CourseInformation* childnext = NULL;
    CourseInformation* queuenext = NULL;
    CourseInformation* termnext = NULL;
    CourseInformation* stacknext = NULL;

    CourseInformation(int cTId) : courseTypeId(cTId) {}

    CourseInformation(int cTId, string cName, int cId, string tName, int c, int tOSch, int sTime, int eTime, int wDay, string dep) {
        courseTypeId = cTId;
        courseName = cName;
        courseId = cId;
        teacherName = tName;
        credit = c;
        termOnSchedule = tOSch;
        startTime = sTime;
        endTime = eTime;
        day = wDay;
        dependenceTxt = dep;
        atomType = 1;
    }

    string GetDependence() { return dependenceTxt; }
    void SetAtomType(int type) { atomType = type; }
};

class GeneralList {
private:
public:
    CourseInformation* head = NULL;
    CourseInformation* currentTableId = NULL;
    CourseInformation* currentChild = NULL;
    int courseNum = MAX;
    GeneralList() {}

    //创建广义表，默认创建的广义表有MAX个表链节点
    void CreateGeneralList(int size)
    {
        for (int i = 1; i <= size; i++)
        {
            CourseInformation* tmp = new CourseInformation(i);
            if (head == NULL)
            {
                head = tmp;
                currentTableId = tmp;
            }
            else
            {
                currentTableId->next = tmp;
                currentTableId = currentTableId->next;
            }
        }
    }

    // 在广义表中添加课程
    void AddCourseInEachType(CourseInformation* courseInformation)
    {
        currentTableId = head;
        while (currentTableId->courseTypeId != courseInformation->courseTypeId)
        {
            currentTableId = currentTableId->next;
        }

        if (currentTableId->childnext == NULL)
        {
            currentChild = courseInformation;
            currentTableId->childnext = currentChild;
            currentTableId->dependenceTxt = courseInformation->dependenceTxt;
        }
        else
        {
            currentChild->childnext = courseInformation;
            currentChild = currentChild->childnext;
        }
    }

    //从广义表中获取某一课程类型下的所有课程信息
    CourseInformation* GetCourseInformationInGeneralList(int cTId)
    {
        currentTableId = head;
        while (currentTableId->courseTypeId != cTId)
        {
            currentTableId = currentTableId->next;
        }

        return currentTableId;
    }

    //简短广义表，删去无课程的课程类型
    void ShortGeneralList()
    {
        currentTableId = head;
        while (currentTableId != NULL)
        {
            if (currentTableId->childnext == NULL)
            {
                CourseInformation* tmp = currentTableId;
                currentTableId = currentTableId->next;
                if (tmp == head)
                {
                    head = currentTableId;
                }
                delete tmp;
                courseNum--;
            }
            else
            {
                currentTableId->courseName = currentTableId->childnext->courseName;
                currentTableId->credit = currentTableId->childnext->credit;
                currentTableId->termOnSchedule = currentTableId->childnext->termOnSchedule;
                currentTableId = currentTableId->next;
            }
        }
    }

    //获取课程种类数
    int GetTotalCourseTypeNum()
    {
        return courseNum;
    }
};

class QueueList {
private:
public:
    int size = 0;
    CourseInformation* head = NULL;
    CourseInformation* current = NULL;
    QueueList() {}

    bool In(int num)
    {
        current = head;
        while (current != NULL)
        {
            if (current->courseTypeId == num)
            {
                return true;
            }
            else
            {
                current = current->queuenext;
            }
        }

        return false;
    }

    void AddCourse(CourseInformation* course)
    {
        if (head == NULL)
        {
            head = course;
            current = course;
        }
        else
        {
            current = head;
            while (current->queuenext != NULL)
            {
                current = current->queuenext;
            }
            current->queuenext = course;
        }
        size++;
    }

    void AddCourseForTerm(CourseInformation* course)
    {
        if (head == NULL)
        {
            head = course;
            current = course;
        }
        else
        {
            current->termnext = course;
            current = current->termnext;
        }
        size++;
    }

    CourseInformation* GetCourseInNumber(int num)
    {
        current = head;
        for (int i = 0; i < num; i++)
        {
            current = current->queuenext;
        }

        return current;
    }

    void ChangeCourse(CourseInformation* front, CourseInformation* change, CourseInformation* back)
    {
        if (front->childnext == change)
        {
            front = change;
            head = change;
        }
        else
        {
            front->queuenext = change;
            change->queuenext = back;
        }
    }

    int GetSize()
    {
        return size;
    }
};

class CourseStack {
private:
    int size = 0;
public:
    CourseInformation* head = NULL;
    CourseInformation* current = NULL;

    CourseStack() {}

    void Push(CourseInformation* add)
    {
        if (head == NULL)
        {
            head = add;
            current = add;
        }
        else
        {
            current->stacknext = add;
            current = current->stacknext;
        }
        size++;
    }

    CourseInformation* Top()
    {
        return current;
    }

    void Pop()
    {
        CourseInformation* tmp = head;

        if (current == head)
        {
            head = NULL;
            current = NULL;
        }
        else
        {
            while (tmp->stacknext != current)
            {
                tmp = tmp->stacknext;
            }

            current = tmp;
            current->stacknext = NULL;
        }
        size--;
    }

    int GetSize()
    {
        return size;
    }

    string GetCourseName(int courseTypeId)
    {
        if (courseTypeId == 0)
        {
            return "";
        }
        else
        {
            current = head;
            while (current != NULL)
            {
                if (current->courseTypeId == courseTypeId)
                {
                    return current->courseName + "_" + to_string(current->courseId) + " " + current->teacherName;
                }
                current = current->stacknext;
            }
        }
        
    }
};

class Graph {
private:
    int courseNum;
    vector<CourseInformation*> adj;
public:
    vector<CourseInformation*> result; // 存储拓扑排序结果
    Graph(int cN) : courseNum(cN) {}

    void AddCourse(CourseInformation* course)
    {
        adj.push_back(course);
    }

    void AddPrerequisite(CourseInformation* before, CourseInformation* after)
    {
        after->prerequisites.push_back(before);
        before->successors.push_back(after);
    }

    void TopologicalSort()
    {
        vector<int> in_degree(courseNum, 0); // 存储每个课程的入度

        // 初始化入度
        for (int i = 0; i < courseNum; ++i)
        {
            CourseInformation* course = adj[i];
            for (CourseInformation* pre : course->prerequisites)
            {
                in_degree[i]++;
            }
        }

        // 将所有入度为0的课程放入队列
        queue<int> q;
        for (int i = 0; i < courseNum; ++i)
        {
            if (in_degree[i] == 0) {
                q.push(i);
            }
        }

        // 执行拓扑排序
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            result.push_back(adj[u]);

            for (CourseInformation* v : adj[u]->successors)
            {
                int index;
                auto it = find(adj.begin(), adj.end(), v);
                if (it != adj.end())
                {
                    index = it - adj.begin();
                }
                in_degree[index]--;
                if (in_degree[index] == 0) {
                    q.push(index);
                }
            }
        }
        if (result.size() != courseNum)
        {
            cout << "存在循环依赖课程，无法安排课表！" << endl;
            exit(0);
        }
    }
};

class CourseInformationOverview {
public:
    CourseInformationOverview() {}

    void Title(int i)
    {
        cout << "+----------------------------------------------------------------------------------------------------------------------+" << endl;
        cout << "|                                                   第" << i << "学期课表总览                                                    |" << endl;
        cout << "+----------------+--------------------+-------------------+-------------+-----------+---------------+------------------+" << endl;
        cout << "|   CourseType   |     Course Name    |    Teacher Name   |    Credit   |    Term   |    Sessions   |   Prerequisite   |" << endl;
        cout << "+----------------+--------------------+-------------------+-------------+-----------+---------------+------------------+" << endl;
    }

    void CenterOut(int a)
    {
        for (int i = 0; i < a; i++)
        {
            cout << " ";
        }
    }

    void Line(string courseTypeId, string courseName, string teacherName, string courseCredit, string courseTerm, string sessions, string limitation)
    {
        int paddingId = (16 - courseTypeId.length()) / 2;
        int paddingName = (20 - courseName.length()) / 2;
        int paddingTeacher = (19 - teacherName.length()) / 2;
        int paddingCredit = (13 - courseCredit.length()) / 2;
        int paddingTerm = (11 - courseTerm.length()) / 2;
        int paddingSessions = (15 - sessions.length()) / 2;
        int paddingLimitation = (18 - limitation.length()) / 2;

        cout << "|";
        CenterOut(paddingId);
        cout << courseTypeId;
        if (courseTypeId.length() % 2 != 0) {
            paddingId++;
        }
        CenterOut(paddingId);
        cout << "|";

        CenterOut(paddingName);
        cout << courseName;
        if (courseName.length() % 2 != 0) {
            paddingName++;
        }
        CenterOut(paddingName);
        cout << "|";

        CenterOut(paddingTeacher);
        cout << teacherName;
        if (teacherName.length() % 2 == 0) {
            paddingTeacher++;
        }
        CenterOut(paddingTeacher);
        cout << "|";

        CenterOut(paddingCredit);
        cout << courseCredit;
        if (courseCredit.length() % 2 == 0) {
            paddingCredit++;
        }
        CenterOut(paddingCredit);
        cout << "|";

        CenterOut(paddingTerm);
        cout << courseTerm;
        CenterOut(paddingTerm);
        cout << "|";

        CenterOut(paddingSessions);
        cout << sessions;
        if (sessions.length() % 2 == 0) {
            paddingSessions++;
        }
        CenterOut(paddingSessions);
        cout << "|";

         CenterOut(paddingLimitation);
        if (limitation == "0")
        {
            cout << " ";
        }
        else
        {
            cout << limitation;
        }
        CenterOut(++paddingLimitation);
        cout << "|" << endl;
        cout << "+----------------+--------------------+-------------------+-------------+-----------+---------------+------------------+" << endl;
    }
};

class ClassInEachTerm {
public:
    ClassInEachTerm() {}

    void Title()
    {
        cout << "    +-----+--------------+--------------+--------------+--------------+--------------+--------------+--------------+" << endl;
        cout << "    |                                                     课表                                                     |" << endl;
        cout << "    +-----+--------------+--------------+--------------+--------------+--------------+--------------+--------------+" << endl;
        cout << "    |     |    Monday    |    Tuesday   |   Wednesday  |   Thursday   |    Friday    |   Saturday   |    Sunday    |" << endl;
        cout << "    +-----+--------------+--------------+--------------+--------------+--------------+--------------+--------------+" << endl;
    }

    void BreakLine()
    {
        cout << "    |                                                                                                              |" << endl;
        cout << "    +-----+--------------+--------------+--------------+--------------+--------------+--------------+--------------+" << endl;
    }

    string AddLength(string txt)
    {
        int add = 42 - txt.length();
        for (int i = 0; i < add; i++)
        {
            txt += " ";
        }

        return txt;
    }

    void Out(string txt, int s, int e)
    {
        for (int i = s; i < e; i++)
        {
            cout << txt[i];
        }
    }

    void Line(int i, string mon, string tue, string wed, string thu, string fri, string sat, string sun)
    {
        int k = 0;
        for (int j = 0; j < 3; j++)
        {
            if (j == 1)
            {
                if (i + 1 >= 10)
                {

                    cout << "    | " << i + 1 << "  ";
                    cout << "|";
                }
                else
                {
                    cout << "    |  " << i + 1 << "  ";
                    cout << "|";
                }
                Out(mon, k * 14, (k + 1) * 14);
                cout << "|";
                Out(tue, k * 14, (k + 1) * 14);
                cout << "|";
                Out(wed, k * 14, (k + 1) * 14);
                cout << "|";
                Out(thu, k * 14, (k + 1) * 14);
                cout << "|";
                Out(fri, k * 14, (k + 1) * 14);
                cout << "|";
                Out(sat, k * 14, (k + 1) * 14);
                cout << "|";
                Out(sun, k * 14, (k + 1) * 14);
                cout << "|" << endl;
            }
            else
            {
                cout << "    |     ";
                cout << "|";
                Out(mon, k * 14, (k + 1) * 14);
                cout << "|";
                Out(tue, k * 14, (k + 1) * 14);
                cout << "|";
                Out(wed, k * 14, (k + 1) * 14);
                cout << "|";
                Out(thu, k * 14, (k + 1) * 14);
                cout << "|";
                Out(fri, k * 14, (k + 1) * 14);
                cout << "|";
                Out(sat, k * 14, (k + 1) * 14);
                cout << "|";
                Out(sun, k * 14, (k + 1) * 14);
                cout << "|" << endl;
            }
            k++;
        }
        cout << "    +-----+--------------+--------------+--------------+--------------+--------------+--------------+--------------+" << endl;
    }

};

void Introduction()
{
    std::string courseInfo = "四川大学计算机学院 2024-2025 学年数据结构与算法分析课程实验课作业四";
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

void MapZero(int courseMap[14][7])
{

    for (int m = 0; m < 14; m++)
    {
        for (int n = 0; n < 7; n++)
        {
            courseMap[m][n] = 0;
        }
    }
}


int main()
{
    string courseFileName = "SCU-Course.csv";
    ifstream courseFileName_csv(courseFileName, ios::in);

    if (!courseFileName_csv.is_open())
    {
        cout << "错误：SCU-Course.csv文件读取失败！" << endl;
        exit(1);
    }

    int term[8]; //存储八个学期中需要排的课程数量
    string eachTerm;
    string courseInformation;
    vector<string> courseInformations;

    //读取八个学期每个学期中需要排的课程数量
    getline(courseFileName_csv, eachTerm);
    istringstream iss(eachTerm);
    for (int i = 0; i < 8; i++)
    {
        iss >> term[i];
    }

    //读取CSV文件中每行课程的课程信息并存储到广义表中
    GeneralList eachCourseInformationGeneralList;//存储所有课程的广义表
    eachCourseInformationGeneralList.CreateGeneralList(MAX);
    QueueList eachTermTheCourseMustStudy[16]; //存储每个学期的必修课程

    while (getline(courseFileName_csv, courseInformation))
    {
        istringstream iss(courseInformation);
        string token;
        int cTId, cId, c, tOSch, sTime, eTime, wDay;
        string cName, tName, dep;

        // 读取课程类型ID
        iss >> cTId;    iss.ignore();
        // 读取课程名称
        getline(iss, cName, ' ');
        // 读取课程ID
        iss >> cId;     iss.ignore();
        // 读取教师名称
        getline(iss, tName, ' ');
        // 读取学分
        iss >> c;   iss.ignore();
        // 读取学期
        iss >> tOSch;   iss.ignore();
        // 读取上课时间
        iss >> sTime;   iss.ignore();
        // 读取下课时间
        iss >> eTime;   iss.ignore();
        // 读取周几上课
        iss >> wDay;    iss.ignore();
        // 读取依赖项数量
        iss >> dep;

        CourseInformation* tmpCourseInformation = new CourseInformation(cTId, cName, cId, tName, c, tOSch, sTime, eTime, wDay, dep);
        eachCourseInformationGeneralList.AddCourseInEachType(tmpCourseInformation); //在广义表中添加课程信息
        if (tmpCourseInformation->termOnSchedule != 0)
        {
            if (!eachTermTheCourseMustStudy[tOSch - 1].In(tmpCourseInformation->courseTypeId))
            {
                eachTermTheCourseMustStudy[tOSch - 1].AddCourse(tmpCourseInformation); //在每个学期的队列中存储学期必修课
            }
        }

    }
    eachCourseInformationGeneralList.ShortGeneralList(); //简短广义表，删去没有课程内容的课程类型

    //创建拓补图，安排每学期的课程
    Graph courseGraph(eachCourseInformationGeneralList.GetTotalCourseTypeNum());
    CourseInformation* eachCourseInformationGeneralListCurrent = eachCourseInformationGeneralList.head;
    int courseNum = eachCourseInformationGeneralList.courseNum;
    int c = 0;
    while (eachCourseInformationGeneralListCurrent != NULL && c != courseNum)
    {
        courseGraph.AddCourse(eachCourseInformationGeneralListCurrent);

        int dependenceNum = 0;
        int dependenceArrray[1000];
        string dependenceTxt = eachCourseInformationGeneralListCurrent->GetDependence();
        if (dependenceTxt != "0")
        {
            istringstream iss(dependenceTxt);
            string token;
            while (getline(iss, token, ','))
            {
                dependenceArrray[dependenceNum] = stoi(token);
                dependenceNum++;
            }

            for (int i = 0; i < dependenceNum; i++)
            {
                CourseInformation* findId = eachCourseInformationGeneralList.head;
                while (findId->courseTypeId != dependenceArrray[i] && findId != NULL)
                {
                    findId = findId->next;
                }
                // 增添一部分，用于判断依赖中是否存在以后学期才能上的必修课
                if (findId->termOnSchedule >= eachCourseInformationGeneralListCurrent->termOnSchedule)
                {
                    cout << findId->courseName << "的开设学期为第" << findId->termOnSchedule << "学期，而开设于" << eachCourseInformationGeneralListCurrent->termOnSchedule << "学期的课程" << eachCourseInformationGeneralListCurrent->courseName << "对其存在依赖！" << endl;
                    cout << "存在非法超前课程学习依赖，无法安排课表!" << endl;
                    exit(0);
                }
                else
                {
                    //一切正常的话继续程序
                    courseGraph.AddPrerequisite(findId, eachCourseInformationGeneralListCurrent);
                }
            }
        }
        eachCourseInformationGeneralListCurrent = eachCourseInformationGeneralListCurrent->next;
        c++;
    }

    // 拓补排序课程
    courseGraph.TopologicalSort(); 

    //课程信息概览
    Introduction(); //声明作者
    vector<CourseInformation*> result = courseGraph.result; //将拓补排序的课程结果取出

    for (int i = 0; i < 2; i++)
    {

        QueueList storeCurrentChild; //用于保存置入课表中的上一个必修课在广义表中的位置，以便后续调整
        CourseInformation* qListCurrent = eachTermTheCourseMustStudy[i].head;
        CourseStack courseStack;
        int courseMap[14][7] = { 0 };
        int numberOfCourseInQueue = eachTermTheCourseMustStudy[i].GetSize();
        int returntoNum = 0;

        // 创建默认上一个必修课在广义表中的位置为子表第一个元素
        for (int k = 0; k < numberOfCourseInQueue; k++)
        {
            storeCurrentChild.AddCourseForTerm(eachCourseInformationGeneralList.GetCourseInformationInGeneralList(qListCurrent->courseTypeId)->childnext);
            qListCurrent = qListCurrent->queuenext;
        }


        bool judge = false; //判断该必修课是否成功置入；
        bool rewritePermission = false;
        qListCurrent = eachTermTheCourseMustStudy[i].head;
        while (qListCurrent != NULL)
        {
            int courseTypeId = 0;
            CourseInformation* tmp = storeCurrentChild.GetCourseInNumber(returntoNum);
            //将必修课在现有的课表中插入
            while (tmp != NULL)
            {
                if (returntoNum == 0)
                {
                    MapZero(courseMap);
                }

                if (rewritePermission)
                {
                    CourseInformation* a = courseStack.Top();
                    for (int j = a->startTime - 1; j < a->endTime; j++)
                    {
                        courseMap[j][a->day - 1] = 0;
                    }
                    courseStack.Pop();
                    rewritePermission = false;

                    if (tmp->ifReutrn)
                    {
                        tmp->ifReutrn = 0;
                        courseTypeId = tmp->courseTypeId;
                        break;
                    }
                }
                if (courseMap[tmp->startTime - 1][tmp->day - 1] == 0 && courseMap[tmp->endTime - 1][tmp->day - 1] == 0)
                {
                    for (int j = tmp->startTime - 1; j < tmp->endTime; j++)
                    {
                        courseMap[j][tmp->day - 1] = tmp->courseTypeId;
                    }
                    judge = true;
                    break;
                }
                else
                {
                    if (tmp->childnext == NULL)
                    {
                        courseTypeId = tmp->courseTypeId;
                    }
                    tmp = tmp->childnext;
                    judge = false;
                }
            }

            if (judge) //如果在现有课表情况下顺利置入，则继续置入下一个必修课
            {
                eachCourseInformationGeneralList.GetCourseInformationInGeneralList(tmp->courseTypeId)->study = true;
                qListCurrent = qListCurrent->queuenext;
                if (tmp->childnext != NULL)
                {
                    CourseInformation* a = tmp;
                    tmp = tmp->childnext;
                    tmp->queuenext = a->queuenext;
                    courseStack.Push(a);
                    storeCurrentChild.ChangeCourse(storeCurrentChild.GetCourseInNumber(returntoNum - 1), tmp, storeCurrentChild.GetCourseInNumber(returntoNum + 1));
                }
                else
                {
                    tmp->ifReutrn = 1;
                    courseStack.Push(tmp);
                }
                returntoNum++;
            }
            else //表明没法在现有课表情况下顺利置入，则需要返回对上一步中的必修课时间进行调整
            {
                storeCurrentChild.ChangeCourse(storeCurrentChild.GetCourseInNumber(returntoNum - 1),
                    eachCourseInformationGeneralList.GetCourseInformationInGeneralList(courseTypeId)->childnext,
                    storeCurrentChild.GetCourseInNumber(returntoNum + 1));
                returntoNum--;
                rewritePermission = true;
                qListCurrent = eachTermTheCourseMustStudy[i].GetCourseInNumber(returntoNum);
            }

            if (returntoNum < 0)
            {
                cout << "第" << i << "学期必修课程之间始终存在冲突，无法安排课表！" << endl;
                break;
            }
        }

       
        //如果必修课课程数量未超过了学期最少课程数，则需要添加额外的其他课程
        int currentCourseSize = courseStack.GetSize();
        int t = 0;
        while (courseStack.GetSize() < term[i])
        {
            //从拓补排序中读取与当前课程不冲突的课程填入
            if (!result[t]->study && !eachCourseInformationGeneralList.GetCourseInformationInGeneralList(result[t]->courseTypeId)->study)
            {
                CourseInformation* tmp = eachCourseInformationGeneralList.GetCourseInformationInGeneralList(result[t]->courseTypeId)->childnext;
                while (tmp != NULL)
                {
                    if (courseMap[tmp->startTime - 1][tmp->day - 1] == 0 && courseMap[tmp->endTime - 1][tmp->day - 1] == 0)
                    {
                        for (int j = tmp->startTime - 1; j < tmp->endTime; j++)
                        {
                            courseMap[j][tmp->day - 1] = tmp->courseTypeId;
                        }
                        judge = true;
                        courseStack.Push(tmp);
                        eachCourseInformationGeneralList.GetCourseInformationInGeneralList(result[t]->courseTypeId)->study = true;
                        currentCourseSize++;
                        break;
                    }
                    else
                    {
                        tmp = tmp->childnext;
                    }
                }
            }
            t++;
        }

        //经过上述过程学期课程已经存储在courseStack栈中，调用输出课程概览和课表可视化
        CourseInformationOverview courseInformationOverview;
        courseInformationOverview.Title(i + 1);

        CourseInformation* stackCurrent = courseStack.head;
        while (stackCurrent != NULL)
        {
            courseInformationOverview.Line(to_string(stackCurrent->courseTypeId),
                stackCurrent->courseName + "_" + to_string(stackCurrent->courseId),
                stackCurrent->teacherName,
                to_string(stackCurrent->credit),
                to_string(stackCurrent->termOnSchedule),
                to_string(stackCurrent->day) + ": " + to_string(stackCurrent->startTime) + "-" + to_string(stackCurrent->endTime),
                stackCurrent->dependenceTxt);
            stackCurrent = stackCurrent->stacknext;
        }

        ClassInEachTerm classInEachTerm;
        classInEachTerm.Title();
        int j = 0;
        for (j = 0; j < 4; j++)
        {
            classInEachTerm.Line(j, 
                classInEachTerm.AddLength(courseStack.GetCourseName(courseMap[j][0])),
                classInEachTerm.AddLength(courseStack.GetCourseName(courseMap[j][1])),
                classInEachTerm.AddLength(courseStack.GetCourseName(courseMap[j][2])),
                classInEachTerm.AddLength(courseStack.GetCourseName(courseMap[j][3])),
                classInEachTerm.AddLength(courseStack.GetCourseName(courseMap[j][4])), 
                classInEachTerm.AddLength(courseStack.GetCourseName(courseMap[j][5])),
                classInEachTerm.AddLength(courseStack.GetCourseName(courseMap[j][6])));
        }
        classInEachTerm.BreakLine();
        for (j = 4; j < 9; j++)
        {
            classInEachTerm.Line(j,
                classInEachTerm.AddLength(courseStack.GetCourseName(courseMap[j][0])),
                classInEachTerm.AddLength(courseStack.GetCourseName(courseMap[j][1])),
                classInEachTerm.AddLength(courseStack.GetCourseName(courseMap[j][2])),
                classInEachTerm.AddLength(courseStack.GetCourseName(courseMap[j][3])),
                classInEachTerm.AddLength(courseStack.GetCourseName(courseMap[j][4])),
                classInEachTerm.AddLength(courseStack.GetCourseName(courseMap[j][5])),
                classInEachTerm.AddLength(courseStack.GetCourseName(courseMap[j][6])));
        }
        classInEachTerm.BreakLine();
        for (j = 9; j < 12; j++)
        {
            classInEachTerm.Line(j,
                classInEachTerm.AddLength(courseStack.GetCourseName(courseMap[j][0])),
                classInEachTerm.AddLength(courseStack.GetCourseName(courseMap[j][1])),
                classInEachTerm.AddLength(courseStack.GetCourseName(courseMap[j][2])),
                classInEachTerm.AddLength(courseStack.GetCourseName(courseMap[j][3])),
                classInEachTerm.AddLength(courseStack.GetCourseName(courseMap[j][4])),
                classInEachTerm.AddLength(courseStack.GetCourseName(courseMap[j][5])),
                classInEachTerm.AddLength(courseStack.GetCourseName(courseMap[j][6])));
        }
    }

    return 0;
}

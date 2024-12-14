#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>

using namespace std;

int MAX = 5;

class CourseInformation {
private:
public:
    //课程信息变量
    int courseTypeId;
    string courseName;
    int courseId;
    string teacherName;
    int credit;
    int termOnSchedule;
    int startTime;
    int endTime;
    string dependenceTxt;
    vector<CourseInformation*> prerequisites; // 先修课程列表
    vector<CourseInformation*> successors; // 后继课程列表

    //广义表变量
    int atomType = 0;//0表示为子表，1表示为原子，通过后面程序进行定义
    CourseInformation* next = NULL;
    CourseInformation* childnext = NULL;
    CourseInformation* queuenext = NULL;

    CourseInformation(int cTId) : courseTypeId(cTId) {}

    CourseInformation(int cTId, string cName, int cId, string tName, int c, int tOSch, int sTime, int eTime, string dep) {
        courseTypeId = cTId;
        courseName = cName;
        courseId = cId;
        teacherName = tName;
        credit = c;
        termOnSchedule = tOSch;
        startTime = sTime;
        endTime = eTime;
        dependenceTxt = dep;
        atomType = 1;
    }

    string GetDependence(){ return dependenceTxt; }
    void SetAtomType(int type) { atomType = type; }
};

class GeneralList {
private:
public:
    CourseInformation* head = NULL;
    CourseInformation* currentTableId = NULL;
    CourseInformation* currentChild = NULL;
    int courseNum = MAX;
    GeneralList(){}

    //创建广义表，默认创建的广义表有MAX个表链节点
    void CreateGeneralList(int size)
    {
        for (int i = 1; i <= size; i++)
        {
            CourseInformation *tmp = new CourseInformation(i);
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
    void AddCourseInEachType(CourseInformation *courseInformation)
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

        currentChild = currentTableId->childnext;

        return currentChild;
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
    CourseInformation* head = NULL;
    CourseInformation* current = NULL;
    QueueList(){}
    
    void AddCourse(CourseInformation* course)
    {
        if (head == NULL)
        {
            head = course;
            current = course;
        }
        else
        {
            current->queuenext = course;
            current = current->queuenext;
        }
    }
};

class Graph {
private:
    int courseNum;
    vector<CourseInformation*> adj;
public:
    vector<CourseInformation*> result; // 存储拓扑排序结果
    Graph(int cN) : courseNum(cN){}

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
            cout << "存在无节循环依赖课程，无法安排课表！" << endl;
        }
    }
};

class CourseInformationOverview {
public:
    CourseInformationOverview() {}

    void CenterOut(int a)
    {
        for (int i = 0; i < a; i++)
        {
            cout << " ";
        }
    }

    void Line(string courseTypeId, string courseName, string teacherName, string courseCredit, string courseTerm, string limitation)
    {
        int paddingId = (16 - courseTypeId.length()) / 2;
        int paddingName = (20 - courseName.length()) / 2;
        int paddingTeacher = (19 - teacherName.length()) / 2;
        int paddingCredit = (13 - courseCredit.length()) / 2;
        int paddingTerm = (11 - courseTerm.length()) / 2;
        int paddingLimitation = (25 - limitation.length()) / 2;

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
        CenterOut(paddingCredit);
        cout << "|";

        CenterOut(paddingTerm);
        cout << courseTerm;
        CenterOut(paddingTerm);

        cout << "|"; CenterOut(paddingLimitation);
        if (limitation == "0")
        {
            cout << " ";
        }
        else
        {
            cout << limitation;
        }
        CenterOut(paddingLimitation);
        cout << "|" << endl;
        cout << "+----------------+--------------------+-------------------+-------------+-----------+-------------------------+" << endl;
    }
};

void Introduction()
{
    std::string courseInfo = "四川大学计算机学院 2024-2025 学年数据结构与算法分析课程实验课作业三";
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
        int cTId, cId, c, tOSch, sTime, eTime;
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
        // 读取依赖项数量
        iss >> dep; 

        CourseInformation *tmpCourseInformation = new CourseInformation(cTId, cName, cId, tName, c, tOSch, sTime, eTime, dep);
        eachCourseInformationGeneralList.AddCourseInEachType(tmpCourseInformation); //在广义表中添加课程信息
        eachTermTheCourseMustStudy[tOSch - 1].AddCourse(tmpCourseInformation); //在每个学期的队列中存储学期必修课

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
                courseGraph.AddPrerequisite(findId, eachCourseInformationGeneralListCurrent);
            }
        }
        eachCourseInformationGeneralListCurrent = eachCourseInformationGeneralListCurrent->next;
        c++;
     }
    
    courseGraph.TopologicalSort();

    Introduction();
    //课程信息概览
    cout << "+----------------+--------------------+-------------------+-------------+-----------+-------------------------+" << endl;
    cout << "|   CourseType   |     Course Name    |    Teacher Name   |    Credit   |    Term   |   Prerequisite Course   |" << endl;
    cout << "+----------------+--------------------+-------------------+-------------+-----------+-------------------------+" << endl;
    vector<CourseInformation*> result = courseGraph.result;
    CourseInformationOverview courseInformationOverview;
    for (CourseInformation* v : result)
    {
        courseInformationOverview.Line(to_string(v->courseTypeId), v->courseName, v->teacherName, to_string(v->credit), to_string(v->termOnSchedule), v->dependenceTxt);
    }
	return 0;
}

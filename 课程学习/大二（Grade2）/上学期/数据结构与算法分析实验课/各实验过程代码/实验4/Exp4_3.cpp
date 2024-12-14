#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <queue>

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

    void TopologicalSort() {
        vector<CourseInformation*> result; // 存储拓扑排序结果
        vector<int> in_degree(courseNum, 0); // 存储每个课程的入度

        // 初始化入度
        for (int i = 0; i < courseNum; ++i) {
            CourseInformation* course = adj[i];
            for (CourseInformation* pre : course->prerequisites) {
                in_degree[i]++;
            }
        }

        // 将所有入度为0的课程放入队列
        queue<int> q;
        for (int i = 0; i < courseNum; ++i) {
            if (in_degree[i] == 0) {
                q.push(i);
            }
        }

        // 执行拓扑排序
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            result.push_back(adj[u]);

            for (CourseInformation* v : adj[u]->successors) {
                if (--in_degree[v - adj[0]] == 0) {
                    q.push(v - adj[0]);
                }
            }
        }

        // 检查是否存在环
        if (result.size() != courseNum) {
            cout << "Graph contains a cycle" << endl;
        }
        else {
            cout << "Topological sorting of courses:" << endl;
            for (CourseInformation* course : result) {
                cout << course->courseName << " ";
            }
            cout << endl;
        }
    }
};

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
    while (eachCourseInformationGeneralListCurrent->next != NULL)
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
                cout << findId->courseTypeId;
                courseGraph.AddPrerequisite(findId, eachCourseInformationGeneralListCurrent);
            }
        }
        eachCourseInformationGeneralListCurrent = eachCourseInformationGeneralListCurrent->next;
     }
        

	return 0;
}


//CourseInformation* head = eachTermTheCourseMustStudy[0].head;
//CourseInformation* current = head;
//while (current != NULL)
//{
//    cout << current->courseName;
//    current = current->queuenext;
//}



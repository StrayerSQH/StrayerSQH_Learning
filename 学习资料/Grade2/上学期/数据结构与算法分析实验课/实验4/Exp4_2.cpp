#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

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
    int dependenceNum = 0;
    int dependence[1000]; // 假设依赖项不会超过1000个

    //广义表变量
    int atomType = 0;//0表示为子表，1表示为原子，通过后面程序进行定义
    CourseInformation* next = NULL;
    CourseInformation* childnext = NULL;

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

        istringstream iss(dependenceTxt);
        string token;
        while (getline(iss, token, ',')) {
            dependence[dependenceNum] = stoi(token);
            dependenceNum++;
        }
    }

    void SetAtomType(int type) { atomType = type; }
};

class GeneralList {
private:
    CourseInformation* head = NULL;
    CourseInformation* currentTableId = NULL;
    CourseInformation* currentChild = NULL;
public:
    GeneralList(){}

    //创建广义表，默认创建的广义表有MAX个表链节点
    void CreateGeneralList()
    {
        for (int i = 1; i <= MAX; i++)
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
                cout << tmp->courseTypeId;
                delete tmp;
            }
            else
            {
                currentTableId = currentTableId->next;
            }
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

    //读取CSV文件中每行课程的课程信息并存储到一个名为eachCourseInformationGeneralList的广义表中
    GeneralList eachCourseInformationGeneralList;
    eachCourseInformationGeneralList.CreateGeneralList();

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
	}

    eachCourseInformationGeneralList.ShortGeneralList(); //简短广义表，删去没有课程内容的课程类型

    //创建拓补图，安排每学期的课程

	return 0;
}

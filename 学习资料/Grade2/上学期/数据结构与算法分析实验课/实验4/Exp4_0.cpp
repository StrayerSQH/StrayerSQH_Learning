#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;

class GeneralList {
private:
    CourseInformation* head = NULL;
    CourseInformation* currentTable = NULL;
    CourseInformation* current = NULL;
    CourseInformation* tableNext = NULL;
    CourseInformation* cTableNext = NULL;
public:
    GeneralList(){}

    void CreateGeneralList(CourseInformation node)
    {
        if (head = NULL)
        {
            node.SetAtomType(0);
            head = &node;
            currentTable = &node;
            current = &node;
            node.SetAtomType(1);
            cTableNext = &node;
        }
        else
        {

        }
    }

    void SetTableNext(CourseInformation node)
    {
        
    }

    CourseInformation* GetWhichPlaceToInsert(int cTId)
    {
        while (currentTable->courseTypeId != cTId)
        {
            currentTable = currentTable->tableNext;
        }
    }

    CourseInformation* SetCTableNext(CourseInformation node)
    {
        cTableNext = &node;
        return cTableNext;
    }

};

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
    CourseInformation() {};

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

        istringstream iss(dependenceTxt);
        string token;
        while (getline(iss, token, ',')) {
            dependence[dependenceNum] = stoi(token);
            dependenceNum++;
        }
    }

    string GetCourseName() { return courseName; }
    int GetCourseId() { return courseId; }
    string GetTeacherName() { return teacherName; }
    int GetCredit() { return credit; }
    int GetTermOnSchedule() { return termOnSchedule; }
    int* GetDependence() { return dependence; }
    int GetDependenceNum() { return dependenceNum; } 
    
    void SetAtomType(int type) { atomType = type; }
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

        CourseInformation tmpCourseInformation(cTId, cName, cId, tName, c, tOSch, sTime, eTime, dep);
        cout << tmpCourseInformation.GetCourseName();
	}

	return 0;
}

#ifndef COURSEINFORMATION_H
#define COURSEINFORMATION_H

#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>

using namespace std;

class CourseInformation {
private:
public:
    //�γ���Ϣ����
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
    bool study = false; // ���ڴ洢�ÿγ��Ƿ��б�ѧϰ��
    string dependenceTxt;
    vector<CourseInformation*> prerequisites; // ���޿γ��б�
    vector<CourseInformation*> successors; // ��̿γ��б�

    //��������
    int atomType = 0;//0��ʾΪ�ӱ�1��ʾΪԭ�ӣ�ͨ�����������ж���
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

#endif // !COURSEINFORMATION_H

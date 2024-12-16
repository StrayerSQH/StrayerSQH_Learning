#ifndef GENERALLIST_H
#define GENERALLIST_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>
#include "CourseInformation.h"
#include "QueueList.h"
#include "CourseStack.h"
#include "Graph.h"
#include "CourseInformationOverview.h"
#include "ClassInEachTerm.h"

using namespace std;

class GeneralList {
private:
public:
    CourseInformation* head = NULL;
    CourseInformation* currentTableId = NULL;
    CourseInformation* currentChild = NULL;
    int courseNum = 20;
    GeneralList() {}

    //���������Ĭ�ϴ����Ĺ������MAX�������ڵ�
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

    // �ڹ��������ӿγ�
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

    //�ӹ�����л�ȡĳһ�γ������µ����пγ���Ϣ
    CourseInformation* GetCourseInformationInGeneralList(int cTId)
    {
        currentTableId = head;
        while (currentTableId->courseTypeId != cTId)
        {
            currentTableId = currentTableId->next;
        }

        return currentTableId;
    }

    //��̹����ɾȥ�޿γ̵Ŀγ�����
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

    //��ȡ�γ�������
    int GetTotalCourseTypeNum()
    {
        return courseNum;
    }
};
#endif // !GENERALLIST_H


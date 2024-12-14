#include <opencv2\opencv.hpp>  

using namespace cv;
using namespace std;

int main()
{
	Mat frame,imgHSV,image,kernel;
	vector<Mat>temporary;
	int H_W_L = 0, H_W_H = 180;
	int S_W_L = 0, S_W_H = 30;
	int V_W_L = 221, V_W_H = 255;
	VideoCapture capture(1);//��ȡ������ͷʵʱ�������ݣ�0Ĭ���ǱʼǱ�������ͷ��������������ͷ�������Ϊ1

	while (true)
	{

		capture >> frame; //��ȡ��ǰ֡


		
		if (!frame.empty())
		{          //�ж��������Ƶ֡�Ƿ�Ϊ�յ�
			cvtColor(frame, imgHSV, COLOR_BGR2HSV);
			//threshold(gray, two_value, 125, 255, THRESH_BINARY);//��ֵͼ��
			
			split(imgHSV,temporary);
			equalizeHist(temporary[2], temporary[2]);
			/*equalizeHist(temporary[1], temporary[1]);
			equalizeHist(temporary[0], temporary[0]);*/
			merge(temporary, imgHSV);   //��HSVͼ��ָ�ͨ����������ֱ��ͼ�ľ��⻯

			inRange(imgHSV, Scalar(H_W_L, S_W_L, V_W_L), Scalar(H_W_H, S_W_H, V_W_H), image);

			kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
			morphologyEx(image,image,2,kernel);
			morphologyEx(image,image,3,kernel);
			imshow("ֱ��ͼ", imgHSV);
			imshow("ԭͼ",frame);
			imshow("window", image);  //��window������ʾframe����ͷ���ݻ���
		}

		if (waitKey(20) == 'q')   //��ʱ20ms,��ȡ�û��Ƿ񰴼���������������q�����Ƴ����� 
			break;
	}

	capture.release();     //�ͷ�����ͷ��Դ
	destroyAllWindows();   //�ͷ�ȫ������
	return 0;
}



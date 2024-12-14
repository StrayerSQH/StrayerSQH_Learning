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
	VideoCapture capture(1);//读取视摄像头实时画面数据，0默认是笔记本的摄像头；如果是外接摄像头，这里改为1

	while (true)
	{

		capture >> frame; //读取当前帧


		
		if (!frame.empty())
		{          //判断输入的视频帧是否为空的
			cvtColor(frame, imgHSV, COLOR_BGR2HSV);
			//threshold(gray, two_value, 125, 255, THRESH_BINARY);//二值图像
			
			split(imgHSV,temporary);
			equalizeHist(temporary[2], temporary[2]);
			/*equalizeHist(temporary[1], temporary[1]);
			equalizeHist(temporary[0], temporary[0]);*/
			merge(temporary, imgHSV);   //将HSV图像分割通道，并且做直方图的均衡化

			inRange(imgHSV, Scalar(H_W_L, S_W_L, V_W_L), Scalar(H_W_H, S_W_H, V_W_H), image);

			kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
			morphologyEx(image,image,2,kernel);
			morphologyEx(image,image,3,kernel);
			imshow("直方图", imgHSV);
			imshow("原图",frame);
			imshow("window", image);  //在window窗口显示frame摄像头数据画面
		}

		if (waitKey(20) == 'q')   //延时20ms,获取用户是否按键的情况，如果按下q，会推出程序 
			break;
	}

	capture.release();     //释放摄像头资源
	destroyAllWindows();   //释放全部窗口
	return 0;
}



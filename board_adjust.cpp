#include "board_adjust.h"

/*
method3说明：
step1:检测四个圆的圆心
step2:换算出四个角点坐标
step3:透视变换矫正
step4:裁剪变换结果，获得800*800的标准棋盘
*/

Mat board_adjust(Mat src,int* flag)
{
	/*******************寻找四个角上的圆*******************/
	Mat mid;
	cvtColor(src, mid, COLOR_BGR2GRAY);
	vector<Vec3f> circles;
	HoughCircles(mid, circles, HOUGH_GRADIENT, 1.5, 600, 180, 30, 15, 25);

	/*如果找到的圆不是4个，那么停止程序*/
	if (circles.size() != 4)
	{
		cout << "没有正确找到四个角点的圆" << endl;
		system("pause");
		*flag = 0;
		Mat dst= Mat::zeros(800, 800, CV_8UC3);
		return dst;
	}

	/*若找到4个角点圆，画出图中的圆，显示识别效果*/
	Scalar color[4] = { Scalar(255,0,0),Scalar(0,255,0),Scalar(0,0,255),
		Scalar(0,255,255) };
	Point2f findCorner[4] = {};
	for (size_t i = 0; i < circles.size(); i++)
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		findCorner[i] = center;
		cout << "角点" << i + 1 << "圆心坐标：" << findCorner[i] <<"  ";
		int radius = cvRound(circles[i][2]);
		cout << "角点" << i + 1 << "半径：" << radius << endl;
		circle(src, center, radius, color[i], 3, 8, 0);
	}
	//imshow("寻找四个角点的结果", src);

	/************************ 透视变换 *************************/

	/*给找到的四个角点坐标排序，使与标准角点一一对应*/
	int index = 0;
	Point2f srcCorner[4] = {};
	for (int i = 0; i < 4; i++)
	{
		if (findCorner[i].x < 500 && findCorner[i].y < 500)//找到的第一个角点为左上角的点
		{
			index = 0;
		}
		else if (findCorner[i].x < 500 && findCorner[i].y>500)//找到的第一个角点为左下角的点
		{
			index = 1;
		}
		else if (findCorner[i].x > 500 && findCorner[i].y > 500)//找到的第一个角点为右下角的点
		{
			index = 2;
		}
		else //找到的第一个角点为右上角的点
		{
			index = 3;
		}
		srcCorner[index] = findCorner[i];
	}
	cout << endl;
	cout << "找到的四个角点的坐标排序的结果如下：" << endl;
	cout << "第1个找到的角点坐标:" << srcCorner[0] << endl;
	cout << "第2个找到的角点坐标:" << srcCorner[1] << endl;
	cout << "第3个找到的角点坐标:" << srcCorner[2] << endl;
	cout << "第4个找到的角点坐标:" << srcCorner[3] << endl;
	cout << endl;

	/*设定标准棋盘的四个角点的坐标*/
	Point2f dstCorner[4] = { { 40,40 },{ 40,760 },{ 760,760 },{ 760,40 } };
	cout << "设定的四个标准角点的坐标如下：" << endl;
	cout << "第1个标准角点坐标" << dstCorner[0] << endl;
	cout << "第2个标准角点坐标" << dstCorner[1] << endl;
	cout << "第3个标准角点坐标" << dstCorner[2] << endl;
	cout << "第4个标准角点坐标" << dstCorner[3] << endl;
	cout << endl;

	/*坐标一一对应之后就执行透视变换*/
	Mat dst;
	Mat transArray = getPerspectiveTransform(srcCorner, dstCorner);
	warpPerspective(src, dst, transArray, dst.size(), INTER_LINEAR, BORDER_CONSTANT);
	//imshow("矫正后的棋盘", dst);

	/*裁剪矫正的结果，获得标准棋盘*/
	Mat cutdst = dst(Rect(0, 0, 800, 800));
	//imshow("裁剪矫正后的棋盘的结果", cutdst);

	/*返回转换出来的标准棋盘*/
	*flag = 1;
	return cutdst;
}
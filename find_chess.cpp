#include "find_chess.h"
extern Mat src;

bool get_best_circles(vector<Vec3f> circles[], vector<Vec3f>&best_circles)
{

	/*判断寻找到的圆的数量是否相等*/
	if (circles[0].size() != circles[1].size() || circles[0].size() != circles[2].size() ||
		circles[0].size() != circles[3].size())
	{
		cout << "错误：找到的圆的数量不相等！" << endl;
		return false;
	}
	/*将旋转得到的圆通过坐标变换回到未旋转的圆心坐标*/
	//对旋转90度的图像的还原
	for (int i = 0; i < circles[1].size(); i++)
	{
		int x = circles[1][i][0]-400;
		int y = circles[1][i][1]-400;
		int x1, y1;
		y1 = x;
		x1 = -y;
		circles[1][i][0] = x1 + 400;
		circles[1][i][1] = y1 + 400;
		cout << "逆时针90度的图片坐标转换回来的结果:" << " " << "x: " << circles[1][i][0] << "y: " << circles[1][i][1] << endl;
	}
	cout << endl;

	//对旋转180度的图像的还原
	for (int i = 0; i < circles[2].size(); i++)
	{
		int x = circles[2][i][0] - 400;
		int y = circles[2][i][1] - 400;
		int x1, y1;
		y1 = -y;
		x1 = -x;
		circles[2][i][0] = x1 + 400;
		circles[2][i][1] = y1 + 400;
		cout << "逆时针180度的图片坐标转换回来的结果:" << " " << " x: " << circles[2][i][0] << " y: " << circles[2][i][1] << endl;
	}
	cout << endl;

	//对旋转270度的图像的还原
	for (int i = 0; i < circles[3].size(); i++)
	{
		int x = circles[3][i][0] - 400;
		int y = circles[3][i][1] - 400;
		int x1, y1;
		y1 =-x;
		x1 = y;
		circles[3][i][0] = x1 + 400;
		circles[3][i][1] = y1 + 400;
		cout << "逆时针270度的图片坐标转换回来的结果:" << " " << " x: " << circles[3][i][0] << " y: " << circles[3][i][1] << endl;
	}
	cout << endl;

	/*将旋转全部还原后，确定哪些圆是代表着同一个棋子的圆*/
	float X[16] = { 0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0,0 };
	float Y[16] = { 0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0,0 };
	float R[16] = { 0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0,0 };
	for (int i = 0; i < circles[0].size(); i++)
	{
		X[i] = circles[0][i][0];
		Y[i] = circles[0][i][1];
		R[i] = circles[0][i][2];
		cout << "逆时针0度的图片坐标转换回来的结果:" << " " << " x: " << circles[0][i][0] << " y: " << circles[0][i][1] << endl;
	}
	for(int i=1;i<4;i++)
		for (int j = 0; j < circles[i].size(); j++)
		{
			int label = 0;
			float min_dis = 100;
			float dis = 0;
			for (int k = 0; k < circles[0].size(); k++)
			{
				float x1 = circles[i][j][0];
				float y1 = circles[i][j][1];
				float x2 = circles[0][k][0];
				float y2 = circles[0][k][1];
				dis = (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2);
				if(dis<min_dis)
				{
					label = k;
					min_dis=dis;
				}
					
			}
			X[label] += circles[i][j][0];
			Y[label] += circles[i][j][1];
			R[label] += circles[i][j][2];
		}

	//取平均
	cout << endl;
	for (int i = 0; i < circles[0].size(); i++)
	{
		Vec3f cir;
		cir[0] = X[i] / 4;
		cir[1] = Y[i] / 4;
		cir[2] = R[i] / 4;
		best_circles.push_back(cir);
		cout << "best_circles" << i << " x: " << best_circles[i][0] << " y: " << best_circles[i][1] << " r: " << best_circles[i][2] << endl;
	}

	return true;
}

bool find_chess(Mat picture, vector<Vec3f>&best_circles)
{
	/*利用图像旋转得到4个方向的图像:逆时针旋转*/
	Mat picture_to_findchess[4] = {};
	Point2f rotation_center(picture.rows / 2, picture.cols / 2);//旋转中心
	double angle = 90;		//旋转角度为90
	picture_to_findchess[0] = picture.clone();

	for (int i = 1; i < 4; i++)
	{
		//获取旋转变换用的矩阵
		Mat rotation_array = getRotationMatrix2D(rotation_center, angle, 1.0);
		//通过仿射变换进行旋转
		warpAffine(picture_to_findchess[i - 1], picture_to_findchess[i], rotation_array, picture_to_findchess[i - 1].size());
	}

	vector<Vec3f> circles[4] = {};

	/*分别寻找四个方向的图像中的圆*/
	for (int j = 0; j < 4; j++)
	{
		/*利用霍夫变换寻找圆*/
		
		HoughCircles(picture_to_findchess[j], circles[j], HOUGH_GRADIENT, 1.5, 20, 300, 80, 20, 40);

		/*画出图中的圆，显示识别效果*/
		for (size_t i = 0; i < circles[j].size(); i++)
		{
			Point center(cvRound(circles[j][i][0]), cvRound(circles[j][i][1]));
			int radius = cvRound(circles[j][i][2]);

			//circle(picture_to_findchess[i], center, radius, Scalar(249, 249, 249), 16, 8, 0);
			circle(picture_to_findchess[j], center, radius, Scalar(0, 0, 255), 3, 8, 0);
		}
	}

	imshow("寻找棋子效果图1",picture_to_findchess[0]);
	imshow("寻找棋子效果图2", picture_to_findchess[1]);
	imshow("寻找棋子效果图3", picture_to_findchess[2]);
	imshow("寻找棋子效果图4", picture_to_findchess[3]); 

	imwrite("寻找棋子效果图1.jpg", picture_to_findchess[0]);
	imwrite("寻找棋子效果图2.jpg", picture_to_findchess[1]);
	imwrite("寻找棋子效果图3.jpg", picture_to_findchess[2]);
	imwrite("寻找棋子效果图4.jpg", picture_to_findchess[3]);
	/*从四个图像中的圆中整理出一组平均的圆：为了获得更准确的中心坐标*/

	bool flag = false;
	flag=get_best_circles(circles,best_circles);
	if (flag == false)
	{
		return false;
	}
	else
	{
		/*画出图中的圆，显示识别效果*/
		for (size_t i = 0; i < best_circles.size(); i++)
		{
			Point center(cvRound(best_circles[i][0]), cvRound(best_circles[i][1]));
			int radius = cvRound(best_circles[i][2]);

			circle(picture, center, radius, Scalar(249, 249, 249), 13, 8, 0);
			circle(src, center, radius, Scalar(0, 0, 255), 3, 8, 0);
			imshow("最终的找棋子效果", src);
		}
	}
	return true;
}
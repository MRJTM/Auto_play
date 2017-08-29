#include "find_chess.h"
extern Mat src;

bool get_best_circles(vector<Vec3f> circles[], vector<Vec3f>&best_circles)
{

	/*�ж�Ѱ�ҵ���Բ�������Ƿ����*/
	if (circles[0].size() != circles[1].size() || circles[0].size() != circles[2].size() ||
		circles[0].size() != circles[3].size())
	{
		cout << "�����ҵ���Բ����������ȣ�" << endl;
		return false;
	}
	/*����ת�õ���Բͨ������任�ص�δ��ת��Բ������*/
	//����ת90�ȵ�ͼ��Ļ�ԭ
	for (int i = 0; i < circles[1].size(); i++)
	{
		int x = circles[1][i][0]-400;
		int y = circles[1][i][1]-400;
		int x1, y1;
		y1 = x;
		x1 = -y;
		circles[1][i][0] = x1 + 400;
		circles[1][i][1] = y1 + 400;
		cout << "��ʱ��90�ȵ�ͼƬ����ת�������Ľ��:" << " " << "x: " << circles[1][i][0] << "y: " << circles[1][i][1] << endl;
	}
	cout << endl;

	//����ת180�ȵ�ͼ��Ļ�ԭ
	for (int i = 0; i < circles[2].size(); i++)
	{
		int x = circles[2][i][0] - 400;
		int y = circles[2][i][1] - 400;
		int x1, y1;
		y1 = -y;
		x1 = -x;
		circles[2][i][0] = x1 + 400;
		circles[2][i][1] = y1 + 400;
		cout << "��ʱ��180�ȵ�ͼƬ����ת�������Ľ��:" << " " << " x: " << circles[2][i][0] << " y: " << circles[2][i][1] << endl;
	}
	cout << endl;

	//����ת270�ȵ�ͼ��Ļ�ԭ
	for (int i = 0; i < circles[3].size(); i++)
	{
		int x = circles[3][i][0] - 400;
		int y = circles[3][i][1] - 400;
		int x1, y1;
		y1 =-x;
		x1 = y;
		circles[3][i][0] = x1 + 400;
		circles[3][i][1] = y1 + 400;
		cout << "��ʱ��270�ȵ�ͼƬ����ת�������Ľ��:" << " " << " x: " << circles[3][i][0] << " y: " << circles[3][i][1] << endl;
	}
	cout << endl;

	/*����תȫ����ԭ��ȷ����ЩԲ�Ǵ�����ͬһ�����ӵ�Բ*/
	float X[16] = { 0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0,0 };
	float Y[16] = { 0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0,0 };
	float R[16] = { 0,0,0,0,0 ,0,0,0,0,0 ,0,0,0,0,0,0 };
	for (int i = 0; i < circles[0].size(); i++)
	{
		X[i] = circles[0][i][0];
		Y[i] = circles[0][i][1];
		R[i] = circles[0][i][2];
		cout << "��ʱ��0�ȵ�ͼƬ����ת�������Ľ��:" << " " << " x: " << circles[0][i][0] << " y: " << circles[0][i][1] << endl;
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

	//ȡƽ��
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
	/*����ͼ����ת�õ�4�������ͼ��:��ʱ����ת*/
	Mat picture_to_findchess[4] = {};
	Point2f rotation_center(picture.rows / 2, picture.cols / 2);//��ת����
	double angle = 90;		//��ת�Ƕ�Ϊ90
	picture_to_findchess[0] = picture.clone();

	for (int i = 1; i < 4; i++)
	{
		//��ȡ��ת�任�õľ���
		Mat rotation_array = getRotationMatrix2D(rotation_center, angle, 1.0);
		//ͨ������任������ת
		warpAffine(picture_to_findchess[i - 1], picture_to_findchess[i], rotation_array, picture_to_findchess[i - 1].size());
	}

	vector<Vec3f> circles[4] = {};

	/*�ֱ�Ѱ���ĸ������ͼ���е�Բ*/
	for (int j = 0; j < 4; j++)
	{
		/*���û���任Ѱ��Բ*/
		
		HoughCircles(picture_to_findchess[j], circles[j], HOUGH_GRADIENT, 1.5, 20, 300, 80, 20, 40);

		/*����ͼ�е�Բ����ʾʶ��Ч��*/
		for (size_t i = 0; i < circles[j].size(); i++)
		{
			Point center(cvRound(circles[j][i][0]), cvRound(circles[j][i][1]));
			int radius = cvRound(circles[j][i][2]);

			//circle(picture_to_findchess[i], center, radius, Scalar(249, 249, 249), 16, 8, 0);
			circle(picture_to_findchess[j], center, radius, Scalar(0, 0, 255), 3, 8, 0);
		}
	}

	imshow("Ѱ������Ч��ͼ1",picture_to_findchess[0]);
	imshow("Ѱ������Ч��ͼ2", picture_to_findchess[1]);
	imshow("Ѱ������Ч��ͼ3", picture_to_findchess[2]);
	imshow("Ѱ������Ч��ͼ4", picture_to_findchess[3]); 

	imwrite("Ѱ������Ч��ͼ1.jpg", picture_to_findchess[0]);
	imwrite("Ѱ������Ч��ͼ2.jpg", picture_to_findchess[1]);
	imwrite("Ѱ������Ч��ͼ3.jpg", picture_to_findchess[2]);
	imwrite("Ѱ������Ч��ͼ4.jpg", picture_to_findchess[3]);
	/*���ĸ�ͼ���е�Բ�������һ��ƽ����Բ��Ϊ�˻�ø�׼ȷ����������*/

	bool flag = false;
	flag=get_best_circles(circles,best_circles);
	if (flag == false)
	{
		return false;
	}
	else
	{
		/*����ͼ�е�Բ����ʾʶ��Ч��*/
		for (size_t i = 0; i < best_circles.size(); i++)
		{
			Point center(cvRound(best_circles[i][0]), cvRound(best_circles[i][1]));
			int radius = cvRound(best_circles[i][2]);

			circle(picture, center, radius, Scalar(249, 249, 249), 13, 8, 0);
			circle(src, center, radius, Scalar(0, 0, 255), 3, 8, 0);
			imshow("���յ�������Ч��", src);
		}
	}
	return true;
}
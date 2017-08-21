#include "board_adjust.h"

/*
method3˵����
step1:����ĸ�Բ��Բ��
step2:������ĸ��ǵ�����
step3:͸�ӱ任����
step4:�ü��任��������800*800�ı�׼����
*/

Mat board_adjust(Mat src,int* flag)
{
	/*******************Ѱ���ĸ����ϵ�Բ*******************/
	Mat mid;
	cvtColor(src, mid, COLOR_BGR2GRAY);
	vector<Vec3f> circles;
	HoughCircles(mid, circles, HOUGH_GRADIENT, 1.5, 600, 180, 30, 15, 25);

	/*����ҵ���Բ����4������ôֹͣ����*/
	if (circles.size() != 4)
	{
		cout << "û����ȷ�ҵ��ĸ��ǵ��Բ" << endl;
		system("pause");
		*flag = 0;
		Mat dst= Mat::zeros(800, 800, CV_8UC3);
		return dst;
	}

	/*���ҵ�4���ǵ�Բ������ͼ�е�Բ����ʾʶ��Ч��*/
	Scalar color[4] = { Scalar(255,0,0),Scalar(0,255,0),Scalar(0,0,255),
		Scalar(0,255,255) };
	Point2f findCorner[4] = {};
	for (size_t i = 0; i < circles.size(); i++)
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		findCorner[i] = center;
		cout << "�ǵ�" << i + 1 << "Բ�����꣺" << findCorner[i] <<"  ";
		int radius = cvRound(circles[i][2]);
		cout << "�ǵ�" << i + 1 << "�뾶��" << radius << endl;
		circle(src, center, radius, color[i], 3, 8, 0);
	}
	//imshow("Ѱ���ĸ��ǵ�Ľ��", src);

	/************************ ͸�ӱ任 *************************/

	/*���ҵ����ĸ��ǵ���������ʹ���׼�ǵ�һһ��Ӧ*/
	int index = 0;
	Point2f srcCorner[4] = {};
	for (int i = 0; i < 4; i++)
	{
		if (findCorner[i].x < 500 && findCorner[i].y < 500)//�ҵ��ĵ�һ���ǵ�Ϊ���Ͻǵĵ�
		{
			index = 0;
		}
		else if (findCorner[i].x < 500 && findCorner[i].y>500)//�ҵ��ĵ�һ���ǵ�Ϊ���½ǵĵ�
		{
			index = 1;
		}
		else if (findCorner[i].x > 500 && findCorner[i].y > 500)//�ҵ��ĵ�һ���ǵ�Ϊ���½ǵĵ�
		{
			index = 2;
		}
		else //�ҵ��ĵ�һ���ǵ�Ϊ���Ͻǵĵ�
		{
			index = 3;
		}
		srcCorner[index] = findCorner[i];
	}
	cout << endl;
	cout << "�ҵ����ĸ��ǵ����������Ľ�����£�" << endl;
	cout << "��1���ҵ��Ľǵ�����:" << srcCorner[0] << endl;
	cout << "��2���ҵ��Ľǵ�����:" << srcCorner[1] << endl;
	cout << "��3���ҵ��Ľǵ�����:" << srcCorner[2] << endl;
	cout << "��4���ҵ��Ľǵ�����:" << srcCorner[3] << endl;
	cout << endl;

	/*�趨��׼���̵��ĸ��ǵ������*/
	Point2f dstCorner[4] = { { 40,40 },{ 40,760 },{ 760,760 },{ 760,40 } };
	cout << "�趨���ĸ���׼�ǵ���������£�" << endl;
	cout << "��1����׼�ǵ�����" << dstCorner[0] << endl;
	cout << "��2����׼�ǵ�����" << dstCorner[1] << endl;
	cout << "��3����׼�ǵ�����" << dstCorner[2] << endl;
	cout << "��4����׼�ǵ�����" << dstCorner[3] << endl;
	cout << endl;

	/*����һһ��Ӧ֮���ִ��͸�ӱ任*/
	Mat dst;
	Mat transArray = getPerspectiveTransform(srcCorner, dstCorner);
	warpPerspective(src, dst, transArray, dst.size(), INTER_LINEAR, BORDER_CONSTANT);
	//imshow("�����������", dst);

	/*�ü������Ľ������ñ�׼����*/
	Mat cutdst = dst(Rect(0, 0, 800, 800));
	//imshow("�ü�����������̵Ľ��", cutdst);

	/*����ת�������ı�׼����*/
	*flag = 1;
	return cutdst;
}
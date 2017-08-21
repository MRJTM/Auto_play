#include "find_chess.h"
extern Mat src;
vector<Vec3f> find_chess(Mat picture)
{
	/*���û���任Ѱ��Բ*/
	vector<Vec3f> circles;
	HoughCircles(picture, circles, HOUGH_GRADIENT, 1.5, 20, 200, 105, 20, 70);

	/*����ͼ�е�Բ����ʾʶ��Ч��*/
	for (size_t i=0;i<circles.size();i++)
	{
	Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
	int radius = cvRound(circles[i][2]);

	circle(picture, center, radius, Scalar(249, 249, 249), 16, 8, 0);
	circle(src, center, radius, Scalar(0, 0, 255), 3, 8, 0);
	}
	imshow("Ѱ������Ч��ͼ",src);

	return circles;
}
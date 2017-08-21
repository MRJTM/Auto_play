#include "find_chess.h"
extern Mat src;
vector<Vec3f> find_chess(Mat picture)
{
	/*利用霍夫变换寻找圆*/
	vector<Vec3f> circles;
	HoughCircles(picture, circles, HOUGH_GRADIENT, 1.5, 20, 200, 105, 20, 70);

	/*画出图中的圆，显示识别效果*/
	for (size_t i=0;i<circles.size();i++)
	{
	Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
	int radius = cvRound(circles[i][2]);

	circle(picture, center, radius, Scalar(249, 249, 249), 16, 8, 0);
	circle(src, center, radius, Scalar(0, 0, 255), 3, 8, 0);
	}
	imshow("寻找棋子效果图",src);

	return circles;
}
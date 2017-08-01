#include "position.h"
#include "char_match.h"

String chess_name[14] = { "r_general","r_advisor","r_elephant","r_chariot","r_horse","r_cannon","r_solider",
"b_general","b_advisor","b_elephant","b_chariot","b_horse","b_cannon","b_solider" };
String piece_name1[16] = { "a1.png","a2.png","a3.png","a4.png","a5.png","a6.png","a7.png",
"a8.png","a9.png","a10.png","a11.png","a12.png","a13.png","a14.png","a15.png","a16.png" };

int dst_X[32] = {0,  0,0,  0,0,  0,0,  0,0,  0,0,  0,	0, 0, 0, 0,
				 0,  30,-25,  60,-55,  105,-105,   80,-77,  80,-80,  105,  55,  0,  -55,  -100};
int dst_Y[32] = { 0,  0,0,  0,0,  0,0,  0,0,  0,0, 0,  0,  0,  0,  0, 
			  135,  135,135,  135,135,  130,140,  135,135,  180,180,  200, 200, 200, 200, 200};

int calculate_position(int label, int key[])
{
	int LABEL = 0;
	if (label == 0)
		LABEL = label;
	else if (label < 7)
		LABEL = 2 * label - 1;
	else if (label == 7)
		LABEL = 16;
	else
		LABEL = 2 * label + 1;

	for (int i = 0; i < 4; i++)
	{
		if (key[LABEL] == 0)
		{
			key[LABEL] = 1;
			break;
		}
		else
			LABEL++;
	}
	return LABEL;
}

Point get_src_position(Point figure_position)
{
	Point src_position;
	src_position.x = 2 * figure_position.x + 10;
	src_position.y = 2 * figure_position.y + 10;
	return src_position;
}

Point get_dst_position(int index)
{
	Point dst_position;
	dst_position.x = dst_X[index];
	dst_position.y = dst_Y[index];
	return dst_position;
}

void get_position(Mat src,vector<Vec3f> circles,Mat char_lib[],
					Point src_position[], Point dst_position[])
{
	int key[32];
	for (int k = 0; k < 31; k++)
	{
		key[k] = 0;
	}
	double score = 0;
	for (int j = 0; j < circles.size(); j++)
	{
		/*获取棋子切片*/
		Mat piece;
		int r = 24;
		piece = src(Rect(circles[j][0] - r, circles[j][1] - r, 2 * r, 2 * r));

		/*调整切片的大小，方便进行匹配*/
		Mat dst_piece = Mat::zeros(150, 150, CV_8UC1);
		resize(piece, dst_piece, dst_piece.size());
		//imshow(piece_name[j], dst_piece);
		//imwrite(piece_name1[j], dst_piece);

		/*识别棋子切片属于什么棋子*/
		double minsocre = 100;
		int label = 0;
		for (int l = 0; l < 14; l++)
		{
			//计算相似度，score越小越相似
			score = 10 * calculate_similarity(dst_piece, char_lib[l]);
			//确定最匹配的对象;
			if (score < minsocre)
			{
				minsocre = score;
				label = l;
			}
		}
		cout << piece_name1[j] << ":" << label << " ";

		/*识别出棋子后，为每个棋子分配它们的终点坐标*/
		int index = calculate_position(label, key);
		dst_position[j] = get_dst_position(index);
		cout << ":" << index << "  ";
		cout << "dst_position" << dst_position[j] << endl;

		/*计算棋子的起点坐标*/
		Point center(cvRound(circles[j][0]), cvRound(circles[j][1]));
		//src_position[j] = grt_src_position(center);

	}
}




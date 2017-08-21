#include "position.h"
#include "char_match.h"

//ÿ�����ӵ�����
String chess_name[14] = { "r_general","r_advisor","r_elephant","r_chariot","r_horse","r_cannon","r_solider",
"b_general","b_advisor","b_elephant","b_chariot","b_horse","b_cannon","b_solider" };

//ÿ����Ƭ������
String piece_name1[16] = { "a1.png","a2.png","a3.png","a4.png","a5.png","a6.png","a7.png",
"a8.png","a9.png","a10.png","a11.png","a12.png","a13.png","a14.png","a15.png","a16.png" };

//�յ��X��Y����
int dst_X[32] = {0,  0,0,  0,0,  0,0,  0,0,  0,0,  0,	0, 0, 0, 0,
				 4,  30,-25,  58,-53,  109,-101,   80,-77,  82,-77,  105,  58,  2,  -52,  -100};
int dst_Y[32] = { 0,  0,0,  0,0,  0,0,  0,0,  0,0, 0,  0,  0,  0,  0, 
			  135,  135,135,  135,135,  130,135,  135,135,  176,180,  197, 197, 200, 200, 200};

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
	src_position.y = (int)(6.0 / 19 * figure_position.x + 100.37);
	src_position.x = (int)(139.0 / 428 * figure_position.y - 125.77);
	cout << "���ص�ַ" << "x:" << figure_position.x;
	cout << " " << "y:" << figure_position.y << "   ";
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
		/*��ȡ������Ƭ*/
		Mat piece;
		int r = 30;
		piece = src(Rect(circles[j][0] - r, circles[j][1] - r, 2 * r, 2 * r));

		/*������Ƭ�Ĵ�С���������ƥ��*/
		Mat dst_piece = Mat::zeros(60, 60, CV_8UC1);
		resize(piece, dst_piece, dst_piece.size());
		//imshow(piece_name[j], dst_piece);
		imwrite(piece_name1[j], dst_piece);

		/*ʶ��������Ƭ����ʲô����*/
		double minsocre = 100;
		int label = 0;
		for (int l = 7; l < 14; l++)//l��7��ʼ������������ƥ����̣��ӿ��ٶ�
		{
			//�������ƶȣ�scoreԽСԽ����
			score = 10 * calculate_similarity(dst_piece, char_lib[l]);
			//ȷ����ƥ��Ķ���;
			if (score < minsocre)
			{
				minsocre = score;
				label = l;
			}
		}
		cout << piece_name1[j] << ":ʶ��Ϊ:" << chess_name[label] << "   ";

		/*ʶ������Ӻ�Ϊÿ�����ӷ������ǵ��յ�����*/
		int index = calculate_position(label, key);
		dst_position[j] = get_dst_position(index);
		cout << "���ӱ��:" << index << "  ";
		cout << "�յ����꣺" << dst_position[j] <<"   ";

		/*�������ӵ��������*/
		Point center(cvRound(circles[j][0]), cvRound(circles[j][1]));
		src_position[j] = get_src_position(center);
		cout << "������꣺" << src_position[j] << endl;

	}
}




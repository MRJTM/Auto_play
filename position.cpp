#include "position.h"
#include "char_match.h"

//ÿ�����ӵ�����
String chess_name[14] = { "r_general","r_advisor","r_elephant","r_chariot","r_horse","r_cannon","r_solider",
"b_general","b_advisor","b_elephant","b_chariot","b_horse","b_cannon","b_solider" };

//ÿ����Ƭ������
String piece_name[16] = { "a1.png","a2.png","a3.png","a4.png","a5.png","a6.png","a7.png",
"a8.png","a9.png","a10.png","a11.png","a12.png","a13.png","a14.png","a15.png","a16.png" };

//�յ��X��Y����
int dst_X[32] = {0,  0,0,  0,0,  0,0,  0,0,  0,0,  0,	0, 0, 0, 0,
				 4,  30,-27,  58,-53,  109,-105,   83,-79,  82,-77,  105,  58,  2,  -52,  -100};
int dst_Y[32] = { 0,  0,0,  0,0,  0,0,  0,0,  0,0, 0,  0,  0,  0,  0, 
			  135,  135,135,  135,135,  135,135,  135,135,  176,180,  197, 197, 200, 200, 200};

//Ϊÿ�����ӷ���һ����λ���ܹ�32����λ����Ϊ�ڶ���ʶ����ͬ��������Ҫ�ŵ���ͬ��λ�ò���
int key[32] = { 0,0,0,0,0,  0,0,0,0,0,  0,0,0,0,0, 0,0,0,0,0, 
				0,0,0,0,0,  0,0,0,0,0,  0,0 };


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
	float x = figure_position.x;
	float y = figure_position.y;
	src_position.y = (int)(-1.648e-5*x*x+0.2995*x+114.7);
	src_position.x = (int)(-1.933e-7*y*y*y+2.714e-4*y*y+0.2033*y-114.3);
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

void get_position(Mat src,vector<Vec3f> circles,Mat char_lib[],int circles_index,
					Point &src_position, Point &dst_position)
{
	/*��ȡ������Ƭ*/
	Mat piece[8] = {};
	int r = 30;
	piece[0] = src(Rect(circles[circles_index][0] - r, circles[circles_index][1] - r, 2 * r, 2 * r));

	/*������Ƭ�Ĵ�С���������ƥ��*/
	int r_piece = 60;//��Ƭ�ı߳�

	Mat dst_piece = Mat::zeros(r_piece, r_piece, CV_8UC1);
	resize(piece[0], piece[0], dst_piece.size());

	imwrite(piece_name[circles_index], piece[0]);
	

	/*������ת���8�������ͼƬ*/
	Point2f rotation_center(r_piece/2, r_piece/2);//��ת����
	double angle = 45;		//��ת�Ƕ�Ϊ45��
	for (int i = 1; i < 8; i++)
	{
		//��ȡ��ת�任�õľ���
		Mat rotation_array = getRotationMatrix2D(rotation_center, angle, 1.0);
		//ͨ������任������ת
		warpAffine(piece[i - 1], piece[i], rotation_array, piece[i - 1].size());
	}
		
	/*��8���������Ƭȥ����ƥ�䣬ȷ��������ʲô����*/
	int LABEL[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	//����Ƭ��8����ͬ�ķ�����ȥʶ��һ�£�ͳ��ʶ����,����LABEL[]��
	for (int i = 0; i < 8; i++)
	{
		double minsocre = 100;
		int tag = 0;
		for (int l = 7; l < 14; l++)//l��7��ʼ������������ƥ����̣��ӿ��ٶ�
		{		
			//�������ƶȣ�scoreԽСԽ����
			double score = 10 * calculate_similarity(piece[i], char_lib[l]);
			//ȷ��piece[i]ʶ��Ϊ�ĸ�����;
			if (score < minsocre)
			{
				minsocre = score;
				tag = l;
			}
		}
		LABEL[tag]++;
	}
	//ͳ��8����Ƭ��౻ʶ��Ϊ�ĸ����ӣ�����Ϊ��Ƭ���ĸ�����
	int label = 0;
	int largest = 0;
	for (int i = 0; i < 14; i++)
	{
		if (LABEL[i] > largest)
		{
			largest = LABEL[i];
			label = i;
		}
	}
	//���ʶ����
	cout << piece_name[circles_index] << ":ʶ��Ϊ:" << chess_name[label] << "   ";


	/*ʶ������Ӻ�ȷ�������ӵ��յ�����*/
	int index = calculate_position(label, key);
	dst_position = get_dst_position(index);
	cout << "���ӱ��:" << index << "  ";
	cout << "�յ����꣺" << dst_position <<"   ";

	/*�������ӵ��������*/
	Point center(cvRound(circles[circles_index][0]), cvRound(circles[circles_index][1]));
	src_position = get_src_position(center);
	cout << "������꣺" << src_position<< endl;
}




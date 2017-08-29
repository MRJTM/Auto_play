#include "char_match.h"

//A,Bλ�Ƚ����ƶȵ�����ͼƬ��simΪ����������ƶ�
double calculate_similarity(Mat A, Mat B)
{
	//����SURF�������ȡ������
	Ptr<Feature2D> surf = xfeatures2d::SURF::create();
	std::vector<KeyPoint>KeyPointsA, KeyPointsB;
	surf->detect(A, KeyPointsA);
	surf->detect(B, KeyPointsB);

	//������������
	Mat descrpA, descrpB;
	surf->compute(A, KeyPointsA, descrpA);
	surf->compute(B, KeyPointsB, descrpB);

	//ʹ�ñ���ƥ�䷽����Ѱ��������ƥ���
	BFMatcher matcher;
	//FlannBasedMatcher matcher;
	vector<DMatch>matches;
	matcher.match(descrpA, descrpB, matches);

	//��matches�е������԰��վ����С��������
	sort(matches.begin(), matches.end());

	//�������Ƴ̶�
	int num_matches = 3;
	double score_similarity = 0;
	for (int j = 0; j < num_matches; j++)
	{
		score_similarity += matches[j].distance;
	}

	return score_similarity*10/ num_matches;
}

//�����ֿ�
void load_char_lib(Mat char_lib[])
{
	char_lib[0] = imread("r_general.png", 0);	//�졪��˧
	char_lib[1] = imread("r_advisor.png", 0);	//�졪����
	char_lib[2] = imread("r_elephant.png", 0);	//�졪����
	char_lib[3] = imread("r_chariot.png", 0);	//�졪��܇
	char_lib[4] = imread("r_horse.png", 0);		//�졪���R
	char_lib[5] = imread("r_cannon.png", 0);	//�졪����
	char_lib[6] = imread("r_solider.png", 0);	//�졪����

	char_lib[7] = imread("b_general.png", 0);	//�ڡ�����
	char_lib[8] = imread("b_advisor.png", 0);	//�ڡ���ʿ
	char_lib[9] = imread("b_elephant.png", 0);	//�ڡ�����
	char_lib[10] = imread("b_chariot.png", 0);	//�ڡ�����
	char_lib[11] = imread("b_horse.png", 0);	//�ڡ�����
	char_lib[12] = imread("b_cannon.png", 0);	//�ڡ����h
	char_lib[13] = imread("b_solider.png", 0);	//�ڡ�����

	//�����ֿ�Ĵ�С
	int char_lib_size =60;
	for (int i = 0; i < 14; i++)
	{
		resize(char_lib[i], char_lib[i], Size(60,60));
	}
	
}
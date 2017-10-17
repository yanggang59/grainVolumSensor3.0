

#include <opencv2/opencv.hpp>
#include "preprocessing.h" 
#include "cal_3d_coor.h"
#include "cal_area.h"


using namespace cv;
using namespace std;

float a, b, c, A, B, C; //abc��ax+by+cz+1=0����ʽ�Ĳ�����ABC��Z=Ax+By+C��ʽ�Ĳ���,��Ҫת��һ��

/*�ڲ�������֮ǰ�Ѿ��궨��*/
Mat K = (Mat_<float>(3, 3) << 518.8906422065566, 0, 294.5896218285499,
	0, 520.0230989700873, 226.11902054293,
	0, 0, 1);

int main()
{
	/*�����ṹ�������֮ǰ�궨�õ�*/
	//FileStorage fs("C://test_pic//test.yml",FileStorage::READ);
	//fs["A"]>>A;
	//fs["B"]>>B;
	//fs["C"]>>C;
	//fs.release();

	a = 0;
	b = -0.0153846;
	c = -0.0068497;

	A = -a / c;
	B = -b / c;
	C = -1 / c;

	Vec3f coeff(A, B, C);

	Mat Input = imread("../../res/right01.jpg",1);
	Mat uMask = imread("../../res/u_mask.jpg", 0);
	int width = 640;
	int height = 480;
	Size ImageSize(width, height);
	Mat skeleton(Size(width, height), CV_8UC1, Scalar(0));
	int count = 0;

	//ͼ��Ԥ������,Input��ʾ����ͼ��uMask��ģͼ��skeleton������ĹǼܣ�count�����Ǽܵ�ĸ���
	preprocessing(Input, uMask, skeleton, ImageSize,count);
	imshow("skeleton", skeleton);
	cout << "count is " << count << endl;

	//�����ĸ�������50������Ϊ�ǿհ�֡,continue��������һ��ѭ��

	if (count<50)
	{
		cout << "��֡�ǿհ�֡" << endl;
		//continue;
	}

	

	/*����ֱ�������е���������������*/
	vector<Vec3f> allPoints;

	int z_count = 0;  //��¼����z_threshold�ĵ�ĸ���
	int z_threshold = 20;
	cal_3d_coor(skeleton, allPoints, K, coeff, z_count, z_threshold);
	cout << "����z_threshold�ĵ�ĸ�����" << z_count << endl;
	cout << "��ĸ���Ϊ��" << allPoints.size() << endl;

	if (z_count<50)
	{
		cout << "�ǹΰ�֡" << endl;
	}


	//�������
	int distance=40, board_width = 190;
	float area_oneside = cal_area(allPoints, distance, board_width);
	cout << "area is " << area_oneside << endl;

	waitKey();
}

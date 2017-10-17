

#include <opencv2/opencv.hpp>
#include "preprocessing.h" 
#include "cal_3d_coor.h"
#include "cal_area.h"


using namespace cv;
using namespace std;

float a, b, c, A, B, C; //abc是ax+by+cz+1=0的形式的参数，ABC是Z=Ax+By+C形式的参数,需要转换一下

/*内参数矩阵，之前已经标定好*/
Mat K = (Mat_<float>(3, 3) << 518.8906422065566, 0, 294.5896218285499,
	0, 520.0230989700873, 226.11902054293,
	0, 0, 1);

int main()
{
	/*读出结构光参数，之前标定好的*/
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

	//图像预处理函数,Input表示输入图像，uMask掩模图像，skeleton是输出的骨架，count计数骨架点的个数
	preprocessing(Input, uMask, skeleton, ImageSize,count);
	imshow("skeleton", skeleton);
	cout << "count is " << count << endl;

	//如果点的个数少于50，则认为是空白帧,continue，进行下一次循环

	if (count<50)
	{
		cout << "此帧是空白帧" << endl;
		//continue;
	}

	

	/*计算直线上所有点相对于相机的坐标*/
	vector<Vec3f> allPoints;

	int z_count = 0;  //记录大于z_threshold的点的个数
	int z_threshold = 20;
	cal_3d_coor(skeleton, allPoints, K, coeff, z_count, z_threshold);
	cout << "大于z_threshold的点的个数是" << z_count << endl;
	cout << "点的个数为：" << allPoints.size() << endl;

	if (z_count<50)
	{
		cout << "是刮板帧" << endl;
	}


	//计算面积
	int distance=40, board_width = 190;
	float area_oneside = cal_area(allPoints, distance, board_width);
	cout << "area is " << area_oneside << endl;

	waitKey();
}

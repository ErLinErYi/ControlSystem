#include "LaneLineDetector.h"

LaneLineDetector::LaneLineDetector()
{
}

LaneLineDetector::~LaneLineDetector()
{
}

/** 去除蓝色通道，车道线检测中会对白色和黄色敏感，白色rgb(255,255,255),黄色rgb(255,255,0)
* 去除无效的蓝色通道
*/
cv::Mat LaneLineDetector::assignColorChannelWeights(cv::Mat& inputImage)
{
	std::vector<cv::Mat>mv;
#ifdef SHOW
	cv::imshow("原图", inputImage);
#endif
	split(inputImage, mv);

	cv::Mat out;
	mv[0] = 0;
	merge(mv, out);
#ifdef SHOW
	cv::imshow("重新分配RGB通道权重", out);
#endif
	return out;
}

// 透视图转换为俯视图
cv::Mat LaneLineDetector::perspectiveTransformation(cv::Mat& inputImage)
{
	cv::Mat warpMatrix = convertPerspective();
	cv::Mat dst;
	warpPerspective(inputImage, dst, warpMatrix, dst.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT);
#ifdef SHOW
	cv::imshow("透视变换", dst);
#endif
	return dst;
}

//图像模糊
/**
*@ 将高斯滤波器应用于输入图像去噪
*@param inputImage 是视频的帧
*@返回 模糊和去噪后的图像
*/
cv::Mat LaneLineDetector::getImageDeNoise(cv::Mat& inputImage)
{
	cv::Mat output;
	cv::GaussianBlur(inputImage, output, cv::Size(3, 3), 0, 0);
#ifdef SHOW
	cv::imshow("deNoise", output);
#endif
	return output;
}

// 边缘检测
/**
*@brief 通过对图像进行滤波，检测模糊帧中的所有边缘
*@param img_noise 是之前被模糊处理的画面
*@return 只有边缘用白色表示的二值图像
*/
cv::Mat LaneLineDetector::getImageEdge(cv::Mat& inputImage)
{
	cv::Mat output;
	cv::cvtColor(inputImage, output, cv::COLOR_BGR2GRAY);// 将图像从RGB转换为灰度图
#ifdef SHOW
	cv::imshow("灰度图", output);
#endif
	cv::adaptiveThreshold(output,     //源图像
		output,                       //输出二值图像
		255,                          //输出的最大值
		cv::ADAPTIVE_THRESH_MEAN_C,   //方法(局部平均值)
		cv::THRESH_BINARY,            //阈值类型
		31,                           //邻域大小
		-18);                         //使用的阈值
#ifdef SHOW
	cv::imshow("二值化", output);
#endif
	return output;
}

// 遮罩边缘图像
/**
*@brief 遮罩图像，以便仅检测构成车道一部分的边缘
*@param img_edges 是上一个函数中的边缘图像
*@return 所需边缘的二进制图像
*/
cv::Mat LaneLineDetector::getMaskImage(cv::Mat& inputImage)
{
	cv::Mat output;
	cv::Point pts[4] = {
		cv::Point(170, 340),
		cv::Point(280, 200),
		cv::Point(370, 200),
		cv::Point(500, 340)
	};
	/*cv::Point pts[4] = {
		cv::Point(250, 660),
		cv::Point(540, 450),
		cv::Point(750, 450),
		cv::Point(1180, 660)
	};*/

	cv::Mat mask = cv::Mat::zeros(inputImage.size(), inputImage.type());

	// 创建一个二元多边形遮罩
	cv::fillConvexPoly(mask, pts, 4, cv::Scalar(255, 255, 255));

	// 将图像边缘和遮罩相乘以获得输出
	cv::bitwise_and(inputImage, mask, output);

	return output;
}

/*
@param inputImage 为需要计算车道线的变形后且二值化的图像
@NOTE 该函数用于计算车道线坐标点以及偏离距离
*/
void LaneLineDetector::detectLaneLine(cv::Mat& inputImage)
{
	int nwindows = 9;//选择滑窗个数
	int window_height = int(inputImage.rows / nwindows);//设置窗口高度
	int margin = 50;//设置窗口宽度
	int minpix = 50;//设置非零像素坐标最少个数

	//TODO 加入if设置图像连续性，如果leftx_current和rightx_current为零，则认为第一次执行，需要计算该两点，如果已经计算了，则不许再次计算。
	//rowrange图像区域分割
	//将图像处理为一行，以行相加为方法  
	cv::Mat hist;
	cv::reduce(inputImage.rowRange(inputImage.rows / 2, inputImage.rows), hist, 0, cv::REDUCE_SUM, CV_32S);

	auto midpoint = int(hist.cols / 2);

	//将hist分为左右分别储存，并找出最大值
	//minMaxIdx针对多通道，minMaxLoc针对单通道
	cv::Point leftBase, rightBase;
	cv::minMaxLoc(hist.colRange(0, midpoint), NULL, NULL, NULL, &leftBase);
	cv::minMaxLoc(hist.colRange(midpoint, hist.cols), NULL, NULL, NULL, &rightBase);

	//左右车道线基础点
	auto leftx_current = leftBase.x;
	auto rightx_current = rightBase.x + midpoint;

	std::vector<cv::Point> nonZeroLeft, nonZeroRight, leftPoint, rightPoint;

	// 提前存入该基础点坐标
	leftPoint.push_back(cv::Point(leftx_current, inputImage.rows));
	rightPoint.push_back(cv::Point(rightx_current, inputImage.rows));

	for (int i = 0; i < nwindows; i++) {
		int win_y_low = inputImage.rows - (i + 1) * window_height;

		//计算选框x坐标点，并将计算结果限制在图像坐标内
		int win_xleft_low = leftx_current - margin;
		win_xleft_low = win_xleft_low > 0 ? win_xleft_low : 0;
		win_xleft_low = win_xleft_low < inputImage.rows ? win_xleft_low : inputImage.rows;
		//int win_xleft_high = leftx_current + margin;
		int win_xright_low = rightx_current - margin;
		win_xright_low = win_xright_low > 0 ? win_xright_low : 0;
		win_xright_low = win_xright_low < inputImage.rows ? win_xright_low : inputImage.rows;
		//int win_xright_high = rightx_current + margin;
		//NOTE要确保参数都大于0，且在src图像范围内，不然会报错
		//NOTE 设置为ROI矩形区域选择
		auto leftROI = inputImage(cv::Rect(win_xleft_low, win_y_low, 2 * margin, window_height));
		auto rightROI = inputImage(cv::Rect(win_xright_low, win_y_low, 2 * margin, window_height));
		
		//NOTE 把像素值不为零的像素坐标存入矩阵
		cv::findNonZero(leftROI, nonZeroLeft);
		cv::findNonZero(rightROI, nonZeroRight);
		//计算每个选框的leftx_current和rightx_current中心点
		if (nonZeroLeft.size() > minpix) {
			int leftx = 0;
			for (auto& n : nonZeroLeft) {
				leftx += n.x;
			}
			leftx_current = win_xleft_low + leftx / nonZeroLeft.size();
		}
		if (nonZeroRight.size() > minpix) {
			int rightx = 0;
			for (auto& n : nonZeroRight) {
				rightx += n.x;
			}
			rightx_current = win_xright_low + rightx / nonZeroRight.size();
		}
		//将中心点坐标存入容器
		leftPoint.push_back(cv::Point(leftx_current, win_y_low));
		rightPoint.push_back(cv::Point(rightx_current, win_y_low));
	}
	//拟合左右车道线坐标
	cv::Mat leftx = polyfit(leftPoint, 2);
	cv::Mat rightx = polyfit(rightPoint, 2);

	//计算拟合曲线坐标
	_leftLaneLine = polyval(leftx, leftPoint, 2);
	_rightLaneLine = polyval(rightx, rightPoint, 2);

	//计算车道偏离距离
	int lane_width = abs(rightPoint.front().x - leftPoint.front().x);
	double lane_xm_per_pix = 3.7 / lane_width;
	double veh_pos = (((rightPoint.front().x + leftPoint.front().x) * lane_xm_per_pix) / 2);
	double cen_pos = ((inputImage.cols * lane_xm_per_pix) / 2);
	_distanceFromCenter = veh_pos - cen_pos;
}

/*
@param inputImage 为原始图像
@NOTE 该函数用于绘制车道线和可行使区域
*/
cv::Mat LaneLineDetector::drawIdentificationArea(cv::Mat& inputImage)
{
	std::vector<cv::Point> rflip, ptr;
	cv::Mat colormask = cv::Mat::zeros(inputImage.rows, inputImage.cols, CV_8UC3);
	cv::Mat dst, midst;

	//绘制车道线
	cv::polylines(colormask, _leftLaneLine, false, cv::Scalar(0, 0, 255), 5);
	cv::polylines(colormask, _rightLaneLine, false, cv::Scalar(0, 0, 255), 5);
	//反转坐标，以便绘制填充区域
	cv::flip(_rightLaneLine, rflip, 1);
	//拼接坐标
	cv::hconcat(_leftLaneLine, rflip, ptr);
	//绘制填充区域
	const cv::Point* em[1] = { &ptr[0] };
	int nop = (int)ptr.size();
	cv::fillPoly(colormask, em, &nop, 1, cv::Scalar(200, 200, 0));
#ifdef MEETING
	//将车道线图片和原始图片叠加
	cv::addWeighted(inputImage, 1, colormask, 0.5, 0, dst);
#else
	//反变形
	cv::warpPerspective(colormask, midst, convertPerspective(false), inputImage.size(), cv::INTER_LINEAR);
	//将车道线图片和原始图片叠加
	cv::addWeighted(inputImage, 1, midst, 0.5, 0, dst);
#endif

	//绘制文字
	//cv::putText(dst, "distance bias:" + std::to_string(_distanceFromCenter) + "m", cv::Point(50, 50), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);

	//cv::imshow("video", dst);

	return dst;
}

void LaneLineDetector::drawLaneLine(cv::Mat& inputImage)
{
	cv::Mat colormask;
	inputImage.copyTo(colormask);
	cv::cvtColor(colormask, colormask, cv::COLOR_GRAY2BGR);

	cv::polylines(colormask, _leftLaneLine, false, cv::Scalar(0, 255, 0), 3);
	cv::polylines(colormask, _rightLaneLine, false, cv::Scalar(0, 0, 255), 3);
#ifdef SHOW
	cv::imshow("colormask", colormask);
#endif
}

void LaneLineDetector::drawPixelDistribution(cv::Mat& inputImage)
{
	std::vector<int> pixel(inputImage.size().width, 0);
	for (int i = 0; i < inputImage.size().height; ++i)
	{
		for (int j = 0; j < inputImage.size().width; ++j)
		{
			pixel[j] += inputImage.at<uchar>(i, j);
		}
	}

	//std::cout << inputImage.size().height << std::endl;
	for (int i = 0; i < inputImage.size().width; ++i)
	{
		pixel[i] /= inputImage.size().height / 2;
	}

	cv::Mat histImage(480, 640, CV_8UC3, cv::Scalar(20, 20, 20));

	for (int i = 0; i < inputImage.size().width; ++i)
	{
		line(histImage, cv::Point(i, 470), cv::Point(i, 470 - pixel[i]), cv::Scalar(255, 255, 255));
	}
#ifdef SHOW
	imshow("像素分布图", histImage);
#endif
}

/*
@param mat_k 为多项式的系数矩阵
@param src 为需要计算的坐标点
@param n 为计算n次多项式
@NOTE 该函数用于计算拟合过后的曲线点坐标，该函数以y为基础坐标轴，如果要以x为基础坐标轴，则修改y为x
*/
std::vector<cv::Point> LaneLineDetector::polyval(const cv::Mat& input, const std::vector<cv::Point>& src, int n)
{
	std::vector<cv::Point> ip;
	for (int i = src.back().y; i < src.front().y; i++) {//从y=0开始计算，分别计算出x的值
		cv::Point ipt;
		ipt.x = 0;
		ipt.y = i;
		for (int j = 0; j < n + 1; j++) {
			ipt.x += input.at<double>(j, 0) * pow(i, j);//NOTE多项式计算
		}
		ip.push_back(ipt);
	}
	return ip;
}

/*
@param inPoint 为离散坐标点
@param n 为n次多项式
@param mat_k 为返回多项式的k系数，为n*1的矩阵
@NOTE 该函数用于拟合曲线
*/
cv::Mat LaneLineDetector::polyfit(std::vector<cv::Point>& inPoint, int n)
{
	int size = inPoint.size();	//所求未知数个数	
	int x_num = n + 1;	        //构造矩阵U和Y	
	cv::Mat mat_u(size, x_num, CV_64F);
	cv::Mat mat_y(size, 1, CV_64F);
	for (int i = 0; i < mat_u.rows; ++i) {
		for (int j = 0; j < mat_u.cols; ++j) {
			mat_u.at<double>(i, j) = pow(inPoint[i].y, j);//in_point[i].y为以y为递增坐标
		}
	}
	for (int i = 0; i < mat_y.rows; ++i) {
		mat_y.at<double>(i, 0) = inPoint[i].x;
	} 	//矩阵运算，获得系数矩阵K	
	cv::Mat mat_k(x_num, 1, CV_64F);
	mat_k = (mat_u.t() * mat_u).inv() * mat_u.t() * mat_y;
	// cout << mat_k << endl;	
	return mat_k;
}

cv::Mat LaneLineDetector::convertPerspective(bool flag)
{
	std::vector<cv::Point2f>src(4);
	src[0] = cv::Point2f(170, 200);
	src[1] = cv::Point2f(500, 200);
	src[2] = cv::Point2f(170, 340);
	src[3] = cv::Point2f(500, 340);

	std::vector<cv::Point2f>dst(4);
	dst[0] = cv::Point2f(-600, 0);
	dst[1] = cv::Point2f(1400, 0);
	dst[2] = cv::Point2f(170, 480);
	dst[3] = cv::Point2f(500, 480);

	/*std::vector<Point2f>src(4);
	src[0] = Point2f(203, 720);
	src[1] = Point2f(585, 460);
	src[2] = Point2f(695, 460);
	src[3] = Point2f(1127, 720);

	std::vector<Point2f>dst(4);
	dst[0] = Point2f(320, 720);
	dst[1] = Point2f(320, 0);
	dst[2] = Point2f(960, 0);
	dst[3] = Point2f(960, 720);*/

	return flag ?
		cv::getPerspectiveTransform(src, dst) :
		cv::getPerspectiveTransform(dst, src);
}

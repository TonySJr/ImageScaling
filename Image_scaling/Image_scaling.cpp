// Image_scaling.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream> // for standard I/O
#include <string>   // for strings
#include <vector>
#include <boost/config.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <opencv2/core.hpp>     // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/imgproc.hpp>  // Gaussian Blur
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>  // OpenCV window I/O

using namespace std;
using namespace cv;

//	global	variables
const char* img_src_name = "Orig_Image";
const char* img_scl_name = "Scaled_Image";

string img_path;
int x_scale = 600;
int y_scale = 600;
Mat src_img;
Mat scl_img;
Mat mask;
/*
	работает только при определенных коэфиициентах расширения,
	картинку 512 в 600 - увеличивает
	работает от 1 до 2 (коэффициент увеличения)
*/
int main()
{
	//	read config.ini file
	boost::property_tree::ptree pt;
	boost::property_tree::ini_parser::read_ini("config.ini", pt);

	img_path = pt.get<string>("Section1.img_path");
	std::cout << img_path << endl;
	/*
	x_scale = pt.get<int>("Section1.x_scale");
	std::cout << x_scale << endl;
	y_scale = pt.get<int>("Section1.y_scale");
	std::cout << y_scale << endl;
	*/
	namedWindow(img_src_name, WINDOW_AUTOSIZE);
	moveWindow(img_src_name, 0, 0);
	namedWindow(img_scl_name, WINDOW_AUTOSIZE);
	moveWindow(img_scl_name, 600, 0);

	//	read image
	src_img = imread(img_path,IMREAD_GRAYSCALE);
	if (src_img.empty())
	{
		cout << "Image can't load, check name \n";
		waitKey(0);
		return 0;
	}
	//	Scale image
	scl_img.create(y_scale,x_scale,CV_8UC1);
	mask.create(y_scale, x_scale, CV_8UC1);
	mask = (Mat::zeros(scl_img.rows, scl_img.cols, CV_8UC1));

	//	x things
	//vector<int> help;
	float scale_factor = ((float)x_scale / src_img.cols);
	cout << scale_factor << endl;
	for (int y = 0; y < src_img.rows; y++)
	{
		for (int i = 0; i < src_img.cols; i++)
		{
			int x_coord = (int)((float)i * scale_factor);
			scl_img.at<uint8_t>(y, x_coord) = src_img.at<uint8_t>(y, i);
			mask.at<uint8_t>(y, x_coord) = 255;
		}
		
	}

	//	y things
	Mat temp = scl_img.clone();
	Mat mask_temp = mask.clone();
	mask = Mat::zeros(scl_img.rows, scl_img.cols, CV_8UC1);
	scl_img = Mat::zeros(scl_img.rows, scl_img.cols, CV_8UC1);
	for (int x = 0; x < scl_img.cols; x++)
		for (int i = 0; i < src_img.rows; i++)
		{
			int y_coord = (int)((float)i * scale_factor);
			scl_img.at<uint8_t>(y_coord, x) = temp.at<uint8_t>(i, x);
			mask.at<uint8_t>(y_coord, x) = mask_temp.at<uint8_t>(i, x);
		}
	//	add missed pixels
	
	for (int y = 0; y < mask.rows; y++)
	{
		for (int x = 1; x < mask.cols - 1; x++)
		{
			if(mask.at<uint8_t>(y,x) == 0)
			{
				uint8_t color = (scl_img.at<uint8_t>(y, x - 1) + scl_img.at<uint8_t>(y, x + 1)) / 2;
				scl_img.at<uint8_t>(y, x) = color;
			}
		}
	}
	bitwise_or(mask, scl_img, mask);
	for (int x = 0; x < mask.cols; x++)
	{
		for (int y = 1; y < mask.rows - 1; y++)
		{
			if (mask.at<uint8_t>(y, x) == 0)
			{
				uint8_t color = (scl_img.at<uint8_t>(y - 1, x) + scl_img.at<uint8_t>(y + 1, x)) / 2;
				scl_img.at<uint8_t>(y, x) = color;
			}
		}
	}
	
	//bitwise_or(mask, scl_img, scl_img);

	imshow(img_src_name, src_img);
	imshow(img_scl_name, scl_img);

	waitKey(0);

	return EXIT_SUCCESS;
}

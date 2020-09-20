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
#include "Loger.h"

using namespace std;
using namespace cv;

//	 functions
cv::Mat img_scale(cv::Mat &src,double scalefactor);

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
	Loger Log;
	//	read config.ini file
	boost::property_tree::ptree pt;
	boost::property_tree::ini_parser::read_ini("config.ini", pt);

	img_path = pt.get<string>("Section1.img_path");
	x_scale = pt.get<int>("Section1.x_scale");
	Log.ConsoleLog("x_scale", (size_t)x_scale);
	y_scale = pt.get<int>("Section1.y_scale");
	Log.ConsoleLog("y_scale", (size_t)y_scale);

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
	//	Scale Image
	double scale = ((float)x_scale / src_img.cols);
	//	 new algo
	if (scale > 2)
	{
		int scale_counter = 0;
		for (;scale > 2;scale /=2)
		{
			scale_counter++;
		}
		Log.ConsoleLog("Scale value", scale);
		Log.ConsoleLog("Scale_counter", (size_t)scale_counter);
		scl_img = img_scale(src_img, scale);
		do
		{
			scl_img = img_scale(scl_img, 2);
			scale_counter--;
		} while (scale_counter > 0);
	}
	else
	{
		Log.ConsoleLog("Scale value", scale);		
		scl_img = img_scale(src_img, scale);
	}

	imshow(img_src_name, src_img);
	imshow(img_scl_name, scl_img);

	waitKey(0);

	return EXIT_SUCCESS;
}


cv::Mat img_scale(cv::Mat& src, double scalefactor)
{
	//	Scale image
	Mat scaled_img = (Mat::zeros(src.rows * scalefactor, src.cols * scalefactor, CV_8UC1));
	Mat	mask = (Mat::zeros(scaled_img.rows, scaled_img.cols, CV_8UC1));

	//	x things
	for (int y = 0; y < src.rows; y++)
	{
		for (int x = 0; x < src.cols; x++)
		{
			int x_coord = (int)((float)x * scalefactor);
			int y_coord = (int)((float)y * scalefactor);
			scaled_img.at<uint8_t>(y_coord, x_coord) = src.at<uint8_t>(y, x);
			mask.at<uint8_t>(y_coord, x_coord) = 255;
		}
	}
	if (scalefactor > 1)
	{
		//	add missed pixels
		for (int y = 0; y < mask.rows; y++)
		{
			for (int x = 1; x < mask.cols - 1; x++)
			{
				if (mask.at<uint8_t>(y, x) == 0)
				{
					uint8_t color = (scaled_img.at<uint8_t>(y, x - 1) + scaled_img.at<uint8_t>(y, x + 1)) / 2;
					scaled_img.at<uint8_t>(y, x) = color;
				}
			}
		}
		bitwise_or(mask, scaled_img, mask);
		for (int x = 0; x < mask.cols; x++)
		{
			for (int y = 1; y < mask.rows - 1; y++)
			{
				if (mask.at<uint8_t>(y, x) == 0)
				{
					uint8_t color = (scaled_img.at<uint8_t>(y - 1, x) + scaled_img.at<uint8_t>(y + 1, x)) / 2;
					scaled_img.at<uint8_t>(y, x) = color;
				}
			}
		}
	}
	return scaled_img;
}

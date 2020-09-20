#pragma once
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
using namespace std;
class Loger
{
public:
	void ConsoleLog(size_t value);
	void ConsoleLog(const char* text);
	void ConsoleLog(const char* text, size_t value);
	void ConsoleLog(const char* text, double value);
private:
};


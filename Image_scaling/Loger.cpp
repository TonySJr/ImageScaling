#include "Loger.h"

//	CONSOLE	___________________________________________
void Loger::ConsoleLog(size_t value)
{
	cout << "\nValue = " << value << endl;
}
void Loger::ConsoleLog(const char* text)
{
	cout << endl << "\t" << text << endl;
}
void Loger::ConsoleLog(const char* text, size_t value)
{
	cout << endl << "\t" << text << " : " << value << endl;
}

void Loger::ConsoleLog(const char* text, double value)
{
	cout << endl << "\t" << text << " : " << value << endl;
}
//	IMSHOW	____________________________________________

#include <opencv2/opencv.hpp>
#include <iostream>
#include "BW.h"
#include "DR.h"
#include "counter.h"
#include "ImageProcesses.h"

using namespace std;
using namespace cv;

int main()
{
	cout << "Welcome to our Toolbox" << endl;
	cout << "Choose the Lab you want to run (1-4): " << endl;
	int choice;
	cin >> choice;
	switch (choice) {
	case 1:
		Lab1();
		break;
	case 2:
		Lab2();
		break;
	case 3:
		Lab3();
		break;
	case 4:
		Lab4();
		break;
}
	return 0;
}
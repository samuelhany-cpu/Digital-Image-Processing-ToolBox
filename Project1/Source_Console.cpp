#include <opencv2/opencv.hpp>
#include <iostream>
#include "counter.h"
#include "BW.h"
#include "DR.h"
#include "ImageProcesses.h"

using namespace std;
using namespace cv;
int main() {
	int x;
	cout << "Welcome to Image Processing Lab" << endl;
	cin >> x;
	while (true)
	{
		switch (x)
		{
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
		default:
			cout << "Exiting..." << endl;
			return 0;
		}
		cout << "Enter another lab number to continue or any other number to exit: ";
		cin >> x;

	}

}
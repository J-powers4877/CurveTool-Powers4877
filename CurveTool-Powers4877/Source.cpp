#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
	VideoCapture cap("../barriers.avi");

	if (!cap.isOpened())
	{
		cout << "Error reading image!" << endl;
		return -1;
	}

	namedWindow("Test", WINDOW_NORMAL);
	resizeWindow("Test", 500, 500);

	int sliderBrightness = 50;
	createTrackbar("Brightness", "Test", &sliderBrightness, 100);

	int sliderContrast = 50;
	createTrackbar("Contrast", "Test", &sliderContrast, 100);

	int g_switch_value = 0;
	createTrackbar("Switch", "Test", &g_switch_value, 1);

	double dFrames = cap.get(CAP_PROP_POS_FRAMES);

	while (true)
	{
		Mat frame;

		cap >> frame;

		if (frame.empty())
			break;

		/*
		Display in new window first, once user hits button, push to full video.
		*/
		imshow("Frame", frame);

		if (g_switch_value == 1)
		{
			imshow("Frame", frame);
		}

		int iKey = waitKey(20);

		if (iKey == 27)
		{
			break;
		}
	}

	// When everything done, release the video capture object
	cap.release();

	// Closes all the frames
	return 0;
}
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

	double dFrames = cap.get(CAP_PROP_POS_FRAMES);

	int sliderBrightness = -1;
	int sliderContrast = -1;

	while (true)
	{
		createTrackbar("Brightness", "Test", &sliderBrightness, 100);

		createTrackbar("Contrast", "Test", &sliderContrast, 100);

		int iBrightness = sliderBrightness - 50;
		double dContrast = sliderContrast / 50.0;

		if (sliderBrightness == -1)
		{
			sliderBrightness = 50;
		}
		if (sliderContrast == -1)
		{
			sliderContrast = 50;
		}

		Mat oFrame;
		Mat nFrame;

		cap.read(oFrame);
		cap.read(nFrame);

		oFrame.convertTo(nFrame, -1, dContrast, iBrightness);

		if (nFrame.empty() || oFrame.empty())
		{
			break;
		}

		imshow("Original Video", oFrame);
		imshow("New Video", nFrame);

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
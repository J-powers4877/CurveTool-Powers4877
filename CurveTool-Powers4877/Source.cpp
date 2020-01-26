#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <Windows.h>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
	VideoCapture cap("../barriers.avi");
	VideoCapture oCap("../barriers.avi");

	if (!cap.isOpened())
	{
		cout << "Error reading image!" << endl;
		return -1;
	}

	namedWindow("Test", WINDOW_NORMAL);
	resizeWindow("Test", 500, 500);

	int sliderBrightness = -1;
	int sliderContrast = -1;

	int iBrightness = 0;
	double dContrast = 0;

	int frame_counter = 0;

	int frames = cap.get(CAP_PROP_FRAME_COUNT);

	Mat oFrame;
	Mat nFrame;

	while (true)
	{
		//frame_counter++;

		//if (frame_counter == frames) {
		//	cap.set(CAP_PROP_POS_FRAMES, 0);
		//}
		createTrackbar("Brightness", "Test", &sliderBrightness, 100);
		createTrackbar("Contrast", "Test", &sliderContrast, 100);

		iBrightness = sliderBrightness - 50;
		dContrast = sliderContrast / 50.0;

		if (sliderBrightness == -1)
		{
			sliderBrightness = 50;
		}
		if (sliderContrast == -1)
		{
			sliderContrast = 50;
		}

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

	int frame_width = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
	int frame_height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));

	Size frame_size(frame_width, frame_height);
	int fps = 50;

	VideoWriter oVideoWriter("../MyVideo.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, frame_size, true);

	Mat Test;

	for (;;)
	{
		bool is = oCap.grab();

		if (is == false)
		{
			cout << "Cannot grab video frame" << endl;
			break;
		}
		else
		{
			oCap.retrieve(Test);
			Test.convertTo(Test, -1, dContrast, iBrightness);

			cout << "Saving video" << endl;
			oVideoWriter.write(Test); 
			waitKey(10);
		}
	}

	// When everything done, release the video capture object
	oVideoWriter.release();
	oCap.release();
	cap.release();

	// Closes all the frames
	return 0;
}
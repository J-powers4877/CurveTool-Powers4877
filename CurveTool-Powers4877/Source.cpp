#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <Windows.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

//Creates the Histogram for the frames
//windowName is the name of the Histogram Window Name; Don't make it same as video output
//frame is the name of the frame to grab the histogram from
void show_histogram(string windowName, Mat frame)
{
	int histogram[256];

	for (int i = 0; i < 255; i++)
	{
		histogram[i] = 0;
	}

	// calculate the n of pixels for each intensity values
	for (int y = 0; y < frame.rows; y++)
		for (int x = 0; x < frame.cols; x++)
			histogram[(int)frame.at<uchar>(y, x)]++;


	// draw the histograms
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / 256);

	Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(255, 255, 255));

	// find the maximum intensity element from histogram
	int max = histogram[0];
	for (int i = 1; i < 256; i++) {
		if (max < histogram[i]) {
			max = histogram[i];
		}
	}

	// normalize the histogram between 0 and histImage.rows

	for (int i = 0; i < 255; i++) {
		histogram[i] = ((double)histogram[i] / max) * histImage.rows;
	}


	// draw the intensity line for histogram
	for (int i = 0; i < 255; i++)
	{
		line(histImage, Point(bin_w * (i), hist_h),
			Point(bin_w * (i), hist_h - histogram[i]),
			Scalar(0, 0, 0), 1, 8, 0);
	}

	// display histogram
	imshow(windowName, histImage);

	waitKey(25);
	return;
}

int main(int argc, char* argv[])
{
	//originalCap: Capture object; Used for showing the Original Video
	VideoCapture originalCap("../barriers.avi");
	//changedCap: Capture object; Used for saving the Changed video
	VideoCapture changedCap("../barriers.avi");

	//originalFrame: Mat object; Used with the Original Video
	Mat originalFrame;
	//newFrame: Mat object; Used with the New, Changed Video
	Mat newFrame;
	//savedFrame: Mat object; Used to with the Final, Saved Video
	Mat savedFrame;

	//iSliderBrightness: Integer; Used for setting Brightness Slider Starting Value
	int iSliderBrightness = -1;
	//iSliderContrast: Integer; Used for setting Contrast Slider Starting Value
	int iSliderContrast = -1;

	//iBrightness: Interger; Used to get Brightness adjustment.
	int iBrightness = 0;
	//dContrast: Double; Used to get Contrast Adjustment.
	double dContrast = 0;

	//iFrameCounter: Integer; Used to keep track of what frame the video is on
	int iFrameCounter = 0;
	//iFPS: Integer; Populated from Original Video; Used to match input video FPS when saving
	int iFPS = originalCap.get(CAP_PROP_FPS);

	//iFrameWidth: Integer; Populated from Original Video; Used to in frameSize
	int iFrameWidth = static_cast<int>(originalCap.get(CAP_PROP_FRAME_WIDTH));
	//iFrameHeight: Integer; Populated from Original Video; Used in frameSize
	int iFrameHeight = static_cast<int>(originalCap.get(CAP_PROP_FRAME_HEIGHT));

	//frameSize: Size object; Populted from iFrameWidth & iFrameHeight; Used to match input & output window sizes
	Size frameSize(iFrameWidth, iFrameHeight);

	//oVideoWriter: VideoWriter object; Used to create final, edited video and save it
	VideoWriter oVideoWriter("../MyVideo.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), iFPS, frameSize, true);

	//Create "Sliders" window for sliders
	namedWindow("Sliders", WINDOW_NORMAL);
	//Set "Test" window size
	resizeWindow("Sliders", 500, 500);

	//Check if originalCap was opened;
	//If not, print error and return -1
	if (!originalCap.isOpened())
	{
		cout << "Error reading image!" << endl;
		return -1;
	}

	//First Loop; Used for user to change video contrast/brightness; Esc key used to progress to save loop 
	while (true)
	{
		//iFrameCounter++;

		//if (iFrameCounter == frames) {
		//	originalCap.set(CAP_PROP_POS_FRAMES, 0);
		//}

		//Creation of Brightness slider/trackbar
		createTrackbar("Brightness", "Sliders", &iSliderBrightness, 100);
		//Creation of Contrast slider/trackbar
		createTrackbar("Contrast", "Sliders", &iSliderContrast, 100);

		//Get brightness value from slider, subtract starting value to increase or decrease brightness
		iBrightness = iSliderBrightness - 50;
		//Get contrast value from slider, divide by 50.0 to get value to increase or decrease contrast
		dContrast = iSliderContrast / 50.0;

		//Check if iSliderBrightness is -1 / hasn't been changed
		//If so, set it to equal 50
		if (iSliderBrightness == -1)
		{
			iSliderBrightness = 50;
		}
		//Check if iSliderContrast is -1 / hasn't been changed
		//If so, set it to equal 50
		if (iSliderContrast == -1)
		{
			iSliderContrast = 50;
		}

		//Read new frame in from originalCap to originalFrame
		originalCap.read(originalFrame);
		//Read new frame in from originalCap to newFrame
		originalCap.read(newFrame);

		//Set user adjusted Brightness and Contrast to newFrame
		originalFrame.convertTo(newFrame, -1, dContrast, iBrightness);

		//Check if the current frame is empty on either
		//If so, break the while loop.
		if (newFrame.empty() || originalFrame.empty())
		{
			break;
		}

		//Display Original Video from originalFrame
		imshow("Original Video", originalFrame);
		//Display Histogram for Original Video
		show_histogram("Original Histogram",originalFrame);
		//Display changed video from newFrame
		imshow("New Video", newFrame);
		//Display Histogram for newFrame
		show_histogram("New Histogram", newFrame);

		//iKey: Integer; Checks to see if key has been pressed for 20 ms
		int iKey = waitKey(20);

		//Check if Esc key pressed;
		//If pressed, break While loop
		if (iKey == 27)
		{
			break;
		}
	}

	//Second Loop; Used to save the changes the user has made to the video
	while (true)
	{
		//canGrab: Boolean; True if next frame can be grabbed/non-empty, 
		//	     False if next frame cannot be grabbed/is empty
		bool canGrab = changedCap.grab();

		//Check if canGrab is false
		//If false, print error, break loop.
		if (canGrab == false)
		{
			cout << "Cannot grab video frame" << endl;
			break;
		}
		//If true, retrieve the frame.
		//Change brightness and contrast values to user set values
		//Write frame to file.
		//Wait
		else
		{
			changedCap.retrieve(savedFrame);
			savedFrame.convertTo(savedFrame, -1, dContrast, iBrightness);

			cout << "Saving frame." << endl;
			oVideoWriter.write(savedFrame); 
			waitKey(10);
		}
	}

	// When everything done, release the video capture object
	oVideoWriter.release();
	changedCap.release();
	originalCap.release();

	// Closes all the frames
	return 0;
}
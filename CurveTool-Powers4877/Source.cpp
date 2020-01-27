#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <Windows.h>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
	//cap: Capture object; Used for showing the Original Video
	VideoCapture cap("../barriers.avi");
	//oCap: Capture object; Used for saving the Changed video
	VideoCapture oCap("../barriers.avi");

	//oFrame: Mat object; Used with the Original Video
	Mat oFrame;
	//nFrame: Mat object; Used with the New, Changed Video
	Mat nFrame;
	//Test: Mat object; Used to with the Final, Saved Video
	Mat Test;

	//sliderBrightness: Integer; Used for setting Brightness Slider Starting Value
	int sliderBrightness = -1;
	//sliderContrast: Integer; Used for setting Contrast Slider Starting Value
	int sliderContrast = -1;

	//iBrightness: Interger; Used to get Brightness adjustment.
	int iBrightness = 0;
	//dContrast: Double; Used to get Contrast Adjustment.
	double dContrast = 0;

	//frame_counter: Integer; Used to keep track of what frame the video is on
	int frame_counter = 0;
	//fps: Integer; Populated from Original Video; Used to match input video FPS when saving
	int fps = cap.get(CAP_PROP_FPS);

	//frame_width: Integer; Populated from Original Video; Used to in frame_size
	int frame_width = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
	//frame_height: Integer; Populated from Original Video; Used in frame_size
	int frame_height = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));

	//frame_size: Size object; Populted from frame_width & frame_height; Used to match input & output window sizes
	Size frame_size(frame_width, frame_height);

	//oVideoWriter: VideoWriter object; Used to create final, edited video and save it
	VideoWriter oVideoWriter("../MyVideo.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, frame_size, true);

	//Create "Test" window for sliders
	namedWindow("Test", WINDOW_NORMAL);
	//Set "Test" window size
	resizeWindow("Test", 500, 500);

	//Check if cap was opened;
	//If not, print error and return -1
	if (!cap.isOpened())
	{
		cout << "Error reading image!" << endl;
		return -1;
	}

	//First Loop; Used for user to change video contrast/brightness; Esc key used to progress to save loop 
	while (true)
	{
		//frame_counter++;

		//if (frame_counter == frames) {
		//	cap.set(CAP_PROP_POS_FRAMES, 0);
		//}

		//Creation of Brightness slider/trackbar
		createTrackbar("Brightness", "Test", &sliderBrightness, 100);
		//Creation of Contrast slider/trackbar
		createTrackbar("Contrast", "Test", &sliderContrast, 100);

		//Get brightness value from slider, subtract starting value to increase or decrease brightness
		iBrightness = sliderBrightness - 50;
		//Get contrast value from slider, divide by 50.0 to get value to increase or decrease contrast
		dContrast = sliderContrast / 50.0;

		//Check if sliderBrightness is -1 / hasn't been changed
		//If so, set it to equal 50
		if (sliderBrightness == -1)
		{
			sliderBrightness = 50;
		}
		//Check if sliderContrast is -1 / hasn't been changed
		//If so, set it to equal 50
		if (sliderContrast == -1)
		{
			sliderContrast = 50;
		}

		//Read new frame in from cap to oFrame
		cap.read(oFrame);
		//Read new frame in from cap to nFrame
		cap.read(nFrame);

		//Set user adjusted Brightness and Contrast to nFrame
		oFrame.convertTo(nFrame, -1, dContrast, iBrightness);

		//Check if the current frame is empty on either
		//If so, break the while loop.
		if (nFrame.empty() || oFrame.empty())
		{
			break;
		}

		//Display Original Video from oFrame
		imshow("Original Video", oFrame);
		//Display changed video from nFrame
		imshow("New Video", nFrame);

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
		bool canGrab = oCap.grab();

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
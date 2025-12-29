#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/bgsegm.hpp>
#include <iostream>
#include <cstdlib>
#include <array>
#include <functional>


using namespace cv;
using namespace std;
std::string openFileDialog() {
    std::array<char, 512> buffer;
    std::string result;

    FILE* pipe = popen("zenity --file-selection", "r");
    if (!pipe) return "Error";

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }

    pclose(pipe);

    // Remove trailing newline character
    if (!result.empty() && result.back() == '\n') {
        result.pop_back();
    }

    return result;
}


void extractBackground(std::string inputVideoPath, std::string outputVideoPath, std::string outputBgImage) {
		outputVideoPath = "/home/timwalter/out.mp4";
		outputBgImage = "/home/timwalter/out.jpg";
		VideoCapture videoInput(inputVideoPath);
		if (!videoInput.isOpened())
		{
			cerr << "Error opening video file!" << endl;
		}

		int width = (int)videoInput.get(CAP_PROP_FRAME_WIDTH);
		int height = (int)videoInput.get(CAP_PROP_FRAME_HEIGHT);
		double fps = videoInput.get(CAP_PROP_FPS);

		VideoWriter writer(
			outputVideoPath,
			VideoWriter::fourcc('m', 'p', '4', 'v'),
			fps,
			cv::Size(width, height)
		);

		cv::Ptr<cv::BackgroundSubtractor> bgSubtractor =
			cv::bgsegm::createBackgroundSubtractorLSBP(
			cv::bgsegm::LSBP_CAMERA_MOTION_COMPENSATION_NONE,
			20,
			16,
			true,
			0.5f
		);
		// Ptr<BackgroundSubtractor> bgSubtractor =
		// createBackgroundSubtractorMOG2(500, 16, true);

		Mat frame, fgMask, fgMaskClean, foreground;

		// Floating-point background accumulator
		Mat bgFloat(height, width, CV_32FC3, Scalar(0, 0, 0));
		Mat bgCount(height, width, CV_32FC1, Scalar(0));

		Mat kernel = getStructuringElement(MORPH_ELLIPSE, cv::Size(5, 5));

		while (true)
		{
			videoInput >> frame;
			if (frame.empty())
				break;

			// Foreground mask
			bgSubtractor->apply(frame, fgMask);
			threshold(fgMask, fgMask, 200, 255, THRESH_BINARY);

			morphologyEx(fgMask, fgMaskClean, MORPH_OPEN, kernel);
			morphologyEx(fgMaskClean, fgMaskClean, MORPH_DILATE, kernel);

			// Foreground video
			foreground = Mat::zeros(frame.size(), frame.type());
			frame.copyTo(foreground, fgMaskClean);
			writer.write(foreground);

			// Background update mask (background = inverse of fg)
			Mat bgMask;
			bitwise_not(fgMaskClean, bgMask);

	

            Mat frameFloat;

            frame.convertTo(frameFloat, CV_32FC3);

            // Accumulate background
            frameFloat.copyTo(frameFloat, bgMask);  // zero out foreground
            bgFloat += frameFloat;

            // Count background samples
            Mat ones = Mat::ones(frame.size(), CV_32FC1);
            ones.setTo(0, fgMaskClean);
            bgCount += ones;
		}

		videoInput.release();
		writer.release();
		
		
		// Final background image
				Mat bgFinal;
		// divide(bgFloat, bgCount, bgFinal);
		Mat bgCount3;
		cvtColor(bgCount, bgCount3, COLOR_GRAY2BGR);

		divide(bgFloat, bgCount3, bgFinal);


		// Replace NaNs (never-seen pixels)
		bgFinal.setTo(0, bgCount == 0);

		bgFinal.convertTo(bgFinal, CV_8UC3);
		imwrite(outputBgImage, bgFinal);
		imwrite(outputBgImage, bgFinal);

		cout << "Foreground video saved to: " << outputVideoPath << endl;
		cout << "Background image saved to: " << outputBgImage << endl;

		
	}


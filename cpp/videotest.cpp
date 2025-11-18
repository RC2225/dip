#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;
Mat performSVD(Mat& input)
{
	Mat U, S, Vt;
	SVD::compute(input, S, U, Vt, SVD::FULL_UV);

	Mat newS = Mat::zeros(S.size(), S.type());
	newS.at<float>(0, 0) = S.at<float>(0, 0);

	Mat diagS = Mat::diag(newS);
	Mat reconstructed = U * diagS * Vt;
	return reconstructed;
}
int main(){
	VideoCapture capture("/home/timwalter/testin.mp4");
	if (!capture.isOpened()) {
		std::cerr << "Error opening video file!" << std::endl;
	}

	int frameCount = 1; //static_cast<int>(capture.get(CAP_PROP_FRAME_COUNT));
	int height = static_cast<int>(capture.get(CAP_PROP_FRAME_HEIGHT));
	int width = static_cast<int>(capture.get(CAP_PROP_FRAME_WIDTH));


	// Combined image tensor: (pixels, frames, color)
	int pixelCount = width * height;
	Mat combinedBlue(pixelCount, frameCount, CV_32F, Scalar(0));
	Mat combinedGreen(pixelCount, frameCount, CV_32F, Scalar(0));
	Mat combinedRed(pixelCount, frameCount, CV_32F, Scalar(0));

	// Read all frames and flatten them
	for (int frameIndex = 0; frameIndex < frameCount; frameIndex++) {
		Mat frame;
		if (!capture.read(frame)) {
			break;
		}

		Mat channels[3];
		split(frame, channels);

		Mat blueFlat = channels[0].reshape(1, pixelCount);
		Mat greenFlat = channels[1].reshape(1, pixelCount);
		Mat redFlat = channels[2].reshape(1, pixelCount);

		blueFlat.convertTo(blueFlat, CV_32F);
		greenFlat.convertTo(greenFlat, CV_32F);
		redFlat.convertTo(redFlat, CV_32F);

		blueFlat.copyTo(combinedBlue.col(frameIndex));
		greenFlat.copyTo(combinedGreen.col(frameIndex));
		redFlat.copyTo(combinedRed.col(frameIndex));
	}

	capture.release();
	
	Mat svdB = performSVD(combinedBlue);
	Mat svdG = performSVD(combinedGreen);
	Mat svdR = performSVD(combinedRed);

	Mat resultB, resultG, resultR;
	normalize(svdB, resultB, 0, 255, NORM_MINMAX);
	normalize(svdG, resultG, 0, 255, NORM_MINMAX);
	normalize(svdR, resultR, 0, 255, NORM_MINMAX);

	resultB.convertTo(resultB, CV_8U);
	resultG.convertTo(resultG, CV_8U);
	resultR.convertTo(resultR, CV_8U);

	// --- Write output video ---
	int fourcc = VideoWriter::fourcc('m', 'p', '4', 'v');

	
	VideoWriter outputVideo("/home/timwalter/test.mp4", fourcc, 30, cv::Size(width, height));

	for (int frameIndex = 0; frameIndex < frameCount; frameIndex++) {
		Mat blueFrame = resultB.col(frameIndex).reshape(1, height);
		Mat greenFrame = resultG.col(frameIndex).reshape(1, height);
		Mat redFrame = resultR.col(frameIndex).reshape(1, height);

		Mat channels[3] = { blueFrame, greenFrame, redFrame };
		Mat frame;
		merge(channels, 3, frame);
		outputVideo.write(frame);
	}

	outputVideo.release();
	
}

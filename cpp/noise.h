#include <opencv2/opencv.hpp>

#include "simple.h"
// #include "std"
using namespace cv;

double estimateNoisePatch(const cv::Mat& img, int patch = 8)
{
    cv::Mat gray;
    if (img.channels() == 3)
        cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    else
        gray = img.clone();

    int h = gray.rows, w = gray.cols;

    std::vector<double> vars;
    for (int y = 0; y < h - patch; y += patch)
    {
        for (int x = 0; x < w - patch; x += patch)
        {
            cv::Mat block = gray(cv::Rect(x, y, patch, patch));

            cv::Scalar mean, stddev;
            cv::meanStdDev(block, mean, stddev);
            double v = stddev[0] * stddev[0];

            // smooth region → good for noise detection
            if (v < 50)
                vars.push_back(v);
        }
    }

    if (vars.empty())
        return 0;

    double avg = 0;
    for (double v : vars) avg += v;
    avg /= vars.size();

    return std::sqrt(avg);  // noise sigma
}



void displayImage(const std::string &path)
{
   Mat img = imread(path, IMREAD_GRAYSCALE);
 

  

    // /// build histogram
    // int bins = 256;
    // float range[] = { 0,256 };
    // const float* histRange = { range };

    // Mat hist;
    // calcHist(&noiseShift, 1, 0, Mat(), hist, 1, &bins, &histRange, true, false);

    // // draw histogram
    // int width = 512, height = 400;
    // int binW = cvRound((double)width / bins);

    // Mat histImg(height, width, IMREAD_GRAYSCALE);
    // histImg.setTo(Scalar(255));

    // normalize(hist, hist, 0, histImg.rows, NORM_MINMAX);

    // for (int i = 1; i < bins; i++) {
    //     line(
    //         histImg,
    //         Point(binW * (i - 1), height - cvRound(hist.at<float>(i - 1))),
    //         Point(binW * (i), height - cvRound(hist.at<float>(i))),
    //         Scalar(0), 2
    //     );
    // }

    imshow("Original", img);
    // imshow("Noise (gray offset)", noiseShift);
    // imshow("Noise Histogram", histImg);
    waitKey(0);
}

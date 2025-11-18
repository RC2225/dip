#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core.hpp"
#include "opencv2/face.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "memory"
#include "iostream"
#include <vector>
#include <memory>
#include "hello.h"

using namespace cv;

int main(int argc, char **args)
{

    Mat B = imread(openFileDialog(), IMREAD_COLOR);
    Mat G = imread(openFileDialog(), IMREAD_COLOR);
    B.convertTo(B, CV_32F);
    G.convertTo(G, CV_32F);
    std::vector<Mat> bC, gC;
    split(B, bC);
    split(G, gC);
    std::vector<Mat> bg_channel(3), gb_channel(3);
    for (int c = 0; c < 3; ++c)
    {
        cv::SVD svdB(bC[c], cv::SVD::FULL_UV);
        Mat wB = svdB.w.clone();
        Mat uB = svdB.u.clone();
        Mat vtB = svdB.vt.clone();

        cv::SVD svdG(gC[c], cv::SVD::FULL_UV);
        Mat wG = svdG.w.clone();
        Mat uG = svdG.u.clone();
        Mat vtG = svdG.vt.clone();

        Mat diag_wG = Mat::diag(wG);
        bg_channel[c] = uB * diag_wG * vtB;

        Mat diag_wB = Mat::diag(wB);
        gb_channel[c] = uG * diag_wB * vtG;
    }

    Mat BG, GB;
    merge(bg_channel, BG);
    merge(gb_channel, GB);

    normalize(BG, BG, 0, 255, NORM_MINMAX);
    BG.convertTo(BG, CV_8UC3);

    normalize(GB, GB, 0, 255, NORM_MINMAX);
    GB.convertTo(GB, CV_8UC3);

    imshow("BG swapped", BG);
    waitKey();

    imshow("GB swapped", GB);
    waitKey();

    return 0;
}

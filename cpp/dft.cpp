#include "dft.h"

using namespace cv;
namespace dft{

    cv::Mat dft1d (cv::Mat input){

        int m = cv::getOptimalDFTSize(input.rows);
        int n = cv::getOptimalDFTSize(input.cols);

        cv::Mat img_padded;

        cv::copyMakeBorder(input, img_padded, 0, m - input.rows, 0, n - input.cols, cv::BORDER_CONSTANT, Scalar::all(0));

        Mat img_planes[] = {Mat_<float>(img_padded), Mat::zeros(img_padded.size(), CV_32F)}; 
        //img_planes[0] = img_padded
        Mat img_complex;
        // merge(img_planes, 2, img_complex);
        // Mat img_fdft; // Fourier coefficients
        // cv::dft(img_complex, img_fdft, cv::DFT_SCALE|cv::DFT_COMPLEX_OUTPUT); 

        // // split to compute magtitude and phase
        // Mat img_fdft_planes[] = {Mat::zeros(img_padded.size(), CV_32F), Mat::zeros(img_padded.size(), CV_32F)}; 
        // split(img_fdft, img_fdft_planes); 
        // // img_fdft_planes[0] = Re(DFT(img_padded), img_fdft_planes[1] = Im(DFT(img_padded))

        // Mat img_Re = img_fdft_planes[0];
        // Mat img_Im = img_fdft_planes[1];

        // // compute magtitude of image
        // Mat img_magnitude = Mat::zeros(img_padded.size(), CV_32F);
        // magnitude(img_Re, img_Im, img_magnitude);

        // //normalize(img_magnitude, img_magnitude, 0, 255, CV_MINMAX);
        // //imshow("mag", img_magnitude);
        // // compute phase of image
        // Mat img_phase = Mat::zeros(img_padded.size(), CV_32F);
        // phase(img_Re, img_Im, img_phase);

        // normalize(img_phase, img_phase, 0, 255); 
        return input;
        
    }

    cv::Mat dft2d (cv::Mat input){
        
    }
}

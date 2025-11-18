#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
// #include <libadwaita-1/adwaita.h>
// #include <gtk/gtk.h>
#include <iostream>
#include <cstdlib>
#include <array>
#include <functional>



/// @brief a basic function which applies an function to each pixel.
/// @param img 
void loop_img(cv::Mat &img, void (*f)(cv::Mat&));

/// @brief negates all color in an image. 
/// @param img 
cv::Mat negative(cv::Mat img);

/// @brief Converts an image to a logarithmic scale
/// @param img 
cv::Mat logImage(cv::Mat img);

cv::Mat threshold(cv::Mat img, int threshold);
cv::Mat threshold_zero(cv::Mat img);
cv::Mat threshold_binary(cv::Mat img);
cv::Mat threshold_contrast(cv::Mat img);

void sobel(cv::Mat &image);
void roberts(cv::Mat &image);
void previtt(cv::Mat &image);

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

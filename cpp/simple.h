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

// /// @brief a basic function which applies an function to each pixel.
// /// @param img 
// void loop_img(cv::Mat &img, void (*f)(cv::Mat&));

// /// @brief negates all color in an image. 
// /// @param img 
// cv::Mat negative(cv::Mat img);

// /// @brief Converts an image to a logarithmic scale
// /// @param img 
// cv::Mat logImage(cv::Mat img);

// cv::Mat threshold(cv::Mat img, int threshold);
// cv::Mat threshold_zero(cv::Mat img);
// cv::Mat threshold_binary(cv::Mat img);
// cv::Mat threshold_contrast(cv::Mat img);

// void sobel(cv::Mat &image);
// void roberts(cv::Mat &image);
// void previtt(cv::Mat &image);

cv::Mat threshold_contrast(cv::Mat img);

void sobel(cv::Mat &image);
void roberts(cv::Mat &image);
void previtt(cv::Mat &image);

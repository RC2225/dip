#include <vector>
#include <memory>

using namespace cv;

void loop_img(const cv::Mat &img, void (*f)(cv::Mat &))
{
}

cv::Mat negative()
{
    Mat dst = Mat::zeros(img.size(), img.type());

    for (int i = 0; i < img.cols; i++)
    {

        for (int j = 0; j < img.rows; j++)
        {
            dst.at<Vec3b>(cv::Point(i, j))[0] = 255 - img.at<Vec3b>(cv::Point(i, j))[0];
            dst.at<Vec3b>(cv::Point(i, j))[1] = 255 - img.at<Vec3b>(cv::Point(i, j))[1];
            dst.at<Vec3b>(cv::Point(i, j))[2] = 255 - img.at<Vec3b>(cv::Point(i, j))[2];
        }
    }
    return dst;
};
void processHistgramm(const cv::Mat &mat, const int numBins)
{
    // const int binSize = 255 - 255 / numBins;
    int b_bin[16];

    for (int i = 0; i < mat.cols; i++)
    {
        for (int j = 0; j < mat.rows; j++)
        {
            Vec3b brightness = mat.at<cv::Vec3b>(i, j);
            uchar b = brightness.val[0];
            uchar g = brightness.val[1];
            uchar r = brightness.val[2];
            if (b < numBins)
            {
                auto index = numBins * brightness;
                b_bin[1]++;
            }
            else
            {
                // b_hist[numBins -1]++;
            }
        }
    }
}

cv::Mat logImage(cv::Mat img)
{
    Mat img_float;
    img.convertTo(img_float, CV_32F);
    img_float += 1;
    Mat dst;
    dst = img_float.clone();
    cv::log(dst, dst);
    cv::convertScaleAbs(dst, dst);
    cv::normalize(dst, dst, 0, 255, cv::NORM_MINMAX);
    return dst;
}

cv::Mat threshold(const cv::Mat img, int threshold)
{
    Mat dst = Mat::zeros(img.size(), img.type());
    ;
    int failed = 0;
    for (int i = 0; i < img.cols; i++)
    {

        for (int j = 0; j < img.rows; j++)
        {
            for (int i = 0; i < 3; i++)
            {
                if (dst.at<Vec3b>(cv::Point(i, j))[i] > threshold)
                {
                    dst.at<Vec3b>(cv::Point(i, j))[i] = 255;
                }
                else
                {
                    dst.at<Vec3b>(cv::Point(i, j))[i] = 0;
                    failed++;
                }
            }
        }
    }
    std::cout << failed << std::endl;
    return dst;
}

void roberts(cv::Mat &image)
{
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y, dst;
    // Define the Roberts kernels
    // Note: Kernels must be of type CV_32F or CV_64F for filter2D to work well with derivatives
    Mat kernel_x = (Mat_<float>(2, 2) << 1, 0, 0, -1);
    Mat kernel_y = (Mat_<float>(2, 2) << 0, 1, -1, 0);
    // Apply the kernels using filter2D
    // ddepth is set to CV_32F to avoid saturation issues with gradients
    filter2D(image, grad_x, CV_32F, kernel_x);
    filter2D(image, grad_y, CV_32F, kernel_y);
    // Compute the magnitude of the gradient
    // This can be done using the approximate method (abs) or the more accurate method (magnitude)
    // Here we use magnitude for better results
    magnitude(grad_x, grad_y, dst);
    // The result is in floating point. Convert to 8-bit image for display
    convertScaleAbs(dst, dst);
}
void previtt(cv::Mat &image)
{
    cv::Mat kernel_x = (cv::Mat_<float>(3, 3) << -3, 0, 1, -1, 0, 1, -1, 0, 1);
    cv::Mat kernel_y = (cv::Mat_<float>(3, 3) << -3, -1, -1, 0, 0, 0, 1, 1, 1);
    // Apply convolution
    cv::Mat grad_x, grad_y;
    cv::filter2D(image, grad_x, CV_32F, kernel_x); // Use CV_32F for floating-point

    cv::filter2D(image, grad_y, CV_32F, kernel_y);
    // Calculate gradient magnitude
    cv::Mat magnitude;
    cv::magnitude(grad_x, grad_y, magnitude);
    // Normalize and convert to 8-bit for display
    cv::normalize(magnitude, magnitude, 0, 255, cv::NORM_MINMAX, CV_8U);
    // Optional: Apply thresholding
    cv::Mat edges;
    cv::threshold(magnitude, edges, 50, 255, cv::THRESH_BINARY); // Example threshold
    // Display results
    cv::imshow("Original Image", image);
    cv::imshow("Prewitt Edges", edges);
}

void sobel(cv::Mat &src_gray)
{
    
    // Apply Gaussian blur to reduce noise (optional)
    GaussianBlur(src_gray, src_gray, cv::Size(3, 3), 0, 0, BORDER_DEFAULT);
    // Declare variables for gradients
    Mat grad_x, grad_y, abs_grad_x, abs_grad_y, grad;

    // Calculate the X gradient
    Sobel(src_gray, grad_x, CV_16S, 1, 0, 3);
    // convert to CV_8U and absolute values
    convertScaleAbs(grad_x, abs_grad_x);
    // Calculate the Y gradient
    Sobel(src_gray, grad_y, CV_16S, 0, 1, 3);
    // convert to CV_8U and absolute values
    convertScaleAbs(grad_y, abs_grad_y);
    // Combine the gradient components
    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
    // Display results
    imshow("Original Image", src_gray);
    imshow("Sobel X Gradient", abs_grad_x);
    imshow("Sobel Y Gradient", abs_grad_y);
    imshow("Combined Sobel Edge Map", grad);
}

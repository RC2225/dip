
#include "hello.h"
#include <vector>
#include <memory>

using namespace cv;

// int[]  m_g_hist;
int main(int argc, char **argv)
{) -> an
    std::string filename = openFileDialog();
    std::cout << "Selected file: " << filename << std::endl;
    if (filename == "")
    {
        std::cout << " No file selected";
        return 1;
    }
    Mat img = imread(filename, IMREAD_COLOR);
    Mat res;
    resize(img, res, Size(), 0.5, 0.5, INTER_LINEAR);
    // imshow("lskdf",negative(res));
    imshow("lskd2f",threshold(res, 150));

    int k = waitKey();
  
    k = waitKey();

    return 0;
}

void loop_img(const cv::Mat &img, void (*f)(cv::Mat &))
{
}

cv::Mat negative(const cv::Mat &img)
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
}
 ;
void processHistgramm(const cv::Mat &mat, const int numBins ){
    // const int binSize = 255 - 255 / numBins;
     int b_bin[16];
     
     for (int i = 0; i < mat.cols; i++){
        for( int j = 0; j < mat.rows; j++){
            Vec3b brightness = mat.at<cv::Vec3b>(i,j);
            uchar b = brightness.val[0];
            uchar g = brightness.val[1];
            uchar r = brightness.val[2];
            if (b < numBins)
            {
                auto index = numBins * brightness;
                b_bin[1]++;
            } else {
                // b_hist[numBins -1]++;
            }
        }
     }
}


// cv::Mat logImage(cv::Mat img)
// {
//     Mat img_float;
//     img.convertTo(img_float, CV_32F);
//     img_float += 1;
//     Mat dst;
//     dst = img_float.clone();
//     cv::log(dst, dst);
//     cv::convertScaleAbs(dst, dst);
//     cv::normalize(dst, dst, 0, 255, cv::NORM_MINMAX);
//     return dst;
// }

cv::Mat threshold(const cv::Mat img, int threshold)
{
    Mat dst = Mat::zeros(img.size(), img.type());;
    int failed = 0;
    for (int i = 0; i < img.cols; i++)
    {

        for (int j = 0; j < img.rows; j++)
        {
            for (int i = 0; i < 3; i++)
            {
                if( dst.at<Vec3b>(cv::Point(i, j))[i] > threshold){
                    dst.at<Vec3b>(cv::Point(i, j))[i] = 255;
                } else {
                    dst.at<Vec3b>(cv::Point(i, j))[i] = 0;
                    failed++;
                }
                
            }
        }
    }
    std::cout << failed << std::endl;
    return dst;
}
// typedef Vec<uchar, 3> Vec3b;

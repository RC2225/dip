#include <memory>
#include <iostream>
class histogram
{
private:
    int m_num_bins;
    std::unique_ptr<int[]> m_b_data;
    std::unique_ptr<int[]> m_g_data;
    std::unique_ptr<int[]> m_r_data;
public:
    histogram(/* args */);
    ~histogram();

};

histogram::histogram(/* args */)
{
}

histogram::~histogram()
{
}


void histogram::processHistgramm(const cv::Mat &mat, const int numBins ){
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

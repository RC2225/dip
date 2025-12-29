#include <opencv2/opencv.hpp>

namespace dft
{
    /// @brief Does a 1d DFT Transform
    /// @param input 
    /// @return 
   cv::Mat dft1d(cv::Mat input);

   /// @brief Does a 2d DFT Transform
   /// @param input 
   /// @return 
   cv::Mat dft2d(cv::Mat input);
} 
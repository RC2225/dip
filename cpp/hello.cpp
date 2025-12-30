
#include "hello.h"
#include <vector>
#include <memory>

// int[]  m_g_hist;
int main(int argc, char **argv)
{
 
    string filename = openFileDialog();
    cout << "Selected file: " << filename << std::endl;
    
    if (filename == "")
    {
        std::cout << " No file selected";
        return 1;
    }
    
    Ptr<BackgroundSubtractorKNN> knn = createBackgroundSubtractorKNN();
    // Set parameters (or set during creation)
    knn->setHistory(500);
    // Number of frames used for background modeling, More = slower adaptation, less noise
    knn->setDist2Threshold(400.0); // Distance threshold, Lower = more sensitive, higher = less sensitive
    knn->setDetectShadows(true); // Detect shadows, Detect shadows as gray (127)
    knn->setShadowValue(127); // Shadow value in mask (gray)
    knn->setShadowThreshold(0.5); // Shadow threshold (0-1)

    Ptr<BackgroundSubtractor> mog2 = createBackgroundSubtractorMOG2(500, 16, true);
        
    Mat img = imread(filename, IMREAD_COLOR);
    extractBackground(filename, "/home/timwalter/out.mp4", "/home/timwalter/out.jpg", mog2 );
    return 0;
}

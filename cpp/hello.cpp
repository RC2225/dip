
#include "hello.h"

#include <ncurses/curses.h>
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
    knn->setDetectShadows(false); // Detect shadows, Detect shadows as gray (127)
    knn->setShadowValue(127); // Shadow value in mask (gray)
    knn->setShadowThreshold(0.5); // Shadow threshold (0-1)

    Ptr<BackgroundSubtractor> mog2 = createBackgroundSubtractorMOG2(500, 16, false);

    Mat img = imread(filename, IMREAD_COLOR);
    extractBackground(filename, "/home/timwalter/outknn.mp4", "/home/timwalter/outknn.jpg", knn );
    extractBackground(filename, "/home/timwalter/outmog2.mp4", "/home/timwalter/outmog2.jpg", mog2 );
    analyzeImage(filename);
    analyzeImage("/home/timwalter/outmog2.mp4");

    return 0;
}


#include "hello.h"
#include "simple.h"
#include "noise.h"
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
    
    
    Mat img = imread(filename, IMREAD_COLOR);
    extractBackground(filename, "/home/timwalter/out.mp4", "/home/timwalter/out.jpg", createBackgroundSubtractorMOG2(500, 16, true));

    return 0;
}

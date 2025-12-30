
#include "hello.h"

#include <ncurses/curses.h>

void segment(string filename)
{
    Ptr<BackgroundSubtractorKNN> knn = createBackgroundSubtractorKNN();
    // Set parameters (or set during creation)
    knn->setHistory(500);
    // Number of frames used for background modeling, More = slower adaptation, less noise
    knn->setDist2Threshold(400.0); // Distance threshold, Lower = more sensitive, higher = less sensitive
    knn->setDetectShadows(false);  // Detect shadows, Detect shadows as gray (127)
    knn->setShadowValue(127);      // Shadow value in mask (gray)
    knn->setShadowThreshold(0.5);  // Shadow threshold (0-1)

    Ptr<BackgroundSubtractor> mog2 = createBackgroundSubtractorMOG2(500, 16, false);

    Mat img = imread(filename, IMREAD_COLOR);
    extractBackground(filename, "/home/timwalter/outknn.mp4", "/home/timwalter/outknn.jpg", knn);
    extractBackground(filename, "/home/timwalter/outmog2.mp4", "/home/timwalter/outmog2.jpg", mog2);
}

void analyze(string filename)
{
    analyzeImage(filename);
    analyzeImage("/home/timwalter/outmog2.mp4");
}

void draw_button(int y, int x, const char *label)
{
    int width = strlen(label) + 4;

    mvprintw(y, x, "+");
    for (int i = 0; i < width - 2; i++)
        printw("-");
    printw("+");

    mvprintw(y + 1, x, "| %s |", label);

    mvprintw(y + 2, x, "+");
    for (int i = 0; i < width - 2; i++)
        printw("-");
    printw("+");
}

int main(void)
{
    std::string filename = "";
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    mvprintw(1, 2, "Hello World (ncurses)");
    mvprintw(3, 2, "Press 1, 2, or 3 to activate a button. Press q to quit.");

    draw_button(5, 2, "OpenFile");
    draw_button(5, 15, "Segmentation");
    draw_button(5, 35, "Analyze");

    int ch;
    while ((ch = getch()) != 'q')
    {
        move(18, 2);
        clrtoeol();

        switch (ch)
        {
        case '1':
            filename = openFileDialog();
            mvprintw(13, 2, filename.data());
            break;
        case '2':
            segment(filename);
            mvprintw(18, 2, "Run Segmentation button pressed");
            break;
        case '3':
            analyze(filename);
            mvprintw(18, 2, "Analyze button pressed");
            break;
        }
        refresh();
    }

    endwin();
    return 0;
}

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

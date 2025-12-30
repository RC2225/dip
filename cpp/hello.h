#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
// #include <opencv2/bgsegm.hpp>
// #include <opencv2/core/utils/logger.hpp>
#include <iostream>
#include <cstdlib>
#include <array>
#include <ncurses.h>
// #include <functional>
// #include <cmath>
// #include <vector>
// #include <memory>

using namespace cv;
using namespace std;
std::string openFileDialog()
{
    std::array<char, 512> buffer;
    std::string result;

    FILE *pipe = popen("zenity --file-selection", "r");
    if (!pipe)
        return "Error";

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
    {
        result += buffer.data();
    }

    pclose(pipe);

    // Remove trailing newline character
    if (!result.empty() && result.back() == '\n')
    {
        result.pop_back();
    }

    return result;
}

void extractBackground(std::string inputVideoPath, std::string outputVideoPath, std::string outputBgImage, Ptr<BackgroundSubtractor> bgSubtractor)
{
    // std::cout << cv::getBuildInformation() << std::endl;
    // cv::utils::logging::setLogLevel(
    // 	cv::utils::logging::LOG_LEVEL_INFO);

    VideoCapture cap(inputVideoPath, cv::CAP_FFMPEG);
    if (!cap.isOpened())
    {
        cerr << "Error opening video file!" << endl;
        std::exit(1);
    }

    int width = (int)cap.get(CAP_PROP_FRAME_WIDTH);
    int height = (int)cap.get(CAP_PROP_FRAME_HEIGHT);
    double fps = cap.get(CAP_PROP_FPS);

    VideoWriter writer(
        outputVideoPath,
        VideoWriter::fourcc('m', 'p', '4', 'v'),
        fps,
        cv::Size(width, height));

    // bgSubtractor =
    // 			cv::bgsegm::createBackgroundSubtractorLSBP(
    // 			cv::bgsegm::LSBP_CAMERA_MOTION_COMPENSATION_NONE, // Motion compensation
    // 			20,
    // 			// Number of samples
    // 			16,
    // 			// LSBPattern radius
    // 			true,
    // 			// Use color
    // 			0.5f
    // 			// Lower threshold
    // );
    Mat frame, fgMask, fgMaskClean, foreground;

    // Floating-point background accumulator
    Mat bgFloat(height, width, CV_32FC3, Scalar(0, 0, 0));
    Mat bgCount(height, width, CV_32FC1, Scalar(0));

    Mat kernel = getStructuringElement(MORPH_ELLIPSE, cv::Size(5, 5));

    while (true)
    {
        cap >> frame;
        if (frame.empty())
            break;

        // Foreground mask
        bgSubtractor->apply(frame, fgMask);
        threshold(fgMask, fgMask, 200, 255, THRESH_BINARY);

        morphologyEx(fgMask, fgMaskClean, MORPH_OPEN, kernel);
        morphologyEx(fgMaskClean, fgMaskClean, MORPH_DILATE, kernel);

        // Foreground video
        foreground = Mat::zeros(frame.size(), frame.type());
        frame.copyTo(foreground, fgMaskClean);
        writer.write(foreground);

        // Background update mask (background = inverse of fg)
        Mat bgMask;
        bitwise_not(fgMaskClean, bgMask);

        Mat frameFloat, bgMaskFloat;

        frame.convertTo(frameFloat, CV_32FC3);

        frameFloat.copyTo(frameFloat, bgMask); // zero out foreground
        bgFloat += frameFloat;

        bgMask.convertTo(bgMaskFloat, CV_32FC1, 1.0 / 255.0);
        bgCount += bgMaskFloat;
    }

    cap.release();
    writer.release();

    // Final background image
    Mat bgFinal;
    // divide(bgFloat, bgCount, bgFinal);
    Mat bgCount3;
    cvtColor(bgCount, bgCount3, COLOR_GRAY2BGR);

    divide(bgFloat, bgCount3, bgFinal);

    // Replace NaNs (never-seen pixels)
    bgFinal.setTo(0, bgCount == 0);

    bgFinal.convertTo(bgFinal, CV_8UC3);
    imwrite(outputBgImage, bgFinal);

    cout << "Foreground video saved to: " << outputVideoPath << endl;
    cout << "Background image saved to: " << outputBgImage << endl;
}

// --------------------------------------------------
// Compute mean, variance, skewness, kurtosis
// --------------------------------------------------
void computeMoments1(const vector<float> &data,
                     double &mean, double &var,
                     double &skew, double &kurt)
{
    int N = data.size();
    mean = var = skew = kurt = 0.0;

    for (float x : data)
        mean += x;
    mean /= N;

    for (float x : data)
        var += (x - mean) * (x - mean);
    var /= N;

    double stddev = sqrt(var + 1e-8);

    for (float x : data)
    {
        skew += pow((x - mean) / stddev, 3);
        kurt += pow((x - mean) / stddev, 4);
    }

    skew /= N;
    kurt = kurt / N - 3.0; // excess kurtosis
}

// --------------------------------------------------
// Plot histogram (noise PDF visualization)
// --------------------------------------------------
void plotHistogram1(const Mat &noise)
{
    int histSize = 256;
    float range[] = {-128, 128};
    const float *histRange = {range};

    Mat hist;
    calcHist(&noise, 1, 0, Mat(), hist, 1, &histSize, &histRange);

    Mat histImg(400, 512, CV_8UC3, Scalar(0, 0, 0));
    normalize(hist, hist, 0, histImg.rows, NORM_MINMAX);

    for (int i = 1; i < histSize; i++)
    {
        line(histImg,
             Point((i - 1) * 2, histImg.rows - cvRound(hist.at<float>(i - 1))),
             Point(i * 2, histImg.rows - cvRound(hist.at<float>(i))),
             Scalar(255, 255, 255), 2);
    }

    imshow("Noise Histogram", histImg);
}

// --------------------------------------------------
// Classify noise type from statistics
// --------------------------------------------------
string classifyNoise1(double skew, double kurt, double impulseRatio)
{
    if (impulseRatio > 0.01)
        return "Salt-and-Pepper";

    if (abs(skew) < 0.3 && abs(kurt) < 1.0)
        return "Gaussian";

    if (abs(skew) < 0.3 && kurt < -1.0)
        return "Uniform";

    if (skew > 0.7 && skew < 1.8 && kurt > 0)
        return "Rayleigh";

    if (skew >= 2.0 && kurt > 4.0)
        return "Exponential";

    if (skew > 1.0 && kurt > 1.0)
        return "Gamma / Erlang";

    return "Unknown / Mixed";
}

// --------------------------------------------------
// Main
// --------------------------------------------------
int analyzeImage(std::string path)
{
    VideoCapture cap(path);
    if (!cap.isOpened())
    {
        cout << "Error opening video" << endl;
        return -1;
    }

    map<string, int> voteCounter;

    int frameIndex = 0;     // actual frame number
    int analyzedFrames = 0; // frames used for statistics
    int frameCount = 0;

    while (true)
    {

        Mat frame;
        cap >> frame;
        if (frame.empty())
            break; 

        // ------------------------------
        // PROCESS EVERY 3RD FRAME ONLY
        // ------------------------------
        if (frameIndex % 3 != 0)
        {
            frameIndex++;
            continue;
        }

        // Convert to grayscale
        Mat gray;
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        gray.convertTo(gray, CV_32F);

        // ------------------------------
        // Noise extraction
        // ------------------------------
        Mat median, smooth;
        medianBlur(gray, median, 5);
        GaussianBlur(median, smooth, Size(5, 5), 1.2);

        Mat noise = gray - smooth;
        normalize(noise, noise, -128, 127, NORM_MINMAX);

        // Collect samples
        vector<float> samples;
        int impulseCount = 0;

        for (int y = 0; y < noise.rows; y++)
        {
            for (int x = 0; x < noise.cols; x++)
            {
                float v = noise.at<float>(y, x);
                samples.push_back(v);
                if (abs(v) > 120)
                    impulseCount++;
            }
        }

        double impulseRatio = (double)impulseCount / samples.size();

        // Statistics
        double mean, var, skew, kurt;
        computeMoments1(samples, mean, var, skew, kurt);

        // Classification
        string noiseType = classifyNoise1(skew, kurt, impulseRatio);
        voteCounter[noiseType]++;

        cout << "Frame " << frameIndex
             << " | Noise: " << noiseType
             << " | Skew: " << skew
             << " | Kurt: " << kurt
             << " | Impulse: " << impulseRatio * 100 << "%" << endl;

        // Visualization
        plotHistogram1(noise);
        putText(frame, noiseType, Point(30, 40), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 255, 0), 2);

        imshow("Video", frame);
        if (waitKey(30) == 27)
            break;

        analyzedFrames++;

        frameIndex++;
    }

    // ------------------------------
    // FINAL DECISION (MAJORITY VOTE)
    // ------------------------------
    string finalNoise = "Unknown";
    int maxVotes = 0;
    int totalVotes = 0;

    for (const auto &p : voteCounter)
    {
        totalVotes += p.second;
        if (p.second > maxVotes)
        {
            maxVotes = p.second;
            finalNoise = p.first;
        }
    }

    // Confidence (% of frames agreeing)
    double confidence = 100.0 * maxVotes / totalVotes;

    cout << "\n==============================\n";
    cout << "FINAL DETECTED NOISE TYPE : " << finalNoise << endl;
    cout << "CONFIDENCE                : " << confidence << " %" << endl;
    cout << "==============================\n";

    waitKey(0);
    return 0;
}
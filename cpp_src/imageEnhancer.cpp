#include "../includes/imageEnhancer.h"
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;

cv::Mat ImageEnhancer::enhanceRegion(const Mat &faceROI) {
    Mat upscaled, denoised, sharpened, finalFace;

    resize(faceROI, upscaled, Size(), 2.0, 2.0, INTER_CUBIC);

    bilateralFilter(upscaled, denoised, 15, 75, 75);

    Mat kernel = (Mat_<float>(3, 3) <<
                  -1, -1, -1,
                  -1,  9, -1,
                  -1, -1, -1);
    filter2D(denoised, sharpened, -1, kernel);

    Mat lab;
    cvtColor(sharpened, lab, COLOR_BGR2Lab);
    vector<Mat> channels;
    split(lab, channels);
    Ptr<CLAHE> clahe = createCLAHE(3.0, Size(8, 8));
    clahe->apply(channels[0], channels[0]);
    merge(channels, lab);
    cvtColor(lab, finalFace, COLOR_Lab2BGR);

    return finalFace;
}

cv::Mat ImageEnhancer::enhanceFaces(const Mat &image, const vector<Rect> &faces) {
    Mat result = image.clone();

    for (const auto &face : faces) {
        // Ensure ROI is inside image
        Rect roi = face & Rect(0, 0, image.cols, image.rows);
        if (roi.width <= 0 || roi.height <= 0) continue;

        Mat faceROI = image(roi);
        if (faceROI.cols < 20 || faceROI.rows < 20) continue;

        Mat enhanced = enhanceRegion(faceROI);

        Mat resizedEnhanced;
        resize(enhanced, resizedEnhanced, roi.size(), 0, 0, INTER_CUBIC);

        resizedEnhanced.copyTo(result(roi));
    }

    return result;
}

#include "../includes/utils.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

namespace Utils {

Mat loadImage(const string &path) {
    Mat img = imread(path);
    if (img.empty()) cerr << "[Error] Could not load image: " << path << endl;
    return img;
}

bool saveImage(const string &path, const Mat &image) {
    bool success = imwrite(path, image);
    if (!success) cerr << "[Error] Could not save image: " << path << endl;
    return success;
}

void showImage(const string &windowName, const Mat &image) {
    namedWindow(windowName, WINDOW_AUTOSIZE);
    imshow(windowName, image);
    waitKey(0);
    destroyWindow(windowName);
}

Mat sharpenImage(const Mat &image) {
    Mat sharpened, kernel;
    kernel = (Mat_<float>(3,3) <<
              0, -1, 0,
             -1,  5, -1,
              0, -1, 0);
    filter2D(image, sharpened, image.depth(), kernel);
    return sharpened;
}

Mat enhanceResolution(const Mat &image, double scaleFactor) {
    Mat resized;
    resize(image, resized, Size(), scaleFactor, scaleFactor, INTER_CUBIC);
    return resized;
}

Mat reduceBlur(const Mat &image) {
    Mat deblurred;
    GaussianBlur(image, deblurred, Size(0, 0), 3);
    addWeighted(image, 1.5, deblurred, -0.5, 0, deblurred);
    return deblurred;
}

Mat toGray(const Mat &image) {
    Mat gray;
    if (image.channels() == 3)
        cvtColor(image, gray, COLOR_BGR2GRAY);
    else
        gray = image.clone();
    return gray;
}

}

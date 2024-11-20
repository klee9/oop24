#include <opencv2/opencv.hpp>
#include <iostream>

cv::VideoCapture camera(0);

// test code
int main() {
    if (!camera.isOpened()) {
        std::cerr << "Error: Could not open the webcam!" << std::endl;
        return -1;
    }
}

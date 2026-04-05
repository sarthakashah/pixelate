#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

cv::Mat rotate(cv::Mat src, double angle)
{
    // Mat object for output image file
    cv::Mat dst;
    // defining point with respect to which to rotate
    cv::Point2f pt(src.cols/2., src.rows/2.);
    // Mat object for storing after rotation
    cv::Mat r = cv::getRotationMatrix2D(pt, angle, 1.0);
    // apply an affine transformation to image.
    cv::warpAffine(src, dst, r, cv::Size(src.cols, src.rows));
    //returning Mat object for output image file
    return dst;
}
cv::Mat invert(cv::Mat img, int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i % 2 == 0) {
                //this statement accesses the individual pixel values of the image
                //and sets the color values of each channel of every coordinate to 0
                img.at<cv::Vec3b>(i, j)[0] = 255 - img.at<cv::Vec3b>(i, j)[0];
                img.at<cv::Vec3b>(i, j)[1] = 255 - img.at<cv::Vec3b>(i, j)[1];
                img.at<cv::Vec3b>(i, j)[2] = 255 - img.at<cv::Vec3b>(i, j)[2];
            }
        }
    }
    return img;
}

int main()
{
    // Load the three fragment images as grayscale — do not change these lines
    cv::Mat frag_a = cv::imread("./assets/fragment_a.png", cv::IMREAD_GRAYSCALE);
    cv::Mat frag_b = cv::imread("./assets/fragment_b.png", cv::IMREAD_GRAYSCALE);
    cv::Mat frag_c = cv::imread("./assets/fragment_c.png", cv::IMREAD_GRAYSCALE);

    
    if (frag_a.empty() || frag_b.empty() || frag_c.empty()) {
        cerr << "Could not load one or more fragment images from ./assets/\n";
        return -1;
    }

    // TODO: reverse the charm on each fragment, merge the recovered channels
    // into a single BGR image, save it as ./assets/memory_restored.png,
    // and print the number of non-zero pixels in the result.

    cv::Mat s1, s2, s3, mat1, mat2, mat3;
    s1 = rotate(frag_a, 90);
    s1 =invert(frag_a, frag_a.size().height, frag_a.size().width);
    s2 = frag_b;
    s3 =rotate(frag_c, 90.00);

    mat1.convertTo(s1, CV_32S);
    mat2.convertTo(s2, CV_32S);
    mat3.convertTo(s3, CV_32S);

    cv::Mat sum32 = s1 + s2 + s3;
    cv::Mat masked;
    cv::bitwise_and(sum32, cv::Scalar(0xFF), masked);
    cv::Mat result_A;
    masked.convertTo(result_A, CV_8U);
    cv::imshow("Output", );

    return 0;
}

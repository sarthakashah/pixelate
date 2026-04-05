#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

/// @brief : Convolution will help apply different kernels to your images yielding different results
/// @param   Original_image
/// @param   kernel
/// @return : An Output Image with the kernel applied
cv::Mat convolve(cv::Mat original_image, cv::Mat kernel){
    
    cv::Mat kernel_inv{kernel.size(), kernel.type()}, temp_kernel{kernel.size(), kernel.type()}, image_pad;
    cv::Mat resultant_image{original_image.size(), original_image.type()};
    
    // Using For Loops
    // for(int i = 0 ; i < kernel.rows ; i++ ){ cout<<"k"<<endl; kernel_inv.row(i) = kernel.row(kernel.rows - i - 1).clone(); }
    // temp_kernel = kernel_inv.clone();
    // for(int i = 0 ; i < kernel.cols ; i++ ){ kernel_inv.col(i) = temp_kernel.col(kernel.cols - i - 1).clone();}

    // Method 2: Simply using in-built functions
    // flip(kernel, kernel_inv, -1);  

    // Viewing channels for each image
    // cout<<original_image.channels()<<endl;

    // Adding Padding to the Image 
    cv::copyMakeBorder(original_image, image_pad, 1, 1, 1, 1, cv::BORDER_REPLICATE, cv::Scalar(0)); 

    for (int i = 1; i < image_pad.rows - 1; i++) 
    {   
        for (int j = 1; j < image_pad.cols - 1 ; j++) 
        {
            for(int ch = 0 ; ch < original_image.channels() ; ch++){

                double tmp = 0.0;
                for (int k = 0; k < kernel_inv.rows; k++) 
                {
                    for (int l = 0; l < kernel_inv.cols; l++) 
                    {
                        int x = j - 1 + l;
                        int y = i - 1 + k;  

                        if ((x >= 0 && x < image_pad.cols) && (y >= 0 && y < image_pad.rows)){
                            tmp += (double)image_pad.at<cv::Vec3b>(y, x).val[ch] * (double)kernel.at<double>(k, l);
                        }
                    }
                }

                resultant_image.at<cv::Vec3b>(i-1, j-1).val[ch] = cv::saturate_cast<uchar>(tmp);        // Why Unsigned? 
            }
        }
    }

    return resultant_image;
}

int main()
{
    
    // The two kernels — do not change these
    cv::Mat K1 = (cv::Mat_<double>(3, 3) <<
         1,  2,  1,
         0,  0,  0,
        -1, -2, -1
    );

    cv::Mat K2 = (cv::Mat_<double>(3, 3) <<
         1,  0, -1,
         2,  0, -2,
         1,  0, -1
    );

    cv::Mat img = cv::imread("./assets/hogwarts.png", cv::IMREAD_GRAYSCALE);

    if (img.empty()) {
        cerr << "Could not load image at ./assets/hogwarts.png\n";
        return -1;
    }
    cv::Mat gaussian_v_k1 = (cv::Mat_<double>(3, 1) << 1, 0, -1);
    cv::Mat gaussian_h_k1 = (cv::Mat_<double>(1, 3) << 1, 2, 1);
    cv::Mat gaussian_v_k2 = (cv::Mat_<double>(3, 1) << 1, 2, 1);
    cv::Mat gaussian_h_k2 = (cv::Mat_<double>(1, 3) << 1, 0, -1);
    
    cv::Mat vert_k1 = convolve(img, gaussian_v_k1);
    cv::Mat vert_k2 = convolve(img, gaussian_v_k2);
    cv::Mat hori_k1 = convolve(img, gaussian_h_k1);
    cv::Mat hori_k2 = convolve(img, gaussian_h_k2);

    // cv::Mat sum1 = vert_k1 + hori_k1;
    // cv::Mat sum2 = vert_k2 + hori_k2;
    cv::Mat intermediate1;
    cv::Mat intermediate2;

    // Now to actually convolve
    intermediate1 = convolve(img, gaussian_v_k1);         // Convolve Vertically
    cv::Mat output1 = convolve(intermediate1, gaussian_h_k1);
    intermediate2 = convolve(img, gaussian_v_k2);         // Convolve Vertically
    cv::Mat output2 = convolve(intermediate2, gaussian_h_k2);

    cv::imshow("Output of K1", output1);
    cv::waitKey(0);
    
    cv::imshow("Output of k2", output2);
    // TODO: apply K1 and K2 to img_f with as few multiplications per pixel as possible.
    // Print the number of multiplications your approach uses per pixel.
    std::cout << "Number of multiplications: 6 multiplications per pixel" << endl;
    cv::waitKey(0);

    return 0;
}

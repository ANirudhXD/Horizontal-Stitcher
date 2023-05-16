#include <iostream>
#include <filesystem>
#include <opencv2/opencv.hpp>

namespace fs = std::filesystem;

int main( int argc, char ** argv){
    if(argc!=3){
        std::cerr << "Usage: " << argv[0] << " input_folder output_folder\n";
        return 1;
    }

    std::string input_folder = argv[1];
    std::string output_folder = argv[2];
    
    // checking if directory exists or not
    if(!fs::is_directory(input_folder)){
        std::cerr << "Folder " <<input_folder<<" does not exist.";
        return 1;
    }

    if(!fs::is_directory(output_folder)) fs::create_directory(output_folder);

    // camera_matrix and distortion coefficients hard-coded here:
    cv::Mat camera_matrix = (cv::Mat_<double>(3, 3) <<
                           6.5703352035292482e+02, -4.6493909143213275e+00, 1.0075847591738344e+03,
                           0., 6.5629778616623673e+02, 6.2949197078676627e+02, 
                           0., 0., 1. 
                          );

   cv::Mat dist_coeffs = (cv::Mat_<double>(5, 1) <<
                          -1.1484518065993182e-02, -2.9694982799085730e-02, 3.4574480925275576e-02, -1.4197954135279965e-02
                          );

     // Loop over all files in input folder
    for (const auto& entry : fs::directory_iterator(input_folder)) {

        // Check if file is an image
        if (entry.path().extension() == ".jpg" ||
            entry.path().extension() == ".jpeg" ||
            entry.path().extension() == ".png") {

            // Load image
            cv::Mat image = cv::imread(entry.path().string());

            // Apply the undistort function
            cv::Mat undistorted_img;
            cv::undistort(image, undistorted_img, camera_matrix, dist_coeffs);

            // Display results
            //cv::imshow("Distorted Image", distorted_img);
            //cv::imshow("Undistorted Image", undistorted_img);


            // Save output image to output folder
            std::string output_filename = output_folder + "/" + entry.path().filename().string();
            cv::imwrite(output_filename, undistorted_img);
        }
    }
    cv::waitKey(0);
    return 0;
}

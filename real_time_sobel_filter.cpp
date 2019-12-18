#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>
#include <iostream>

using namespace std;
using namespace cv;

int main( int argc, char** argv ) 
{

cv::namedWindow( "InputFrame", cv::WINDOW_AUTOSIZE );//creating a window to display image
cv::namedWindow("SobelFilter", cv::WINDOW_AUTOSIZE);
cv::VideoCapture cap;//cap object used to capture image from the input camera
Mat frame_gray, sobel_frame;
int inputCam;
inputCam = atoi(argv[1]);
cap.open(inputCam);//opening the camera which index has been given as input parameter to the program
cv::Mat frame;
for(;;) {
cap >> frame;//acquisition of a new frame
if( frame.empty() )
{
    break;//checking that the frame is not empty
    cout<<"Could not read frame from the input camera"<<std::endl;
}
  GaussianBlur( frame, frame, Size(3,3), 0, 0, BORDER_DEFAULT );//applying Gaussian blur to the image
  cvtColor(frame, frame, CV_BGR2GRAY );
  /// Generate grad_x and grad_y
  Mat grad_x, grad_y;
  Mat abs_grad_x, abs_grad_y;
  /// Gradient X
  Sobel( frame, grad_x, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT );
  convertScaleAbs( grad_x, abs_grad_x );
  /// Gradient Y
  //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
  Sobel(frame, grad_y, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT );
  convertScaleAbs( grad_y, abs_grad_y );

  /// Total Gradient (approximate)
  addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, sobel_frame);
cv::imshow( "InputFrame", frame );
cv::imshow("SobelFilter",sobel_frame);
if( cv::waitKey(33) >= 0 ) break;
}
return 0;
}
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/cuda.hpp> //header added to perform gpu computing
#include <opencv2/cudaarithm.hpp>
#include <opencv2/cudawarping.hpp>

#include <string>
#include <iostream>

using namespace std;
using namespace cv;
using namespace cuda;

int main(int argc, char **argv)
{

  cv::namedWindow("InputFrame", cv::WINDOW_AUTOSIZE); //creating a window to display image
  cv::namedWindow("SobelFilter", cv::WINDOW_AUTOSIZE);
  cv::VideoCapture cap; //cap object used to capture image from the input camera
  setNumThreads(6);
  Mat frame_gray, sobel_frame;
  int inputCam;
  long tick1, tick2;
  double fps;
  int nber_processed_frames = 0;
  double processTime=0;
  inputCam = atoi(argv[1]);
  cap.open(inputCam); //opening the camera which index has been given as input parameter to the program
  cv::Mat frame;
  for (;;)
  {
    tick1 = getTickCount();
    cap >> frame; //acquisition of a new frame
    if (frame.empty())
    {
      break; //checking that the frame is not empty
      cout << "Could not read frame from the input camera" << std::endl;
    }
    cv::GaussianBlur(frame, frame, Size(3, 3), 0, 0, BORDER_DEFAULT); //applying Gaussian blur to the image
    cv::cvtColor(frame, frame, CV_BGR2GRAY);
    /// Generate grad_x and grad_y
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;
    /// Gradient X
    cv::Sobel(frame, grad_x, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT);
    cv::convertScaleAbs(grad_x, abs_grad_x);
    /// Gradient Y
    //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
    cv::Sobel(frame, grad_y, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT);
    cv::convertScaleAbs(grad_y, abs_grad_y);
    cv::imshow("Y gradient",abs_grad_y);
    cv::imshow("X gradient",abs_grad_x);
    /// Total Gradient (approximate)
    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, sobel_frame);
    cv::imshow("InputFrame", frame);
    cv::imshow("SobelFilter", sobel_frame);
    if (cv::waitKey(33) >= 0)
      break;
    //incrementing the number of frames processed
    nber_processed_frames++;
    tick2 = getTickCount();
    processTime += ((double)(tick2 - tick1) / (getTickFrequency()));
    //check if 1 second has been elapsed and if it's the case compute the fps
    if(processTime >=1)
    {
      cout<<"Frame processing speed : "<< nber_processed_frames <<"fps"<<std::endl;
      nber_processed_frames = 0;//resetting the number of processed frames
      processTime = 0;
    }
  }
  return 0;
}
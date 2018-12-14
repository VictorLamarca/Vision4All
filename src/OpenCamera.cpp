#include "opencv2/opencv.hpp"
#include <iostream>
using namespace std;
using namespace cv;

int main(void)
{
    VideoCapture cap ("http://192.168.2.235:8080/videofeed"); 
	// Write 0 for default camera or write "http://" + Android IP + "/videofeed" if you are using IP Webcan 
    // Check VideoCapture documentation.	
	
    for(;;)
    {
          Mat frame;
          if (!cap.read (frame)){
			  cout << "ERRO" << endl;
		  }
          if( frame.empty() ) break; // end of video stream
		  namedWindow ("smile! :)", WINDOW_NORMAL);
          imshow("smile! :)", frame);
          if( waitKey(1) == 27 ) break; // stop capturing by pressing ESC 
    }
    // the camera will be closed automatically upon exit
    // cap.close();
    return 0;
}
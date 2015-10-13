#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int contrast; /**< Simple contrast control */
int brightness;  /**< Simple brightness control */
int saturation;

VideoCapture capture;


void help() {
  cout << "Keys" << endl;
  cout << "  h       : Print this help\n";
  cout << "  Esc     : Exit\n";
  cout << "  s       : Save settings \n";
  cout << "  l       : Load settings \n";

}

void onTrackbar_changed(int, void*)
{
	capture.set(CV_CAP_PROP_BRIGHTNESS,(float)brightness/100);
	capture.set(CV_CAP_PROP_CONTRAST, (float)contrast/100);
	capture.set(CV_CAP_PROP_SATURATION, (float)saturation/100);
}

void save() {
	FileStorage fs("config.xml",FileStorage::WRITE);
	fs << "BRIGHTNESS" << capture.get(CV_CAP_PROP_BRIGHTNESS)*100;
	fs << "CONTRAST" << capture.get(CV_CAP_PROP_CONTRAST)*100;
	fs << "SATURATION" << capture.get(CV_CAP_PROP_SATURATION)*100;
	fs.release();
}

int load() {
	FileStorage fs("config.xml",FileStorage::READ);
	if (!fs.isOpened())
    {
        cout << "Could not open the configuration file: config.xml" << endl;
        return -1;
    }
    fs["BRIGHTNESS"] >> brightness;
    fs["CONTRAST"] >> contrast;
    fs["SATURATION"] >> saturation;
	fs.release();
	onTrackbar_changed(brightness,NULL);
	return 0;
}

int main()
{
    Mat image;
    
    capture.open(0);
    
	brightness = capture.get(CV_CAP_PROP_BRIGHTNESS)*100;
	contrast   = capture.get(CV_CAP_PROP_CONTRAST)*100;
	saturation = capture.get(CV_CAP_PROP_SATURATION)*100;
    
    help();
    
    if(capture.isOpened())
    {
        char c;
        while(c != 27)
        {
            capture >> image;
            
            createTrackbar( "Brightness","Tarea 2", &brightness, 100, onTrackbar_changed );
			createTrackbar( "Contrast","Tarea 2", &contrast, 100, onTrackbar_changed );
			createTrackbar( "Saturation","Tarea 2", &saturation, 100, onTrackbar_changed);
			
			c = waitKey(1);
			
			switch(c) {
				case 's':
					save();
				break;
				case 'l':
					load();
				break;
				case 'h':
					help();
				break;
				default:
				break;
			}
			
			//cout << CV_CAP_PROP_SETTINGS << endl;
            
            if(image.empty())
                break;
            imshow("Tarea 2", image);
        }
        capture.release();
    }
    else
    {
        cout << "No capture" << endl;
        waitKey(0);
    }
    return 0;
}


/*if(image.empty())
                break;
                
            image.convertTo(image_bc, -1, contrast, brightness);
            
			cvtColor(image_bc, image_bc, CV_BGR2HSV);    
			int aux;
			for (int i=0; i < image_bc.rows ; i++)
			{
				  for(int j=0; j < image_bc.cols; j++)
				  {
						aux = image_bc.at<cv::Vec3b>(i,j)[1] + saturation;
						if (aux > 250)
							image_bcs.at<cv::Vec3b>(i,j)[1] = 255;
						else if(aux < 5)
							image_bcs.at<cv::Vec3b>(i,j)[1] = 0;
						else
							image_bcs.at<cv::Vec3b>(i,j)[1] = aux;
						image_bcs.at<cv::Vec3b>(i,j)[2] = image_bc.at<cv::Vec3b>(i,j)[2];
						image_bcs.at<cv::Vec3b>(i,j)[3] = image_bc.at<cv::Vec3b>(i,j)[3];
				  }
			}
			cvtColor(image_bcs, image_bcs, CV_HSV2BGR);
			
			saveCameraParams(image_bcs, imageSize, cameraMatrix, distCoeffs, rvecs ,tvecs, reprojErrs,
                         imagePoints, totalAvgErr);
			*/

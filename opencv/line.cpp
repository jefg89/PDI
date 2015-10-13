#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>

using namespace cv;

// Ensure that the STL streaming is used.
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::swap;

/*  Definir colores */

/* Primary Colors */
static const Vec3b green (0, 255, 0);
static const Vec3b blue(255, 0, 0);
static const Vec3b red(0, 0, 255);

/* Secondary Colors */
static const Vec3b yellow(0, 255, 255);
static const Vec3b magenta(255, 0, 255);
static const Vec3b cyan(255, 255, 0);
static const Vec3b black(0, 0, 0);
static const Vec3b white(255, 255, 255);
/* Others */
static const Vec3b orange(0, 165, 255);
static const Vec3b fusia(125, 0, 255);
static const Vec3b tardisblue(114, 35, 16);
static const Vec3b violet(255, 0, 143);

Vec3b  color_= tardisblue;

bool theEnd=false;
bool thick=false;

int x[2] = {0,0};
int y[2] = {0,0};
int cont = 0;

Point from, to;

template<typename T>
void Bresenham(Mat &img, int x1, int y1, int x2, int y2,  const T& color)
{
        // Bresenham's line algorithm
  bool change = (abs(y2 - y1) > abs(x2 - x1));
  
  if(change)
  {
    std::swap(x1, y1);
    std::swap(x2, y2);
  }
 
  if(x1 > x2)
  {
    std::swap(x1, x2);
    std::swap(y1, y2);
  }
 
  int dx = x2 - x1;
  int dy = abs(y2 - y1);
 
  float error = dx / 2.0f;

  int ystep = -1;
  if(y2 > y1) ystep = 1;

  for(int x=x1; x<x2; x++)
  {
	if((x > -1) && (x < img.cols) && (y1 > -1) && (y1 < img.rows)) {
		if(change)
		{
			img.at<Vec3b>(Point(y1,x)) = color;
		}
		else
		{
			img.at<Vec3b>(Point(x,y1)) = color;
		}
	}
    error -= dy;
    if(error < 0)
    {
        y1 += ystep;
        error += dx;
    }
  }
}


/**
* Draw a line segment between two given points.
*
* If one or two of the points lie outside the image then this function will draw the line segment that passes through the image.
* If there is no line segment that passes through the image, this function will not draw anything.
*
* @param img Image where the line is to be drawn
* @param color Color of the line to be drawn
* @param from Initial point of line segment
* @param end Final point of line segment
*/

template<typename T>
void line(Mat &img, const T& color,
const Point& from, const Point& to){
			 if(thick) {
				 Bresenham(img,from.x-1, from.y-1, to.x-1, to.y-1, color);
				 Bresenham(img,from.x, from.y-1, to.x, to.y-1, color);
				 Bresenham(img,from.x-1, from.y, to.x-1, to.y, color);
				 Bresenham(img,from.x, from.y, to.x, to.y, color);
				 Bresenham(img,from.x+1, from.y+1, to.x+1, to.y+1, color);	   
				 Bresenham(img,from.x, from.y+1, to.x, to.y+1, color);	
				 Bresenham(img,from.x+1, from.y, to.x+1, to.y, color);
			 }
			 else {
				 Bresenham(img,from.x, from.y, to.x, to.y, color);
			 }
		   }

/*
 * Help 
 */
void usage() {
  cout << "Usage: line [image] [-option] [-option]" << endl;
  cout << "Draw a line fron two mouse points on the given image\n";
  cout << "  -h show this help." << endl;
  cout << "  -t change line's thickness." << endl;
  cout << "  -g change line's color to green" << endl;
  cout << "  -b change line's color to blue" << endl;
  cout << "  -r change line's color to red" << endl;
  cout << "  -y change line's color to yellow" << endl;
  cout << "  -m change line's color to magenta" << endl;
  cout << "  -c change line's color to cyan" << endl;
  cout << "  -k change line's color to black" << endl;
  cout << "  -w change line's color to white" << endl;
  cout << "  -o change line's color to orange" << endl;
  cout << "  -f change line's color to fusia" << endl;
  cout << "  -v change line's color to violet" << endl;
}

void parseArgs(int argc, char*argv[], 
               std::string& filename) {
  filename.clear();
  // check each argument of the command line
  for (int i=1; i<argc; i++) {
    if (*argv[i] == '-') {
      switch (argv[i][1]) {
        case 'h':
          usage();
		  exit(EXIT_SUCCESS);
          break;
        case 't':
          thick = true;
          break;
        case 'r':
          color_ = red;
          break;
		case 'g':
          color_ = green;
          break;
        case 'b':
          color_ = blue;
          break;
		case 'y':
          color_ = yellow;
          break;
		case 'm':
          color_ = magenta;
          break;
		case 'c':
          color_ = cyan;
          break;
		case 'w':
          color_ = white;
          break;
		case 'k':
          color_ = black;
          break;
		case 'o':
          color_ = orange;
          break;
		case 'f':
          color_ = fusia;
          break;
		case 'v':
          color_ = violet;
          break;
        default:
          break;
      }
    } else {
      filename = argv[i]; // guess that this is the filename
    }
  }
}


void mouseHandler(int event, int xm , int ym , int flags, void* param) {
	if(event == EVENT_LBUTTONDOWN) {
		if (cont < 2) {
			x[cont] = xm;
			y[cont] = ym;
			cont++;
		}
        if (cont > 1){
			from.x = x[0]; from.y = y[0];
			to.x = x[1]; to.y = y[1];
			cont=0;
        }
	}
}
int main(int argc, char* argv[]) {
	
	std::string filename;
	parseArgs(argc,argv,filename);
    
	Mat image = imread(filename, CV_LOAD_IMAGE_COLOR);
	
	if(image.empty())
    {
        cout << "can not open " << filename << endl;
        return -1;
    }
    imshow("lines", image);
    void *name = cvGetWindowHandle("lines");
    setMouseCallback("lines",mouseHandler,&image);
    char c;

	while(c != 27 && name == cvGetWindowHandle("lines"))
	{
		line(image, color_, from, to);
		imshow("lines", image);
		c = waitKey(30);
	}
    return 0;
}

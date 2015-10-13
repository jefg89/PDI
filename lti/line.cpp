/*
 * Copyright (C) 2009-2015 by Pablo Alvarado
 * 
 * This file is part of the course MP-6123 Digital Image Processing,
 * at the Costa Rica Institute of Technology.
 *
 * The LTI-Lib-2 is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the authors nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */



// Standard Headers: from ANSI C and GNU C Library
#include <cstdlib>  // Standard Library for C++
#include <getopt.h> // Functions to parse the command line arguments

// Standard Headers: STL
#include <iostream>
#include <string>
#include <fstream>

#include <unistd.h>

// LTI-Lib Headers
#include <ltiObject.h>
#include <ltiMath.h>     // General lti:: math and <cmath> functionality
#include <ltiTimer.h>    // To measure time

#include <ltiIOImage.h> // To read/write images from files (jpg, png, and bmp)
#include <ltiViewer2D.h>

#include <ltiLispStreamHandler.h>

#include <ltiV4l2.h>

#include <pthread.h>
#include <iostream>


// Ensure that the STL streaming is used.
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::swap;

/*  Definir colores */

/* Primary Colors */
static const lti::rgbaPixel green(0, 255, 0);
static const lti::rgbaPixel blue(0, 0, 255);
static const lti::rgbaPixel red(255, 0, 0);

/* Secondary Colors */
static const lti::rgbaPixel yellow(255, 255, 0);
static const lti::rgbaPixel magenta(255, 0, 255);
static const lti::rgbaPixel cyan(0, 255, 255);
static const lti::rgbaPixel black(0, 0, 0);
static const lti::rgbaPixel white(255, 255, 255);
/* Others */
static const lti::rgbaPixel orange(255, 165, 0);
static const lti::rgbaPixel fusia(255, 0, 125);
static const lti::rgbaPixel tardisblue(16, 35, 114);
static const lti::rgbaPixel violet(143, 0, 255);


lti::rgbaPixel color_ = tardisblue;


bool theEnd=false;
bool thick=false;







template<typename T>
void Bresenham(lti::matrix<T>& img, int x1, int y1, int x2, int y2,  const T& color)
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
	if((x > -1) && (x < img.columns()) && (y1 > -1) && (y1 < img.rows())) {
		if(change)
		{
			img.at(x,y1) = color; 
		}
		else
		{
			img.at(y1,x) = color; 
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
void line(lti::matrix<T>& img, const T& color,
const lti::ipoint& from, const lti::ipoint& to){
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
  cout << "Draw a line on the given image\n";
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

/*
 * Main method
 */
int main(int argc, char* argv[]) {
	
	 std::string filename;
	 parseArgs(argc,argv,filename);
	 lti::viewer2D view;
	 lti::ipoint from;
	 lti::ipoint to;
  
	// we need an object to load images
    lti::ioImage loader;

    // we also need an image object
    lti::image img;

    // load the image
    if (loader.load(filename,img)) {
      // image successfully loaded
	  
      // and get its parameters
      lti::viewer2D::parameters vpar(view.getParameters());
      vpar.title = filename; // set the image name at the window's title bar
      view.setParameters(vpar);

      view.show(img); // show the image
       
      // wait for the user to close the window or to indicate
      lti::viewer2D::interaction action;
      lti::ipoint pos;
		
		for(int i=0;i<img.rows();i++);
		
	   int  x [2] = {0,0};
       int y [2]  = {0,0};
       int cont = 0;
       	
      do {
        view.waitInteraction(action,pos); // wait for something to happen
        if ((action == lti::viewer2D::Closed) || (action.key == lti::viewer2D::EscKey)) { // window closed?
          theEnd = true; // we are ready here!
        } 
        else if (cont <2) {
			if (action==lti::viewerBase::ButtonPressed) {
				x[cont]=pos.x;
				y[cont]=pos.y;
				cont++;
			}
		}
        if (cont > 1){
        from.set(x[0],y[0]);
        to.set(x[1],y[1]);
        line(img, color_,from, to);
        cont=0;
        }
        view.show(img);
        
      } while(!theEnd);
      
      
      return EXIT_SUCCESS;
    }
	
	
}


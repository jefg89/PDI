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

// Ensure that the STL streaming is used.
using std::cout;
using std::cerr;
using std::endl;
using std::swap;

/*  Definir colores */
static const lti::rgbaPixel mypixel(0,255,0);


bool theEnd=false;




/**
* Draw a line segment between two given points.
*
* If one or two of the points lie outside the image then this function ...
*
* @param img Image where the line is to be drawn
* @param color Color of the line to be drawn
* @param from Initial point of line segment
* @param end Final point of line segment
*/


template<typename T>
void Bresenham(lti::matrix<T>& img, float x1, float y1, float x2, float y2,  const T& color)
{
        // Bresenham's line algorithm
  const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
  if(steep)
  {
    std::swap(x1, y1);
    std::swap(x2, y2);
  }
 
  if(x1 > x2)
  {
    std::swap(x1, x2);
    std::swap(y1, y2);
  }
 
  const float dx = x2 - x1;
  const float dy = fabs(y2 - y1);
 
  float error = dx / 2.0f;
  const int ystep = (y1 < y2) ? 1 : -1;
  int y = (int)y1;
 
  const int maxX = (int)x2;
 
  for(int x=(int)x1; x<maxX; x++)
  {
    if(steep)
    {
        img.at(x,y) = color; 
    }
    else
    {
        img.at(y,x) = color; 
    }
 
    error -= dy;
    if(error < 0)
    {
        y += ystep;
        error += dx;
    }
  }
}




template<typename T>
void line(lti::matrix<T>& img, const T& color,
const lti::ipoint& from, const lti::ipoint& to){
			 Bresenham(img,from.x, from.y, to.x, to.y, color);  		   
		   }

/*
 * Help 
 */
void usage() {
  cout << "Usage: matrixTransform [image] [-h]" << endl;
  cout << "Try some image transformations on the given image\n";
  cout << "  -h show this help." << endl;
}

void parse(int argc, char*argv[],std::string& filename) {

  int c;

  // We use the standard getopt.h functions here to parse the arguments.
  // Check the documentation of getopt.h for more information on this
  
  // structure for the long options. 
  static struct option lopts[] = {
    {"help",no_argument,0,'h'},
    {0,0,0,0}
  };

  int optionIdx;

  while ((c = getopt_long(argc, argv, "h", lopts,&optionIdx)) != -1) {
    switch (c) {
    case 'h':
      exit(EXIT_SUCCESS); 
    default:
      cerr << "Option '-" << static_cast<char>(c) << "' not recognized." 
           << endl;
    }
  }

  // Now try to read the image name
  if (optind < argc) {           // if there are still some arguments left...
    filename = argv[optind];  // with the given image file
  } else {
    filename = "";
  }
}

/*
 * Main method
 */
int main(int argc, char* argv[]) {
	
	 std::string filename;
	 parse(argc,argv,filename);
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
		
		
      do {
        view.waitInteraction(action,pos); // wait for something to happen
        if ((action == lti::viewer2D::Closed) || (action.key == lti::viewer2D::EscKey)) { // window closed?
          theEnd = true; // we are ready here!
        } 
        //Bresenham(img, 200, 220, 210, 500);
        from.set(100,200);
        to.set(200, 100);
        /*from.x=40;
        from.y=200;
        to.x=500;
        to.y=500;
        line(img, mypixel,from, to );*/
        line(img, mypixel,from, to );
        view.show(img);
        
      } while(!theEnd);
      
      
      return EXIT_SUCCESS;
    }
	
	
}


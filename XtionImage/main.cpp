/*
 *
 *  Copyright (C) Roberto Calvo
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/. 
 *
 *  Authors : Roberto Calvo <rocapal [at] gmail [dot] com>
 *
 */

#include <string.h>
#include <stdlib.h>
#include <OpenNI.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

#define MAX_LENGHT 10000
#define MAX_TIMES 3

int xtion_get_image(int resolution, char sensor_option, char* filename)
{
  openni::Device device;

  try {
    openni::OpenNI::initialize();
    
    openni::Device device;
    int ret = device.open(openni::ANY_DEVICE);
    if ( ret != openni::STATUS_OK ) {
      printf(" Error(%d) to open Xtion sensor, be sure xtion is connected\n", ret);
      return -1;
    }
    
    openni::SensorType sensor; 
    
    if (sensor_option =='c' )
      sensor = openni::SENSOR_COLOR;
    else if (sensor_option=='d')
      sensor = openni::SENSOR_DEPTH;
    else if (sensor_option=='i')
      sensor = openni::SENSOR_IR;
    else
      {
	printf("sensor_option not allowed: %c\n", sensor_option);
	exit(-2);
      }
    
    
    openni::VideoStream videoStream;
    videoStream.create(device, sensor);
    videoStream.setMirroringEnabled(false);
    
    const openni::SensorInfo *colorSensorInfo = device.getSensorInfo(sensor);
    if (resolution == 0)
      videoStream.setVideoMode(colorSensorInfo->getSupportedVideoModes()[0]);
    else if (resolution == 1)
      videoStream.setVideoMode(colorSensorInfo->getSupportedVideoModes()[4]);
    else
      {
	printf("resolution not allowed: %c\n", resolution);
	exit(-2);
      }
    
    videoStream.start();
    
    
    std::vector<openni::VideoStream*> streams;
    streams.push_back( &videoStream );
    
    cv::Mat frame;
	
	
    openni::VideoFrameRef sensorFrame;

    // Repeat at least MAX_TIMES to wait Xtion is not cold
    for (int i=0; i<MAX_TIMES; i++)
      {
	int changedIndex;
	openni::OpenNI::waitForAnyStream( &streams[0], streams.size(), &changedIndex );
	
	if (changedIndex != 0)
	  continue;
	
	videoStream.readFrame( &sensorFrame );
      }
    
    if ( !sensorFrame.isValid())
      {
	printf("Error in sensorFrame.isValid()\n");
	exit(-1);
      }
    
    if (sensor == openni::SENSOR_COLOR)
      {		    
	openni::RGB888Pixel* imageBuffer =  (openni::RGB888Pixel*)sensorFrame.getData();
	frame.create(sensorFrame.getHeight(), sensorFrame.getWidth(), CV_8UC3);
	memcpy( frame.data, imageBuffer,   3*sensorFrame.getHeight()*sensorFrame.getWidth()*sizeof(uint8_t) );
	cv::cvtColor(frame,frame, CV_RGB2BGR); 
	
      }
    
    else if (sensor == openni::SENSOR_DEPTH)
      {
	const openni::DepthPixel* pDepth = (const openni::DepthPixel*)sensorFrame.getData();
	
	frame = cv::Mat(videoStream.getVideoMode().getResolutionY(),
			videoStream.getVideoMode().getResolutionX(),
			CV_8UC3 );
	
	for (int y=0; y<sensorFrame.getHeight(); y++)
	  {
	    for (int x=0; x<sensorFrame.getWidth(); ++x, ++pDepth)
	      {
		if (*pDepth != 0)
		  frame.data[(y*sensorFrame.getWidth()+ x)*3+1] = (float(*pDepth)/(float)MAX_LENGHT)*255.;
	      }
	  }	    
      }
    else if (sensor == openni::SENSOR_IR)
      {
	
	frame = cv::Mat(videoStream.getVideoMode().getResolutionY(),
			videoStream.getVideoMode().getResolutionX(),
			CV_16U, (char*)sensorFrame.getData() );
	
	frame.convertTo( frame, CV_8U );	    
      }	
      
    imwrite(filename, frame);

    printf("Image saved in %s\n", filename);
    
    videoStream.stop();    
    openni::OpenNI::shutdown();	
  }
  catch ( std::exception& ) {
    return -1;
  }
  
  return 0;
}

void usage ()
{

  printf("Usage:\n");
  printf("\t -r : Resolution of image\n");
  printf("\t\t 0: for 320x240\n");
  printf("\t\t 1: for 640x480\n\n");
  printf("\t -s : Sensor Type\n");
  printf("\t\t c: for color image\n");
  printf("\t\t d: for depth image\n");
  printf("\t\t i: for ir image\n\n");
  printf("\t -o : file name path\n");

  printf("\n");
  printf("\t Example: XtionImage -r 0 -s d -o /tmp/image.jpg\n");
  printf("\n");
  exit(-1);
}

int main(int argc,char *argv[])
{

  if (argc != 7)
    usage();

  if ( strcmp(argv[1],"-r")!=0  ||  strcmp(argv[3],"-s")!=0 || strcmp(argv[5],"-o")!=0 )
    usage();

  if ( (strcmp(argv[2],"0")!=0 && strcmp(argv[2],"1")!=0) || (strcmp(argv[4],"c")!=0 && strcmp(argv[4],"d")!=0 && strcmp(argv[4],"i")!=0 ))
    usage();
  

  xtion_get_image(atoi(argv[2]), argv[4][0], argv[6]);
  return 0;
}

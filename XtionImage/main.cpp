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

int xtion_get_image(int resolution, char sensor_option)
{
  openni::Device device;

  try {
    openni::OpenNI::initialize();
    
    openni::Device device;
    int ret = device.open(openni::ANY_DEVICE);
    if ( ret != openni::STATUS_OK ) {
      return 0;
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
    
    // Repeat at least 5 times to wait Xtion is not cold
    for (int i=0; i<5; i++)
      {
	
	int changedIndex;
	openni::OpenNI::waitForAnyStream( &streams[0], streams.size(), &changedIndex );
	
	if (changedIndex != 0)
	  continue;
	
	openni::VideoFrameRef sensorFrame;
	videoStream.readFrame( &sensorFrame );
	
	if ( !sensorFrame.isValid())
	  continue;
	
	if (sensor == openni::SENSOR_COLOR)
	  {		    
	    openni::RGB888Pixel* imageBuffer =  (openni::RGB888Pixel*)sensorFrame.getData();
	    frame.create(sensorFrame.getHeight(), sensorFrame.getWidth(), CV_8UC3);
	    memcpy( frame.data, imageBuffer,   3*sensorFrame.getHeight()*sensorFrame.getWidth()*sizeof(uint8_t) );
	    cv::cvtColor(frame,frame, CV_RGB2BGR); 
	    imwrite( "image.jpg" , frame );
	    
	  }
	
	else if (sensor == openni::SENSOR_DEPTH)
	  {
		
	  }
	else if (sensor == openni::SENSOR_IR)
	  {
	    
	    frame = cv::Mat(videoStream.getVideoMode().getResolutionY(),
			    videoStream.getVideoMode().getResolutionX(),
			    CV_16U, (char*)sensorFrame.getData() );
	    
	    frame.convertTo( frame, CV_8U );
	    
	  }
	
      }
    
    imwrite( "image.jpg" , frame );

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
  printf("\t\t i: for ir image\n");
  printf("\n");
  printf("\t Example: XtionImage -r 0 -s d\n");
  printf("\n");
  exit(-1);
}

int main(int argc,char *argv[])
{

  if (argc != 5)    
    usage();

  if ( strcmp(argv[1],"-r")!=0  ||  strcmp(argv[3],"-s")!=0)
    usage();

  if ( (strcmp(argv[2],"0")!=0 && strcmp(argv[2],"1")!=0) || (strcmp(argv[4],"c")!=0 && strcmp(argv[4],"d")!=0 && strcmp(argv[4],"i")!=0 ))
    usage();
  

  //for (int i=0; i<argc; i++)    
  //  printf ("[%d] - %s\n",i, argv[i]);


    
  xtion_get_image(atoi(argv[2]), argv[4][0]);
  return 0;
}

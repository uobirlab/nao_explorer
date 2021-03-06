#include "ros/ros.h"
#include "sensor_msgs/Image.h"
#include <iostream>
#include <fstream>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include "ImageMethods.h"
#include "ImagePublisher.h"

using namespace std;
using namespace NaoExplorer;

namespace enc = sensor_msgs::image_encodings;

cv_bridge::CvImagePtr convertToOpenCV(sensor_msgs::Image msg);

int filesCaptured = 0;
ImagePublisher maskPublisher;
ImagePublisher resizedPublisher;

void findColors(const sensor_msgs::Image msg)
{

//Get Image Mask
cv_bridge::CvImagePtr imagePtr = convertToOpenCV(msg);

CvImagePtr mask = getMask(imagePtr);


//Publish results of colour finding
maskPublisher.publishImage(*mask);

//Resize Image
CvImagePtr resizedImage = resizeImage(mask, 4, 3);

resizedPublisher.publishImage(*resizedImage);





//stringstream info;
//
//////info << msg.step << ",";
////info << mask.size() << ",";
////info << msg.height << ",";
//////info << msg.width;
////
////
////ROS_INFO(info.str().c_str());



	//TODO:Write function that takes this data and turns it into an image
	//TODO:Write some stuff that can actually output the data from my image analysis
	//TODO:Write a node that takes this image and passes it through camera processing broadcasting results of colour search
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "ColorFinder");

	ros::NodeHandle n;


	ros::Subscriber sub = n.subscribe("image_raw", 1000, findColors);

	while(ros::ok())
	{
	maskPublisher.startPublishing(n, string("image_mask"));
	resizedPublisher.startPublishing(n, string("image_resized"));
	}



	ros::spin();

	return 0;
}

cv_bridge::CvImagePtr convertToOpenCV(sensor_msgs::Image msg)
{
	cv_bridge::CvImagePtr cv_ptr;
		    try
		    {
		      cv_ptr = cv_bridge::toCvCopy(msg, enc::BGR8);
		    }
		    catch (cv_bridge::Exception& e)
		    {
		      ROS_ERROR("cv_bridge exception: %s", e.what());
		    }

		    return cv_ptr;
}

void saveRawImage(sensor_msgs::Image msg)
{
	if(filesCaptured < 50)
		{

			stringstream fileNameStream;
			fileNameStream << "image" << filesCaptured << ".txt";
			string fileName = fileNameStream.str();

			ofstream capturedData;
			capturedData.open(fileName.c_str());

			capturedData << msg.encoding << endl;

			for(uint i = 0; i < msg.step; i++)
			{
				int num = int(msg.data[i]);

				string data;


				ostringstream convert;
				convert << num;
	//
	          	capturedData << convert.str() << ",";
			}

			capturedData.close();
			filesCaptured++;
		}
}






#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <sensor_msgs/Image.h>
#include <image_transport/image_transport.h>


#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

class TrackSequential
{
public:
	TrackSequential(ros::NodeHandle nh_);
	~TrackSequential();
	image_transport::ImageTransport _imageTransport;
	image_transport::Subscriber image_sub;

protected:
	void imageCB(const sensor_msgs::ImageConstPtr& msg);
	void ImageProcessing();
	void ContourDetection(cv::Mat thresh_in, cv::Mat &output_);

private:
	cv::Mat img_bgr, img1, img2, thresh, diff;
	int i;
};

const std::string win1 = "Live Camera Feed";
const std::string win2 = "Threshold Difference";

TrackSequential::TrackSequential(ros::NodeHandle nh_): _imageTransport(nh_)
{
	image_sub = _imageTransport.subscribe("turtlebot3_burger/camera1/image_raw", 1, &TrackSequential::imageCB, this, image_transport::TransportHints("compressed"));
	cv::namedWindow(win1, CV_WINDOW_FREERATIO);
	cv::namedWindow(win2, CV_WINDOW_FREERATIO);
	i=0;
}

TrackSequential::~TrackSequential()
{
	cv::destroyWindow(win1);
	cv::destroyWindow(win2);
}

void TrackSequential::imageCB(const sensor_msgs::ImageConstPtr& msg)
{
	cv_bridge::CvImagePtr cvPtr;
	try
	{ 
		cvPtr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
	}
	catch (cv_bridge::Exception& e) 
	{
		ROS_ERROR("cv_bridge exception: %s", e.what());
		return;
	}
	cvPtr->image.copyTo(img_bgr);
		cv::cvtColor(cvPtr->image, img1, cv::COLOR_BGR2GRAY);
		this->ImageProcessing();

	img1.copyTo(img2);
}

void TrackSequential::ImageProcessing()
{
	if(i!=0)
	{	
		cv::absdiff(img1, img2, diff);
		cv::threshold(diff, thresh, 20, 255, cv::THRESH_BINARY);
		cv::morphologyEx(thresh, thresh, 2, cv::getStructuringElement( 2, cv::Size(3, 3)));
		this->ContourDetection(thresh, img_bgr);
		cv::imshow(win1, img_bgr);
		cv::imshow(win2, thresh);
	}
	++i;
	cv::waitKey(1);
}

void TrackSequential::ContourDetection(cv::Mat thresh_in, cv::Mat &output_)
{
	cv::Mat temp;
	cv::Rect objectBoundingRectangle = cv::Rect(0,0,0,0);
	thresh_in.copyTo(temp);
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(temp, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	if(contours.size()>0)
	{
		std::vector<std::vector<cv::Point> > largest_contour;
		largest_contour.push_back(contours.at(contours.size()-1));
		objectBoundingRectangle = cv::boundingRect(largest_contour.at(0));
		int x = objectBoundingRectangle.x+objectBoundingRectangle.width/2;
		int y = objectBoundingRectangle.y+objectBoundingRectangle.height/2;
		cv::circle(output_,cv::Point(x,y),20,cv::Scalar(0,255,0),2);
	}
}


int main(int argc, char** argv)
{
	ros::init(argc, argv, "TrackSequential");
	ros::NodeHandle nh;
	TrackSequential ts(nh);
	ros::spin();
}

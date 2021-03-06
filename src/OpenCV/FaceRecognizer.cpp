
#include "OpenCV/FaceRecognizer.h"
#include <QDebug>

using namespace cv;

// constructor loads classifier file with learned faces and set start parameters
OpenCV::FaceRecognizer::FaceRecognizer()
{
	if (!this->haar_cascade.load(file))
	{
		qDebug() << "Cannot load file: " << QString::fromStdString(file);
	}
	this->detected=false;
	this->isMovement=false;
	this->firstdetection=true;
	this->queue = new Util::SizedQueue(8,0.0f);
	this->queueDistance = new Util::SizedQueue(8,0.0f);
}

OpenCV::FaceRecognizer::~FaceRecognizer()
{
	this->faces.clear();
}

// face detection at given frame
void OpenCV::FaceRecognizer::detectFaces(Mat gray)
{
	// If we detected face in previous frame, detect next face in region
	// of previous detection
	if (this->detected)
	{
			cv::Mat subImg = gray(cv::Rect(this->rect.x,this->rect.y,
										   this->rect.width,this->rect.height));
			this->haar_cascade.detectMultiScale(subImg, this->faces,1.15, 5,
												0|CV_HAAR_SCALE_IMAGE|
												CV_HAAR_FIND_BIGGEST_OBJECT|
												CV_HAAR_DO_ROUGH_SEARCH);
	}
	// else find face again in whole frame
	else
	{
		this->haar_cascade.detectMultiScale(gray, this->faces,1.15, 5,
											0|CV_HAAR_SCALE_IMAGE|
											CV_HAAR_FIND_BIGGEST_OBJECT|
											CV_HAAR_DO_ROUGH_SEARCH);
	}
}

// used to draw rectangle and compute eyes coordinates
void OpenCV::FaceRecognizer::annotateFaces(Mat frame)
{
	// if there was at least 1 face
	if (this->faces.size()>0)
	{
		// we select only first face
		Rect face_i = this->faces[0];
		if (this->firstdetection){
			this->drawrect=this->faces[0];
			this->firstdetection=false;
		}
		if (detected)
		{
			// if the movement was under the threshold
			if ((abs(1.0f-(float)this->drawrect.x/(float)(this->rect.x+face_i.x))<0.10f)&&
					(abs(1.0f-(float)this->drawrect.y/(float)(this->rect.y+face_i.y))<0.10f) &&
					(abs(1.0f-(float)this->drawrect.width/(float)(face_i.width))<0.10f) &&
					(abs(1.0f-(float)this->drawrect.height/(float)(face_i.height))<0.10f))
			{
				//TODO conversion to INT,
				// rectangle around the head
				face_i.x=(int)(face_i.x-face_i.width*0.4+this->rect.x);
				if (face_i.x<0) face_i.x=0;
				if (face_i.x>frame.cols-1) face_i.x=frame.cols-1;
				face_i.width=(int)(face_i.width*1.8);
				if (face_i.x+face_i.width>frame.cols-1) face_i.width=frame.cols-1-face_i.x;
				face_i.y=(int)(face_i.y-face_i.height*0.4+this->rect.y);
				if (face_i.y<0) face_i.y=0;
				if (face_i.y>frame.rows-1) face_i.y=frame.rows-1;
				face_i.height=(int)(face_i.height*1.8);
				if (face_i.y+face_i.height>frame.rows-1) face_i.height=frame.rows-1-face_i.y;

				this->rect=face_i;
				isMovement=false;
			} else {
				//TODO conversion to INT,
				// rectangle to be drawn as the head was detected
				this->drawrect.x=(int)(face_i.x+this->rect.x); //-face_i.width*0.1
				if (this->drawrect.x<0) this->drawrect.x=0;
				if (this->drawrect.x>frame.cols-1) this->drawrect.x=frame.cols-1;
				this->drawrect.width=(int)(face_i.width);//*1.2;
				if (this->drawrect.x+this->drawrect.width>frame.cols-1) this->drawrect.width=frame.cols-1-this->drawrect.x;
				this->drawrect.y=(int)(face_i.y+this->rect.y); //-face_i.height*0.1
				if (this->drawrect.y<0) this->drawrect.y=0;
				if (this->drawrect.y>frame.rows-1) this->drawrect.y=frame.rows-1;
				this->drawrect.height=(int)(face_i.height);//*1.2;
				if (this->drawrect.y+this->drawrect.height>frame.rows-1) this->drawrect.height=frame.rows-1-this->drawrect.y;

				// determine the searching window for next frame
				face_i.x=(int)(face_i.x-face_i.width*0.4+this->rect.x);
				if (face_i.x<0) face_i.x=0;
				if (face_i.x>frame.cols-1) face_i.x=frame.cols-1;
				face_i.width=(int)(face_i.width*1.8);
				if (face_i.x+face_i.width>frame.cols-1) face_i.width=frame.cols-1-face_i.x;
				face_i.y=(int)(face_i.y-face_i.height*0.4+this->rect.y);
				if (face_i.y<0) face_i.y=0;
				if (face_i.y>frame.rows-1) face_i.y=frame.rows-1;
				face_i.height=(int)(face_i.height*1.8);
				if (face_i.y+face_i.height>frame.rows-1) face_i.height=frame.rows-1-face_i.y;

				this->rect=face_i;
				isMovement=true;
			}
		}
		// when head was lost in previous frame
		if (!detected)
		{
			detected=true;
			isMovement=true;
			this->drawrect=face_i;
			face_i.x=(int)(face_i.x-face_i.width*0.4);
			if (face_i.x<0) face_i.x=0;
			if (face_i.x>frame.cols-1) face_i.x=frame.cols-1;
			face_i.width=(int)(face_i.width*1.8);
			if (face_i.x+face_i.width>frame.cols-1) face_i.width=frame.cols-1-face_i.x;
			face_i.y=(int)(face_i.y-face_i.height*0.4);
			if (face_i.y<0) face_i.y=0;
			if (face_i.y>frame.rows-1) face_i.y=frame.rows-1;
			face_i.height=(int)(face_i.height*1.8);
			if (face_i.y+face_i.height>frame.rows-1) face_i.height=frame.rows-1-face_i.y;
			this->rect=face_i;
		}
		// draw the results & compute eyes coordinates
		rectangle(frame, drawrect, CV_RGB(0, 255,0), 1);
		rectangle(frame, face_i, CV_RGB(255, 0,0), 1);
		computeEyesCoordinations(this->drawrect,frame.size()); //face_i
	}
	else
	{
		detected=false;
	}
}

cv::CascadeClassifier OpenCV::FaceRecognizer::getCascadeClassifier()
{
	return this->haar_cascade;
}

// compute eyes coordinates based on the location of face in the frame
void OpenCV::FaceRecognizer::computeEyesCoordinations(Rect face, Size size)
{
	float x = ((((float)(face.x+face.width/2) / (float)size.width-0.5f)/0.5f)*100);
	float y = ((((float)(face.y+face.height/3) / (float)size.height-0.5f)/0.5f)*100);
	if (this->queue->getAvgBasedOnValue(x,y))
	{
		this->eyesCoord.x = x;
		this->eyesCoord.y = y;
	}
}
cv::Point2i OpenCV::FaceRecognizer::getEyesCoords()
{
	return this->eyesCoord;
}

// distance of the face is determined by the size of the drawn rectangle
float OpenCV::FaceRecognizer::getHeadDistance(double screenWidth)
{
	if (this->faces.size()>0)
	{
		Rect face = this->drawrect; // faces[0]
		// 1.0 when the face width is 3.5 times smaller than frame
		float dist = ((float)screenWidth/(float)face.width)/12.5f;
		this->queueDistance->getAvgBasedOnValue(dist);
		qDebug() << "dist value: " << dist;
		return dist;
	}
	return 0.0f;
}


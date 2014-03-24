#ifndef FACERECOGNIZERTHREAD_H
#define FACERECOGNIZERTHREAD_H

#include <QThread>


namespace OpenCV{
class CapVideo;
class FaceRecognizer;
}

namespace QOpenCV

{
class FaceRecognitionWindow;

/**
	 * @brief Class FaceRecognitionThread
	 * @author Autor: Marek Jakab
	 * @date 18.11.2013
	 */
class FaceRecognitionThread : public QThread
	{
	Q_OBJECT

public:

	FaceRecognitionThread( OpenCV::FaceRecognizer *faceRecognizer, QObject *parent = 0);
	~FaceRecognitionThread(void);

	/**
		 * @author Autor: Marek Jakab
		 * @brief run Starts thread
		 */
	void run();

signals:
	/**
			 * @author Autor: Marek Jakab
			 * @brief pushImage Send image to FaceRecognitionWindow
			 * @param Image cv::Mat
			 */
	void pushImage( cv::Mat Image );

	/**
			 * @author Autor: Marek Jakab
			 * @brief sendEyesCoords
			 * @param x % distance from middle on X axis
			 * @param y % distance from middle on Y axis
			 */
	void sendEyesCoords( float x, float y, float distance);

public slots:
	/**
			 * @author Autor: Marek Jakab
			 * @brief setCancel Sets cancel=true
			 */
	void setCancel( bool );
	/**
			 * @author Autor: Marek Jakab
			 * @brief pauseWindow Pauses recognition window
			 */
	void pauseWindow();
	/**
			* @author Dávid Durčák
			* @brief setSendImgEnabling Set emiting of actual frame.
			* @param sendImgEnabled
	*/
	void setSendImgEnabled( bool sendImgEnabled );

	/**
			* @author Dávid Durčák
			* @brief setCapVideo Set member mCapVideo.
			* @param capVideo
	*/
	void setCapVideo( OpenCV::CapVideo *capVideo);

private:

	OpenCV::FaceRecognizer	*mFaceRecognizer;
	OpenCV::CapVideo		*mCapVideo;
	bool					mCancel;
	bool					mSendImgEnabled;

	};
}

#endif //FACERECOGNIZERTHREAD_H

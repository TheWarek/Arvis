#ifndef KINECTSPEECHTHREAD_H
#define KINECTSPEECHTHREAD_H

#include <QThread>
//#pragma component(interface, on, references, __interface)
#include <SpeechSDK.h>

namespace Speech

{
class KinectSpeechThread;

/**
	 * @brief Class KinectSpeechThread
	 * @author Autor: Marek Jakab
	 * @date 22.3.2014
	 */
class KinectSpeechThread : public QThread
{
	Q_OBJECT

public:
	KinectSpeechThread();
	~KinectSpeechThread(void);
	/**
		 * @brief initializeSpeech
		 * @author Autor: Marek Jakab
		 * @return 0/1 if the initialization was succesfful
		 */
	int initializeSpeech();
	/**
		 * @brief m_SpeechClass Object which holds basic functions for speech recognition
		 */
	SpeechSDK::SpeechSDKClass *m_SpeechClass;
	/**
		 * @brief isConnected Used to store information if the kinect device have been initialized
		 */
	bool isConnected;
	/**
		 * @brief cancel Boolean for canceling the thread
		 */
	bool cancel;

	/**
		 * @author Autor: Marek Jakab
		 * @brief run Starts thread
		 */
	void run();

};
}


#endif // KINECTSPEECHTHREAD_H

#ifndef SIZEDQUEUE_H
#define SIZEDQUEUE_H

#include <deque>

class SizedQueue{

public:

	SizedQueue(int size, float threshold);
	~SizedQueue();

	int getAvgBasedOnValue(float &x, float &y);

private:
	std::deque< std::pair<float,float> > stack;
	// size of queue
	int size;
	// threshold <0.0,1.0>
	float threshold;
	// current sum for values in queue
	std::pair<float,float> sum;
	// sum considering threshold
	std::pair<float,float> presum;

};

#endif // SIZEDQUEUE_H

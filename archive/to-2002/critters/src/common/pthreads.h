

#include <pthread.h>

#define RUNNING 1
#define STOPPED 2



void ThreadEnd();

class Thread
{
private:
	pthread_t thread_var;
	char status;
	void *p;

public:
	Thread(void *f);
	void Start();
	void Stop();
	char Status();
	void Wait();
};

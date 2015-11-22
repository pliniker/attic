
#include "pthreads.h"


void ThreadEnd()
{
	pthread_exit(0);
}


Thread::Thread(void *f)
{
	p = f;
}


void Thread::Start()
{
	status = RUNNING;
	pthread_create(&thread_var, NULL, p, NULL);
}


void Thread::Stop()
{
	status = STOPPED;
}


char Thread::Status()
{
	return status;
}


void Thread::Wait()
{
	pthread_join(thread_var, NULL);
}

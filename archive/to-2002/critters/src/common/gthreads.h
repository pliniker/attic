


#define RUNNING 1
#define STOPPED 2



class Idle
{
private:
	int tag;
	void *p;

public:
	Idle(void *f);
	void Start();
	void Stop();
};


class Timer
{
private:
	int tag;
	int t;
	void *p;
	
public:
	Timer(void *f, int r);
	void Start();
	void Stop();
};
	
	
class IOWait
{
private:
	int tag;
	int s;
	void *p;
	
public:
	IOWait(void *f, int d);
	void Start();
	void Stop();
};
	
	
	
	
	
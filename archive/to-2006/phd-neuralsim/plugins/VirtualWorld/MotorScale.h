#ifndef VIRTUALWORLD_MOTORSCALE_H
#define VIRTUALWORLD_MOTORSCALE_H


#include <NodeLayer.h>
#include "VirtualWorld.h"


using namespace brainlib;


class MotorScale : public NodeLayer {
public:
	MotorScale(std::string name);
	~MotorScale();
	
	std::string type();
	void read(xmlNodePtr from);
	
	bool check();
	void recall();
	void learn();
	
private:
	VirtualWorldPtr world;
	int step;
	int xRes;
	int yRes;
	int xDim;
	int yDim;
	int maxDim;
	
public:
	static SerializablePtr Create(std::string name);
	
	static std::string TypeName;
};


#endif

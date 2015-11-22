#ifndef VIRTUALWORLD_MOTORSNRMSTYLE_H
#define VIRTUALWORLD_MOTORNRMSTYLE_H


#include <NodeLayer.h>
#include "VirtualWorld.h"


using namespace brainlib;


class MotorNRMStyle : public NodeLayer {
public:
	MotorNRMStyle(std::string name);
	~MotorNRMStyle();
	
	std::string type();
	void read(xmlNodePtr from);
	
	bool check();
	void recall();
	void learn();
	
private:
	VirtualWorldPtr world;
	
public:
	static SerializablePtr Create(std::string name);
	
	static std::string TypeName;
};


#endif

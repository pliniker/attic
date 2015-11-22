#ifndef VIRTUALWORLD_FOVEA_H
#define VIRTUALWORLD_FOVEA_H


#include <NodeLayer.h>
#include "VirtualWorld.h"


using namespace brainlib;


class Fovea : public NodeLayer {
public:
	Fovea(std::string name);
	~Fovea();
	
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

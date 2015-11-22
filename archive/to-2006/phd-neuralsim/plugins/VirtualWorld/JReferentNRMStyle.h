#ifndef VIRTUALWORLD_JREFERENTNRMSTYLE_H
#define VIRTUALWORLD_JREFERENTNRMSTYLE_H


#include <NodeLayer.h>
#include "VirtualWorld.h"


using namespace brainlib;


class JReferentNRMStyle : public NodeLayer {
public:
	JReferentNRMStyle(std::string name);
	~JReferentNRMStyle();
	
	std::string type();
	void read(xmlNodePtr from);
	
	bool check();
	void recall();
	void learn();
	
private:
	VirtualWorldPtr world;
	
public:
	static SerializablePtr Create(std::string name);

    static std::string TypeString;
};


#endif


#include <stdlib.h>
#include <Common.h>
#include "MotorScale.h"
#include "VirtualWorld.h"


using namespace brainlib;


MotorScale::MotorScale(std::string name) :
NodeLayer( name                                          ),
world(     VirtualWorld::Create(VirtualWorld::NameSpace) ) {
	add(world);
}


MotorScale::~MotorScale() {}


std::string MotorScale::type() {
	return TypeName;
}


void MotorScale::read(xmlNodePtr from) {
	NodeLayer::read(from);
	world = get(VirtualWorld::NameSpace);
}


bool MotorScale::check() {
	bool passed = true;
	
	step = world->foveaStep->get();
	xRes = world->gridSize->x();
	yRes = world->gridSize->y();
	xDim = xRes / step;
	yDim = yRes / step;
	maxDim = xDim > yDim ? xDim : yDim;
	
	if ( (outputState->x() != maxDim) || (outputState->y() != 2) ) 
		outputState->create(maxDim, 2, Image::Gray8bpp);
	
	passed = NodeLayer::check();
	
	if ( inputs->get() != 1 ) {
		layerMessage("inputs must = 1");
		passed = false;
	}
	
	return passed;
}


void MotorScale::recall() {
	int xWeight = 0;
	int yWeight = 0;
	
	// calculate "center of gravity" of x scale
	for (int i = 0; i < xDim; i++)
		xWeight += Common::HdistLookup[ inputState->at(i) ] * i;
	xWeight /= xDim;
		
	// calculate "center of gravity" of y scale	
	for (int i = 0; i < yDim; i++)
		yWeight += Common::HdistLookup[ inputState->at(i + maxDim) ] * i;
	yWeight /= yDim;
	
	int xPos = (int)((float)xWeight / (float)xDim) * xRes;
	int yPos = (int)((float)yWeight / (float)yDim) * yRes;
	
	// snap to grid
	yPos -= yPos % step;
	xPos -= xPos % step;
	
	// update gridPos
	world->gridPos->set(xPos, yPos);
	
	// copy inputState to outputState
	int size = inputState->size();
	while (size--) { outputState->at(size) = inputState->at(size); }
}


void MotorScale::learn() {
	//recall();
}


SerializablePtr MotorScale::Create(std::string name) {
	return RefCountPtr<MotorScale>( new MotorScale(name) );
}


std::string MotorScale::TypeName = "VirtualWorld_Motor_Scale";


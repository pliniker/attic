
#include <stdlib.h>
#include <Common.h>
#include "JReferentScale.h"
#include "VirtualWorld.h"


using namespace brainlib;


JReferentScale::JReferentScale(std::string name) :
NodeLayer ( name                                          ),
world     ( VirtualWorld::Create(VirtualWorld::NameSpace) ) {
	add(world);
}


JReferentScale::~JReferentScale() {}


std::string JReferentScale::type() {
	return TypeName;
}


void JReferentScale::read(xmlNodePtr from) {
	NodeLayer::read(from);
	world = get(VirtualWorld::NameSpace);
}


bool JReferentScale::check() {
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
	
	if ( inputs->get() != 0 ) {
		layerMessage("inputs must = 0");
		passed = false;
	}

	return passed;
}


void JReferentScale::recall() {
	int xPos = world->gridPos->x() / step;
	int yPos = world->gridPos->y() / step;
	
	for (int i = 0; i < xDim; i++) {
		int f = abs(xPos - i);
		int g = (Unit)( 8.0 / (float)(f == 0 ? 1: f) );
		outputState->at(i) = Common::BitGradient[g];
	}
		
	for (int i = 0; i < xDim; i++) {
		int f = abs(yPos - i);
		int g = (Unit)( 8.0 / (float)(f == 0 ? 1: f) );
		outputState->at(i + maxDim) = Common::BitGradient[g];
	}	
}


void JReferentScale::learn() {
	recall();
}


SerializablePtr JReferentScale::Create(std::string name) {
	return RefCountPtr<JReferentScale>( new JReferentScale(name) );
}


std::string JReferentScale::TypeName = "VirtualWorld_JReferent_Scale";



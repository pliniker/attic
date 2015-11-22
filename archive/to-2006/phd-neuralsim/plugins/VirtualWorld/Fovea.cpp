
#include <Common.h>

#include "Fovea.h"
#include "VirtualWorld.h"


using namespace brainlib;


Fovea::Fovea(std::string name) :
NodeLayer( name                                          ),
world(     VirtualWorld::Create(VirtualWorld::NameSpace) ) {
	outputState = world->fovea;
	add(world);
	add(outputState);
}


Fovea::~Fovea() {}


std::string Fovea::type() {
	return TypeName;
}


void Fovea::read(xmlNodePtr from) {
	NodeLayer::read(from);
	world = get(VirtualWorld::NameSpace);
}


bool Fovea::check() {
	bool passed = NodeLayer::check();
	
	if ( inputs->get() != 0 ) {
		std::string msg = displayName->get() + ": inputs must be zero";
		ErrorInterface::Log(msg);
		passed = false;
	} else {
		Image::ColorEncoding enc = world->state->encoding();
		
		if ( (outputState->encoding() != enc) )
			outputState->create( outputState->x(), outputState->y(), enc );
	}
	
	return passed;
}


void Fovea::recall() {
	int stateX = world->state->x();
	int xPos   = world->gridPos->x() * (world->state->bpp() / 8);
	int yPos   = world->gridPos->y();
	int foveaX = outputState->x();
	int foveaY = outputState->y();
	
	Image& inData = *(world->state);
	
	for (int j = 0; j < foveaY; j++) {
		for (int i = 0; i < foveaX; i++) {
			outputState->at(j * foveaX + i)	= inData.at( (j + yPos) * stateX + (i + xPos) );
		}
	}
}


void Fovea::learn() {
	recall();
}


SerializablePtr Fovea::Create(std::string name) {
	return RefCountPtr<Fovea>( new Fovea(name) );
}


std::string Fovea::TypeName = "VirtualWorld_Fovea";


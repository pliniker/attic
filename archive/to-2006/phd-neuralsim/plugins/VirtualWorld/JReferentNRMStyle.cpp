
#include <stdlib.h>
#include <Common.h>
#include "JReferentNRMStyle.h"
#include "VirtualWorld.h"


using namespace brainlib;


JReferentNRMStyle::JReferentNRMStyle(std::string name) :
NodeLayer ( name                                          ),
world     ( VirtualWorld::Create(VirtualWorld::NameSpace) ) {
	add(world);

    outputState->setReadonly(true);
    outputState->create(16, 2, false, Image::Gray1bpp);
}


JReferentNRMStyle::~JReferentNRMStyle() {}


std::string JReferentNRMStyle::type() {
	return TypeString;
}


void JReferentNRMStyle::read(xmlNodePtr from) {
	NodeLayer::read(from);
	world = get(VirtualWorld::NameSpace);
}


bool JReferentNRMStyle::check() {
	return NodeLayer::check();
}


void JReferentNRMStyle::recall() {
	short unsigned int xPos = world->gridPos->x();
	short unsigned int yPos = world->gridPos->y();

    (short unsigned int&)outputState->at(0) = xPos;
    (short unsigned int&)outputState->at(2) = yPos;
}


void JReferentNRMStyle::learn() {
	recall();
}


SerializablePtr JReferentNRMStyle::Create(std::string name) {
	return RefCountPtr<JReferentNRMStyle>( new JReferentNRMStyle(name) );
}


std::string JReferentNRMStyle::TypeString = "VirtualWorld_JReferent_NRMStyle";


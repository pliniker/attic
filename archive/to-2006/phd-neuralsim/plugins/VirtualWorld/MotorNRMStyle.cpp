
#include <stdlib.h>
#include <Common.h>
#include "MotorNRMStyle.h"
#include "VirtualWorld.h"


using namespace brainlib;


MotorNRMStyle::MotorNRMStyle(std::string name) :
NodeLayer( name                                          ),
world    ( VirtualWorld::Create(VirtualWorld::NameSpace) ) {
	add(world);
}


MotorNRMStyle::~MotorNRMStyle() {}


std::string MotorNRMStyle::type() {
	return TypeName;
}


void MotorNRMStyle::read(xmlNodePtr from) {
	NodeLayer::read(from);
	world = get(VirtualWorld::NameSpace);
}


bool MotorNRMStyle::check() {
	bool passed = NodeLayer::check();
	
	if ( inputs->get() > 1 ) {
		layerMessage("inputs must no greater than 1");
		passed = false;
	}
	
	return passed;
}


void MotorNRMStyle::recall() {
	if ( inputs->get() == 1 ) {
		
		int step = world->foveaStep->get();
		int segment = inputState->size() / 4;
		
		// calculate movement forces
		int up_count = 0;
		for (int i = 0; i < segment; i++)
			up_count += Common::HdistLookup[ inputState->at(i) ];

		int down_count = 0;
		for (int i = segment; i < 2*segment; i++)
			down_count += Common::HdistLookup[ inputState->at(i) ];
		
		int left_count = 0;
		for (int i = 2*segment; i < 3*segment; i++)
			left_count += Common::HdistLookup[ inputState->at(i) ];
		
		int right_count = 0;
		for (int i = 3*segment; i < 4*segment; i++)
			right_count += Common::HdistLookup[ inputState->at(i) ];
		
		// copy inputState to outputState
		int size = inputState->size();
		while (size--) { outputState->at(size) = inputState->at(size); }
		
		// calculate movement vectors
		int yvec = down_count - up_count;
		int xvec = right_count - left_count;
		
		// snap to grid
		yvec -= yvec % step;
		xvec -= xvec % step;
		
		// update gridPos
		world->gridPos->set(xvec, yvec);
	} else {
		learn();
	}
}


void MotorNRMStyle::learn() {
	int size = outputState->size();
	while (size--) { outputState->at(size) = 0; }
	
	// fill in outputState with motor movement information
	
	int segment = outputState->size() / 4;
	
	int yvec = world->gridDelta->y() / 8;
	int xvec = world->gridDelta->x() / 8;
	int ymod = world->gridDelta->y() % 8;
	int xmod = world->gridDelta->x() % 8;
	
	int offset = 0;
	if ( yvec < 0 ) {
		for (int i = offset; i < offset - yvec; i++)
			outputState->at(i) = 255;
		outputState->at(offset - yvec + 1) = Common::BitGradient[ymod];
	}
	
	offset += segment;
	if ( yvec > 0 ) {
		for (int i = offset; i < offset + yvec; i++)
			outputState->at(i) = 255;
		outputState->at(offset + yvec + 1) = Common::BitGradient[ymod];
	}
	
	offset += segment;
	if ( xvec < 0 ) {
		for (int i = offset; i < offset - xvec; i++)
			outputState->at(i) = 255;
		outputState->at(offset - xvec + 1) = Common::BitGradient[xmod];
	}
	
	offset += segment;
	if ( xvec > 0 ) {
		for (int i = offset; i < offset + xvec; i++)
			outputState->at(i) = 255;
		outputState->at(offset + xvec + 1) = Common::BitGradient[xmod];
	}
}


SerializablePtr MotorNRMStyle::Create(std::string name) {
	return RefCountPtr<MotorNRMStyle>( new MotorNRMStyle(name) );
}


std::string MotorNRMStyle::TypeName = "VirtualWorld_Motor_NRMStyle";


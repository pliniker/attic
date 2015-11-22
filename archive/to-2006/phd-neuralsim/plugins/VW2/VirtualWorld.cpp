
#include "VirtualWorld.h"
#include "VirtualView.h"
#include "Entity.h"
#include "Eye.h"


VirtualWorld::VirtualWorld(std::string name) :
NodeLayer( name                           ),
view     ( new VirtualView("VirtualView") ) {
	add(view);
}


VirtualWorld::~VirtualWorld() {}


std::string VirtualWorld::type() {
    return TypeName;
}


void VirtualWorld::read(xmlNodePtr from) {
    NodeLayer::read(from);
	
	view = get("VirtualView");
}


bool VirtualWorld::check() {
    view->check(outputState);
	outputState->clearDirty();
	return true;
}


void VirtualWorld::recall() {
	view->exec(outputState);
}


void VirtualWorld::learn() {
	view->exec(outputState);
}


SerializablePtr VirtualWorld::Create(std::string name) {
    return RefCountPtr<VirtualWorld>( new VirtualWorld(name) );
}


std::string VirtualWorld::TypeName = "VW2_VirtualWorld";


extern "C" void Initialize() {
	Serializable::AddFactory(VirtualWorld::TypeName, VirtualWorld::Create);
	StringList::Get("node_layers")->add(VirtualWorld::TypeName);
	
	Serializable::AddFactory(VirtualView::TypeName, VirtualView::Create);
	
	Serializable::AddFactory(Entity::TypeName, Entity::Create);
	
	Serializable::AddFactory(Eye::TypeName, Eye::Create);
}


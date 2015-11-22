#ifndef VW2_VIRTUALWORLD_H
#define VW2_VIRTUALWORLD_H


#include <NodeLayer.h>
#include "VirtualView.h"


using namespace brainlib;


// NodeLayer
class VirtualWorld : public NodeLayer {
    /// object methods
public:
    VirtualWorld(std::string name);
    ~VirtualWorld();
    
    std::string      type();
    void             read(xmlNodePtr from);
    
    bool             check();
    void             recall();
    void             learn();
	
    /// object data
private:
	VirtualViewPtr   view;
	
    /// class methods
public:
	static SerializablePtr Create(std::string name);
	
public:
	/// class data
	static std::string TypeName;
};

typedef RefCountPtr<VirtualWorld> VirtualWorldPtr;


#endif

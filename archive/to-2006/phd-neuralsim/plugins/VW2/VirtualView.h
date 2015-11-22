#ifndef VW2_VIRTUALVIEW_H
#define VW2_VIRTUALVIEW_H


#include <RenderObject.h>
#include <ControlPoint.h>
#include <ObjectMaker.h>
#include <Vector3d.h>
#include <Image.h>


using namespace brainlib;


// RenderObject for full virtual world view
class VirtualView : public RenderObject {
    /// object methods
public:
    VirtualView(std::string name);
    ~VirtualView();
    
	std::string      type();
	void             read(xmlNodePtr from);
	MapPtr           displaysMap();
	
    GLuint           makeList();
    GLuint           updateList();
    GLuint           renderList();
	
	Float3dPtr       position();
    
    void             check(ImagePtr bgImage);
    void             exec(ImagePtr bgImage);
	
    /// object data
private:        
	ControlPointPtr  handle;
	
    MapPtr           entities;
	
    ObjectMakerPtr   entity_maker;
	
	ImagePtr         double_buffer;
	Int3dPtr         tex_res;
    IntPtr           grid_spacing;
	
	GLuint			 view_list;
	GLuint			 texture;
	
    /// class methods
public:
	static SerializablePtr Create(std::string name);
	
public:
	/// class data
	static std::string TypeName;
};


typedef RefCountPtr<VirtualView> VirtualViewPtr;


#endif

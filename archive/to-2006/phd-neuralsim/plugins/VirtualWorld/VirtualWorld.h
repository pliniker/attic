#ifndef VIRTUALWORLD_VIRTUALWORLD_H
#define VIRTUALWORLD_VIRTUALWORLD_H


#include <ErrorInterface.h>
#include <Brainlib.h>
#include <Floating.h>
#include <Image.h>
#include <Map.h>

#include <Navigator.h>
#include <RenderObject.h>
#include <ControlPoint.h>


using namespace brainlib;


class VirtualWorld : public RenderObject {
public:
    VirtualWorld(std::string name);
	~VirtualWorld();
	
	std::string     type();
	void            read(xmlNodePtr from);
	MapPtr          displaysMap();
	
    GLuint          makeList();
    GLuint          updateList();
    GLuint          renderList();
    
	Float3dPtr      position();
	void            position(GLVector& delta);
    GLfloat         scale();
    void            scale(GLfloat scaleDelta);
	
public:
	ImagePtr        state;
	ImagePtr        fovea;
	
	IntPtr          foveaStep;
	Int3dPtr        gridSize;
	Int3dPtr        gridPos;
	Int3dPtr        gridDelta;
		
private:
	void            makeGrid();
	GLuint          makeFoveaList();
	GLuint          makeStateList();
	void            screenToGrid();
	void            gridToScreen();
	
private:
	MapPtr          display_map;
	MapPtr          state_map;
	MapPtr          fovea_map;
	MapPtr          jreferent_map;
	MapPtr          motor_map;
	
	FloatPtr        scale_factor;
	
	ControlPointPtr state_point;
	ControlPointPtr fovea_point;
	
	Int3dPtr        tex_res;
	Float3dPtr      state_pos;
	Float3dPtr      fovea_pos;
	
	Int3dPtr        state_orig;
	Int3dPtr        fovea_orig;
	Int3dPtr        grid_orig;
	
    GLuint          state_list, fovea_list, texture;	
	GLfloat         aspect_ratio;

public:
	static SerializablePtr Create(std::string name);
	
	static bool singleton;
	static RefCountPtr<VirtualWorld> vworld;
	static std::string NameSpace;
};


typedef RefCountPtr<VirtualWorld> VirtualWorldPtr;


#endif

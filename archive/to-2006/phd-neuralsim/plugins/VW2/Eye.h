#ifndef VW2_EYE_H
#define VW2_EYE_H


#include <RenderObject.h>
#include <ControlPoint.h>
#include <Vector3d.h>
#include <Boolean.h>
#include <Image.h>
#include <NodeLayer.h>


using namespace brainlib;


// renderer for fovea + perifovea
class Eye : public RenderObject {
	/// object methods
public:
    Eye(std::string name);
    ~Eye();
	
    std::string      type();
    virtual void     read(xmlNodePtr from);
	virtual MapPtr   displaysMap();
    
    GLuint			 makeList();
    GLuint			 updateList();
	GLuint			 updateList(ImagePtr buffer, IntPtr grid);
    GLuint			 renderList();
	
	Float3dPtr       position();
    
    void             exec(ImagePtr buffer, IntPtr grid);
	
private:
	void             setupLayer(BooleanPtr condition, NodeLayerPtr& layer, Image::ColorEncoding enc,
                                std::string type, std::string name);
	void			 boundsCheck(Float3dPtr boundme, Float3dPtr bounds);
	void			 snapToGrid(Float3dPtr snapme, IntPtr grid);
    
	/// object data
private:
	ControlPointPtr  mouse;
	
	Float3dPtr		 fovea_res;
	Float3dPtr		 perifovea_res;
	Float3dPtr		 world_res;
	
	Float3dPtr		 eye_pos;
    Float3dPtr       motor_pos;
	
    BooleanPtr       has_fovea;
    BooleanPtr       has_perifovea;
	BooleanPtr		 has_motor;
	BooleanPtr		 has_jreferent;
	
	NodeLayerPtr	 fovea;
	NodeLayerPtr	 perifovea;
	NodeLayerPtr	 motor;
	NodeLayerPtr	 jreferent;

	GLuint			 eye_list;

    Image::ColorEncoding color_scheme;
   
	/// class methods
public:
	static SerializablePtr Create(std::string name);
	
public:
	/// class data
	static std::string TypeName;
};


typedef RefCountPtr<Eye> EyePtr;


#endif

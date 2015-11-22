#ifndef WV2_ENTITY_H
#define WV2_ENTITY_H


#include <Vector3d.h>
#include <Boolean.h>
#include <Image.h>
#include <Map.h>
#include "Eye.h"


using namespace brainlib;


// virtual entity existing in the virtual world
class Entity : public Map {
	/// object methods
public:
    Entity(std::string name);
    ~Entity();
	
	std::string      type();
	void             read(xmlNodePtr from);
	
    GLuint           makeList();
    GLuint           updateList();
	GLuint			 updateList(ImagePtr buffer, IntPtr grid);
    GLuint           renderList();
    
    void             check(ImagePtr buffer, IntPtr grid);
    void             exec(ImagePtr buffer, IntPtr grid);
	
private:
	void			 checkEye(ImagePtr buffer, IntPtr grid);

	/// object data
private:
	BooleanPtr       has_eye;
	
    EyePtr           eye;
	
	GLuint			 entity_list;
	
	/// class methods
public:
	static SerializablePtr Create(std::string name);
	
public:
	/// class data
	static std::string TypeName;
};


typedef RefCountPtr<Entity> EntityPtr;


#endif

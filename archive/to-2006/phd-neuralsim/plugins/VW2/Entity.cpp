
#include "Entity.h"


Entity::Entity(std::string name) :
Map    ( name                  ),
has_eye( new Boolean("HasEye") ) {
	add(has_eye);
	
	entity_list = glGenLists(1);
	
	makeList();
}


Entity::~Entity() {
	glDeleteLists( entity_list, 1 );
}


std::string Entity::type() {
	return TypeName;
}


void Entity::read(xmlNodePtr from) {
	Map::read(from);
	
	has_eye = get("HasEye");
	eye     = get("Eye");
	
	makeList();
}


GLuint Entity::makeList() {
	glNewList( entity_list, GL_COMPILE );
	
	if ( eye.get() ) {
		glPushMatrix();
		//glLoadIdentity();
		Float3dPtr pos( eye->position() );
		glTranslatef( pos->x(), pos->y(), pos->z() );
		glCallList( eye->renderList() );
		glPopMatrix();
	}
	
	glEndList();
	
	return entity_list;
}


GLuint Entity::updateList() {
	if ( eye.get() )
		eye->updateList();
	return makeList();;
}


GLuint Entity::updateList(ImagePtr buffer, IntPtr grid) {
	check(buffer, grid);
	if ( eye.get() )
		eye->updateList(buffer, grid);
	return makeList();
}


GLuint Entity::renderList() {
	return entity_list;
}


void Entity::check(ImagePtr buffer, IntPtr grid) {
	checkEye(buffer, grid);	
}


void Entity::exec(ImagePtr buffer, IntPtr grid) {
	if ( eye.get() ) {
		eye->exec(buffer, grid);
	}
	
	makeList();
}


void Entity::checkEye(ImagePtr buffer, IntPtr grid) {
	if ( has_eye->get() ) {
		if ( !eye.get() ) {
			eye = EyePtr( new Eye("Eye") );
			add(eye);
		}
		eye->check(buffer, grid);
	} else {
		if ( eye.get() ) {
			erase("Eye");
			eye = EyePtr(0);
		}
	}
}


SerializablePtr Entity::Create(std::string name) {
	return RefCountPtr<Entity>( new Entity(name) );
}


std::string Entity::TypeName = "WV2_Entity";

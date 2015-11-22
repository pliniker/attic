

#include <Operations.h>
#include <Navigator.h>
#include "Consts.h"
#include "Eye.h"

#include <iostream>


Eye::Eye(std::string name) :
RenderObject    ( name                          ),
fovea_res       ( new Float3d("FoveaRes")       ),
perifovea_res   ( new Float3d("PerifoveaRes")   ),
world_res       ( new Float3d("WorldRes")       ),
eye_pos         ( new Float3d("EyePos")         ),
motor_pos       ( new Float3d("MotorPos")       ),
has_fovea       ( new Boolean("HasFovea")       ),
has_perifovea   ( new Boolean("HasPerifovea")   ),
has_motor       ( new Boolean("HasMotor")       ),
has_jreferent   ( new Boolean("HasJReferent")   ) {
	
	Navigator::EraseRenderer(this);

	GLuint id = ControlPoint::Allocate("Mouse", this);
    mouse = ControlPointPtr( ControlPoint::Get(id) );
	mouse->setAttachable(false);
	mouse->setPrivate(true);
	mouse->position()->set(0.0f, 0.0f);
	
	add(mouse);
	add(fovea_res);
	add(perifovea_res);
	add(world_res);
	add(eye_pos);
	add(motor_pos);
	add(has_fovea);
	add(has_perifovea);
	add(has_motor);
	add(has_jreferent);
	
	eye_list = glGenLists(1);
	
	makeList();
}


Eye::~Eye() {}


std::string Eye::type() {
	return TypeName;
}


void Eye::read(xmlNodePtr from) {
	RenderObject::read(from);
	
	mouse            = get("Mouse");
	fovea_res        = get("FoveaRes");
	perifovea_res    = get("PerifoveaRes");
	world_res        = get("WorldRes");
	eye_pos		     = get("EyePos");
	motor_pos        = get("MotorPos");
	has_fovea        = get("HasFovea");
	has_perifovea    = get("HasPerifovea");
	has_motor		 = get("HasMotor");
	has_jreferent    = get("HasJReferent");
	
	mouse->owner(this);
	
	makeList();
}


MapPtr Eye::displaysMap() { return MapPtr(0); }


GLuint Eye::makeList() {
    glNewList( eye_list, GL_COMPILE );
	glPushMatrix();
	
    glPushName( mouse->glid() );
		
	glColor3f( 1.0f, 0.6f, 0.6f );
	glLineWidth( 4.0f );
	
	if ( perifovea.get() ) {
		GLfloat x = perifovea_res->x() / SCALE;
		GLfloat y = perifovea_res->y() / SCALE;
		glBegin( GL_LINE_LOOP );
		glVertex3f( 0.0f, 0.0f, 0.0f );
		glVertex3f( 0.0f, y,    0.0f );
		glVertex3f( x,    y,    0.0f );
		glVertex3f( x,    0.0f, 0.0f );
		glEnd();
	}
	
	if ( fovea.get() ) {
		if ( perifovea.get() ) {
			GLfloat xofs = ( perifovea_res->x() - fovea_res->x() ) / (2.0f * SCALE);
			GLfloat yofs = ( perifovea_res->y() - fovea_res->y() ) / (2.0f * SCALE);
			glTranslatef( xofs, yofs, 0.0f );
		}
		GLfloat x = fovea_res->x() / SCALE;
		GLfloat y = fovea_res->y() / SCALE;
		glBegin( GL_LINE_LOOP );
		glVertex3f( 0.0f, 0.0f, 0.0f );
		glVertex3f( 0.0f, y,    0.0f );
		glVertex3f( x,    y,    0.0f );
		glVertex3f( x,    0.0f, 0.0f );
		glEnd();
	}
	
	glPopName();
	
	glPopMatrix();
	glEndList();
	
	return eye_list;
}


GLuint Eye::updateList() { return eye_list; }


GLuint Eye::updateList(ImagePtr buffer, IntPtr grid) {
	color_scheme = buffer->encoding();
	setupLayer(has_fovea,     fovea,     "ImageInputLayer", "Fovea"    );
	setupLayer(has_perifovea, perifovea, "ImageInputLayer", "Perifovea");
	setupLayer(has_motor,     motor,     "NullOutputLayer", "Motor"    );
	setupLayer(has_jreferent, jreferent, "ImageInputLayer", "JReferent");
	
	bool buffer_dirty        = buffer->isDirty();
	bool has_fovea_dirty     = has_fovea->isDirty();
	bool has_perifovea_dirty = has_perifovea->isDirty();
	bool fovea_dirty         = fovea.get() ? fovea->outputState->isDirty() : false;
	bool perifovea_dirty     = perifovea.get() ? perifovea->outputState->isDirty() : false;

	if ( fovea_dirty ) {
		fovea_res->set( fovea->outputState->xPixels(), fovea->outputState->y() );
	}
	
	if ( perifovea_dirty ) {
		perifovea_res->set( perifovea->outputState->xPixels(), perifovea->outputState->y() );
		if ( fovea.get() ) {
			perifovea_res *= fovea_res;
		}
	}
	
	if ( buffer_dirty ) {
		world_res->set( buffer->xPixels(), buffer->y() );
		if ( perifovea.get() ) {
			world_res -= perifovea_res;
		} else if ( fovea.get() ) {
			world_res -= fovea_res;
		}
        world_res /= SCALE;      
	}
	
	if ( has_fovea_dirty || has_perifovea_dirty || fovea_dirty || perifovea_dirty ) {
		makeList();
	}
	
	if ( mouse->position()->isDirty() ) {
		eye_pos->set( mouse->position() );
		boundsCheck( eye_pos, world_res );
		mouse->position()->set( eye_pos );
		mouse->position()->clearDirty();
	}
	
	has_fovea->clearDirty();
	has_perifovea->clearDirty();
	if ( fovea_dirty ) fovea->outputState->clearDirty();
	if ( perifovea_dirty ) perifovea->outputState->clearDirty();
		
	return eye_list;
}


GLuint Eye::renderList() { return eye_list; }


Float3dPtr Eye::position() { return eye_pos; }


void Eye::exec(ImagePtr buffer, IntPtr grid) {
//	if ( has_motor->get() )     execMotor();
//
//	if ( eye_pos != mouse->position() ) { // case: mouse moved eye
//		eye_pos->set( mouse->position() );
//		if ( has_motor->get() ) 
//			feedbackMotor();
//	} else {                             // case: motor moved eye
//		eye_pos->set( motor_pos );
//	}
//	
//	snapToGrid(eye_pos, grid);
//	mouse->position()->set(eye_pos);
//
//	if ( has_fovea->get() )     execFovea(buffer);
//	if ( has_perifovea->get() ) execPerifovea(buffer);
//	if ( has_jreferent->get() ) execJReferent();
}


void Eye::setupLayer(BooleanPtr condition, NodeLayerPtr& layer, Image::ColorEncoding enc,
                     std::string type, std::string name) {
	if ( condition->get() ) {
		if ( !layer.get() ) {
			layer = NodeLayerPtr( NodeLayer::New(type) );
			layer->displayName->set(name);
            layer->outputState->create( 4, 4, enc );
			add(layer);
		}
	} else {
		if ( layer.get() ) {
			erase( layer->name() );
			layer = NodeLayerPtr(0);
		}
	}
}


void Eye::boundsCheck(Float3dPtr boundme, Float3dPtr bounds) {
	if ( boundme->x() < 0.0 ) boundme->x( 0.0 );
	if ( boundme->y() < 0.0 ) boundme->y( 0.0 );
	if ( boundme->z() < 0.0 ) boundme->z( 0.0 );
	if ( boundme->x() > bounds->x() ) boundme->x( bounds->x() );
	if ( boundme->y() > bounds->y() ) boundme->y( bounds->y() );
	if ( boundme->z() > bounds->z() ) boundme->z( bounds->z() );
}


void Eye::snapToGrid(Float3dPtr snapme, IntPtr grid) {
	GLfloat x = snapme->x();
	GLfloat y = snapme->y();
	
	x -= (int)x % (int)((float)grid->get() / SCALE);
	y -= (int)y % (int)((float)grid->get() / SCALE);
	
	snapme->set(x, y);
}


SerializablePtr Eye::Create(std::string name) {
	return RefCountPtr<Eye>( new Eye(name) );
}


std::string Eye::TypeName = "VW2_Eye";


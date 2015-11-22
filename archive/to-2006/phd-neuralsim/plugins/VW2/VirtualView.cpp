
#include "Consts.h"
#include "VirtualView.h"
#include "Entity.h"


VirtualView::VirtualView(std::string name) :
RenderObject ( name                           ),
entities     ( new Map("Entities")            ),
entity_maker ( new ObjectMaker("EntityMaker") ),
double_buffer( new Image("DoubleBuffer")      ),
tex_res      ( new Int3d("TextureRes")        ),
grid_spacing ( new Int("GridSpacing")         ) {
	
    GLuint id = ControlPoint::Allocate("Handle", this);
    handle = ControlPointPtr( ControlPoint::Get(id) );
	handle->setAttachable(false);
	
	double_buffer->setPrivate(true);
	handle->setPrivate(true);
	
	entity_maker->setTarget(entities);
	entity_maker->setChoices(Entity::TypeName);
	
	grid_spacing->setMin(1);
	grid_spacing->set(1);
	
	add(handle);
	add(entities);
	add(entity_maker);
	add(double_buffer);
	add(tex_res);
	add(grid_spacing);
	
	view_list = glGenLists(1);
	texture = 0;
	
	makeList();
}


VirtualView::~VirtualView() {
	glDeleteLists( view_list, 1 );
    glDeleteTextures( 1, &texture );
}


std::string VirtualView::type() {
    return TypeName;
}


void VirtualView::read(xmlNodePtr from) {
    RenderObject::read(from);
	
	handle        = get("Handle");
	entities      = get("Entities");
	entity_maker  = get("EntityMaker");
	double_buffer = get("DoubleBuffer");
	grid_spacing  = get("GridSpacing");
	tex_res       = get("TextureRes");
	
	handle->owner(this);
	
	makeList();
}


MapPtr VirtualView::displaysMap() {
	return MapPtr(0);
}


GLuint VirtualView::makeList() {
	
	int x = 1; while ( x < double_buffer->x() ) { x *= 2; }
	int y = 1; while ( y < double_buffer->y() ) { y *= 2; }
	
    tex_res->x(x * 2); 
	tex_res->y(y * 2);
	
    MakeTempBuffer(double_buffer->x() * double_buffer->y() * 4, &TempBuffer1, &TempSize1);
    MakeTempBuffer(tex_res->x() * tex_res->y() * 4, &TempBuffer2, &TempSize2);
    
    if ( double_buffer->size() ) {
        double_buffer->convertToRGBA(TempBuffer1);
		RenderObject::ScaleRGBA( (int*)TempBuffer1, double_buffer->x(), double_buffer->y(),
								 (int*)TempBuffer2, tex_res->x(), tex_res->y() );
    }
	
    // make the gl texture
	if (texture) glDeleteTextures( 1, &texture );
    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, tex_res->x(), tex_res->y(),
				  0, GL_RGBA, GL_UNSIGNED_BYTE, TempBuffer2 );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	
	// make the draw list
    glNewList( view_list, GL_COMPILE );
	glPushMatrix();
	
    glEnable( GL_BLEND );
    glBlendFunc( GL_ONE, GL_ONE );
	
	glPushMatrix();
	for (Map::ObsIt i = entities->begin(); i != entities->end(); i++) {
		EntityPtr e( entities->get(i) );
		glCallList( e->renderList() );
	}
	glPopMatrix();
	
    glPushName( handle->glid() );
    
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, texture );
	
	glColor3f( 1.0f, 1.0f, 1.0f );
    glBegin( GL_QUADS );
    glTexCoord2f( 0.0f, 1.0f );
    glVertex3f(   0.0f,                       0.0f,                       0.0f );   // - - 0 0
    glTexCoord2f( 0.0f, 0.0f );
    glVertex3f(   0.0f,                       double_buffer->y() / SCALE, 0.0f );   // - + 0 1
    glTexCoord2f( 1.0f, 0.0f );
    glVertex3f(   double_buffer->x() / SCALE, double_buffer->y() / SCALE, 0.0f );   // + + 1 1
    glTexCoord2f( 1.0f, 1.0f );
    glVertex3f(   double_buffer->x() / SCALE, 0.0f,                       0.0f );   // + - 1 0
    glEnd();
	
    glDisable( GL_TEXTURE_2D );
    glPopName();
	
	glDisable( GL_BLEND );
	
	glPopMatrix();
	glEndList();
	
	return view_list;
}


GLuint VirtualView::updateList() {
	
    if ( double_buffer->size() ) {
        double_buffer->convertToRGBA(TempBuffer1);
		RenderObject::ScaleRGBA( (int*)TempBuffer1, double_buffer->x(), double_buffer->y(),
								 (int*)TempBuffer2, tex_res->x(), tex_res->y() );
        glBindTexture( GL_TEXTURE_2D, texture );
        glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, tex_res->x(), tex_res->y(),
						 GL_RGBA, GL_UNSIGNED_BYTE, TempBuffer2 );
    }
	
	for (Map::ObsIt i = entities->begin(); i != entities->end(); i++) {
		EntityPtr e( entities->get(i) );
		e->updateList(double_buffer, grid_spacing);
	}
	
    return view_list;
}


GLuint VirtualView::renderList() { return view_list; }


Float3dPtr VirtualView::position() { return handle->position(); }


void VirtualView::check(ImagePtr bgImage) {
	
	double_buffer->create( bgImage->xPixels(), bgImage->y(), bgImage->encoding() );
	
	for (Map::ObsIt i = entities->begin(); i != entities->end(); i++) {
		EntityPtr e( entities->get(i) );
		e->check(double_buffer, grid_spacing);
	}
	
	makeList();
}


void VirtualView::exec(ImagePtr bgImage) {
	
	for (int i = 0; i < double_buffer->size(); i++) {
		double_buffer->at(i) = bgImage->at(i);
	}
	
	for (Map::ObsIt i = entities->begin(); i != entities->end(); i++) {
		EntityPtr e( entities->get(i) );
		e->exec(double_buffer, grid_spacing);
	}	
}


SerializablePtr VirtualView::Create(std::string name) {
    return RefCountPtr<VirtualView>( new VirtualView(name) );
}


std::string VirtualView::TypeName = "VW2_VirtualView";


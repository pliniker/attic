

#include <Operations.h>
#include "VirtualWorld.h"
#include "Fovea.h"
#include "JReferentScale.h"
#include "MotorScale.h"
#include "JReferentNRMStyle.h"
#include "MotorNRMStyle.h"


VirtualWorld::VirtualWorld(std::string name) :
RenderObject ( name                      ),
state        ( new Image("outputState")  ),
fovea        ( new Image("outputState")  ),
foveaStep    ( new Int("foveaStep")      ),
gridSize     ( new Int3d("gridSize")     ),
gridPos	     ( new Int3d("gridPos")      ),
gridDelta    ( new Int3d("gridDelta")    ),
display_map  ( new Map("VirtualWorld")   ),
state_map    ( new Map("state")          ),
fovea_map    ( new Map("fovea")          ),
scale_factor ( new Float("scale_factor") ),
tex_res      ( new Int3d("tex_res")      ),
state_pos    ( new Float3d("state_pos")  ),
fovea_pos    ( new Float3d("fovea_pos")  ),
state_orig   ( new Int3d("state_orig")   ),
fovea_orig   ( new Int3d("fovea_orig")   ),
grid_orig    ( new Int3d("grid_orig")    ) {

	foveaStep->set(16);
	foveaStep->setMin(1);
	foveaStep->setMax(1024);
	
	state->create(48, 48, Image::Gray8bpp);
	fovea->create(16, 16, Image::Gray8bpp);
	
	display_map->add(state_map);
	display_map->add(fovea_map);
	
	state_map->add(state);
	fovea_map->add(fovea);
	fovea_map->add(foveaStep);

    GLuint id = ControlPoint::Allocate("state_point", this);
    state_point = ControlPointPtr( ControlPoint::Get(id) );
	state_pos->set( state_point->position() );
	
	id = ControlPoint::Allocate("fovea_point", this);
	fovea_point = ControlPointPtr( ControlPoint::Get(id) );
	fovea_pos->set( fovea_point->position() );

	state_point->setAttachable(false);
	fovea_point->setAttachable(false);
	
	scale_factor->set(2.0);

	add(display_map);
	add(state_point);	
	add(fovea_point);
	add(state_pos);
	add(fovea_pos);
	add(scale_factor);
	add(gridSize);
	add(gridPos);
	add(gridDelta);
	add(grid_orig);
	add(tex_res);
	add(state_orig);
	add(fovea_orig);

	state_list = glGenLists(1);
	fovea_list = glGenLists(1);

	makeList();
}


VirtualWorld::~VirtualWorld() {
	glDeleteLists( state_list, 1 );
	glDeleteLists( fovea_list, 1 );
    glDeleteTextures( 1, &texture );
	ControlPoint::Erase( state_point->glid() );
	ControlPoint::Erase( fovea_point->glid() );
}


MapPtr VirtualWorld::displaysMap() { 
	return display_map;
}


std::string VirtualWorld::type() { 
	return VirtualWorld::NameSpace;
}


void VirtualWorld::read(xmlNodePtr from) { 
	RenderObject::read(from);
	
	display_map  = get("VirtualWorld");
	
	state_map    = display_map->get("state");
	fovea_map    = display_map->get("fovea");
	
	state        = state_map->get("outputState");
	fovea        = fovea_map->get("outputState");
	
	state_point  = get("state_point");
	fovea_point  = get("fovea_point");
	state_pos    = get("state_pos");
	fovea_pos    = get("fovea_pos");
	
	foveaStep    = fovea_map->get("foveaStep");
	scale_factor = get("scale_factor");
	
	gridSize     = get("gridSize");
	gridPos      = get("gridPos");
	gridDelta    = get("gridDelta");
	grid_orig    = get("grid_orig");
	
	tex_res      = get("tex_res");
	state_orig   = get("state_orig");
	fovea_orig   = get("fovea_orig");
		
	state_point->owner(this);
	fovea_point->owner(this);
	
	makeList();
}

//
///////////////////////////////////////////////////////////////////////
//

GLuint VirtualWorld::makeList() {
	makeGrid();
	makeFoveaList();
	makeStateList();
    return state_list;
}


GLuint VirtualWorld::updateList() {
	
	// State size changed? (see Brainlib:Operations.h)
	if ( neqDim(state_orig, state) ) {
		glDeleteTextures( 1, &texture );
		makeList();
		fovea->create( fovea->x(), fovea->y(), state->encoding() );
	}

	// Fovea size changed?
	if ( neqDim(fovea_orig, fovea) ) {
		screenToGrid();
		makeGrid();
		makeFoveaList();
		fovea->create( fovea->x(), fovea->y(), state->encoding() );
	// Fovea position changed by user?
	} else if ( fovea_pos != fovea_point->position() ) {
		screenToGrid();
		makeFoveaList();
	// Fovea position changed by motor?
	} else if ( grid_orig != gridPos ) {
		gridToScreen();
		makeFoveaList();
	}
		
	// update texture
    if ( state->size() ) {
        state->convertToRGBA(TempBuffer1);
		RenderObject::ScaleRGBA( (int*)TempBuffer1, state_orig->x(), state_orig->y(),
								 (int*)TempBuffer2, tex_res->x(), tex_res->y() );
        glBindTexture( GL_TEXTURE_2D, texture );
        glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, tex_res->x(), tex_res->y(),
						 GL_RGBA, GL_UNSIGNED_BYTE, TempBuffer2 );
    }

	// update delta vars
	gridDelta->set( gridPos );
	gridDelta -= grid_orig;
	
	grid_orig->set( gridPos );
	fovea_pos->set( fovea_point->position() );

    return state_list;
}


GLuint VirtualWorld::renderList() {
	return state_list;
}

//
///////////////////////////////////////////////////////////////////////
//

Float3dPtr VirtualWorld::position() {
	return state_point->position();
}


void VirtualWorld::position(GLVector& delta) {
	state_point->move(delta);
	fovea_point->move(delta);
}


GLfloat VirtualWorld::scale() { 
	return scale_factor->get();
}


void VirtualWorld::scale(GLfloat scaleDelta) {
    scale_factor->set( scale_factor->get() + scaleDelta );
    if (scale_factor->get() >= 16.0) scale_factor->set(16.0);
    if (scale_factor->get() <= 1.0) scale_factor->set(1.0);
}

//
///////////////////////////////////////////////////////////////////////
//

void VirtualWorld::makeGrid() {
	gridSize->x( state->x() - fovea->x() );
	gridSize->y( state->y() - fovea->y() );
	gridPos->set(0, 0);
	aspect_ratio = ( (float)state->x() / (float)state->y() ) / 2.0;
}


GLuint VirtualWorld::makeFoveaList() {
	fovea_orig->x( fovea->x() );
	fovea_orig->y( fovea->y() );
	
    glNewList( fovea_list, GL_COMPILE );
	glPushMatrix();
	
    glPushName( fovea_point->glid() );
	
	GLfloat sf = scale_factor->get();
	GLVector pos( fovea_point->position() );
	pos.x /= sf;
	pos.y /= sf;

	GLfloat width  = (float)fovea->x() / (float)state->x() * aspect_ratio;
	GLfloat height = ( (float)fovea->y() / (float)state->y() ) / 2.0;

	if ( pos.x - width < -aspect_ratio ) pos.x = width - aspect_ratio;
	if ( pos.x + width > aspect_ratio  ) pos.x = aspect_ratio - width;
	if ( pos.y - height < -0.5        ) pos.y = height - 0.5;
	if ( pos.y + height > 0.5         ) pos.y = 0.5 - height;
	
	glTranslatef( pos.x, pos.y, 0.0f );
	
	glColor3f( 0.3, 0.3, 0.3 );
	glBegin( GL_POLYGON );
    glVertex3f( -width, height, 0.0f );
    glVertex3f( -width, -height, 0.0f );
    glVertex3f( width, -height, 0.0f );
    glVertex3f( width, height, 0.0f );
	glEnd();
	
	glPopName();
	
	glPopMatrix();
	glEndList();
	
	return fovea_list;
}


GLuint VirtualWorld::makeStateList() {
	state_orig->x( state->x() );
	state_orig->y( state->y() );
	
	int x = 1; while ( x < state_orig->x() ) { x *= 2; }
	int y = 1; while ( y < state_orig->y() ) { y *= 2; }
	
    tex_res->x(x * 2); 
	tex_res->y(y * 2);
	
    MakeTempBuffer(state_orig->x() * state_orig->y() * 4, &TempBuffer1, &TempSize1);
    MakeTempBuffer(tex_res->x() * tex_res->y() * 4, &TempBuffer2, &TempSize2);
    
    if ( state->size() ) {
        state->convertToRGBA(TempBuffer1);
		RenderObject::ScaleRGBA( (int*)TempBuffer1, state_orig->x(), state_orig->y(),
								 (int*)TempBuffer2, tex_res->x(), tex_res->y() );
    }
	
    // make the gl texture
    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, tex_res->x(), tex_res->y(),
				  0, GL_RGBA, GL_UNSIGNED_BYTE, TempBuffer2 );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	
	// make the draw list
    glNewList( state_list, GL_COMPILE );
	glPushMatrix();
	
    glEnable( GL_BLEND );
    glBlendFunc( GL_ONE, GL_ONE );
	
	glCallList( fovea_list );
	
    glPushName( state_point->glid() );
    
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, texture );
	
    glBegin( GL_QUADS );
    glTexCoord2f( 0.0f, 1.0f );
    glVertex3f( -aspect_ratio, -0.5f, 0.0f );
    glTexCoord2f( 0.0f, 0.0f );
    glVertex3f( -aspect_ratio, 0.5f, 0.0f );
    glTexCoord2f( 1.0f, 0.0f );
    glVertex3f( aspect_ratio, 0.5f, 0.0f );
    glTexCoord2f( 1.0f, 1.0f );
    glVertex3f( aspect_ratio, -0.5f, 0.0f );
    glEnd();
	
    glDisable( GL_TEXTURE_2D );
    glPopName();
	
	glDisable( GL_BLEND );
	
	glPopMatrix();
	glEndList();
	
	return state_list;
}


void VirtualWorld::screenToGrid() {
	Float3dPtr pos( fovea_point->position() );
	float scale = scale_factor->get();
	int step = foveaStep->get();
	
	float fxPos = pos->x() / (aspect_ratio * 2.0 * scale) + 0.5;
	float fyPos = 1.0 - (pos->y() / scale + 0.5);
	
	int xOffset = fovea->x() / 2;
	int yOffset = fovea->y() / 2;
	
	int ixPos = (int)( fxPos * (float)state->x() ) - xOffset;
	int iyPos = (int)( fyPos * (float)state->y() ) - yOffset;
	
	if ( ixPos < 0 ) ixPos = 0;
	if ( ixPos > state->x() - fovea->x() ) ixPos = state->x() - fovea->x();
	if ( iyPos < 0 ) iyPos = 0;
	if ( iyPos > state->y() - fovea->y() ) iyPos = state->y() - fovea->y();
	
	ixPos -= ixPos % step;
	iyPos -= iyPos % step;
	
	gridPos->set(ixPos, iyPos);
}


void VirtualWorld::gridToScreen() {
	Float3dPtr pos( fovea_point->position() );
	float scale = scale_factor->get();
	
	int ixPos = gridPos->x();
	int iyPos = gridPos->y();
	
	int xOffset = fovea->x() / 2;
	int yOffset = fovea->y() / 2;
	
	float fxPos = (float)(ixPos + xOffset) / (float)state->x();
	float fyPos = (float)(iyPos + yOffset) / (float)state->y();
	
	pos->x( (fxPos - 0.5) * (aspect_ratio * 2.0 * scale) );
	pos->y( (1.0 - fyPos) * scale - 0.5 );
}


//
///////////////////////////////////////////////////////////////////////
//

SerializablePtr VirtualWorld::Create(std::string name) {
	if ( !vworld.get() ) {
		vworld = VirtualWorldPtr( new VirtualWorld(name) );
	}
	return vworld;
}



std::string     VirtualWorld::NameSpace = "VirtualWorld";
VirtualWorldPtr VirtualWorld::vworld = VirtualWorldPtr(0);



extern "C" void Initialize() {
	Serializable::AddFactory(VirtualWorld::NameSpace, VirtualWorld::Create);
	StringList::Get(VirtualWorld::NameSpace)->add(VirtualWorld::NameSpace);
	
	Serializable::AddFactory(Fovea::TypeName, Fovea::Create);
	StringList::Get(VirtualWorld::NameSpace)->add(Fovea::TypeName);
	StringList::Get("node_layers")->add(Fovea::TypeName);	
	
	Serializable::AddFactory(JReferentScale::TypeName, JReferentScale::Create);
	StringList::Get(VirtualWorld::NameSpace)->add(JReferentScale::TypeName);
	StringList::Get("node_layers")->add(JReferentScale::TypeName);
	
	Serializable::AddFactory(MotorScale::TypeName, MotorScale::Create);
	StringList::Get(VirtualWorld::NameSpace)->add(MotorScale::TypeName);
	StringList::Get("node_layers")->add(MotorScale::TypeName);

    Serializable::AddFactory(JReferentNRMStyle::TypeString, JReferentNRMStyle::Create);
    StringList::Get(VirtualWorld::NameSpace)->add(JReferentNRMStyle::TypeString);
    StringList::Get("node_layers")->add(JReferentNRMStyle::TypeString);
	
    Serializable::AddFactory(MotorNRMStyle::TypeName, MotorNRMStyle::Create);
    StringList::Get(VirtualWorld::NameSpace)->add(MotorNRMStyle::TypeName);
    StringList::Get("node_layers")->add(MotorNRMStyle::TypeName);	
}



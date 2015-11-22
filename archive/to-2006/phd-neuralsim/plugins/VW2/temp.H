
#include <RenderObject.h>
#include <ControlPoint.h>
#include <Vector3d.h>
#include <Image.h>

// renderer for sprites
class Object : public RenderObject {
public:
    Object(std::string name);
    ~Object();

    std::string      type();
    virtual void     read(xmlNodePtr from);

    virtual GLuint   makeList();
    virtual GLuint   updateList();
    virtual GLuint   renderList();

    virtual bool     check(ImagePtr buffer, Int3dPtr grid);
    virtual void     exec(ImagePtr buffer, Int3dPtr grid);
    
    ControlPointPtr  Handle;
    Int3dPtr         GridPos;
    ImagePtr         Object;
};

typedef RefCountPtr<Object> ObjectPtr;

/*****************************************************************************/

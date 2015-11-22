#ifndef VECTOR3DVIEW_H
#define VECTOR3DVIEW_H


#include <Vector3d.h>
#include "ObjectView.h"


class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QFrame;
class QSpinBox;
class QLabel;


class Vector3dView : public ObjectView
{
    Q_OBJECT
    
    /// Object methods
public:
    Vector3dView(   nnet::SerializablePtr   object,
                    QWidget*                parent = 0,
                    const char*             name = 0,
                    WFlags                  fl = 0 );
    ~Vector3dView();
    
    QFrame*         frame21;
    QLabel*         label;
    QLabel*         xLabel;
    QSpinBox*       x;
    QLabel*         yLabel;
    QSpinBox*       y;
    QLabel*         zLabel;
    QSpinBox*       z;
    
public slots:
    virtual void    refreshView();
    virtual void    applyChanges();
    
protected:
    QGridLayout*    Vector3dViewLayout;
    QGridLayout*    frame21Layout;
    
protected slots:
    virtual void    languageChange();
    
    /// Class methods
public:
    static ObjectView* Create(  nnet::SerializablePtr   object,
                                QWidget*                parent = 0,
                                const char*             name = 0 );
};


#endif // VECTOR3DVIEW_H

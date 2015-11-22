#ifndef OBJECTVIEW_H
#define OBJECTVIEW_H


#include <qvariant.h>
#include <qwidget.h>
#include <Serializable.h>


class ObjectView : public QWidget
{
    Q_OBJECT
    
public:
    ObjectView(     nnet::SerializablePtr   object,
                    QWidget*                parent = 0,
                    const char*             name = 0,
                    WFlags                  fl = 0 );
                    
    virtual ~ObjectView();

    
public slots:
    virtual void    refreshView();
    virtual void    applyChanges();
};


#endif

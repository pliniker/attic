

#include "ObjectView.h"


ObjectView::ObjectView( nnet::SerializablePtr,
                        QWidget*                parent,
                        const char*             name,
                        WFlags                  fl ) :
    QWidget( parent, name, fl )
{
}


ObjectView::~ObjectView()
{
}


void ObjectView::refreshView()
{
}


void ObjectView::applyChanges()
{
}


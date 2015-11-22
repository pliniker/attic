#ifndef BOOLEANVIEW_H
#define BOOLEANVIEW_H


#include <Boolean.h>
#include "ObjectView.h"


class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QCheckBox;


class BooleanView : public ObjectView
{
    Q_OBJECT
    
    /// Object methods
public:
    BooleanView(    nnet::SerializablePtr   object,
                    QWidget*                parent = 0,
                    const char*             name = 0,
                    WFlags                  fl = 0 );
    virtual ~BooleanView();
    
    QLabel*         label;
    QCheckBox*      check;
    
public slots:
    virtual void    refreshView();
    virtual void    applyChanges();
    
protected:
    QHBoxLayout*    BooleanViewLayout;
    
protected slots:
    virtual void    languageChange();
    
private:
    nnet::BooleanPtr boolean;
    
    /// Class methods
public:
    static ObjectView* Create( nnet::SerializablePtr    object,
                               QWidget*                 parent = 0,
                               const char*              name = 0 );
};


#endif // BOOLEANVIEW_H

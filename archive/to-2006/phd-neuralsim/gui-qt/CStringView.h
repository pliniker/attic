#ifndef CSTRINGVIEW_H
#define CSTRINGVIEW_H


#include <CString.h>
#include "ObjectView.h"


class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QLineEdit;


class CStringView : public ObjectView
{
    Q_OBJECT
    
    /// Object methods
public:
    CStringView(    nnet::SerializablePtr   object,
                    QWidget*                parent = 0,
                    const char*             name = 0,
                    WFlags                  fl = 0 );
    ~CStringView();
    
    QLabel*         label;
    QLineEdit*      edit;
    
public slots:
    virtual void    refreshView();
    virtual void    applyChanges();
    
protected:
    QHBoxLayout*    CStringViewLayout;
    
protected slots:
    virtual void    languageChange();
    
private:
    nnet::CStringPtr cstr;
    
    /// Class methods
public:
    static ObjectView* Create(  nnet::SerializablePtr   object,
                                QWidget*                parent = 0,
                                const char*             name = 0 );
};


#endif // CSTRINGVIEW_H

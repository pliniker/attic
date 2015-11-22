#ifndef FILECHOOSERVIEW_H
#define FILECHOOSERVIEW_H


#include <CString.h>
#include "ObjectView.h"


class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QPushButton;


class FileChooserView : public ObjectView
{
    Q_OBJECT
    
    /// Object methods
public:
    FileChooserView(    nnet::SerializablePtr   object,
                        QWidget*                parent = 0,
                        const char*             name = 0,
                        WFlags                  fl = 0 );
    ~FileChooserView();
    
    QLabel*             label;
    QPushButton*        edit;
    
public slots:
    virtual void        refreshView();
    virtual void        applyChanges();
    
protected:
    QHBoxLayout*        FileChooserViewLayout;
    
protected slots:
    virtual void        pickFile();
    virtual void        languageChange();
    
private:
    nnet::CStringPtr    cstr;
    
    /// Class methods
public:
    static ObjectView*  Create( nnet::SerializablePtr   object,
                                QWidget*                parent = 0,
                                const char*             name = 0 );
};


#endif // FILECHOOSERVIEW_H

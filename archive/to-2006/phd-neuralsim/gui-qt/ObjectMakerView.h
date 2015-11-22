#ifndef OBJECTMAKERVIEW_H
#define OBJECTMAKERVIEW_H


#include <qdialog.h>
#include <OptionList.h>
#include "ObjectView.h"


class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QListBox;
class QListBoxItem;
class QToolButton;
class QLineEdit;




class NameObject : public QDialog
{
    Q_OBJECT
    
    /// Object methods
public:
    NameObject(     QWidget* parent = 0,
                    const char* name = 0,
                    WFlags fl = 0 );
                    
    std::string     name();
    
public slots:
    void            returnPressed();
    
    /// Object data
protected:
    QHBoxLayout*    nameObjectLayout;
    
private:
    QLineEdit*      edit;
    std::string     objectName;
};



class ObjectMakerView : public ObjectView
{
    Q_OBJECT
    
    /// Object methods
public:
    ObjectMakerView(    nnet::SerializablePtr   object,
                        QWidget*                parent = 0,
                        const char*             name = 0,
                        WFlags                  fl = 0 );
    ~ObjectMakerView();
    
    QLabel*             label;
    QListBox*           objects;
    QToolButton*        addButton;
    QToolButton*        delButton;
    
public slots:
    virtual void        refreshView();
    virtual void        applyChanges();
    virtual void        newSelection( QListBoxItem* );
    virtual void        addObject();
    virtual void        delObject();
    
protected:
    QVBoxLayout*        ObjectMakerViewLayout;
    QHBoxLayout*        ButtonLayout;
    NameObject*         nameObject;
    
protected slots:
    virtual void        languageChange();
    
private:
    nnet::OptionListPtr optionList;
    std::string         currentItem;
    
    /// Class methods
public:
    static ObjectView* Create(  nnet::SerializablePtr   object,
                                QWidget*                parent = 0,
                                const char*             name = 0 );
};


#endif // ObjectMakerView

#ifndef OPTIONLISTVIEW_H
#define OPTIONLISTVIEW_H


#include <OptionList.h>
#include "ObjectView.h"


class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QListBox;
class QListBoxItem;



class OptionListView : public ObjectView
{
    Q_OBJECT
    
    /// Object methods
public:
    OptionListView(     nnet::SerializablePtr   object,
                        QWidget*                parent = 0,
                        const char*             name = 0,
                        WFlags                  fl = 0 );
    ~OptionListView();
    
    QLabel*             label;
    QListBox*           selected;
    
public slots:
    virtual void        rightClickedSlot();
    virtual void        refreshView();
    virtual void        applyChanges();
    virtual void        newSelection( QListBoxItem* );
    
protected:
    QVBoxLayout*        OptionListViewLayout;
    
protected slots:
    virtual void        languageChange();
    
private:
    nnet::OptionListPtr optionList;
    
    /// Class methods
public:
    static ObjectView*  Create( nnet::SerializablePtr   object,
                                QWidget*                parent = 0,
                                const char*             name = 0 );
};


#endif // OPTIONLISTVIEW_H

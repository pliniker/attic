#ifndef NEWOBJECT_H
#define NEWOBJECT_H


#include <qvariant.h>
#include <qdialog.h>
#include <StringList.h>


class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QIconView;
class QIconViewItem;
class QPushButton;


class NewObject : public QDialog
{
    Q_OBJECT
    
    /// Object methods
public:
    NewObject(      QWidget*    parent = 0,
                    const char* name = 0,
                    bool        modal = FALSE,
                    WFlags      fl = 0 );
    ~NewObject();
    
    void            clear();
    void            populateWith( nnet::StringListBase* types );
    QString         typeSelected();
    
    QIconView*      iconView;
    QPushButton*    createButton;
    QPushButton*    cancelButton;
    
protected:
    QVBoxLayout*    NewObjectLayout;
    QHBoxLayout*    layout2;
    QSpacerItem*    spacer;
    
protected slots:
    void            selectionChangedSlot( QIconViewItem* item );
    void            selectIconSlot( QIconViewItem* item );
    void            languageChange();
    
private:
    QIconViewItem*  currentItem;
};


#endif // NEWOBJECT_H

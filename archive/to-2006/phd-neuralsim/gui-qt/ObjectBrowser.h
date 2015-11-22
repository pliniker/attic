#ifndef OBJECTBROWSER_H
#define OBJECTBROWSER_H


#include <qvariant.h>
#include <qdialog.h>
#include <Map.h>
#include <Serializable.h>
#include <string>
#include <list>
#include <map>
#include "ObjectView.h"


class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QSplitter;
class QListView;
class QListViewItem;
class QFrame;
class QPushButton;


typedef ObjectView* ( *viewer ) ( nnet::SerializablePtr, QWidget*, const char* );


class ObjectBrowser : public QDialog
{
    Q_OBJECT
    
    /// Object methods
public:
    ObjectBrowser(      QWidget*    parent = 0,
                        const char* name = 0,
                        bool        modal = FALSE,
                        WFlags      fl = 0 );
    ~ObjectBrowser();
    
    void                display( nnet::MapPtr root );
    
    QSplitter*          splitter;
    QListView*          mapList;
    QFrame*             objectView;
    QPushButton*        refreshButton;
    QPushButton*        applyButton;
    QPushButton*        closeButton;
    
protected:
    void                display( nnet::MapPtr root, QListViewItem* item );
    
    QVBoxLayout*        ObjectBrowserLayout;
    QVBoxLayout*        objectLayout;
    QHBoxLayout*        bottomLayout;
    QSpacerItem*        spacing;
    
protected slots:
    void                selectMap( QListViewItem* item );
    void                applyChanges();
    void                refreshView();
    void                languageChange();
    
private:
    typedef std::list<ObjectView*> ViewList;
    typedef ViewList::iterator ViewListIt;
    
    nnet::MapPtr        currentRoot;
    ViewList            viewList;
    
    /// Class methods
public:
    static void         Add(    std::string type, viewer ptr );
    static void         Remove( std::string type );
    static ObjectView*  Get(    nnet::SerializablePtr   object,
                                QWidget*                parent,
                                const char*             name );
    
private:
    typedef std::map<std::string, viewer, std::less<std::string> > ViewMap;
    typedef ViewMap::iterator ViewMapIt;
    
    static ViewMap      viewMap;
};


#endif // OBJECTBROWSER_H

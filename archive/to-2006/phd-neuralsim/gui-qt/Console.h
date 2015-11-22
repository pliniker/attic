#ifndef CONSOLE_H
#define CONSOLE_H


#include <qvariant.h>
#include <qdialog.h>
#include <ErrorInterface.h>


class QWidget;
class QListBox;
class QVBoxLayout;
class QMouseEvent;


class Console : public QDialog, public nnet::ErrorInterfaceBase
{
    Q_OBJECT
    
public:
    Console(    QWidget*    parent = 0,
                const char* name = 0,
                bool        modal = FALSE,
                WFlags      fl = 0 );
    ~Console();
    
    QListBox* text;
    
    void alert( std::string err );
    void log(   std::string err );
    bool ask(   std::string err );
    void clear();
    
protected:
    QVBoxLayout* consoleLayout;
    
private:
    QWidget*    parentWidget;
    QString     caption;
};


#endif


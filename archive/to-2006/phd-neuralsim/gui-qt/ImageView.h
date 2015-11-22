#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H


#include <Image.h>
#include "ObjectView.h"


class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QFrame;
class QSpinBox;
class QLabel;
class QToolButton;
class QCheckBox;
class QComboBox;



class ImageView : public ObjectView 
{
    Q_OBJECT
    
    /// Object methods
public:
    ImageView(      nnet::SerializablePtr   object,
                    QWidget*                parent = 0,
                    const char*             name = 0, 
                    WFlags                  fl = 0 );
    ~ImageView();
    
    QFrame*         frame;
    QSpinBox*       bpp;
    QLabel*         bppLabel;
    QSpinBox*       xres;
    QLabel*         xresLabel;
    QSpinBox*       yres;
    QLabel*         yresLabel;
    QToolButton*    loadButton;
    QToolButton*    saveButton;
    QLabel*         label;
    QCheckBox*      color;
    
    QLabel*         encLabel;
    QComboBox*      enc;
    
    
public slots:
    virtual void    loadSlot();
    virtual void    saveSlot();
    virtual void    refreshView();
    virtual void    applyChanges();
    
protected:
    QGridLayout*    ImageViewLayout;
    QGridLayout*    frameLayout;
    
protected slots:
    virtual void    languageChange();
    
private:
    nnet::ImagePtr  image;
    
    /// Class methods
public:
    static ObjectView* Create(  nnet::SerializablePtr   object,
                                QWidget*                parent = 0,
                                const char*             name = 0 );
};


#endif // IMAGEVIEW_H

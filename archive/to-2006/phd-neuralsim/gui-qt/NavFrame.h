#ifndef NAVFRAME_H
#define NAVFRAME_H


#include <boost/signals.hpp>
#include <qgl.h>
#include <qpoint.h>

#include <Navigator.h>
#include <Integer.h>
#include <Image.h>

#include "NewObject.h"
#include "ObjectBrowser.h"


class QAction;
class QPopupMenu;


class NavFrame : public QGLWidget {
    Q_OBJECT

public:
    NavFrame( QWidget* parent, char const* name );
    ~NavFrame();
    
    void reset();
    
    QAction* newNodeAction;
    QAction* newEdgeAction;
    QAction* deleteObjectAction;
    QAction* recallObjectAction;
    QAction* trainObjectAction;
    QAction* checkObjectAction;
    QAction* browseObjectAction;
    QAction* copyImageAction;
    QAction* pasteObjectAction;
    QAction* pasteImageAction;
    QAction* blankImageAction;
    QAction* disconnectObjectAction;
    
    QPopupMenu* objectMenu;
    QPopupMenu* pasteMenu;
    
public slots:
    void newNodeSlot();
    void newEdgeSlot();
    void deleteObjectSlot();
    void browseObjectSlot();
    
    void copyImageSlot();
    void pasteObjectSlot();
    void pasteImageSlot();
    void blankImageSlot();
    
    void recallSlot();
    void trainSlot();
    void recallTrainSlot();
    void recallContSlot();
    void trainContSlot();
    void stopSlot();
    void checkSlot();
    void resetSlot();
    
    void disconnectSlot();
    
protected:
    void initializeGL();
    void paintGL();
    void resizeGL( int w, int h );
    
    void mousePressEvent( QMouseEvent* );
    void mouseReleaseEvent( QMouseEvent* );
    void mouseMoveEvent( QMouseEvent* );
    void wheelEvent( QWheelEvent* );
    
protected slots:
    void languageChange();
    
private:
    NewObject*      newObjectCreator;
    ObjectBrowser*  objectBrowser;
    Navigator*      navigator;
    
    // clipboard objects
    nnet::ImagePtr  nodeAttributes; // x y bpp colour
    nnet::UintPtr   edgeAttributes; // inputs
    
    bool            continueExecution;
    int             executionInterval;
    
    bool            leftPressed;
    bool            rightPressed;

    float           mouseWheelFactor;
};


#endif

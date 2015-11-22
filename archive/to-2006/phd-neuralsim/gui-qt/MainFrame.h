#ifndef MAINFRAME_H
#define MAINFRAME_H


#include <qvariant.h>
#include <qpixmap.h>
#include <qmainwindow.h>
#include <qstring.h>


class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QAction;
class QActionGroup;
class QToolBar;
class QPopupMenu;
class NavFrame;
class Console;


class MainFrame : public QMainWindow 
{
    Q_OBJECT
    
    /// Object methods
public:
    MainFrame(  QWidget*    parent = 0, 
                const char* name = 0, 
                WFlags      fl = WType_TopLevel );
    ~MainFrame();
    
    Console* console;
    NavFrame* navFrame;
    
    QMenuBar* MenuBar;
    QPopupMenu* fileMenu;
    QPopupMenu* editMenu;
    QPopupMenu* Simulation;
    QPopupMenu* helpMenu;
    QAction* fileNewAction;
    QAction* fileOpenAction;
    QAction* fileSaveAction;
    QAction* fileSaveAsAction;
    QAction* fileExitAction;
    QAction* editCutAction;
    QAction* editCopyAction;
    QAction* editPasteAction;
    QAction* helpContentsAction;
    QAction* helpAboutAction;
    QAction* simulationVerifyAction;
    QAction* simulationResetAction;
    QAction* simulationRecallAction;
    QAction* simulationTrainAction;
    QAction* simulationRecallContinuousAction;
    QAction* simulationTrainContinuousAction;
    QAction* simulationRecallTrainAction;
    QAction* simulationStopAction;
    QAction* consoleShowAction;
    
public slots:
    void fileNew();
    void fileOpen();
    void fileSave();
    void fileSaveAs();
    void fileExit();
    void editCut();
    void editCopy();
    void editPaste();
    void helpContents();
    void helpAbout();
    
    void startExecutingSlot();
    void stopExecutingSlot();
    
protected:
    QGridLayout* MainFrameLayout;
    virtual void closeEvent( QCloseEvent *e );
    
protected slots:
    void languageChange();
    
private:
    QString projectPath;
};


#endif // MAINFRAME_H

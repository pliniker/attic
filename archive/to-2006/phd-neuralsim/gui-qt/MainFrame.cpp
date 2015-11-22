

#include <boost/signal.hpp>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qvariant.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qaction.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qtoolbar.h>
#include <qimage.h>
#include <qpixmap.h>

#include <Brainlib.h>
#include <PluginManager.h>
#include <Serializable.h>

#include "MainFrame.h"
#include "Console.h"
#include "NavFrame.h"

#include "../config.h"



MainFrame::MainFrame(   QWidget*    parent, 
                        const char* name, 
                        WFlags      fl ) :
    QMainWindow( parent, name, fl ) 
{
    // BrainLib startup
    nnet::initialize();
    
    if( !name )
    {
        setName( "MainFrame" );
    }

    setIcon( QPixmap::fromMimeSource( "neuron-scaled.png" ) );
    
    setCentralWidget( new QWidget( this, "qt_central_widget" ) );
    MainFrameLayout = new QGridLayout( centralWidget(), 1, 1, 0, 0, "MainFrameLayout" );
    
    navFrame = new NavFrame( centralWidget(), "navFrame" );
    
    console = new Console( navFrame, "Console" );
    console->show();
    nnet::error::set_error_interface( console );

    // load plugins from default installation path
    std::string default_plugin_path = CONFIG_PREFIX;
    default_plugin_path += "/plugins/";
    nnet::PluginManager::LoadFromDir( default_plugin_path );
    
    MainFrameLayout->addWidget( navFrame, 0, 0 );
    
    // actions
    fileNewAction = new QAction( this, "fileNewAction" );
    fileNewAction->setIconSet( QIconSet( QPixmap::fromMimeSource( "filenew" ) ) );
    fileOpenAction = new QAction( this, "fileOpenAction" );
    fileOpenAction->setIconSet( QIconSet( QPixmap::fromMimeSource( "fileopen" ) ) );
    fileSaveAction = new QAction( this, "fileSaveAction" );
    fileSaveAction->setIconSet( QIconSet( QPixmap::fromMimeSource( "filesave" ) ) );
    fileSaveAsAction = new QAction( this, "fileSaveAsAction" );
    fileExitAction = new QAction( this, "fileExitAction" );
    editCutAction = new QAction( this, "editCutAction" );
    editCutAction->setIconSet( QIconSet( QPixmap::fromMimeSource( "editcut" ) ) );
    editCopyAction = new QAction( this, "editCopyAction" );
    editCopyAction->setIconSet( QIconSet( QPixmap::fromMimeSource( "editcopy" ) ) );
    editPasteAction = new QAction( this, "editPasteAction" );
    editPasteAction->setIconSet( QIconSet( QPixmap::fromMimeSource( "editpaste" ) ) );
    helpContentsAction = new QAction( this, "helpContentsAction" );
    helpAboutAction = new QAction( this, "helpAboutAction" );
    simulationVerifyAction = new QAction( this, "simulationVerifyAction" );
    simulationResetAction = new QAction( this, "simulationResetAction" );
    simulationRecallAction = new QAction( this, "simulationRecallAction" );
    simulationTrainAction = new QAction( this, "simulationTrainAction" );
    simulationRecallTrainAction = new QAction( this, "simulationRecallTrainAction" );
    simulationRecallContinuousAction = new QAction( this, "simulationRecallContinuousAction" );
    simulationTrainContinuousAction = new QAction( this, "simulationTrainContinuousAction" );
    simulationStopAction = new QAction( this, "simulationStopAction" );
    consoleShowAction = new QAction( this, "consoleShowAction" );
    
    // menubar
    MenuBar = new QMenuBar( this, "MenuBar" );
    
    fileMenu = new QPopupMenu( this );
    fileNewAction->addTo( fileMenu );
    fileOpenAction->addTo( fileMenu );
    fileSaveAction->addTo( fileMenu );
    fileSaveAsAction->addTo( fileMenu );
    fileMenu->insertSeparator();
    fileExitAction->addTo( fileMenu );
    MenuBar->insertItem( QString( "" ), fileMenu, 1 );
    
    editMenu = new QPopupMenu( this );
    editCutAction->addTo( editMenu );
    editCopyAction->addTo( editMenu );
    editPasteAction->addTo( editMenu );
    MenuBar->insertItem( QString( "" ), editMenu, 2 );
    
    Simulation = new QPopupMenu( this );
    simulationVerifyAction->addTo( Simulation );
    simulationResetAction->addTo( Simulation );
    consoleShowAction->addTo( Simulation );
    Simulation->insertSeparator();
    simulationRecallAction->addTo( Simulation );
    simulationTrainAction->addTo( Simulation );
    simulationRecallTrainAction->addTo( Simulation );
    Simulation->insertSeparator();
    simulationRecallContinuousAction->addTo( Simulation );
    simulationTrainContinuousAction->addTo( Simulation );
    Simulation->insertSeparator();
    simulationStopAction->addTo( Simulation );
    MenuBar->insertItem( QString( "" ), Simulation, 3 );
    
    helpMenu = new QPopupMenu( this );
    helpContentsAction->addTo( helpMenu );
    helpMenu->insertSeparator();
    helpAboutAction->addTo( helpMenu );
    MenuBar->insertItem( QString( "" ), helpMenu, 4 );
    
    languageChange();
    resize( QSize( 640, 480 ).expandedTo( minimumSizeHint() ) );
    clearWState( WState_Polished );
    
    // signals and slots connections
    connect( fileNewAction, SIGNAL( activated() ), this, SLOT( fileNew() ) );
    connect( fileOpenAction, SIGNAL( activated() ), this, SLOT( fileOpen() ) );
    connect( fileSaveAction, SIGNAL( activated() ), this, SLOT( fileSave() ) );
    connect( fileSaveAsAction, SIGNAL( activated() ), this, SLOT( fileSaveAs() ) );
    connect( fileExitAction, SIGNAL( activated() ), this, SLOT( fileExit() ) );
    connect( editCutAction, SIGNAL( activated() ), this, SLOT( editCut() ) );
    connect( editCopyAction, SIGNAL( activated() ), this, SLOT( editCopy() ) );
    connect( editPasteAction, SIGNAL( activated() ), this, SLOT( editPaste() ) );
    connect( helpContentsAction, SIGNAL( activated() ), this, SLOT( helpContents() ) );
    connect( helpAboutAction, SIGNAL( activated() ), this, SLOT( helpAbout() ) );
    connect( simulationVerifyAction, SIGNAL( activated() ), navFrame, SLOT( checkSlot() ) );
    connect( simulationResetAction, SIGNAL( activated() ), navFrame, SLOT( resetSlot() ) );
    connect( simulationRecallAction, SIGNAL( activated() ), navFrame, SLOT( recallSlot() ) );
    connect( simulationRecallContinuousAction, SIGNAL( activated() ), navFrame, SLOT( recallContSlot() ) );
    connect( simulationTrainAction, SIGNAL( activated() ), navFrame, SLOT( trainSlot() ) );
    connect( simulationTrainContinuousAction, SIGNAL( activated() ), navFrame, SLOT( trainContSlot() ) );
    connect( simulationStopAction, SIGNAL( activated() ), navFrame, SLOT( stopSlot() ) );
    
    connect( simulationRecallContinuousAction, SIGNAL( activated() ), this, SLOT( startExecutingSlot() ) );
    connect( simulationTrainContinuousAction, SIGNAL( activated() ), this, SLOT( startExecutingSlot() ) );
    connect( simulationStopAction, SIGNAL( activated() ), this, SLOT( stopExecutingSlot() ) );

    connect( simulationRecallTrainAction, SIGNAL( activated() ), navFrame, SLOT( recallTrainSlot() ) );
    
    connect( consoleShowAction, SIGNAL( activated() ), console, SLOT( show() ) );
    
    stopExecutingSlot();
    
    // file management
    projectPath = QString::null;
}


MainFrame::~MainFrame()
{
    // no need to delete child widgets, Qt does it all for us
    nnet::cleanup();
    
    delete console;
}


void MainFrame::fileNew() 
{
    nnet::file_new();
    projectPath = QString::null;
    navFrame->reset();
}


void MainFrame::fileOpen() 
{
    QString path = QFileDialog::getOpenFileName( 
                                    projectPath, 
                                    "brain (*.xml.brain)", 
                                    this,
                                    "Open Project", 
                                    "Choose a project file" );
                   
    if( path != QString::null ) 
    {
        std::string p = nnet::file_open( path.latin1() );
        if( p == nnet::string::null )
        {
            QMessageBox::warning( 
                            this, 
                            "Opening File...", 
                            "Could not open project" );
        }
        else
        {
            projectPath = p.c_str();
        }
    }
}


void MainFrame::fileSave()
{
    if( projectPath == QString::null ) 
    {
        fileSaveAs();
    } 
    else 
    {
        std::string p = nnet::file_save( projectPath.latin1() );
        if( p == nnet::string::null )
        {
            QMessageBox::warning(   this, 
                                    "Saving File...", 
                                    "Could not save project" );
            projectPath = QString::null;
        } 
        else 
        {
            projectPath = p.c_str();
        }
    }
}


void MainFrame::fileSaveAs() 
{
    QString path = QFileDialog::getSaveFileName( 
                                    projectPath, 
                                    "brain (*.xml.brain)", 
                                    this,
                                    "Save Project", 
                                    "Choose a project name" );
                   
    if( path != QString::null ) 
    {
        std::string p = nnet::file_save( path.latin1() );
        if( p == nnet::string::null )
        {
            QMessageBox::warning(   this, 
                                    "Saving File...", 
                                    "Could not save project" );
            projectPath = QString::null;
        } 
        else 
        {
            projectPath = p.c_str();
        }
    }
}


void MainFrame::fileExit() 
{
    close();
}


void MainFrame::closeEvent( QCloseEvent *e ) 
{
    int status = QMessageBox::information(  this, 
                                            "Brain Builder",
                                            "Save before closing?", 
                                            "Yes", "No", 
                                            "Cancel", 
                                            0, 
                                            1 );
    switch( status ) 
    {
        case 0:
        {
            fileSave();
            e->accept();
            break;
        }
        case 1:
        {
            e->accept();
            break;
        }
        case 2:
        default:
        {
            e->ignore();
            break;
        }
    }
}


void MainFrame::editCut() 
{
    qWarning( "MainFrame::editCut unimplemented" );
}
void MainFrame::editCopy() 
{
    qWarning( "MainFrame::editCopy unimplemented" );
}
void MainFrame::editPaste() 
{
    qWarning( "MainFrame::editPaste unimplemented" );
}
void MainFrame::helpContents() 
{
    qWarning( "MainFrame::helpContents unimplemented" );
}


void MainFrame::helpAbout() 
{
    QMessageBox::about( this, 
                        "Brain Builder",
                        "Brain Builder is a digital neuromodelling visual design tool"
                        "based on the ideas in NRM\n\n"
                        "Built using Trolltech Qt, Kdevelop and Xcode\n\n"
                        "By Peter Liniker and Sunil Rao" );
}


void MainFrame::startExecutingSlot() 
{
    simulationVerifyAction->setEnabled( FALSE );
    simulationRecallAction->setEnabled( FALSE );
    simulationTrainAction->setEnabled( FALSE );
    simulationRecallContinuousAction->setEnabled( FALSE );
    simulationTrainContinuousAction->setEnabled( FALSE );
    simulationStopAction->setEnabled( TRUE );
    
    navFrame->newNodeAction->setEnabled( FALSE );
    navFrame->newEdgeAction->setEnabled( FALSE );
    navFrame->deleteObjectAction->setEnabled( FALSE );
    navFrame->recallObjectAction->setEnabled( FALSE );
    navFrame->trainObjectAction->setEnabled( FALSE );
    navFrame->checkObjectAction->setEnabled( FALSE );
    navFrame->browseObjectAction->setEnabled( FALSE );
    navFrame->disconnectObjectAction->setEnabled( FALSE );
}


void MainFrame::stopExecutingSlot() 
{
    simulationVerifyAction->setEnabled( TRUE );
    simulationRecallAction->setEnabled( TRUE );
    simulationTrainAction->setEnabled( TRUE );
    simulationRecallContinuousAction->setEnabled( TRUE );
    simulationTrainContinuousAction->setEnabled( TRUE );
    simulationStopAction->setEnabled( FALSE );
    
    navFrame->newNodeAction->setEnabled( TRUE );
    navFrame->newEdgeAction->setEnabled( TRUE );
    navFrame->deleteObjectAction->setEnabled( TRUE );
    navFrame->recallObjectAction->setEnabled( TRUE );
    navFrame->trainObjectAction->setEnabled( TRUE );
    navFrame->checkObjectAction->setEnabled( TRUE );
    navFrame->browseObjectAction->setEnabled( TRUE );
    navFrame->disconnectObjectAction->setEnabled( TRUE );
}


void MainFrame::languageChange() 
{
    setCaption( tr( "Brain Builder" ) );
    fileNewAction->setText( tr( "New" ) );
    fileNewAction->setMenuText( tr( "&New" ) );
    fileNewAction->setAccel( tr( "Ctrl+Shift+N" ) );
    fileOpenAction->setText( tr( "Open" ) );
    fileOpenAction->setMenuText( tr( "&Open..." ) );
    fileOpenAction->setAccel( tr( "Ctrl+O" ) );
    fileSaveAction->setText( tr( "Save" ) );
    fileSaveAction->setMenuText( tr( "&Save" ) );
    fileSaveAction->setAccel( tr( "Ctrl+S" ) );
    fileSaveAsAction->setText( tr( "Save As" ) );
    fileSaveAsAction->setMenuText( tr( "S&ave As" ) );
    fileSaveAsAction->setAccel( tr( "Ctrol+Shift+S" ) );
    fileExitAction->setText( tr( "Exit" ) );
    fileExitAction->setMenuText( tr( "E&xit" ) );
    fileExitAction->setAccel( QString::null );
    editCutAction->setText( tr( "Cut" ) );
    editCutAction->setMenuText( tr( "Cu&t" ) );
    editCutAction->setAccel( tr( "Ctrl+X" ) );
    editCopyAction->setText( tr( "Copy" ) );
    editCopyAction->setMenuText( tr( "&Copy" ) );
    editCopyAction->setAccel( tr( "Ctrl+C" ) );
    editPasteAction->setText( tr( "Paste" ) );
    editPasteAction->setMenuText( tr( "&Paste" ) );
    editPasteAction->setAccel( tr( "Ctrl+V" ) );
    helpContentsAction->setText( tr( "Contents" ) );
    helpContentsAction->setMenuText( tr( "&Contents..." ) );
    helpContentsAction->setAccel( QString::null );
    helpAboutAction->setText( tr( "About" ) );
    helpAboutAction->setMenuText( tr( "&About" ) );
    helpAboutAction->setAccel( QString::null );
    simulationRecallAction->setText( tr( "Recall" ) );
    simulationRecallAction->setMenuText( tr( "Recall" ) );
    simulationRecallAction->setAccel( tr( "Ctrl+R" ) );
    simulationTrainAction->setText( tr( "Train" ) );
    simulationTrainAction->setMenuText( tr( "Train" ) );
    simulationTrainAction->setAccel( tr( "Ctrl+T" ) );
    simulationRecallTrainAction->setText( tr( "Recall + Train" ) );
    simulationRecallTrainAction->setMenuText( tr( "Recall + Train" ) );
    simulationRecallTrainAction->setAccel( tr( "Ctrl+D" ) );
    simulationRecallContinuousAction->setText( tr( "Recall Continuous" ) );
    simulationRecallContinuousAction->setMenuText( tr( "Recall Continuous" ) );
    simulationRecallContinuousAction->setAccel( tr( "Ctrl+Shift+R" ) );
    simulationTrainContinuousAction->setText( tr( "Train Continuous" ) );
    simulationTrainContinuousAction->setMenuText( tr( "Train Continuous" ) );
    simulationTrainContinuousAction->setAccel( tr( "Ctrl+Shift+T" ) );
    simulationStopAction->setText( tr( "Stop" ) );
    simulationStopAction->setMenuText( tr( "Stop" ) );
    simulationStopAction->setAccel( tr( "Esc" ) );
    simulationVerifyAction->setText( tr( "Verify" ) );
    simulationVerifyAction->setMenuText( tr( "Verify" ) );
    simulationVerifyAction->setAccel( tr( "Ctrl+G" ) );
    simulationResetAction->setText( tr( "Reset Network" ) );
    simulationResetAction->setMenuText( tr( "Reset Network" ) );
    consoleShowAction->setText( tr( "Show Messages" ) );
    consoleShowAction->setMenuText( tr( "Show Messages" ) );
    consoleShowAction->setAccel( tr( "Ctrl+M" ) );
    if( MenuBar->findItem( 1 ) )
    {
        MenuBar->findItem( 1 ) ->setText( tr( "&File" ) );
    }
    if( MenuBar->findItem( 2 ) )
    {
        MenuBar->findItem( 2 ) ->setText( tr( "&Edit" ) );
    }
    if( MenuBar->findItem( 3 ) )
    {
        MenuBar->findItem( 3 ) ->setText( tr( "Simulation" ) );
    }
    if( MenuBar->findItem( 4 ) )
    {
        MenuBar->findItem( 4 ) ->setText( tr( "&Help" ) );
    }
}


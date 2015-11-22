

#include <iostream>
#include <qapplication.h>
#include "MainFrame.h"


int main( int argc, char ** argv ) {

    QApplication a( argc, argv );
    MainFrame w;
    w.show();
    a.setMainWidget( &w );
    // a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    
    try {
        return a.exec();
    } catch ( ... ) {
        std::cerr << "Some unspecified exception occurred" << std::endl;
    }
}


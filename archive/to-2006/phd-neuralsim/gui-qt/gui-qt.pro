LANGUAGE = C++
TEMPLATE = app 
DESTDIR = $${PREFIX}/bin/ 
RC_FILE = application.icns

TARGET = depictiveLinugistic

CONFIG += warn_on qt opengl thread exceptions stl rtti 

LIBS += -L$${PREFIX}/lib/ -lbrainlib -lnavigator -lxml2 -lz -lpthread -lm -lboost_signals
INCLUDEPATH += ../navigator ../brainlib /usr/include/libxml2

macx:LIBS += -L/opt/local/lib/
macx:INCLUDEPATH += /opt/local/include/
macx:QMAKE_CXXFLAGS_RELEASE += -Wno-long-double
macx:QMAKE_CXXFLAGS_DEBUG   += -Wno-long-double
              
SOURCES += NavFrame.cpp \
           MainFrame.cpp \
           NewObject.cpp \
           ObjectBrowser.cpp \
           ObjectView.cpp \
           BooleanView.cpp \
           CStringView.cpp \
           IntegerView.cpp \
           FloatingView.cpp \
           OptionListView.cpp \
           ImageView.cpp \
           Vector3dView.cpp \
           Console.cpp \
           FileChooserView.cpp \
           main.cpp \
           ObjectMakerView.cpp
           
HEADERS += NavFrame.h \
           MainFrame.h \
           NewObject.h \
           ObjectBrowser.h \
           ObjectView.h \
           BooleanView.h \
           CStringView.h \
           IntegerView.h \
           FloatingView.h \
           OptionListView.h \
           ImageView.h \
           Vector3dView.h \
           FileChooserView.h \
           Console.h \
           ObjectMakerView.h
           
IMAGES += images/filenew \
          images/fileopen \
          images/filesave \
          images/print \
          images/undo \
          images/redo \
          images/editcut \
          images/editcopy \
          images/editpaste \
          images/searchfind \
          images/neuron-scaled.png 

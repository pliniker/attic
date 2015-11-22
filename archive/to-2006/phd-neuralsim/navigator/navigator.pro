TEMPLATE = lib
LANGUAGE = C++
DESTDIR = $${PREFIX}/lib/

CONFIG -= qt
CONFIG += dll warn_on opengl stl exceptions rtti 

LIBS += -L$${PREFIX}/lib -lbrainlib -lxml2 -lz -lpthread -lm -lboost_signals
INCLUDEPATH += . ../brainlib/ /usr/include/libxml2/

macx:LIBS += -L/opt/local/lib/
macx:INCLUDEPATH += /opt/local/include/
macx:QMAKE_CXXFLAGS_RELEASE += -Wno-long-double
macx:QMAKE_CXXFLAGS_DEBUG   += -Wno-long-double

SOURCES += GLVector.cpp \
           ViewPoint.cpp \
           NodeView.cpp \
           EdgeView.cpp \
           RenderObject.cpp \
           ControlPoint.cpp \
           Navigator.cpp \
           NavErr.cpp

HEADERS += OpenGL.h \
           GLVector.h \
           ViewPoint.h \
           NodeView.h \
           EdgeView.h \
           RenderObject.h \
           ControlPoint.h \
           Navigator.h \
           NavErr.h


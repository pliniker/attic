
TEMPLATE = lib
LANGUAGE = C++
DESTDIR  = $${PREFIX}/plugins/

CONFIG += dll qt warn_on stl exceptions rtti 

LIBS += -L$${PREFIX}/lib -L$${PREFIX}/plugins -lbrainlib -lnavigator -lxml2 -lz -lpthread -lm -lboost_signals -lBaseTypes
INCLUDEPATH += . ../ ../../brainlib/ ../../navigator/ /usr/include/libxml2/ ../BaseTypes/

macx:LIBS += -L/opt/local/lib/
macx:INCLUDEPATH += /opt/local/include/
macx:QMAKE_CXXFLAGS_RELEASE += -Wno-long-double
macx:QMAKE_CXXFLAGS_DEBUG   += -Wno-long-double

HEADERS +=  TextIO.h \
            TextIODialog.h 

SOURCES +=  TextIO.cpp \
            TextIODialog.cpp


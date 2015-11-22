
TEMPLATE = lib
LANGUAGE = C++
DESTDIR  = $${PREFIX}/plugins/

CONFIG -= qt
CONFIG += dll warn_on stl exceptions rtti 

LIBS += -L$${PREFIX}/lib -L$${PREFIX}/plugins -lbrainlib -lnavigator -lxml2 -lz -lpthread -lm -lboost_signals -lBaseTypes -lpython2.5 
INCLUDEPATH += . ../ ../../../brainlib/ ../../../navigator/ /usr/include/libxml2/ /usr/include/python2.5/

macx:LIBS -= -lpython2.4
macx:LIBS += -L/opt/local/lib/ -framework Python 
macx:INCLUDEPATH += /opt/local/include/ /System/Library/Frameworks/Python.framework/Versions/2.4/include/python2.4
macx:QMAKE_CXXFLAGS_RELEASE += -Wno-long-double
macx:QMAKE_CXXFLAGS_DEBUG   += -Wno-long-double

SOURCES	+= PythonScript.cpp


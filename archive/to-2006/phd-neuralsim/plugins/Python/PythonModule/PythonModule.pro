
TEMPLATE = lib
LANGUAGE = C++
DESTDIR  = $${PREFIX}/python/
TARGET = nnet
TARGET_EXT = so

CONFIG += qt dll warn_on stl exceptions rtti 

LIBS += -L$${PREFIX}/lib -L$${PREFIX}/plugins -L$${PREFIX}/python -lbrainlib -lnavigator -lxml2 -lz -lpthread -lm -lboost_signals -lboost_python -lpython2.5 -lTextIO
INCLUDEPATH += . ../ ../../../brainlib/ ../../../navigator/ ../../TextIO/ /usr/include/libxml2/ /usr/include/python2.5/

macx:LIBS -= -lpython2.4
macx:LIBS += -L/opt/local/lib/ -framework Python
macx:INCLUDEPATH += /opt/local/include/ /System/Library/Frameworks/Python.framework/Versions/2.4/include/python2.4
macx:QMAKE_CXXFLAGS_RELEASE += -Wno-long-double
macx:QMAKE_CXXFLAGS_DEBUG   += -Wno-long-double

SOURCES	+= PythonModule.cpp

macx:system( ln -sf $${PREFIX}/python/libnnet.dylib $${PREFIX}/python/nnet.so )

unix:system( ln -sf $${PREFIX}/python/libnnet.so $${PREFIX}/python/nnet.so )


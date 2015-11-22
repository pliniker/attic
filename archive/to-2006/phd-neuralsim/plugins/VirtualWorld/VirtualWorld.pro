
TEMPLATE = lib
LANGUAGE = C++
DESTDIR  = $${PREFIX}/plugins/

CONFIG -= qt
CONFIG += dll warn_on stl exceptions rtti 

LIBS += -L$${PREFIX}/lib -L$${PREFIX}/plugins -lbrainlib -lnavigator -lxml2 -lz -lpthread -lm -lboost_signals
INCLUDEPATH += . ../ ../../brainlib/ ../../navigator/ /usr/include/libxml2/

macx:LIBS += -L/opt/local/lib/
macx:INCLUDEPATH += /opt/local/include/
macx:QMAKE_CXXFLAGS_RELEASE += -Wno-long-double
macx:QMAKE_CXXFLAGS_DEBUG   += -Wno-long-double

HEADERS += VirtualWorld.h \
	Fovea.h \
	JReferentScale.h \
	MotorScale.h \
	JReferentNRMStyle.h \
	MotorNRMStyle.h

SOURCES	+= VirtualWorld.cpp \
	Fovea.cpp \
	JReferentScale.cpp \
	MotorScale.cpp \
	JReferentNRMStyle.cpp \
	MotorNRMStyle.cpp


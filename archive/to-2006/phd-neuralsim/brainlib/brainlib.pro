
TEMPLATE = lib
LANGUAGE = C++
DESTDIR  = $${PREFIX}/lib/

CONFIG -= qt
CONFIG += dll warn_on stl exceptions rtti

LIBS += -lxml2 -lz -lpthread -lm -lboost_signals
INCLUDEPATH += . /usr/include/libxml2/

macx:LIBS += -L/opt/local/lib/
macx:INCLUDEPATH += /opt/local/include/
macx:QMAKE_CXXFLAGS_RELEASE += -Wno-long-double
macx:QMAKE_CXXFLAGS_DEBUG   += -Wno-long-double

HEADERS	+= Brainlib.h \
	BinaryFile.h \
	XmlFile.h \
	Serializable.h \
	Boolean.h \
	CString.h \
	Integer.h \
	Floating.h \
	Array.h \
	Image.h \
	OptionList.h \
	ObjectList.h \
	ObjectMaker.h \
	Vector3d.h \
	Map.h \
	Layer.h \
	NodeLayer.h \
	EdgeLayer.h \
	NeuronArray.h \
	SynapseArray.h \
	ExecEngine.h \
	StringList.h \
	Operations.h \
	Common.h \
	RandomInt.h \
	ErrorInterface.h \
	DLLManager.h \
	PluginManager.h \
	BadObject.h \
	Slot.h \
	RefCountPtr.h 

SOURCES	+= Brainlib.cpp \
	BinaryFile.cpp \
	XmlFile.cpp \
	Serializable.cpp \
	Boolean.cpp \
	CString.cpp \
	Integer.cpp \
	Floating.cpp \
	Array.cpp \
	Image.cpp \
	OptionList.cpp \
	ObjectList.cpp \
	ObjectMaker.cpp \
	Vector3d.cpp \
	Map.cpp \
	Layer.cpp \
	NodeLayer.cpp \
	EdgeLayer.cpp \
	NeuronArray.cpp \
	SynapseArray.cpp \
	ExecEngine.cpp \
	StringList.cpp \
	Operations.cpp \
	Common.cpp \
	RandomInt.cpp \
	ErrorInterface.cpp \
	DLLManager.cpp \
	PluginManager.cpp \
    Slot.cpp \
    BadObject.cpp


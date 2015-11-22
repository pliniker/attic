
TEMPLATE = subdirs

SUBDIRS = test \
	BaseTypes \
	SynapticMap \
	RandomInputLayer \
	ImageInputLayer \
	TestNeuralLayer \
	StdNeuralLayer \
	NullOutputLayer \
	StdSynapticLayer \
    HebbianSynapticLayer \
	LearnSynapticLayer \
	StdSynapse \
	InverseSynapse \
    SlowSynapse \
	FVGRAM \
	Python \
#	VirtualWorld  \
	TextIO 
#	VW2 \

# the following do not build correctly
# and/or plain don't work
#	DigitalSOM \
#	VGRAM \

#ifndef PLUGIN_SYNAPTICMAP_H
#define PLUGIN_SYNAPTICMAP_H


#include <Array.h>
#include <Map.h>



class SynapticMap : public nnet::Map
{
	/// Object methods
public:
	SynapticMap( std::string name );
	
	virtual void read( xmlNodePtr from );
	
	// create a mapping of synapses from one neural layer to the next
	// from layer xIn by yIn to layer xOut by yOut with spn synapses per neuron
	// return actual synapses per neuron if modified
	virtual int makeMap( int xIn, int yIn, int spn, int xOut, int yOut ) = 0;

	/// Object public data
public:
	nnet::IntArrayPtr connectionMap;
};


typedef nnet::RefCountPtr<SynapticMap> SynapticMapPtr;


#endif

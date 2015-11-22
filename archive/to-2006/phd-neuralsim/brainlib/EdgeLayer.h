#ifndef BRAINLIB_EDGELAYER_H
#define BRAINLIB_EDGELAYER_H


#include "Layer.h"


namespace nnet {
	
    
    class EdgeLayer;
    typedef RefCountPtr<EdgeLayer> EdgeLayerPtr;

	
	class EdgeLayer : public Layer 
    {
		/// Object methods
	public:
		EdgeLayer(                      std::string name );
		virtual void read(              xmlNodePtr node );
		
		virtual bool check();
        // fetch a set of synapse values for a neuron and return
        // a value for post-neuron-processing
        virtual void recall(            size_t  neuron,
                                        Unit*   synapticOutputs,
                                        Unit*   axonModulation ) = 0;
        // feed the neuron output back into the set of synapses
        virtual void learn(             size_t  neuron,
                                        Unit*   synapticOutputs,
                                        Unit    previousNeuronOutput ) = 0;

        // attach functions that require NodeLayers or fail
		virtual bool attachTo(          LayerPtr object );
		virtual bool attachFrom(        LayerPtr object );
    };
		
    
	EdgeLayerPtr    new_edge(           std::string type );
	void            erase_edge(         EdgeLayerPtr layer );
	
	
	
}


#endif

#ifndef BRAINLIB_NODELAYER_H
#define BRAINLIB_NODELAYER_H


#include "Layer.h"
#include "EdgeLayer.h"


namespace nnet {

    class NodeLayer;
    typedef RefCountPtr<NodeLayer> NodeLayerPtr;

    
    // Boolean "isAsync"
	
	class NodeLayer : public Layer 
    {
		/// Object methods
	public:
		NodeLayer(                      std::string name );
        
        /// Inherited methods
    public:
		virtual void read(              xmlNodePtr from );
		
		virtual bool check();
        virtual void recall(            size_t neuron );
        virtual void learn(             size_t neuron );

        // attach functions that require EdgeLayers or fail
		virtual bool attachTo(          LayerPtr object );
		virtual bool attachFrom(        LayerPtr object );
        
        /// Exec-time fast methods
    public:
        bool asyncRecallOrder();
        
    protected:
        void buildInputList(            size_t  neuron,
                                        Unit**  outputs,
                                        Unit*   outputMod );
        
        void trainInputs(               size_t  neuron,
                                        Unit**  outputs,
                                        Unit    output );

        /// Exec-time fast data
    private:
        std::vector<Unit>               m_inputVector;
    };
		

    /// Global methods
    void            recall(             NodeLayerPtr    layer );
    void            recall(             NodeLayerPtr    layer, size_t neuron );

    void            learn(              NodeLayerPtr    layer );
    void            learn(              NodeLayerPtr    layer, size_t neuron );

    void            blankState(         NodeLayerPtr    layer );
    
    void            randomizeState(     NodeLayerPtr    layer );

    void            randomizeState(     NodeLayerPtr    layer,
                                        int             percent );
		
    NodeLayerPtr    new_node(           std::string     type );
    void            erase_node(         NodeLayerPtr    layer );
	
}


#endif


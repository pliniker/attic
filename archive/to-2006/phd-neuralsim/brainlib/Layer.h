#ifndef BRAINLIB_LAYER_H
#define BRAINLIB_LAYER_H


#include <boost/signals.hpp>
#include "Integer.h"
#include "CString.h"
#include "Boolean.h"
#include "Image.h"
#include "Map.h"


namespace nnet {


    class Layer;
    typedef RefCountPtr<Layer> LayerPtr;

    
	// CString   "displayName"
    // CString   "group"
    // Int       "inputs"
    // Map       "inputList"
    // Map       "outputList"
    // Boolean   "ready"
    // Boolean   "enabled"
    // Image     "state"
    
	class Layer : public Map 
    {
		/// Object methods
	public:
		Layer( std::string name );
		virtual ~Layer();
        
        /// Inherited methods
    public:
		virtual void read(              xmlNodePtr node );
		
		virtual bool check();
		virtual void reset();
		
		virtual void detachAll();

		virtual bool attachTo(          LayerPtr object );
		virtual void detachTo(          LayerPtr object );
		virtual bool attachFrom(        LayerPtr object );
		virtual void detachFrom(        LayerPtr object );
            
        /// Exec-time fast methods
    public:
        size_t numInputs();
        size_t numNeurons();
        
        /// Exec-time fast data
    protected:
        size_t                          m_inputs;
        size_t                          m_neurons;
        Unit*                           m_state;
        Unit*                           m_trainState;
        
		/// Object functions for sub-layers only
	protected:
		void	  layerMessage(         std::string message );
    };	
	
    
    /// Global methods
    bool check(   LayerPtr layer );
	void reset(   LayerPtr layer );
	bool attach(  LayerPtr from, LayerPtr to );
	bool detach(  LayerPtr from, LayerPtr to );
	
	
}


#endif

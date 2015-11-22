#ifndef BRAINLIB_NEURONARRAY_H
#define BRAINLIB_NEURONARRAY_H


#include "Common.h"
#include "CString.h"
#include "Integer.h"
#include "Map.h"


namespace nnet {
	
	
	class NeuronArray : public Map 
    {
		/// Object methods
	public:
		NeuronArray( std::string name );
		
		virtual void read( xmlNodePtr from );
		
		virtual void create( size_t array_size, size_t synapses );
		virtual bool check();
		
        virtual Unit recall( size_t neuron, Unit* pattern );
			
        virtual void learn( size_t neuron, Unit* pattern, Unit response );
		
        virtual void reset();

        /// Fast exec-time data
    protected:
        size_t  m_inputs;
        size_t  m_size;
	};
	
	
	typedef RefCountPtr<NeuronArray> NeuronArrayPtr;
	
}


#endif

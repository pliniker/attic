#ifndef BRAINLIB_SYNAPSEARRAY_H
#define BRAINLIB_SYNAPSEARRAY_H


#include "Common.h"
#include "Integer.h"
#include "CString.h"
#include "Map.h"


namespace nnet {

    
    //  IntP "size"
	
	class SynapseArray : public Map 
    {
		/// Object methods
	public:
		SynapseArray( std::string name );
		
		virtual void read( xmlNodePtr from );

        virtual void setDistance( size_t synapse, int value );

		virtual void create( int array_size );
		virtual bool check();
		
		virtual void recall(            size_t    synapse, 
                                        Unit      input,
                                        Unit*     output,
                                        Unit*     modulator );
		
        virtual void learn(             size_t    synapse, 
                                        Unit      input, 
                                        Unit*     myOutput,
                                        Unit      output );
		
		virtual void reset();
		
		/// Object data
	protected:
		size_t m_size;
	};
	
	
	typedef RefCountPtr<SynapseArray> SynapseArrayPtr;
	
}


#endif

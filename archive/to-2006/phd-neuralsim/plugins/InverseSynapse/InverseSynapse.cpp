

#include <Brainlib.h>
#include <SynapseArray.h>
#include <Common.h>



class InverseSynapse : public nnet::SynapseArray
{
public:
    InverseSynapse( std::string name ) : 
	    nnet::SynapseArray(name) 
    {
    }


    std::string type() 
    { 
        return "InverseSynapse"; 
    }

	
    virtual void recall(                size_t      /*synapse*/, 
                                        nnet::Unit  input,
                                        nnet::Unit* output,
                                        nnet::Unit* /*modulator*/ )
    {
        *output = ~input;
    }
    
        
    virtual void learn(                 size_t      /*synapse*/, 
                                        nnet::Unit  input, 
                                        nnet::Unit* myOutput,
                                        nnet::Unit  /*output*/ )
    {
        *myOutput = input;
    }


	~InverseSynapse() {}
	

public:
	static nnet::SerializablePtr Create( std::string name )
    {
		return nnet::RefCountPtr<InverseSynapse>( new InverseSynapse( name ) );
	}
};


extern "C" void Initialize()
{
	nnet::Serializable::AddFactory( "InverseSynapse", InverseSynapse::Create );
	nnet::string::get_list( "synapse_arrays" )->add( "InverseSynapse" );
}


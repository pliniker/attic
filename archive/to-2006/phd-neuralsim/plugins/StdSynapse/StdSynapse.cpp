

#include <Brainlib.h>
#include <SynapseArray.h>
#include <Common.h>



class StdSynapse : public nnet::SynapseArray
{
public:
    StdSynapse( std::string name ) :
        nnet::SynapseArray( name ) 
    {
    }


	std::string type() 
    { 
        return "StdSynapse"; 
    }


    virtual void recall(                size_t      /*synapse*/, 
                                        nnet::Unit  input,
                                        nnet::Unit* output,
                                        nnet::Unit* /*modulator*/ )
    {
        *output = input;
    }

        
    virtual void learn(                 size_t      /*synapse*/, 
                                        nnet::Unit  input, 
                                        nnet::Unit* myOutput,
                                        nnet::Unit  /*output*/ )
    {
        *myOutput = input;
    }

    
	~StdSynapse() {}


public:
	static nnet::SerializablePtr Create( std::string name )
    {
		return nnet::RefCountPtr<StdSynapse>( new StdSynapse( name ) );
	}
};


extern "C" void Initialize() {
	nnet::Serializable::AddFactory( "StdSynapse", StdSynapse::Create );
	nnet::string::get_list( "synapse_arrays" )->add( "StdSynapse" );
}


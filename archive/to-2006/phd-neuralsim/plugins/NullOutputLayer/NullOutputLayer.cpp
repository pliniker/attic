

#include <ErrorInterface.h>
#include <NodeLayer.h>



class NullOutputLayer : public nnet::NodeLayer
{
public:
    NullOutputLayer( std::string name ) :
        nnet::NodeLayer( name ) 
    {
        nnet::BooleanPtr isAsync( get( "isAsync" ) );
        isAsync->set( false );
    }
	
	
    std::string type() 
    { 
        return "NullOutputLayer"; 
    }


	bool check()
    {
		bool passed = nnet::NodeLayer::check();
		
		if( m_inputs != 1 )
        {
			layerMessage("number of inputs must = 1");
			passed = false;
		}
				
		return passed;
	}


	void recall( size_t neuron )
    {
        nnet::Unit modulation = 0xff;
        nnet::Unit* synapse_states = 0;
        buildInputList( neuron, &synapse_states, &modulation );

        m_state[ neuron ] = *synapse_states;
        m_state[ neuron ] &= modulation;
	}
	
	
    void learn( size_t neuron ) 
    { 
        recall( neuron ); 
    }


	~NullOutputLayer() {}
    

public:
	static nnet::SerializablePtr Create( std::string name )
    {
		return nnet::RefCountPtr<NullOutputLayer>(
                                    new NullOutputLayer( name ) );
	}
};


extern "C" void Initialize()
{
	nnet::Serializable::AddFactory( "NullOutputLayer",
                                    NullOutputLayer::Create );
	nnet::string::get_list( "node_layers" )->add( "NullOutputLayer" );
}


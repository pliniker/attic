

#include <Common.h>
#include <ErrorInterface.h>
#include <NodeLayer.h>
#include <RandomInt.h>



class RandomInputLayer : public nnet::NodeLayer 
{
public:
    RandomInputLayer( std::string name ) :
        nnet::NodeLayer( name ) 
    {
    }
	
	std::string type() { return "RandomInputLayer"; }

    void recall( size_t neuron )
    {
        m_state[ neuron ] =
                (nnet::Unit)( (float)nnet::RandomInt::Get() /
                              (float)nnet::RandomInt::Max * 255.0 );
    }

    ~RandomInputLayer() {}

public:
	static nnet::SerializablePtr Create( std::string name )
    {
		return nnet::RefCountPtr<RandomInputLayer>( new RandomInputLayer( name ) );
	}
};


extern "C" void Initialize()
{
	nnet::Serializable::AddFactory( "RandomInputLayer",
                                    RandomInputLayer::Create );
	nnet::string::get_list( "node_layers" )->add( "RandomInputLayer" );
}


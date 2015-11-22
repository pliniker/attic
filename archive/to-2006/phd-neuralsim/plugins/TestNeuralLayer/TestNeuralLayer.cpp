

#include <Common.h>
#include <ErrorInterface.h>
#include <NodeLayer.h>




class TestNeuralLayer : public nnet::NodeLayer
{
public:
    TestNeuralLayer( std::string name ) :
        nnet::NodeLayer(name)
    {
		//throw std::bad_alloc();
	}
	
	std::string type() { return "TestNeuralLayer"; }
	
	bool check()
    {
		return true;
	}

    void recall()
    {
		std::vector<char> array(10);
		array.at(11);
	}

	void learn()
    {
		throw std::bad_cast();
	}

    ~TestNeuralLayer() {}

public:
	static nnet::SerializablePtr Create( std::string name )
    {
		return nnet::RefCountPtr<TestNeuralLayer>(
                                    new TestNeuralLayer( name ) );
	}
};


extern "C" void Initialize()
{
	nnet::Serializable::AddFactory( "TestNeuralLayer", TestNeuralLayer::Create );
	nnet::string::get_list( "node_layers" )->add( "TestNeuralLayer" );
}


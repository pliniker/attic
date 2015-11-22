

#include <ErrorInterface.h>
#include <StringList.h>
#include <NodeLayer.h>



class ImageInputLayer : public nnet::NodeLayer
{
public:
    ImageInputLayer( std::string name ) :
        nnet::NodeLayer( name ) 
    {
        nnet::BooleanPtr isAsync( get( "isAsync" ) );
        isAsync->set( false );
    }

	
	std::string type() { return "ImageInputLayer"; }

		
	bool check() { return nnet::NodeLayer::check(); }
	
	
	~ImageInputLayer() {}
	
	
public:
	static nnet::SerializablePtr Create( std::string name )
    {
		return nnet::RefCountPtr<ImageInputLayer>(
                                    new ImageInputLayer( name ) );
	}
};


extern "C" void Initialize()
{
	nnet::Serializable::AddFactory( "ImageInputLayer",
                                    ImageInputLayer::Create );
	nnet::string::get_list( "node_layers" )->add( "ImageInputLayer" );
}


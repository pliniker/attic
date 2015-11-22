

#include <ErrorInterface.h>
#include <Serializable.h>
#include <Map.h>



class Test : public nnet::Map
{
public:
    Test( std::string name ) :
	   Map( name )
    {
		nnet::error::log( "Test::Test()" );
    }
	
	std::string type() { return "Test";	}
	
	~Test()
    { 
		nnet::error::log( "Test::~Test()" );
	}
	
public:
	static nnet::SerializablePtr Create( std::string name )
    {
		return nnet::RefCountPtr<Test>( new Test( name ) );
	}
};


extern "C" void Initialize() {
	nnet::Serializable::AddFactory( "Test", Test::Create );
}


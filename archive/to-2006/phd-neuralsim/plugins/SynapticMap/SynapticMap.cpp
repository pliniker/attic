

#include <ErrorInterface.h>
#include <RandomInt.h>
#include <Common.h>
#include <Integer.h>
#include "SynapticMap.h"


/////////////////////////////////////////////////////////////////////////////
////////////////////// Iconic ///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


class Iconic : public SynapticMap
{
public:
    Iconic( std::string name ) :
	    SynapticMap( name ) {}
	
	std::string type() { return "Iconic";	}
	
	int makeMap( int xIn, int yIn, int, int xOut, int yOut )
    {
		int from_size = xIn * yIn;
		int to_size = xOut * yOut;
		
		if( from_size == to_size )
        {
			connectionMap->create( to_size );
			
			for( int i = 0; i < to_size; i++ )
            {
				connectionMap->at( i ) = i;
			}
			
			return 1;
		}
        else
        {
			nnet::error::log(    "Iconic::makeMap requires input and output "
                                 "layers to have identical dimensions" );
			return 0;
		}
	}
	
	~Iconic() {}
	
public:
	static nnet::SerializablePtr Create( std::string name )
    {
		return nnet::RefCountPtr<Iconic>( new Iconic( name ) );
	}
};


/////////////////////////////////////////////////////////////////////////////
////////////////////// FullyConnected ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


class FullyConnected : public SynapticMap
{
public:
    FullyConnected( std::string name ) :
	    SynapticMap( name ) {}
	
	std::string type() { return "FullyConnected"; }
	
	int makeMap( int xIn, int yIn, int, int xOut, int yOut )
    {
		int from_size = xIn * yIn;
		int to_size = xOut * yOut * from_size;
		
		connectionMap->create( to_size );
		
		for( int i = 0; i < to_size; i += from_size )
        {
			for( int j = 0; j < from_size; j++ )
            {
				connectionMap->at( i + j ) = j;
			}
		}
		
		return from_size;
	}
	
	~FullyConnected() {}
	
public:
	static nnet::SerializablePtr Create( std::string name )
    {
		return nnet::RefCountPtr<FullyConnected>(
                                    new FullyConnected( name ) );
	}
};


/////////////////////////////////////////////////////////////////////////////
////////////////////// RandomFullyDistributed ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////


class RandomFullyDistributed : public SynapticMap
{
public:
    RandomFullyDistributed( std::string name ) :
	    SynapticMap( name ) {}
	
	std::string type() { return "RandomFullyDistributed";	}
	
	int makeMap( int xIn, int yIn, int spn, int xOut, int yOut )
    {
		int from_size = xIn * yIn;
		int to_size = xOut * yOut * spn;
		
		connectionMap->create( to_size );
		
		for( int i = 0; i < to_size; ++i )
        {
			int connect = nnet::RandomInt::Get();
            
			connect = (int)( ( (double)connect /
                               (double)nnet::RandomInt::Max ) *
                             (double)from_size );
                             
			connectionMap->at( i ) = connect;
		}
		
		return spn;
	}
	
	~RandomFullyDistributed() {}
	
public:
	static nnet::SerializablePtr Create( std::string name )
    {
		return nnet::RefCountPtr<RandomFullyDistributed>(
                                    new RandomFullyDistributed( name ) );
	}
};


/////////////////////////////////////////////////////////////////////////////
////////////////////// RandomLocallyDistributed /////////////////////////////
/////////////////////////////////////////////////////////////////////////////


class RandomLocallyDistributed : public SynapticMap
{
public:
    RandomLocallyDistributed( std::string name ) :
        SynapticMap( name ),
	    radius( new nnet::Int("radius") )
    {
		radius->set(3);
		add(radius);
	}
	
	std::string type() { return "RandomLocallyDistributed";	}
	
	void read( xmlNodePtr from )
    {
		SynapticMap::read( from );
		radius = get( "radius" );
	}
	
	int makeMap( int xIn, int yIn, int spn, int xOut, int yOut )
    {
		//int from_size = xIn * yIn;
		int to_size = xOut * yOut * spn;
		
		float xInOutRatio = (float)xIn / (float)xOut;
		float yInOutRatio = (float)yIn / (float)yOut;
		
		int r = radius->get();
		
		connectionMap->create( to_size );

        int indexDest = 0;
		
		for( int yDest = 0; yDest < yOut; ++yDest )
        {
			for( int xDest = 0; xDest < xOut; ++xDest )
            {
				for( int i = 0; i < spn; ++i )
                {
					int xDelta = (int)( ( (float)nnet::RandomInt::Get() /
                                          (float)nnet::RandomInt::Max ) *
                                        (float)r * 2 ) - r;
                                        
					int yDelta = (int)( ( (float)nnet::RandomInt::Get() /
                                          (float)nnet::RandomInt::Max ) *
                                        (float)r * 2 ) - r;
					
					int xSrc = (int)( (float)xDest * xInOutRatio );
					int ySrc = (int)( (float)yDest * yInOutRatio );

					if( xSrc + xDelta < 0 )             xSrc = 0;
					else if( xSrc + xDelta >= xIn )     xSrc = xIn - 1;
					else                                xSrc += xDelta;
					
					if( ySrc + yDelta < 0 )             ySrc = 0;
					else if( ySrc + yDelta >= yIn )     ySrc = yIn - 1;
					else                                ySrc += yDelta;

					int indexSrc = (ySrc * xIn) + xSrc;
					
					connectionMap->at( indexDest ) = indexSrc;

                    ++indexDest;
				}
			}
		}
		
		return spn;
	}
	
	~RandomLocallyDistributed() {}

private:
	nnet::IntPtr radius;
	
public:
	static nnet::SerializablePtr Create( std::string name )
    {
		return nnet::RefCountPtr<RandomLocallyDistributed>(
                                    new RandomLocallyDistributed(name) );
	}
};


/////////////////////////////////////////////////////////////////////////////
////////////////////// SynapticMap functions ////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


SynapticMap::SynapticMap( std::string name ) :
    nnet::Map( name ),
    connectionMap( new nnet::IntArray("connectionMap") )
{
	add( connectionMap );
}


void SynapticMap::read( xmlNodePtr from )
{
	nnet::Map::read( from );
	connectionMap = get( "connectionMap" );
}


extern "C" void Initialize()
{
	nnet::Serializable::AddFactory( "Iconic",
                                    Iconic::Create );
	nnet::string::get_list( "connection_maps")->add("Iconic");
	
	nnet::Serializable::AddFactory( "FullyConnected",
                                    FullyConnected::Create );
	nnet::string::get_list( "connection_maps" )->add( "FullyConnected" );
	
	nnet::Serializable::AddFactory( "RandomFullyDistributed",
                                    RandomFullyDistributed::Create );
	nnet::string::get_list( "connection_maps" )->add( "RandomFullyDistributed" );
	
	nnet::Serializable::AddFactory( "RandomLocallyDistributed",
                                    RandomLocallyDistributed::Create );
	nnet::string::get_list( "connection_maps" )->add( "RandomLocallyDistributed" );
}


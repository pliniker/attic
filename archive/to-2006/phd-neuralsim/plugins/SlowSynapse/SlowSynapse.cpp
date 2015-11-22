
#include <queue>
#include <vector>

#include <Brainlib.h>
#include <SynapseArray.h>
#include <Common.h>



struct BucketBrigade
{
    void resize( size_t newsize )
    {
        while( !buckets.empty() )
        {
            buckets.pop();
        }

        for( size_t i = 0; i < newsize; i++ )
        {
            buckets.push( 0 );
        }
    }

    void push( nnet::Unit next )
    {
        buckets.push( next );
    }

    nnet::Unit front()
    {
        return buckets.front();
    }

    void pop()
    {
        buckets.pop();
    }

private:
    std::queue<nnet::Unit> buckets;
};


class SlowSynapse : public nnet::SynapseArray
{
public:
    SlowSynapse( std::string name ) :
        nnet::SynapseArray( name )
    {
    }


	std::string type() 
    {
        return "SlowSynapse"; 
    }


    virtual void setDistance( size_t synapse, int value )
    {
        m_delaylines[ synapse ].resize( value );
    }
	

    virtual void create( int array_size )
    {
        m_delaylines.resize( array_size );
    }


    virtual void recall(                size_t      synapse, 
                                        nnet::Unit  input,
                                        nnet::Unit* output,
                                        nnet::Unit* /*modulator*/ )
    {
        m_delaylines[ synapse ].push( input );
        *output = m_delaylines[ synapse ].front();
        m_delaylines[ synapse ].pop();
    }

        
    virtual void learn(                 size_t      synapse, 
                                        nnet::Unit  /*input*/,
                                        nnet::Unit* myOutput,
                                        nnet::Unit  /*output*/ )
    {
        *myOutput = m_delaylines[ synapse ].front();
    }

    
	~SlowSynapse() {}


private:
    std::vector<BucketBrigade>  m_delaylines;


public:
	static nnet::SerializablePtr Create( std::string name )
    {
		return nnet::RefCountPtr<SlowSynapse>( new SlowSynapse( name ) );
	}
};


extern "C" void Initialize() {
	nnet::Serializable::AddFactory( "SlowSynapse", SlowSynapse::Create );
	nnet::string::get_list( "synapse_arrays" )->add( "SlowSynapse" );
}


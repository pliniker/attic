
// Fixed Virtual Generalised Automatic Classifier

#include <iostream>
#include <ErrorInterface.h>
#include <Integer.h>
#include <Boolean.h>
#include <Array.h>
#include <RandomInt.h>
#include <Common.h>
#include <NeuronArray.h>




class FVGAC : public nnet::NeuronArray
{
	
	enum States
    {
		Undefined = 0x0,
		Defined   = 0x1
	};

	
public:
	FVGAC( std::string name ) : 
	    nnet::NeuronArray  ( name                                ),
	    max_patterns       ( new nnet::Int("max_patterns")       ),
	    patterns           ( new nnet::UcharArray("patterns")    ),
        max_hdists         ( new nnet::IntArray("max_hdists")    )
    {	
		max_patterns->set( 8 );
		max_patterns->setMin( 1 );
		max_patterns->setMax( 8 );
		max_patterns->setStep( 1 );
		
		add( max_patterns );
		add( patterns );
        add( max_hdists );
		
		max_patts = 0;
	}
	
	
	std::string type() 
    {
		return "FVGAC";
	}

	
	void read( xmlNodePtr from )
    {
		nnet::NeuronArray::read( from );
        
		random_output = get( "random_output" );
		max_patterns  = get( "max_patterns" );
		patterns      = get( "patterns" );
        max_hdists    = get( "max_hdists" );

		array_size    = patterns->size();
	}
	
	
	bool check()
    {
        if( nnet::NeuronArray::check() )
        {
    		max_patts           = max_patterns->get();
				
	    	int old_size        = array_size;
		    
            patt_entry          = m_inputs + 2;
    		array_step          = max_patts * patt_entry;
	    	array_size          = array_step * m_neurons;

		    if( old_size != array_size )
            {
                patterns->create( array_size );
                max_hdists->create( m_neurons );
                reset();
            }

        }
		
		return true;
	}
	

    nnet::Unit recall( int neuron, nnet::Unit* pattern )
    {
		int response = 0;
		int min_dist = nnet::Common::IntMax;
		bool defined = false;
		
		int neuron_index = neuron * array_step;

        // for each stored pattern
		for( int i = 0; i < max_patts; i++ )
        {
   			int p_learnt = neuron_index + i * patt_entry;

            // if undefined pattern, ignore it
            if( patterns->at( p_learnt + m_inputs + 1 ) == Defined )
            {
               int p_dist = 0;

                // calc h-dist
                int j = m_inputs;
                while( j-- )
                {
                    p_dist += nnet::Common::HdistLookup[
                        pattern[j] ^ patterns->at( p_learnt + j ) ];
                }

                // if new low dist, use this match
                if( p_dist < min_dist )
                {
                    min_dist = p_dist;
                    response = patterns->at( p_learnt + m_inputs );
                    defined  = true;
                    notset   = false;
                }
                // else if mirrors an earlier match
                else if( p_dist == max_dist )
                {
                    response |= patterns->at( p_learnt + m_inputs );
                }
            }
		}

        // get hdist from zero pattern
        int p_dist = 0;
        int j = m_inputs;
        while( j-- )
        {
            p_dist += nnet::Common::HdistLookup[ patterns->at( p_learnt + j ) ];
        }

        // if the zero pattern is closest, the response is tied to zero
        if( p_dist < min_dist )
        {
            response = 0;
        }

		if( defined )
        {
			return response;
		}
        else
        {
			return 0;
		}
    }
	

    void learn( int neuron, nnet::Unit* pattern, nnet::Unit response )
    {
        int min_dist = nnet::Common::IntMax;
		int max_dist = 0;

		bool defined = false;
        int insertion_point = 0;
		
		int neuron_index = neuron * array_step;

        // for each stored pattern
		for( int i = 0; i < max_patts; i++ )
        {
   			int p_learnt = neuron_index + i * patt_entry;

            // if defined pattern, calculate hdist
            if( patterns->at( p_learnt + m_inputs + 1 ) == Defined )
            {
                int p_dist = 0;

                // calc h-dist
                int j = m_inputs;
                while( j-- )
                {
                    p_dist += nnet::Common::HdistLookup[
                        pattern[j] ^ patterns->at( p_learnt + j ) ];
                }

                // if new low dist, use this match
                if( p_dist < min_dist )
                {
                    min_dist = p_dist;
                    insertion_point = p_learnt;
                }

                // if new high dist, 
                if( p_dist > max_dist )
                {
                    max_dist = p_dist;
                }
            }
            // if undefined pattern, use this slot
            else if( patterns->at( p_learnt + m_inputs + 1 ) == Undefined )
            {
                insertion_point = p_learnt;
                defined = false;
                break;
            }
		}

        // do pattern insert
        if( !defined || ( max_dist > max_hdists->at( neuron ) ) )
        {
            patterns->at( insertion_point + m_inputs + 1 ) = Defined;
            patterns->at( insertion_point + m_inputs ) = response;
            for( int i = 0; i < max_patts; i++ )
            {
                patterns->at( insertion_point + i ) = pattern[i];
            }

            // update max dist for that sort of insert
            if( defined )
            {
                max_hdists->at( neuron ) = max_dist;
            }
        }
	}


    void reset()
    {
        int size = patterns->size();
        while( size-- )
        {
            patterns->at( size ) = 0;
        }

        size = max_hdists->size();
        while( size-- )
        {
            max_hdists->at( size ) = 0;
        }
    }
	
	
    ~FVGAC() {}

	
private:
	nnet::BooleanPtr       random_output;
	nnet::IntPtr           max_patterns;
	nnet::UcharArrayPtr    patterns;
    nnet::IntArrayPtr      max_hdists;

    int                    patt_entry;
	int                    array_size;
	int                    array_step;

	int                    max_patts;
	
	
public:
	static nnet::SerializablePtr Create( std::string name )
    {
		return nnet::RefCountPtr<FVGAC>( new FVGAC( name ) );
	}
};


extern "C" void Initialize()
{
	nnet::Serializable::AddFactory( "FVGAC", FVGAC::Create );
	nnet::string::get_list( "neuron_arrays" )->add( "FVGAC" );
}


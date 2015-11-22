
// Fixed Virtual Generalised Random Access Memory

#include <iostream>
#include <iomanip>

#include <ErrorInterface.h>
#include <Integer.h>
#include <Boolean.h>
#include <Array.h>
#include <RandomInt.h>
#include <Common.h>
#include <NeuronArray.h>




class FVGRAM : public nnet::NeuronArray
{
	
	enum States
    {
		Undefined = 0x0,
		Defined   = 0x1
	};

	
public:
	FVGRAM( std::string name ) : 
	    nnet::NeuronArray  ( name                                ),
	    random_output      ( new nnet::Boolean("random_output")  ),
	    max_patterns       ( new nnet::Int("max_patterns")       ),
	    patterns           ( new nnet::UcharArray("patterns")    ),
        max_hdists         ( new nnet::IntArray("max_hdists")    )
    {	
		random_output->set( true );
		
		max_patterns->set( 16 );
		max_patterns->setMin( 1 );
		max_patterns->setMax( nnet::Common::IntMax );
		max_patterns->setStep( 1 );
		
		add( random_output );
		add( max_patterns );
		add( patterns );
        add( max_hdists );
		
		rand_out            = 0;
		max_patts           = 0;
	}
	
	
	std::string type() 
    {
		return "FVGRAM";
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
            rand_out            = random_output->get();

    		max_patts           = max_patterns->get();
				
	        unsigned int old_size = array_size;
		    
            pattern_size        = m_inputs + 2;
    		neuron_size         = max_patts * pattern_size;
	    	array_size          = neuron_size * m_size;

		    if( old_size != array_size )
            {
                patterns->create( array_size );
                max_hdists->create( m_size );
                reset();
            }

        }
		
		return true;
	}
	
    
    virtual nnet::Unit recall( size_t neuron, nnet::Unit* pattern )
    {
	    unsigned int response = 0;
	    unsigned int min_dist = nnet::Common::IntMax;
		bool defined = false;
		bool notset  = true;
		
	    unsigned int neuron_index = neuron * neuron_size;

        // for each stored pattern
		for( unsigned int i = 0; i < max_patts; i++ )
        {
   		    unsigned int p_learnt = neuron_index + i * pattern_size;

            // if undefined pattern, ignore it
            if( patterns->at( p_learnt + m_inputs + 1 ) == Defined )
            {
               unsigned int p_dist = 0;

                // calc h-dist
                unsigned int j = m_inputs;
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
                else if( p_dist == min_dist )
                {
                    if( notset )
                    {
                        response = patterns->at( p_learnt + m_inputs );
                        notset   = false;
                    }
                    else if( defined &&
                            ( response != patterns->at( p_learnt + m_inputs ) ) )
                    {
                        defined = false;
                    }
                }
            }
		}

		if( defined )
        {
			return response;
		}
        else if( rand_out )
        {
			return nnet::RandomInt::Get() & nnet::Common::UnitMax;
		}
        else
        {
			return 0;
		}
    }
	

    virtual void learn( size_t neuron, nnet::Unit* pattern, nnet::Unit response )
    {
        unsigned int min_dist = nnet::Common::IntMax;
	    unsigned int max_dist = 0;

		bool defined = true;
        unsigned int insertion_point = 0;
		
	    unsigned int neuron_index = neuron * neuron_size;

        // for each stored pattern
		for( unsigned int i = 0; i < max_patts; i++ )
        {
   		    unsigned int p_learnt = neuron_index + i * pattern_size;

            // if defined pattern, calculate hdist
            if( patterns->at( p_learnt + m_inputs + 1 ) == Defined )
            {
                unsigned int p_dist = 0;

                // calc h-dist
                unsigned int j = m_inputs;
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

                // if same pattern, overwrite
                if( p_dist == 0 )
                {
                    insertion_point = p_learnt;
                    break;
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
            for( unsigned int i = 0; i < m_inputs; i++ )
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


    virtual void reset()
    {
        unsigned int size = patterns->size();
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
	
	
    ~FVGRAM() {}

	
private:
	nnet::BooleanPtr        random_output;
	nnet::IntPtr            max_patterns;
	nnet::UcharArrayPtr     patterns;
    nnet::IntArrayPtr       max_hdists;

    unsigned int            pattern_size;
    unsigned int            array_size;
    unsigned int            neuron_size;

	bool                    rand_out;
    unsigned int            max_patts;
	
	
public:
	static nnet::SerializablePtr Create( std::string name )
    {
		return nnet::RefCountPtr<FVGRAM>( new FVGRAM( name ) );
	}
};


extern "C" void Initialize()
{
	nnet::Serializable::AddFactory( "FVGRAM", FVGRAM::Create );
	nnet::string::get_list( "neuron_arrays" )->add( "FVGRAM" );
}


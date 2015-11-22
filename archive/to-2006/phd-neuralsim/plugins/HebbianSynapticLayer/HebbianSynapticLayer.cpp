
#include <vector>
#include <queue>

#include <EdgeLayer.h>
#include <Brainlib.h>
#include <RandomInt.h>
#include <ErrorInterface.h>
#include <SynapseArray.h>
#include <ObjectList.h>
#include <Array.h>
#include <Operations.h>
#include "../SynapticMap/SynapticMap.h"

#define NODUMP 1
#include "Correlation.hpp"

    

struct hebbian_function
{
    // private types
private:
    struct synapse_strength
    {
        synapse_strength( size_t sy, double st ) :
            synapse( sy ), strength( st ) {}

        size_t synapse;
        double strength;

        bool operator<( synapse_strength const& rhs ) const
        {
            return strength > rhs.strength;
        }
    };
    
    typedef std::priority_queue<synapse_strength>   ordered_list;
    
    typedef std::vector<correlation_t>              synapses_t;
    typedef synapses_t::iterator                    synapses_iter;


    // public implementation
public:
    // construct with size N synapses
    hebbian_function( size_t N ) :
        m_synaptic_correlations( N ) {}

    // change size
    inline void resize( size_t N )
    {
        m_synaptic_correlations.resize( N );
    }

    // put the indeces of the N weakest synapses into the container T
    template<typename T>
    inline void getWeakestN( size_t N, T& receptacle )
    {
        ordered_list list;

        // choose a random order, otherwise when the correlations are all very
        // low or zero we'll be biased towards the last in the array
        size_t synapses = m_synaptic_correlations.size();
        size_t* ordering = nnet::RandomInt::UniqueSequence( synapses );

        for( size_t i = 0; i < synapses; ++i )
        {
            size_t synapse = ordering[ i ];
            list.push( synapse_strength(synapse, 
                                m_synaptic_correlations.at( synapse ).calculate() ) );
        }

        receptacle.clear();
        for( size_t j = 0; j < N; j++ )
        {
            synapse_strength entry( list.top() );
            receptacle.push_back( entry.synapse );
            list.pop();
        }
    }

    // get the correlation function for synapse i
    inline correlation_t& at( size_t i )
    {
        return m_synaptic_correlations.at( i );
    }

private:
    synapses_t  m_synaptic_correlations;
};


class HebbianSynapticLayer : public nnet::EdgeLayer
{
public:
    HebbianSynapticLayer( std::string name ) :
        nnet::EdgeLayer     ( name ),
        synapse_list        ( new nnet::ObjectList( "synapse_list" ) ),
        synapse_container   ( new nnet::Map( "synapse_container" )   ),
        mapper_list         ( new nnet::ObjectList( "mapper_list" )  ),
        mapper_container    ( new nnet::Map( "mapper_container" )    ),
        cull_percent        ( new nnet::Int( "cull_percent" ) ),
        correlations        ( 0 )
    {
		synapse_list->setMax( 1 );
		synapse_list->setChoices( "synapse_arrays" );
		synapse_list->setTarget( synapse_container );
		synapse_list->add( "StdSynapse" );
		
		mapper_list->setMax( 1 );
		mapper_list->setChoices( "connection_maps" );
		mapper_list->setTarget( mapper_container );
		mapper_list->add( "RandomFullyDistributed" );

        cull_percent->setMin( 1 );
        cull_percent->setMax( 100 );
        cull_percent->set( 10 );
        cull_percent->setPrivate( false );
		
		add( synapse_list );
		add( synapse_container );
		add( mapper_list );
		add( mapper_container );
        add( cull_percent );
		
		synapses_per_neuron = 0;
		output_size         = 0;
		input_size          = 0;
	}
	
	
	std::string type() { return "HebbianSynapticLayer"; }
	
	
	void read( xmlNodePtr from )
    {
		nnet::EdgeLayer::read( from );
		
		synapse_list      = get( "synapse_list" );
		synapse_container = get( "synapse_container" );
		mapper_list       = get( "mapper_list" );
		mapper_container  = get( "mapper_container" );
        cull_percent      = get( "cull_percent" );
		
		if( !synapse_container->empty() )
        {
            synapse_array = synapse_container->first();
        }
        
		if( !mapper_container->empty()  )
        {
            synaptic_mapper = mapper_container->first();
        }
	}
	
	
	bool check()
    {
		if( nnet::EdgeLayer::check() )
        {	
            nnet::ImagePtr inputState( get( "state" ) );

            nnet::MapPtr outputDetails( get( "outputDetails" ) );
            nnet::ImagePtr outputState( outputDetails->get( "state" ) );
            
            nnet::IntPtr inputs( get( "inputs" ) );

			if( synapse_container->empty() )
            {
                layerMessage( "please supply a synapse type to use" );
				return false;
			}
			
			if( mapper_container->empty() )
            {
			    layerMessage( "please supply a mapping function to use" );
				return false;
			}

            // check for changes since last check
			bool size_changed = false, synapse_changed = false, mapping_changed = false;
	
			// has synapse type selection changed?
			nnet::SynapseArrayPtr pickedArray( synapse_container->first() );
			if( synapse_array.get() != pickedArray.get() )
            {
				synapse_changed = true;
				synapse_array   = pickedArray;
			}
			
			// has connection mapping selection changed?
			SynapticMapPtr pickedMap( mapper_container->first() );
			if( synaptic_mapper.get() != pickedMap.get() )
            {
				mapping_changed = true;
				synaptic_mapper = pickedMap;
			}
			
			// have input or output sizes changed?
			int temp_input_size = m_neurons * m_inputs;
            
			if( ( outputState->size() != output_size         ) ||
				( temp_input_size     != input_size          ) ||
				( inputs->get()       != synapses_per_neuron ) )
            {
				size_changed        = true;
				output_size         = outputState->size();
				input_size          = temp_input_size;
				synapses_per_neuron = inputs->get();
			}
			
			// build new connection mapping
			if( mapping_changed || size_changed )
            {
				inputs->set( synaptic_mapper->makeMap(
                                        outputState->x(),
                                        outputState->y(),
                                        inputs->get(),
						                inputState->x(),
                                        inputState->y() ) );

				// if the mapper function returns a different number of synapses per neuron
				// then make sure the synapse array size is changed too by setting
                // size_changed=true
				if( inputs->get() != synapses_per_neuron )
                {
					size_changed        = true;
					input_size          = inputState->size() * inputs->get();
					synapses_per_neuron = inputs->get();
					layerMessage( "inputs changed by " + synaptic_mapper->name() );
				}
			}
			
			// set up new synapse array
			if( synapse_changed || size_changed )
            {
				synapse_array->create( input_size );
                correlations.resize( input_size );
                reorg_counter = 0;
			}

            // set distances
            if( size_changed || mapping_changed )
            {
                for( int i = 0; i < input_size; i++ )
                {
                    setDistance( i );
                }
            }
			
			return synapse_array->check();
		}
        else
        {
            return false;
        }
	}


    virtual void recall(    size_t          neuron,
                            nnet::Unit*     synapticOutputs,
                            nnet::Unit*     axonModulation )
    { 
        nnet::IntArray& con_map = *( synaptic_mapper->connectionMap );
				
		int synapse_index = neuron * m_inputs;

        for( size_t j = 0; j < m_inputs; ++j )
        {
            synapse_array->recall(  synapse_index + j,
							        m_state[ con_map.at( synapse_index + j ) ],
                                    &synapticOutputs[ j ],
                                    axonModulation );
        }
    }


    virtual void learn(     size_t      neuron,
                            nnet::Unit* synapticOutputs,
                            nnet::Unit  previousNeuronOutput )
    {
		nnet::IntArray& con_map = *( synaptic_mapper->connectionMap );
		
		int synapse_index = neuron * m_inputs;

        for( size_t j = 0; j < m_inputs; ++j )
        {
			synapse_array->learn(   synapse_index + j, 
								    m_trainState[ con_map.at( synapse_index + j ) ],
                                    &synapticOutputs[ j ],
                                    previousNeuronOutput );

            correlations.at( synapse_index + j ).increment( 
                    synapticOutputs[ j ], previousNeuronOutput );
        }

        reorg_counter++;
        if( reorg_counter >= output_size )
        {
            reorganiseUncorrelatedSynapses();
            reorg_counter = 0;
        }
    }
	
	
	void reset() 
    { 
        synapse_array->reset(); 

        for( int i = 0; i < input_size; i++ )
        {
            correlations.at( i ).reset();
        }

        reorg_counter = 0;
    }


    // find bottom 10% uncorrelated synaptic connections and kill them.
    // Rebirth in their place new synaptic connections to different locations.
    void reorganiseUncorrelatedSynapses()
    {
		nnet::IntArray& con_map = *( synaptic_mapper->connectionMap );

        int N = ( input_size / 100 ) * cull_percent->get();

        weakest_synapses.resize( N );

        correlations.getWeakestN( N, weakest_synapses );

        std::vector<size_t>::iterator i = weakest_synapses.begin();
        std::vector<size_t>::iterator iend = weakest_synapses.end();
        while( i != iend )
        {
            int connect = 0;

            do
            {
                // kill existing synapse, grow new one to new random target
	    		connect = nnet::RandomInt::Get();
		    	connect = (int)( ( (double)connect /
                                   (double)nnet::RandomInt::Max ) *
                                 (double)(input_size / synapses_per_neuron) );

                con_map.at( *i ) = connect;

                setDistance( *i );
            }
            while( connect == *i );

            i++;
        }
    }

    void setDistance( size_t synapse )
    {
        nnet::ImagePtr inputState( get( "state" ) );

        nnet::MapPtr outputDetails( get( "outputDetails" ) );
        nnet::ImagePtr outputState( outputDetails->get( "state" ) );

        if( &inputState->at( 0 ) == &outputState->at( 0 ) )
        {
            nnet::IntArray& con_map = *( synaptic_mapper->connectionMap );
            int target = con_map.at( synapse );

            int neuron = synapse / synapses_per_neuron;

            // from neuron to input
            int from_y = neuron / inputState->x();
            int from_x = neuron % inputState->x();

            int to_y = target / outputState->x();
            int to_x = target % outputState->x();

            int ysquared = (from_y - to_y);
            ysquared *= ysquared;

            int xsquared = (from_x - to_x);
            xsquared *= xsquared;

            float dist = sqrt( (float)(ysquared + xsquared) );

            synapse_array->setDistance( synapse, (int)dist );
        }
    }
	
	
    ~HebbianSynapticLayer() {}


private:
    nnet::ObjectListPtr                 synapse_list;
    nnet::MapPtr                        synapse_container;
	nnet::SynapseArrayPtr               synapse_array;
	
	nnet::ObjectListPtr                 mapper_list;
	nnet::MapPtr			            mapper_container;
	SynapticMapPtr                      synaptic_mapper;

    nnet::IntPtr                        cull_percent;
	
	int                                 output_size;
	int				                    synapses_per_neuron;
	int                                 input_size;

    hebbian_function                    correlations;

    int                                 reorg_counter;
    std::vector<size_t>                 weakest_synapses;
    
	
public:
	static nnet::SerializablePtr Create( std::string name )
    {
		return nnet::RefCountPtr<HebbianSynapticLayer>(
                                    new HebbianSynapticLayer(name) );
	}
};


extern "C" void Initialize()
{
	nnet::Serializable::AddFactory( "HebbianSynapticLayer",
                                    HebbianSynapticLayer::Create );
	nnet::string::get_list( "edge_layers" )->add( "HebbianSynapticLayer" );
}




#include <EdgeLayer.h>
#include <Brainlib.h>
#include <RandomInt.h>
#include <ErrorInterface.h>
#include <SynapseArray.h>
#include <ObjectList.h>
#include <Array.h>
#include <Operations.h>
#include "../SynapticMap/SynapticMap.h"




class StdSynapticLayer : public nnet::EdgeLayer
{
public:
    StdSynapticLayer( std::string name ) :
        nnet::EdgeLayer     ( name ),
        synapse_list        ( new nnet::ObjectList( "synapse_list" ) ),
        synapse_container   ( new nnet::Map( "synapse_container" )   ),
        mapper_list         ( new nnet::ObjectList( "mapper_list" )  ),
        mapper_container    ( new nnet::Map( "mapper_container" )    )
    {
		synapse_list->setMax( 1 );
		synapse_list->setChoices( "synapse_arrays" );
		synapse_list->setTarget( synapse_container );
		synapse_list->add( "StdSynapse" );
		
		mapper_list->setMax( 1 );
		mapper_list->setChoices( "connection_maps" );
		mapper_list->setTarget( mapper_container );
		mapper_list->add( "RandomFullyDistributed" );
		
		add( synapse_list );
		add( synapse_container );
		add( mapper_list );
		add( mapper_container );
		
		synapses_per_neuron = 0;
		output_size         = 0;
		input_size          = 0;
	}
	
	
	std::string type() { return "StdSynapticLayer"; }
	
	
	void read( xmlNodePtr from )
    {
		nnet::EdgeLayer::read( from );
		
		synapse_list      = get( "synapse_list" );
		synapse_container = get( "synapse_container" );
		mapper_list       = get( "mapper_list" );
		mapper_container  = get( "mapper_container" );
		
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

        //nnet::Unit axonModulation = 255;

        for( size_t j = 0; j < m_inputs; ++j )
        {
            //synapse_array->recall(  synapse_index + j,
			//				        m_trainState[ con_map.at( synapse_index + j ) ],
            //                        &synapticOutputs[ j ],
            //                        &axonModulation );

			synapse_array->learn(   synapse_index + j, 
								    m_trainState[ con_map.at( synapse_index + j ) ],
                                    &synapticOutputs[ j ],
                                    previousNeuronOutput );
        }
    }
	
	
	void reset() 
    { 
        synapse_array->reset(); 
    }
	
	
    ~StdSynapticLayer() {}


private:
    nnet::ObjectListPtr                 synapse_list;
    nnet::MapPtr                        synapse_container;
	nnet::SynapseArrayPtr               synapse_array;
	
	nnet::ObjectListPtr                 mapper_list;
	nnet::MapPtr			            mapper_container;
	SynapticMapPtr                      synaptic_mapper;
	
	int                                 output_size;
	int				                    synapses_per_neuron;
	int                                 input_size;
	
	
public:
	static nnet::SerializablePtr Create( std::string name )
    {
		return nnet::RefCountPtr<StdSynapticLayer>(
                                    new StdSynapticLayer(name) );
	}
};


extern "C" void Initialize()
{
	nnet::Serializable::AddFactory( "StdSynapticLayer",
                                    StdSynapticLayer::Create );
	nnet::string::get_list( "edge_layers" )->add( "StdSynapticLayer" );
}



#include <NodeLayer.h>
#include <NeuronArray.h>
#include <ObjectList.h>
#include <Operations.h>
#include <ErrorInterface.h>



//
// StdNeuralLayer -> NodeLayer -> Layer -> Map -> Serializable
//
class StdNeuralLayer : public nnet::NodeLayer
{
public:
	
	// Constructor, takes name of Serializable object
    StdNeuralLayer( std::string name ) :
        nnet::NodeLayer ( name ),                                      // pass name down to Serializable
	    neuron_list        ( new nnet::ObjectList("neuron_list") ),    // ObjectList of "neuron_arrays" (see StringList)
	    neuron_container   ( new nnet::Map("neuron_container") )       // Map to put neuron selected from neuron_list in
    {               
		
		neuron_list->setMax( 1 );                                      // select one neuron only
		neuron_list->setChoices( "neuron_arrays" );                    // use "Neuron Arrays" list from StringList
		neuron_list->setTarget( neuron_container );                    // use neuron_container as Map to put selected neuron object in
		neuron_list->add( "FVGRAM" );                                  // by default, make FVGRAM selected
		
		add( neuron_list );                                            // since NodeLayer derives from Map, this is also a Map. Add neuron_list and neuron_container
		add( neuron_container );
		
		output_size         = 0;                                       // local vars used to spot changes to Serializable objects
		synapses_per_neuron = 0;
	}
	
	
    // virtual from Serializable
	std::string type() { return "StdNeuralLayer"; }
	
	
	// virtual from Serializable
	virtual void read( xmlNodePtr from )
    {
		nnet::NodeLayer::read( from );                                 // call next read() in the class hierarchy
		neuron_list      = get( "neuron_list" );                       // get Serializables from Map and
		neuron_container = get( "neuron_container" );                  //  put into local references

        if( !neuron_container->empty() )
        {
            neuron_array = neuron_container->first();
        }
	}
	

	// virtual from Layer
	virtual bool check()
    {
		if( NodeLayer::check() )                                       // call next check() in the class hierarchy
        {   
            nnet::ImagePtr outputState( get( "state" ) );
            nnet::IntPtr inputs( get( "inputs" ) );

			if( neuron_container->empty() )                            // empty neuron_container means no neuron selected
            {
				layerMessage( "please supply a neuron type to use" );
				return false;
			}
			
			if( inputs->get() == 0 )                                   // inputs==0 means no synaptic connections into this layer
            {
				layerMessage( "should have at least 1 input" );
				return false;
			}
		
			bool size_changed = false, neuron_changed = false;
			
			nnet::NeuronArrayPtr picked_array( neuron_container->first() );
			if( neuron_array.get() != picked_array.get() )             // if object pointers are different, a new neuron_array instance has been created...
            {
				neuron_changed = true;                                 // This works because the old pointer is still valid in neuron_array until it is overwritten by picked_array.
				neuron_array   = picked_array;                         // use the new neuron selected: overwrite neuron_array pointer with newly selected picked_array
			}
			
			if( ( outputState->size() != output_size         ) ||
				( inputs->get()       != synapses_per_neuron ) )
            {
				size_changed = true;
				output_size = outputState->size();
				synapses_per_neuron = inputs->get();
			}
			
			if( neuron_changed || size_changed )                       // if either size or selected neuron type has changed...
            {
				neuron_array->create( output_size, synapses_per_neuron );
			}
						
			return neuron_array->check();                              // force the neuron array to check it's possibly new attributes
		}
		return false;
	}
	

	// virtual from Layer
    virtual void recall( size_t neuron )
    {
        nnet::Unit modulation = 0xff;
        nnet::Unit* synapse_states = 0;
        buildInputList( neuron, &synapse_states, &modulation );

        m_state[ neuron ] = neuron_array->recall( neuron, synapse_states );
        m_state[ neuron ] &= modulation;
    }
	
	
	// virtual from Layer
    virtual void learn( size_t neuron )
    {
        nnet::Unit* synapse_states = 0;
        trainInputs( neuron, &synapse_states, m_trainState[ neuron ] );

        neuron_array->learn( neuron, synapse_states, m_trainState[ neuron ] );
    }
	
	
	// virtual from Layer
	void reset() 
    { 
        neuron_array->reset(); 
    }                                                                  // undo all learning
	
	
    ~StdNeuralLayer() {}                                               // RefCountPtr cleans things up
	
	
private:
	nnet::ObjectListPtr     neuron_list;                               // list of NeuronArray objects
	nnet::MapPtr            neuron_container;                          // Map to put selected NeuronArray into
	
	nnet::NeuronArrayPtr    neuron_array;                              // Selected NeuronArray

    int                     output_size;                               // temp locals to monitor change in
	int				        synapses_per_neuron;                       // state of Serializable objects outputState and inputs
	
	
public:
	// see Initialize. Pointer to this function put in Serializable::factories 
	static nnet::SerializablePtr Create( std::string name )
    {
		return nnet::RefCountPtr<StdNeuralLayer>( new StdNeuralLayer( name ) );
	}
};


// C function for libdl to lookup and call
extern "C" void Initialize()
{
	nnet::Serializable::AddFactory( "StdNeuralLayer",
                                    StdNeuralLayer::Create );
	nnet::string::get_list( "node_layers" )->add( "StdNeuralLayer" );
}


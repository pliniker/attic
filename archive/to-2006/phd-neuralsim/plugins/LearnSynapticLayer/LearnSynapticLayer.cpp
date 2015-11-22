

#include <EdgeLayer.h>
#include <Brainlib.h>
#include <RandomInt.h>
#include <ErrorInterface.h>
#include <ObjectList.h>
#include <Array.h>
#include <Operations.h>




class LearnSynapticLayer : public nnet::EdgeLayer
{
public:
    LearnSynapticLayer( std::string name ) :
        nnet::EdgeLayer( name )
    {
        nnet::IntPtr inputs( get( "inputs" ) );
		inputs->set( 0 );
		inputs->setMin( 0 );
		inputs->setMax( 0 );
		inputs->setPrivate( true );
	}
	
	std::string type() { return "LearnSynapticLayer"; }
	
	bool check()
    {
		if( nnet::EdgeLayer::check() )
        {
            nnet::MapPtr outputDetails( get( "outputDetails" ) );
            nnet::ImagePtr outputState( outputDetails->get( "state" ) );

            nnet::ImagePtr inputState( get( "state" ) );

			if ( ( outputState->x()        != inputState->x()        ) ||
				 ( outputState->y()        != inputState->y()        ) ||
				 ( outputState->encoding() != inputState->encoding() ) )
            {
				layerMessage( "both connecting neural layers must have the "
                              "same dimensions and color encoding" );
				return false;
			}
            else
            {
                // make input-state of this layer the dominant-synapse input
                // of the output NodeLayer
                nnet::MapPtr outputList( get( "outputList" ) );
                nnet::NodeLayerPtr outputNode( outputList->first() );
                nnet::MapPtr outputNodeLearn( outputNode->get( "learnState" ) );

                outputNodeLearn->add( inputState );

                // force check to pick up new train state
                outputNode->check();

				return true;
			}
		}
		return false;
	}

    virtual void recall(    size_t          /*neuron*/,
                            nnet::Unit*     /*synapticOutputs*/,
                            nnet::Unit*     /*axonModulation*/ ) {}

    virtual void learn(     size_t          /*neuron*/,
                            nnet::Unit*     /*synapticOutputs*/,
                            nnet::Unit      /*previousNeuronOutput*/ ) {}

	
    ~LearnSynapticLayer() {}

public:
	static nnet::SerializablePtr Create( std::string name ) {
		return nnet::RefCountPtr<LearnSynapticLayer>( new LearnSynapticLayer( name ) );
	}
};


extern "C" void Initialize() {
	nnet::Serializable::AddFactory( "LearnSynapticLayer",
                                    LearnSynapticLayer::Create );
	nnet::string::get_list( "edge_layers" )->add( "LearnSynapticLayer" );
	nnet::string::get_list( "output_modifiers" )->add( "LearnSynapticLayer" );
}


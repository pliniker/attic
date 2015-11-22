
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include "Brainlib.h"
#include "Operations.h"
#include "Slot.h"
#include "EdgeLayer.h"
#include "NodeLayer.h"
#include "BadObject.h"
#include "Common.h"
#include "RandomInt.h"



nnet::NodeLayer::NodeLayer( std::string name ) :
    Layer( name )
{
    BooleanPtr isAsync( new Boolean( "isAsync" ) );
    isAsync->set( true );
	add( isAsync );

    MapPtr learnState( new Map( "learnState" ) );
    learnState->setPrivate( true );
    add( learnState );
}


void nnet::NodeLayer::read( xmlNodePtr from ) 
{
	Layer::read( from );
}


bool nnet::NodeLayer::check() 
{
	bool passed = Layer::check();
	
    IntPtr inputs( get( "inputs" ) );
	inputs->set( 0 );
    
    MapPtr inputList( get( "inputList" ) );
    
	for( iterator i = inputList->begin(); i != inputList->end(); i++ ) 
    {
		EdgeLayerPtr edge( inputList->get( i ) );
        IntPtr edgeInputs( edge->get( "inputs" ) );
		
        inputs->set( inputs->get() + edgeInputs->get() );
	}
    
    m_inputs = inputs->get();
    
    m_inputVector.resize( m_inputs * m_neurons );

    MapPtr learnState( get( "learnState" ) );
    if( learnState->exists( "state" ) )
    {
        ImagePtr learn( learnState->get( "state" ) );
        m_trainState = &learn->at( 0 );
    }
    else
    {
        m_trainState = m_state;
    }
	
	return passed;
}


void nnet::NodeLayer::recall( size_t ) 
{ 
    LOG_UNIMPL
}


void nnet::NodeLayer::learn( size_t ) 
{
    LOG_UNIMPL
}


bool nnet::NodeLayer::attachTo( LayerPtr object ) 
{
    bool ret = false;
    
    try 
    {
        EdgeLayerPtr edge( object );
        
        MapPtr outputList( get( "outputList" ) );
        outputList->add( edge );
        
        ret = true;
    }
    catch( std::bad_cast const& e ) 
    {
        LOG_EXCEPTION_E( e );
    }

    return ret;
}


bool nnet::NodeLayer::attachFrom( LayerPtr object ) 
{
    bool ret = false;
    try 
    {
        EdgeLayerPtr edge( object );

        MapPtr inputList( get( "inputList" ) );
        inputList->add( edge );
        
        ret = true;
    }
    catch( std::bad_cast const& e ) 
    {
        LOG_EXCEPTION_E( e );
    }

    return ret;
}


bool nnet::NodeLayer::asyncRecallOrder()
{
    BooleanPtr isAsync( get( "isAsync" ) );
    return isAsync->get();
}


void nnet::NodeLayer::buildInputList(   size_t          neuron, 
                                        nnet::Unit**    outputs, 
                                        nnet::Unit*     outputMod )
{
    MapPtr inputList( get( "inputList" ) );
    
    size_t offset = m_inputs * neuron;
    
    Map::iterator i = inputList->begin();
    Map::iterator iend = inputList->end();
    while( i != iend )
    {
        EdgeLayerPtr edgeLayer( inputList->get( i ) );
        
        edgeLayer->recall( neuron, &m_inputVector[ offset ], outputMod );

        offset += edgeLayer->numInputs();       
        
        i++;
    }
    
    *outputs = &m_inputVector[ m_inputs * neuron ];
}


void nnet::NodeLayer::trainInputs(      size_t          neuron,
                                        nnet::Unit**    outputs,
                                        nnet::Unit      output )
{
    MapPtr inputList( get( "inputList" ) );
    
    size_t offset = m_inputs * neuron;
    
    Map::iterator i = inputList->begin();
    Map::iterator iend = inputList->end();
    while( i != iend )
    {
        EdgeLayerPtr edgeLayer( inputList->get( i ) );
        
        edgeLayer->learn( neuron, &m_inputVector[ offset ], output );
       
        offset += edgeLayer->numInputs();       
        
        i++;
    }

    *outputs = &m_inputVector[ m_inputs * neuron ];
}


void nnet::recall( nnet::NodeLayerPtr layer ) 
{
    try 
    {
        if( layer.get() ) 
        {
            for( size_t i = 0; i < layer->numNeurons(); ++i )
            {
                layer->recall( i );
            }
            nnet::layer::recall_signal( layer );
        }
    }
    catch( std::exception& e ) 
    {
        LOG_EXCEPTION_E( e );
        throw BadObject( layer, e.what() );
    }
}


void nnet::recall( nnet::NodeLayerPtr layer, size_t neuron ) 
{
    try 
    {
        if( layer.get() ) 
        {
            layer->recall( neuron );
            nnet::layer::recall_signal( layer );
        }
    }
    catch( std::exception& e ) 
    {
        LOG_EXCEPTION_E( e );
        throw BadObject( layer, e.what() );
    }
}


void nnet::learn( nnet::NodeLayerPtr layer ) 
{
    try 
    {
        if( layer.get() ) 
        {
            for( size_t i = 0; i < layer->numNeurons(); ++i )
            {
                layer->learn( i );
            }
            nnet::layer::learn_signal( layer );
        }
    }
    catch( std::exception& e ) 
    {
        LOG_EXCEPTION_E( e );
        throw BadObject( layer, e.what() );
    }
}
    

void nnet::blankState( NodeLayerPtr layer )
{
    ImagePtr state( layer->get( "state" ) );

    for( int i = 0; i < state->size(); i++ )
    {
        state->at( i ) = 0;
    }
}


void nnet::randomizeState( NodeLayerPtr layer )
{
    ImagePtr state( layer->get( "state" ) );

    for( int i = 0; i < state->size(); i++ )
    {
        state->at( i ) = Common::MaxResponse( 
                Common::HdistLookup[ RandomInt::Get() & 0xff ] );
    }
}


void nnet::randomizeState(  NodeLayerPtr    layer,
                            int             percent )
{
    ImagePtr state( layer->get( "state" ) );

    for( int i = 0; i < (state->size() / 100 * percent); i++ )
    {
        int neuron = RandomInt::Get() % state->size();
        state->at( neuron ) = Common::MaxResponse( 
                Common::HdistLookup[ RandomInt::Get() & 0xff ] );
    }
}


void nnet::learn( nnet::NodeLayerPtr layer, size_t neuron ) 
{
    try 
    {
        if( layer.get() ) 
        {
            layer->learn( neuron );
            nnet::layer::learn_signal( layer );
        }
    }
    catch( std::exception& e ) 
    {
        LOG_EXCEPTION_E( e );
        throw BadObject( layer, e.what() );
    }
}


nnet::NodeLayerPtr nnet::new_node( std::string type ) 
{
	MapPtr nodes( nnet::root_map()->get( "Nodes" ) );

	std::string name;
	do 
    {
		std::ostringstream _name;
		_name << type << "-" << std::ostringstream::hex << random();
		name = _name.str();
	} 
    while( nodes->exists( name ) );
	
	try 
    {
		NodeLayerPtr object( Serializable::Create( type, name ) );	
		nodes->add( object );
        nnet::layer::new_node_signal( object );
		return object;
	}
	catch( std::exception& e ) 
    {
        LOG_EXCEPTION_E( e );
		error::std_exception( type, e.what() );
		throw;
	}
}


void nnet::erase_node( NodeLayerPtr layer ) 
{
	std::string layer_name = layer->name();
	MapPtr nodes( nnet::root_map()->get( "Nodes" ) );
	
	if( nodes->exists( layer_name ) ) 
    {
		nodes->erase( layer_name );
        nnet::layer::erase_node_signal( layer );
	}
}




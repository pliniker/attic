

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include "Brainlib.h"
#include "Common.h"
#include "ErrorInterface.h"
#include "Slot.h"
#include "BadObject.h"
#include "NodeLayer.h"
#include "EdgeLayer.h"



nnet::EdgeLayer::EdgeLayer( std::string name ) :
    Layer( name )
{	
    IntPtr inputs( get( "inputs" ) );
	inputs->setReadonly( false );
	inputs->set( 1 );
	inputs->setMin( 1 );
	inputs->setMax( Common::IntMax );

    MapPtr outputDetails( new Map( "outputDetails" ) );
    add( outputDetails );
}


void nnet::EdgeLayer::read( xmlNodePtr node ) 
{
	Layer::read( node );
}


bool nnet::EdgeLayer::check() 
{
	bool passed = Layer::check();

    MapPtr inputList( get( "inputList" ) );
    MapPtr outputList( get( "outputList" ) );
		
	if( inputList->empty() || outputList->empty() ) 
    {
        CStringPtr name( get( "displayName" ) );
		std::string msg = name->get() + ": must be connected at both ends";
		error::log( msg );
		passed = false;
	}

    if( passed )
	{
        // get destination state
        MapPtr outputDetails( get( "outputDetails" ) );

        NodeLayerPtr outputNode = outputList->first();
        ImagePtr outputLayerState = outputNode->get( "state" );

        outputDetails->add( outputLayerState );	

        // get source state
        NodeLayerPtr inputNode = inputList->first();
        ImagePtr inputLayerState = inputNode->get( "state" );

        add( inputLayerState );

        m_neurons = inputLayerState->size();
        m_state = &inputLayerState->at( 0 );

        MapPtr inputTrainState( inputNode->get( "learnState" ) );
        if( inputTrainState->exists( "state" ) )
        {
            ImagePtr trainState( inputTrainState->get( "state" ) );
            m_trainState = &trainState->at( 0 );
        }
        else
        {
            m_trainState = m_state;
        }
    }

	return passed;
}


bool nnet::EdgeLayer::attachTo( LayerPtr object ) 
{
    bool ret = false;
    
    MapPtr outputList( get( "outputList" ) );

    // only allow a connection if we don't already have one
    if( outputList->empty() ) 
    {
        try 
        {
            NodeLayerPtr node( object );
            outputList->add( node );
            ret = true;
        }
        // break if object is not a NodeLayer
        catch( std::bad_cast const& e ) 
        {
            LOG_EXCEPTION_E( e );
        }
    }

    return ret;
}


bool nnet::EdgeLayer::attachFrom( LayerPtr object ) 
{
    bool ret = false;
    
    MapPtr inputList( get( "inputList" ) );
    
    if( inputList->empty() ) 
    {
        try 
        {
            NodeLayerPtr node( object );
            inputList->add( node );
            ret = true;
        }
        catch( std::bad_cast const& e ) 
        {
            LOG_EXCEPTION_E( e );
        }
    }
    return ret;
}


nnet::EdgeLayerPtr nnet::new_edge( std::string type ) 
{	
	MapPtr edges( nnet::root_map()->get( "Edges" ) );

	std::string name;
	do 
    {
		std::ostringstream _name;
		_name << type << "-" << std::ostringstream::hex << random();
		name = _name.str();
	} 
    while( edges->exists( name ) );
	
	try 
    {
		EdgeLayerPtr object( Serializable::Create( type, name ) );	
		edges->add( object );
        nnet::layer::new_edge_signal( object );
		return object;
	}
	catch( std::exception& e ) 
    {
        LOG_EXCEPTION_E( e );
		error::std_exception( type, e.what() );
		throw;
	}
}


void nnet::erase_edge( EdgeLayerPtr layer ) 
{
	std::string layer_name = layer->name();
	MapPtr edges( nnet::root_map()->get( "Edges" ) );
	
	if ( edges->exists( layer_name ) ) 
    {
		edges->erase( layer_name );
        nnet::layer::erase_edge_signal( layer );
	}
}




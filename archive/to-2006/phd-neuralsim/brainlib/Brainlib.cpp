

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <vector>

#include "Brainlib.h"
#include "BinaryFile.h"
#include "RandomInt.h"
#include "ErrorInterface.h"
#include "Serializable.h"
#include "PluginManager.h"
#include "ObjectList.h"
#include "NodeLayer.h"
#include "EdgeLayer.h"
#include "ExecEngine.h"
#include "Map.h"
#include "Slot.h"
#include "BadObject.h"


static nnet::MapPtr nodes_map;
static nnet::MapPtr edges_map;
static nnet::MapPtr system_map;
static nnet::MapPtr exec_map;
static nnet::MapPtr root_map;


void nnet::initialize() 
{
    error::set_error_interface( new ErrorInterface );
    RandomInt::Initialize();
	Serializable::Initialize();
	PluginManager::Initialize();
	
	string::new_list( "node_layers" );
	string::new_list( "edge_layers" );
	string::new_list( "neuron_arrays" );
	string::new_list( "synapse_arrays" );
	string::new_list( "exec_maps" );
	
	::root_map = MapPtr( new Map( "Root" ) );
	::nodes_map = MapPtr( new Map( "Nodes" ) );
	::edges_map = MapPtr( new Map( "Edges" ) );
	::system_map = MapPtr( new Map( "System" ) );
	::exec_map = MapPtr( new Map( "Exec" ) );
	
	::root_map->add( ::nodes_map );
	::root_map->add( ::edges_map );
	::root_map->add( ::system_map );
	::system_map->add( ::exec_map );
	
	ObjectListPtr exec( new ObjectList( "Alternative_Execution_Engine" ) );
	exec->setTarget( ::exec_map );
	exec->setChoices( "exec_maps" );
	exec->setMax( 1 );
	::system_map->add( exec );
	
    nnet::global::initialize_signal();
}


void nnet::cleanup() 
{
	PluginManager::Cleanup();
	Serializable::Cleanup();
    nnet::global::cleanup_signal();
}


void nnet::file_new() 
{
	::nodes_map->clear();
	::edges_map->clear();	
    nnet::global::new_signal();
}


std::string nnet::file_open( std::string path ) 
{
	int slashpos       = path.rfind( "/" ) ;
	std::string filenm = path.substr( slashpos + 1 );
	std::string parent = path.substr( 0, slashpos );
	int brainpos       = filenm.rfind( ".xml.brain" );
	std::string basenm = filenm.substr( 0, brainpos );
	
	if( brainpos != ( int )( filenm.size() - 10 ) ) 
    {
		std::string err = "nnet::Open() path is not a valid project " + path;
		error::log( err );
		return string::null;
	}
 	
	if( chdir( parent.c_str() ) == -1 ) 
    {
		std::string err = "Project::Open() cannot find directory path " + path;
		error::log( err );
		return string::null;
	}
	
	std::string xmlFile = basenm + ".xml.brain";
	std::string binFile = basenm + ".bin.brain";

	bool status = BinaryFile::OpenRead( binFile );
	if( !status ) return string::null;
		 
	::root_map = Serializable::Unserialize( xmlFile );
	if( ::root_map.get() == 0 ) return string::null;
	
	BinaryFile::Close();

	::nodes_map = ::root_map->get( "Nodes" );
	::edges_map = ::root_map->get( "Edges" );
	::system_map = ::root_map->get( "System" );
	::exec_map = ::system_map->get( "Exec" );
	
	check();
		
	error::log( "<<< Project Opened >>>" );
	
    std::string save_path = parent + "/" + basenm;
    nnet::global::open_signal( save_path );
	
	return save_path;
}


std::string nnet::file_save( std::string path ) 
{
	int slashpos       = path.rfind( "/" );
	std::string filenm = path.substr( slashpos + 1 );
	std::string parent = path.substr( 0, slashpos );
 	
	if( chdir( parent.c_str() ) == -1 ) 
    {
		std::string err = "Project::Save() cannot find directory path " + path;
		error::log( err );
		return string::null;
	}
	
	std::string xmlFile = filenm + ".xml.brain";
	std::string binFile = filenm + ".bin.brain";
	
	bool status = BinaryFile::OpenWrite( binFile );
	if( !status ) return false;
	
	status = Serializable::Serialize( ::root_map, xmlFile );
	if( !status ) return false;
	
	BinaryFile::Close();
	
	error::log( "<<< Project Saved >>>" );
		
    nnet::global::save_signal( path );
		
	return path;
}


namespace nnet
{

struct NodeNeuron
{
    NodeNeuron( NodeLayerPtr d, uint32_t n ) :
        node( d.get() ), neuron( n ) {}

    void recall()   { node->recall( neuron ); }
    void learn()    { node->learn( neuron ); }

    bool operator== ( NodeNeuron const& rhs )
    {
        return ( node == rhs.node &&
                 neuron == rhs.neuron );
    }

    NodeLayer*  node;
    uint32_t    neuron;
};

struct RecallOrder
{ 
    std::vector<NodeNeuron> order;
    typedef std::vector<NodeNeuron>::const_iterator iterator;


    void fill()
    {
        order.clear();

        MapPtr nodes = ::root_map->get( "Nodes" );
        
        Map::iterator i = nodes->begin();
        Map::iterator iend = nodes->end();
        while( i != iend )
        {
            NodeLayerPtr node = nodes->get( i );
            BooleanPtr enabled( node->get( "enabled" ) );

            if( node->asyncRecallOrder() && enabled->get() )
            {
                size_t neurons = node->numNeurons();

                for( size_t i = 0; i < neurons; ++i )
                {
                    order.push_back( NodeNeuron( node, i ) );
                }
            }

            i++;
        }
    }

    void randomize()
    {
        size_t i = 0;
        size_t size = order.size();

        while( i < size )
        {
            size_t with = rand() % size;
            std::swap( order[ i ], order[ with ] );
            ++i;
        }

    }

} recall_order;

}


bool nnet::check() 
{
	bool all_correct = true;
	
	for( Map::iterator i = ::nodes_map->begin(); i != ::nodes_map->end(); i++ ) 
    {
		NodeLayerPtr node( ::nodes_map->get( i ) );
		all_correct &= nnet::check( node );
	}
	
	for( Map::iterator i = ::edges_map->begin(); i != ::edges_map->end(); i++ ) 
    {
		EdgeLayerPtr edge( ::edges_map->get( i ) );
		all_correct &= nnet::check( edge );
	}
	
	if( all_correct ) 
    {
        recall_order.fill();
        recall_order.randomize();

		error::log( "<<< All ::nodes_map and ::edges_map verified >>>" );
    }
	else
    {
		error::log( "!!! some ::nodes_map and ::edges_map have errors !!!" );
    }
	
    nnet::global::check_signal();
	
	return all_correct;
}


void nnet::recall() 
{	
	if( ::exec_map->empty() ) 
    {
        // recall synchronous layers first
        {
            MapPtr nodes = ::root_map->get( "Nodes" );

            Map::iterator i = nodes->begin();
            Map::iterator iend = nodes->end();
            while( i != iend )
            {
                NodeLayerPtr node = nodes->get( i );
                BooleanPtr enabled( node->get( "enabled" ) );

                if( !node->asyncRecallOrder() && enabled->get() )
                {
                    recall( node );
                }
                i++;
            }
        }
        // then recall async layers
        {
            RecallOrder::iterator i = recall_order.order.begin();
            RecallOrder::iterator iend = recall_order.order.end();
            while( i != iend )
            {
                i->node->recall( i->neuron );
                i++;
            }
        }
	} 
    else 
    {
        try 
        {
		    ExecEnginePtr exec( ::exec_map->first() );
            exec->recall();
        }
        catch( std::exception& e ) 
        {
            LOG_EXCEPTION_E( e );
            error::std_exception( "nnet::Recall() running ExecEngine", e.what() );
            return;
        }
        catch( ... )
        {
            LOG_EXCEPTION;
            error::alert( "Critical exception in ExecEngine!" );
        }
	}
	
    nnet::global::recall_signal();
}


void nnet::learn() 
{
	if( ::exec_map->empty() ) 
    {
        // train synchronous layers first
        {
            MapPtr nodes = ::root_map->get( "Nodes" );

            Map::iterator i = nodes->begin();
            Map::iterator iend = nodes->end();
            while( i != iend )
            {
                NodeLayerPtr node = nodes->get( i );
                BooleanPtr enabled( node->get( "enabled" ) );

                if( !node->asyncRecallOrder() && enabled->get() )
                {
                    learn( node );
                }
                i++;
            }
        }
        // then train async layers
        {
            RecallOrder::iterator i = recall_order.order.begin();
            RecallOrder::iterator iend = recall_order.order.end();
            while( i != iend )
            {
                i->node->learn( i->neuron );
                i++;
            }
        }
	} 
    else 
    {
        try 
        {
            ExecEnginePtr exec( ::exec_map->first() );
            exec->learn();
        }
        catch( std::exception& e ) 
        {
            LOG_EXCEPTION_E( e );
            error::std_exception( "nnet::Learn() running ExecEngine", e.what() );
            return;
        }
        catch( ... )
        {
            LOG_EXCEPTION;
            error::alert( "Critical exception in ExecEngine!" );
        }
	}
	
    nnet::global::learn_signal();
}


void nnet::reset() 
{
	for( Map::iterator i = ::edges_map->begin(); i != ::edges_map->end(); i++ ) 
    {
		EdgeLayerPtr edge( ::edges_map->get( i ) );
        nnet::reset( edge );
	}
	
	for( Map::iterator i = ::nodes_map->begin(); i != ::nodes_map->end(); i++ ) 
    {
		NodeLayerPtr node( ::nodes_map->get( i ) );
        nnet::reset( node );
	}
	
    nnet::global::reset_signal();
}


nnet::MapPtr nnet::root_map() 
{
	return ::root_map;
}




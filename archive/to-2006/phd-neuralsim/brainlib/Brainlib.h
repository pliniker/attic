#ifndef BRAINLIB_BRAINLIB_H
#define BRAINLIB_BRAINLIB_H


#include <map>
#include <string>
#include "ErrorInterface.h"
#include "PluginManager.h"
#include "Map.h"
#include "NodeLayer.h"
#include "EdgeLayer.h"


namespace nnet {


    /// Other important global functions
    ///
    /// PluginManager::LoadFromDir
    /// RandomInt::Get
    /// RandomInt::Max
    /// ErrorInterface::Alert
    /// ErrorInterface::Log
    /// ErrorInterface::Ask
    /// Serializable::AddFactory
    /// StringList::Add
    /// StringList::Remove
    /// StringList::Get
    /// StringList::Null
    /// NodeLayer               new_node
    /// NodeLayer               erase_node
    /// EdgeLayer               new_edge
    /// EdgeLayer               erase_edge
    /// Layer                   check
    /// Layer                   recall
    /// Layer                   learn
    /// Layer                   reset
    /// Layer                   attach
    /// Layer                   detach


    void                initialize();
    void                cleanup();

    void                file_new();
    std::string         file_open( std::string path );
    std::string         file_save( std::string path );

    bool                check();
    void                recall();
    void                learn();
    void                reset();

    MapPtr              root_map();


} // end namespace


#endif

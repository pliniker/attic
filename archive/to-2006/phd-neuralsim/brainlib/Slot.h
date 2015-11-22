#ifndef BRAINLIB_SLOT_H
#define BRAINLIB_SLOT_H


#include <boost/bind.hpp>
#include <boost/signals.hpp>
#include <string>
#include "Layer.h"
#include "NodeLayer.h"
#include "EdgeLayer.h"


namespace nnet {


    // Commons slot types
    typedef boost::signal<void ()>                      VoidSignal;
    typedef boost::signal<void ( std::string )>         StringSignal;
    typedef boost::signal<void ( LayerPtr )>            LayerSignal;
    typedef boost::signal<void ( LayerPtr, LayerPtr )>  Layer2Signal;


    
    // Global signals
    namespace global {
        extern VoidSignal          initialize_signal;
        extern VoidSignal          cleanup_signal;
        extern VoidSignal          new_signal;
        extern StringSignal        open_signal;
        extern StringSignal        save_signal;
        extern VoidSignal          check_signal;
        extern VoidSignal          recall_signal;
        extern VoidSignal          learn_signal;
        extern VoidSignal          reset_signal;
    }

    // Layer specific signals
    namespace layer {
        extern LayerSignal         check_signal;
        extern LayerSignal         recall_signal;
        extern LayerSignal         learn_signal;
        extern LayerSignal         reset_signal;
        extern Layer2Signal        attach_signal;
        extern Layer2Signal        detach_signal;
        extern LayerSignal         new_node_signal;
        extern LayerSignal         erase_node_signal;
        extern LayerSignal         new_edge_signal;
        extern LayerSignal         erase_edge_signal;
    }
    
}


#endif

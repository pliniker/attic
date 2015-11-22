
#include "FileName.h"




extern "C" void Initialize() {
    nnet::Serializable::AddFactory( "FileName", nnet::FileName::Create );
    nnet::string::get_list( "base_types" )->add( "FileName" );
}


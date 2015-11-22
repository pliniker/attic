

#include <sys/stat.h>
#include "FileName.h"



nnet::FileName::FileName(std::string name) :
    CString     ( name ),
    this_path   (),
    this_lastmod( 0 )
{
}
        

std::string nnet::FileName::type() { return "FileName"; }


bool nnet::FileName::fileChanged()
{
    if( this_path != get() )
    {
        this_path = get();
        struct stat buffer;
        if( stat( get().c_str(), &buffer ) == 0 )
        {
            this_lastmod = buffer.st_mtime;
        }
        return true;
    }

    struct stat buffer;
    if( stat( get().c_str(), &buffer ) == 0 )
    {
        if( buffer.st_mtime > this_lastmod )
        {
            this_lastmod = buffer.st_mtime;
            return true;
        }
    }
    
    return false;
}
        

nnet::SerializablePtr nnet::FileName::Create( std::string name )
{
    return nnet::RefCountPtr<FileName>( new FileName( name ) );
}



#include <cstring>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include "ErrorInterface.h"
#include "PluginManager.h"



void nnet::PluginManager::Initialize() {
}


void nnet::PluginManager::LoadFromDir( std::string dir_name ) {

    DIR* dir;
    dir = opendir( dir_name.c_str() );

    if( !dir ) {
        std::string err = "PluginManager::LoadFromDir() cannot open directory " + dir_name;
        error::log( err );
    } else {
        while( dirent* ent = readdir( dir ) ) {

            std::string filename = ent->d_name;
            std::string fullname = dir_name + filename;
        
            struct stat stat_buf;
            lstat( fullname.c_str(), &stat_buf );
            
            if( !S_ISLNK( stat_buf.st_mode ) ) {
            
			     if( filename != "." && filename != ".." ) {
			
				     DLLManager* mgr = new DLLManager( fullname );
    
				     if( mgr->makeFactory() ) {
					     manager_list.push_back( mgr );
				     } else {
					     delete mgr;
					     std::string msg = "PluginManager:LoadFromDir() invalid plugin " + fullname;
					     error::log( msg );
				     }
			     }
             }
        }
    }
	
    closedir( dir );
}


void nnet::PluginManager::Cleanup() {
    for( MListIt i = manager_list.begin(); i != manager_list.end(); i++ ) {
        DLLManager* factory = *i;
        delete factory;
    }
}


nnet::PluginManager::MList nnet::PluginManager::manager_list;

#ifndef BRAINLIB_PLUGINMANAGER_H
#define BRAINLIB_PLUGINMANAGER_H


#include <list>
#include <string>
#include "StringList.h"
#include "DLLManager.h"


namespace nnet {


    class PluginManager : public StringListBase {
		/// Class methods
    public:
		static void Initialize();
        static void LoadFromDir( std::string dir_name );
		static void Cleanup();
        
		/// Class data
    private:
        typedef std::list<DLLManager*> MList;
        typedef MList::iterator MListIt;
        static  MList manager_list;
    };


} // end namespace


#endif

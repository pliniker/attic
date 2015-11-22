#ifndef BRAINLIB_FILENAME_H
#define BRAINLIB_FILENAME_H


#include <sys/time.h>
#include <CString.h>


namespace nnet {


    /// Serializable string specialized for file names
    class FileName : public CString
    {
        /// Object methods
    public:
        FileName( std::string name );

        virtual std::string type();
        
        bool fileChanged();
        
        /// Object data
    private:
        std::string this_path;
        time_t      this_lastmod;
        
        /// Class methods
    public:
        static SerializablePtr Create( std::string name );
    };
    
    
    typedef RefCountPtr<FileName> FileNamePtr;
    
    
}//namespace brainlib


#endif

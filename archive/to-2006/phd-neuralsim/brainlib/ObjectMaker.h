#ifndef OBJECTMAKER_H
#define OBJECTMAKER_H


#include "OptionList.h"


namespace nnet {


    class ObjectMaker : public OptionList {
        /// Object methods
    public:
        ObjectMaker( std::string name );
        
        virtual std::string type();
        virtual void write( xmlNodePtr to );
        virtual void read( xmlNodePtr from );

        void setTarget( MapPtr target );
    
        virtual bool add( std::string item );
        virtual bool remove( std::string item );
        virtual void clear();
    
        /// Object data
    private:
        MapPtr target_map;
        
        /// Class methods
    public:
        static SerializablePtr Create( std::string name );
    };
    

    typedef RefCountPtr<ObjectMaker> ObjectMakerPtr;


} // namespace nnet


#endif

#ifndef BRAINLIB_OBJECTLIST_H
#define BRAINLIB_OBJECTLIST_H


#include "Map.h"
#include "OptionList.h"


namespace nnet {
	
	
	class ObjectList : public OptionList {
		/// Object methods
	public:
		ObjectList( std::string name );
		
		virtual std::string type();
		virtual void write( xmlNodePtr to );
		virtual void read( xmlNodePtr from );
		
		void setTarget( MapPtr target );
		
		bool add( std::string item );
		bool remove( std::string item );
		void clear();
		
		/// Object data;
	private:
		MapPtr target_map;
		
		/// Class methods
	public:
		static SerializablePtr Create( std::string name );
	};
	
	
	typedef RefCountPtr<ObjectList> ObjectListPtr;
	
}


#endif

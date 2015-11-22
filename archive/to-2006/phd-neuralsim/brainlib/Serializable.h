#ifndef BRAINLIB_SERIALIZABLE_H
#define BRAINLIB_SERIALIZABLE_H


#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "RefCountPtr.h"
#include "StringList.h"


namespace nnet { // TODO add exception handling
	
	
	class Serializable;
	typedef RefCountPtr<Serializable> SerializablePtr;
	typedef SerializablePtr ( *factory )( std::string );	
	
	
	/// Serializing data using XML 
	class Serializable {
		/// Object methods
	public:
		Serializable();
		virtual ~Serializable();
		
		virtual std::string name() = 0;
		virtual std::string type() = 0;
		virtual void        write( xmlNodePtr to ) = 0;
		virtual void        read( xmlNodePtr from ) = 0;
		
		bool isPrivate();
		bool isReadonly();
		bool isDirty();
		void setPrivate( bool status );
		void setReadonly( bool status );
		void clearDirty();
		
	protected:
		void setDirty( bool status );
		
		/// Object data
	private:
		bool reserved;
		bool viewonly;
		bool dirty;
		
		/// Class methods
	public:
		static void Initialize();
		static void Cleanup();

		static void AddFactory( std::string type, factory f );
		
		static bool            Serialize( SerializablePtr root, std::string fname );
		static SerializablePtr Unserialize( std::string fname );
		
		static SerializablePtr Create( std::string type, std::string name );
		
		static void            Serialize( SerializablePtr object, xmlNodePtr to );
		static SerializablePtr Unserialize( xmlNodePtr node );

	protected:
		static void            AddObject( SerializablePtr object );
		static SerializablePtr GetObject( std::string id );
		
		// Class data
	private:
		typedef std::map<std::string, factory, std::less<std::string> > FactoryMap;
		typedef FactoryMap::iterator FactoryMapIt;
		
		typedef std::map<std::string, SerializablePtr, std::less<std::string> > ObjectMap;
		typedef ObjectMap::iterator ObjectMapIt;
		
		static FactoryMap  factories;
		static ObjectMap   unique_objects;
		
		static StringList* factory_list;
		static StringList* base_types;
	};
	
	
} // namespace nnet


#endif

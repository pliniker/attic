#ifndef BRAINLIB_MAP_H
#define BRAINLIB_MAP_H


#include <map>
#include "Serializable.h"


namespace nnet {


    class Map;
    typedef RefCountPtr<Map> MapPtr;

	
	/// Super container
	class Map : public Serializable {
		/// Object methods
	public:
		typedef std::map<   std::string, 
                            SerializablePtr, 
                            std::less<std::string> > Obs;
		typedef Obs::iterator iterator;
        typedef Obs::const_iterator const_iterator;
        

        Map( std::string name );
		std::string name();
		
		virtual std::string type();
		virtual void write( xmlNodePtr to );
		virtual void read( xmlNodePtr from );

		
		iterator    begin();
		iterator    end();
		void        add( SerializablePtr object );
		void        erase( iterator it );
		void        erase( std::string what );
		std::string name( iterator it );
		std::string type( iterator it );
		SerializablePtr get( iterator it );
		SerializablePtr get( std::string what );
		SerializablePtr first();
		int         size();
		bool        exists( std::string what );
		bool        empty();
		void        clear();
		
        
		/// Object data
	private:
		std::string m_name;
		Obs         m_objects;
		

		/// Class methods
	public:
		static SerializablePtr Create( std::string name );
	};
	
	
} // namespace nnet


#endif


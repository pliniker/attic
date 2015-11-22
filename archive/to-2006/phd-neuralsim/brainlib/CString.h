#ifndef BRAINLIB_CSTRING_H
#define BRAINLIB_CSTRING_H


#include "Serializable.h"


namespace nnet {
	
	
	/// Serializable string type
	class CString : public Serializable {
		/// Object methods
	public:
		CString( std::string name );
		std::string name();
		
		virtual std::string type();
		virtual void write( xmlNodePtr to );	
		virtual void read( xmlNodePtr from );
		
		void         set( std::string name );
		std::string& get();
		
		/// Object data
	private:
		std::string this_name;
		std::string this_data;
		
		/// Class methods
	public:
		static SerializablePtr Create( std::string name );
	};
	
	
	/// Specialized definitions
	
	typedef RefCountPtr<CString> CStringPtr;
	
	
} // namespace nnet


#endif

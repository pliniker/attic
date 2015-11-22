#ifndef BRAINLIB_BOOLEAN_H
#define BRAINLIB_BOOLEAN_H


#include "Serializable.h"


namespace nnet {
	
	
	/// Serializable Integer type
	class Boolean : public Serializable {
		/// Object methods
	public:
		Boolean( std::string name );
		std::string name();
		
		virtual std::string type();
		virtual void write( xmlNodePtr to );	
		virtual void read( xmlNodePtr from );
		
		void set( bool name );
		bool get();
		
		/// Object data
	private:
		std::string m_name;
		bool        m_data;
		
		/// Class methods
	public:
		static SerializablePtr Create( std::string name );
	};
	
	
	/// Specialized definitions
	
	typedef RefCountPtr<Boolean> BooleanPtr;
	
	
} // namespace nnet


#endif


#ifndef BRAINLIB_OPTIONLIST_H
#define BRAINLIB_OPTIONLIST_H


#include <vector>
#include "StringList.h"
#include "Serializable.h"


namespace nnet {
	
	
	class OptionList : public Serializable, StringListBase {
		typedef std::vector<std::string> StrVec;
		typedef StrVec::iterator StrVecIt;
		
		/// Object methods
	public:
		OptionList( std::string name );
		std::string name();
		
		virtual std::string type();
		virtual void write( xmlNodePtr to );
		virtual void read( xmlNodePtr from );
		
		void setMax( int max );
		void setChoices( std::string list );
		
		virtual int  max();
        virtual int  size();
		virtual bool add( std::string item );
		virtual bool remove( std::string item );
		virtual void clear();
        virtual bool exists( std::string s );
        virtual const std::string& get( int n );
		
		StringList *choices();
		
		/// Object data;
	protected:
		std::string  choice_name;
		
	private:
		std::string  this_name;
		StringList*  choice_list;
		StrVec       picked_list;
		int          max_picks;
		
		/// Class methods
	public:
		static SerializablePtr Create( std::string name );
	};
	
	
	typedef RefCountPtr<OptionList> OptionListPtr;
	
	
}


#endif

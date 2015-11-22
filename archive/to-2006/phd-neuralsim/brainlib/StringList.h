#ifndef BRAINLIB_STRINGLIST_H
#define BRAINLIB_STRINGLIST_H


#include <map>
#include <string>
#include <vector>


namespace nnet
{

	// StringLists created internally for general use:
	//
	// "all_types"      - by Serializable
	// "base_types"     
	// "node_layers"    - by Brainlib
	// "edge_layers"
	// "neuron_arrays"
	// "synapse_arrays"
	// "exec_engines"	
    
    
    class StringListBase
    {
		/// Object methods
	public:
		virtual ~StringListBase();
		
        virtual int                 size() = 0;
        virtual std::string const&  get( int n ) = 0;
        virtual bool                exists( std::string s ) = 0;
		
        std::string const&          operator[] ( int n );
    };
	
	
	class StringList : public StringListBase
    {		
		/// Object methods
	public:
		~StringList();
		int                         size();
		std::string const&          get( int n );
		bool                        exists( std::string s );
		void                        add( std::string s );
		void                        remove( std::string s );
	
		/// Object data;
	private:
		typedef std::vector<std::string> slist;
		typedef slist::iterator slist_it;
		slist   strings;
    };


    namespace string
    {
        // functions for manipulating the internal database of stringlists
	    StringList*  new_list( std::string name );
    	void         add_list( std::string name, StringList* list );
	    void         erase_list( std::string name );
    	StringList*  get_list( std::string name );


        // generally useful. an empty string.
    	extern std::string const null;
    }
	
	
} // end namespace


#endif

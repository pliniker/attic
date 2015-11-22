

#include "StringList.h"


typedef std::map<   std::string, 
                    nnet::StringList*, 
                    std::less<std::string> > slistmap;
typedef slistmap::iterator slistmap_it;


static slistmap     string_lists;


nnet::StringListBase::~StringListBase() {}


std::string const& nnet::StringListBase::operator[] ( int n ) {
    return get( n );
}


nnet::StringList::~StringList() {}


int nnet::StringList::size() {
	return strings.size();
}


const std::string& nnet::StringList::get( int n ) {
	return strings.at( n );
}


bool nnet::StringList::exists( std::string s ) {
	for( slist_it i = strings.begin(); i != strings.end(); i++ ) {
		if( *i == s ) {
			return true;
		}
	}
	return false;
}


void nnet::StringList::add( std::string s ) {
	if( !exists( s ) ) 
		strings.push_back( s );
}


void nnet::StringList::remove( std::string s ) {
	for( slist_it i = strings.begin(); i != strings.end(); i++ ) {
		if( *i == s ) {
			strings.erase( i );
			return;
		}
	}
}


nnet::StringList* nnet::string::new_list( std::string name ) {
	StringList* existing = string_lists[name];
	if( existing == 0 ) {
		StringList* list = new StringList;
		string_lists[name] = list;
		return list;
	} else {
		return existing;
	}
}


void nnet::string::add_list( std::string name, StringList* list ) {
	string_lists[name] = list;
}


void nnet::string::erase_list( std::string name ) {
	string_lists[name] = 0;
}


nnet::StringList* nnet::string::get_list( std::string name ) {
	return new_list( name );
}


std::string const nnet::string::null = "";


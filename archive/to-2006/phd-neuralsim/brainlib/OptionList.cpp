

#include "XmlFile.h"
#include "OptionList.h"



nnet::OptionList::OptionList( std::string name ) {
	this_name = name;
	max_picks = 1;
}


std::string nnet::OptionList::name() {
	return this_name;
}


std::string nnet::OptionList::type() {
	return "OptionList";
}


void nnet::OptionList::write( xmlNodePtr to ) {
	XmlFile::WriteString( to, "choicesList", choice_name );
	XmlFile::WriteLong( to, "size", ( long )picked_list.size() );
	XmlFile::WriteLong( to, "max", ( long )max_picks );
	
	for( int i = 0; i < ( int )picked_list.size(); i++ ) {
		std::ostringstream stringid;
		stringid << "string" << i;
		XmlFile::WriteString( to, stringid.str(), picked_list[i] );
	}
}


void nnet::OptionList::read( xmlNodePtr from ) {
	if( from ) {
		choice_name = XmlFile::ReadString( from, "choicesList" );
		choice_list = string::get_list( choice_name );
		
		int list_size = XmlFile::ReadLong( from, "size" );
		max_picks =     XmlFile::ReadLong( from, "max" );
		
		for( int i = 0; i < list_size; i++ ) {
			std::ostringstream stringid;
			stringid << "string" << i;
			std::string value = XmlFile::ReadString( from, stringid.str() );
			picked_list.push_back( value );
		}
	}
}


void nnet::OptionList::setMax( int max ) {
	max_picks = max;
	setDirty( true );
}


void nnet::OptionList::setChoices( std::string list ) {
	choice_list = string::get_list( list );
	choice_name = list;
	setDirty( true );
}


int nnet::OptionList::max() {
	return max_picks;
}


bool nnet::OptionList::add( std::string item ) {
	if( picked_list.size() >= ( unsigned int )max_picks ) {
		return false;
	} else {
		picked_list.push_back( item );
		setDirty( true );
		return true;
	}
}


bool nnet::OptionList::remove( std::string item ) {
	for( StrVecIt i = picked_list.begin(); i != picked_list.end(); i++ ) {
		if( *i == item ) {
			picked_list.erase( i );
			setDirty( true );
			return true;
		}
	}
	return false;
}


void nnet::OptionList::clear() {
	picked_list.clear();
	setDirty( true );
}


int nnet::OptionList::size() {
	return picked_list.size();
}


bool nnet::OptionList::exists( std::string s ) {
	for( StrVecIt i = picked_list.begin(); i != picked_list.end(); i++ ) {
		if( *i == s ) {
			return true;
		}
	}
	return false;
}


const std::string& nnet::OptionList::get( int n ) {
	return picked_list[n];
}


nnet::StringList* nnet::OptionList::choices() {
	return choice_list;
}


nnet::SerializablePtr nnet::OptionList::Create( std::string name ) {
	return RefCountPtr<OptionList>( new OptionList( name ) );
}


#ifndef BRAINLIB_XMLFILE_H
#define BRAINLIB_XMLFILE_H


#include <libxml/parser.h>
#include <libxml/tree.h>
#include <string>


namespace nnet {
	
	
	class XmlFile {
		// Class methods - public
	public:
		// functions for use by Serializable classes only		
		static long        ReadLong    ( xmlNodePtr node, std::string name );
		static double      ReadDouble  ( xmlNodePtr node, std::string name );
		static bool        ReadBoolean ( xmlNodePtr node, std::string name );
		static std::string ReadString  ( xmlNodePtr node, std::string name );
		
		static void        WriteLong   (    xmlNodePtr node,
                                            std::string name, 
                                            long value );
		static void        WriteDouble (    xmlNodePtr node, 
                                            std::string name, 
                                            double value );
		static void        WriteBoolean(    xmlNodePtr node, 
                                            std::string name, 
                                            bool value );
		static void        WriteString (    xmlNodePtr node, 
                                            std::string name, 
                                            std::string value );
		
		// Class methods - private
	private:
		static void        NewString(   xmlNodePtr node,
                                        std::string name, 
                                        std::string value );
		static std::string GetString( xmlNodePtr node, std::string name );
	};
	
	
} //end namespace


#endif

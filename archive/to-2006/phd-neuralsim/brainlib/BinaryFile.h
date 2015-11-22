#ifndef BRAINLIB_BINARYFILE_H
#define BRAINLIB_BINARYFILE_H


#include <cstdio>
#include <string>


namespace nnet {
	

	class BinaryFile {
		
		// Class methods - public
	public:
		// functions for use by Serializable classes only		
		static bool OpenRead( std::string name );
		static bool OpenWrite( std::string name );
		static bool Close();		
		
		static void ReadChar   ( char* c, int size );
		static void ReadShort  ( short* s, int size );
		static void ReadInt    ( int* i, int size );
		static void ReadLong   ( long* l, int size );
		static void ReadFloat  ( float* f, int size );
		static void ReadDouble ( double* d, int size );
		static void SetPos     ( long seek );
		
		static void WriteChar  ( char* c, int size );
		static void WriteShort ( short* s, int size );
		static void WriteInt   ( int* i, int size );
		static void WriteLong  ( long* l, int size );
		static void WriteFloat ( float* f, int size );
		static void WriteDouble( double* d, int size );
		static long GetPos     ();
		
		// Class data - private
	private:
		static std::string m_error_string;
		static FILE*       m_file_handle;
	};
	
	
} //end namespace


#endif



#ifdef __APPLE_CC__
#include <arpa/inet.h>
#else
#include <netinet/in.h>
#endif


#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "ErrorInterface.h"
#include "BinaryFile.h"



bool nnet::BinaryFile::OpenRead( std::string name ) {
	m_file_handle = fopen( name.c_str(), "r" );
	if( !m_file_handle ) {
		m_error_string = "BinaryFile::OpenRead() cannot read binary data file " + name;
		error::log( m_error_string );
		return false;
	} else {
		return true;
	}
}


bool nnet::BinaryFile::OpenWrite( std::string name ) {
	m_file_handle = fopen( name.c_str(), "w+" );
	if( !m_file_handle ) {
		m_error_string = "BinaryFile::OpenWrite() cannot write to binary data file " + name;
		error::log( m_error_string );
		return false;
	} else {
		return true;
	}
}


bool nnet::BinaryFile::Close() {
	if( m_file_handle ) 
		fclose( m_file_handle );

	m_file_handle = 0;
	return true;
}


void nnet::BinaryFile::ReadChar( char* c, int size ) {
	if( c && m_file_handle ) fread( c, 1, size, m_file_handle );
}


void nnet::BinaryFile::ReadShort( short* s, int size ) {
	if( s && m_file_handle ) {
		short* buffer = new short[size];
		fread( buffer, sizeof( short ), size, m_file_handle );
		for( int i = 0; i < size; ++i ) {
			s[i] = ntohs( buffer[i] );
		}
		delete [] buffer;		
	}
}


void nnet::BinaryFile::ReadInt( int* i, int size ) {
	if( i && m_file_handle ) {
		int* buffer = new int[size];
		fread( buffer, sizeof( int ), size, m_file_handle );
		for( int j = 0; j < size; ++j ) {
			i[j] = ntohl( buffer[j] );
		}
		delete [] buffer;		
	}
}


void nnet::BinaryFile::ReadLong( long* l, int size ) {
	if( l && m_file_handle ) {
		long* buffer = new long[size];
		fread( buffer, sizeof( long ), size, m_file_handle );
		for( int i = 0; i < size; ++i ) {
			l[i] = ntohl( buffer[i] );
		}
		delete [] buffer;		
	}
}


void nnet::BinaryFile::ReadFloat( float* f, int size ) {
	if( f && m_file_handle ) {
		float* buffer = new float[size];
		fread( buffer, sizeof( float ), size, m_file_handle );
		for( int i = 0; i < size; ++i ) {
            int temp1, temp2;
            *( (float* )( &temp1 )) = buffer[i];
			temp2 = ntohl( temp1 );
            f[i] = *( float* )( &temp2 );
		}
		delete [] buffer;
	}
}


void nnet::BinaryFile::ReadDouble( double* d, int size ) {
	if( d && m_file_handle ) {
		double* buffer = new double[size];
		fread( buffer, sizeof( double ), size, m_file_handle );
		for( int i = 0; i < size; ++i ) {
			int temp1[2], temp2[2];
			*( (double* )temp1 ) = buffer[i];
			temp2[1] = ntohl( temp1[0] );
			temp2[0] = ntohl( temp1[1] );
			d[i] = *( double* )temp2;
		}
		delete [] buffer;
	}
}		


void nnet::BinaryFile::SetPos( long seek ) {
	if( m_file_handle ) {
		fseek( m_file_handle, seek, SEEK_SET );
	}
}


void nnet::BinaryFile::WriteChar( char* c, int size ) {
	if( c && m_file_handle ) fwrite( c, 1, size, m_file_handle );
}


void nnet::BinaryFile::WriteShort( short* s, int size ) {
	if( s && m_file_handle ) {
		short* buffer = new short[size];
		for( int i = 0; i < size; ++i ) {
			buffer[i] = htons( s[i] );
		}
		fwrite( buffer, sizeof( short ), size, m_file_handle );
		delete[]  buffer;		
	}
}


void nnet::BinaryFile::WriteInt( int* i, int size ) {
	if( i && m_file_handle ) {
		int* buffer = new int[size];
		for( int j = 0; j < size; ++j ) {
			buffer[j] = htonl( i[j] );
		}
		fwrite( buffer, sizeof( int ), size, m_file_handle );
		delete [] buffer;		
	}
}


void nnet::BinaryFile::WriteLong( long* l, int size ) {
	if( l && m_file_handle ) {
		long* buffer = new long[size];
		for( int i = 0; i < size; ++i ) {
			buffer[i] = htonl( l[i] );
		}
		fwrite( buffer, sizeof( long ), size, m_file_handle );
		delete [] buffer;		
	}
}


void nnet::BinaryFile::WriteFloat( float* f, int size ) {
	if( f && m_file_handle ) {
		float* buffer = new float[size];
		for( int i = 0; i < size; ++i ) {
        int temp1, temp2;
            *( (float* )( &temp1 )) = f[i];
            temp2 = htonl( temp1 );
            buffer[i] = *( float* )( &temp2 );
		}
		fwrite( buffer, sizeof( float ), size, m_file_handle );
		delete [] buffer;
	}
}


void nnet::BinaryFile::WriteDouble( double* d, int size ) {
	if( d && m_file_handle ) {
		double* buffer = new double[size];
		for( int i = 0; i < size; ++i ) {
			int temp1[2], temp2[2];
			*( (double* )temp1 ) = d[i];
			temp2[1] = htonl( temp1[0] );
			temp2[0] = htonl( temp1[1] );
			buffer[i] = *( double* )temp2;
		}
		fwrite( buffer, sizeof( double ), size, m_file_handle );
		delete [] buffer;
	}
}		


long nnet::BinaryFile::GetPos() {
	if( m_file_handle ) {
		return ftell( m_file_handle );
	} else {
		return 0;
	}
}


std::string nnet::BinaryFile::m_error_string;
FILE*		nnet::BinaryFile::m_file_handle = 0;


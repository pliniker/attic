

#include "ErrorInterface.h"
#include "XmlFile.h"
#include "BinaryFile.h"
#include "Common.h"
#include "Image.h"


nnet::Image::Image( std::string name ) :
this_name( name ), 
this_data() {
	create( 16, 16, Gray8bpp );
}


std::string nnet::Image::name() {
	return this_name;
}


std::string nnet::Image::type() {
	return "Image";
}


void nnet::Image::write( xmlNodePtr to ) {
	XmlFile::WriteLong( to, "encoding", ( long )this_enc );
	XmlFile::WriteLong( to, "x", ( long )xPixels() );
	XmlFile::WriteLong( to, "y", ( long )this_y );
	XmlFile::WriteLong( to, "z", ( long )this_z );
// 	XmlFile::WriteLong( to, "size", ( long )this_size );
	XmlFile::WriteLong( to, "binpos", BinaryFile::GetPos() );
	if( this_size ) 
		BinaryFile::WriteChar( ( char* )&this_data[0], this_size );
}


void nnet::Image::read( xmlNodePtr from ) {
	this_enc = ( ColorEncoding )XmlFile::ReadLong( from, "encoding" );
	this_x =    XmlFile::ReadLong( from, "x" );
	this_y =    XmlFile::ReadLong( from, "y" );
	this_z =    XmlFile::ReadLong( from, "z" );
// 	this_size = XmlFile::ReadLong( from, "size" );
	
	long temp = XmlFile::ReadLong( from, "binpos" );
	BinaryFile::SetPos( temp );
	
	if( this_size ) {
		create( this_x, this_y, this_z, this_enc );
		BinaryFile::ReadChar( ( char* )&this_data[0], this_size );
	}
}


void nnet::Image::create( int xres, int yres, ColorEncoding enc ) {
	create( xres, yres, 1, enc );
}


void nnet::Image::create( int xres, int yres, int zres, ColorEncoding enc ) {
	if( xres && yres && zres ) {
		this_enc = enc;
		this_x = xres;
		this_y = yres;
		this_z = zres;
		
		switch( this_enc ) {
			case Gray1bpp:      this_x /= 8; break;
			case Gray8bpp:      break;
			case Color8bpp332:  break;
			case Color24bpp:    this_x *= 3; break;
			default:            break;
		}

		this_size = this_x * this_y * this_z;
        
		this_data.reserve( this_size );
		std::vector<unsigned char>( this_data ).swap( this_data ); // make sure capacity == size
		this_data.resize( this_size );
		
		for( int i = 0; i < this_size; i++ ) { this_data.at( i ) = 255; }

		setDirty( true );
	}
}


void nnet::Image::convertToRGBA( unsigned char* out ) {
    if( this_size ) {
		switch( this_enc ) {
			case Gray1bpp:      convertGray1bpp( out );   return;
			case Gray8bpp:      convertGray8bpp( out );   return;
			case Color8bpp332:  convertColor8bpp( out );  return;
			case Color24bpp:    convertColor24bpp( out ); return;
			default:            return;
		}
	}
}


void nnet::Image::convertFromRGBA( unsigned char* in ) {
    if( this_size ) {
		switch( this_enc ) {
			case Gray1bpp:      unconvertGray1bpp( in );   return;
			case Gray8bpp:      unconvertGray8bpp( in );   return;
			case Color8bpp332:  unconvertColor8bpp( in );  return;
			case Color24bpp:    unconvertColor24bpp( in ); return;
			default:            return;
		}
	}
}


nnet::Image::ColorEncoding nnet::Image::encoding() {
	return this_enc;
}


int nnet::Image::x() {
	return this_x;
}


int nnet::Image::y() {
	return this_y;
}


int nnet::Image::z() {
	return this_z;
}


int nnet::Image::size() {
	return this_size;
}


int nnet::Image::xPixels() {
	switch( this_enc ) {
		case Gray1bpp:      return this_x * 8;
		case Gray8bpp:      return this_x;
		case Color8bpp332:  return this_x;
		case Color24bpp:    return this_x / 3;
		default:            return this_x;
	}
}


int nnet::Image::bpp() {
	switch( this_enc ) {
		case Gray1bpp:      return 1;
		case Gray8bpp:      return 8;
		case Color8bpp332:  return 8;
		case Color24bpp:    return 24;
		default:            return 8;
	}
}


void nnet::Image::convertGray1bpp( unsigned char* out ) {
	int size = this_x * this_y;
	unsigned char* in = &this_data[0];
	for( int i = 0; i < size; i++ ) {
		out[i * 32 +  0] = out[i * 32 +  1] = out[i * 32 +  2] = ( in[i] & 0x80 );
		out[i * 32 +  4] = out[i * 32 +  5] = out[i * 32 +  6] = ( in[i] & 0x40 ) << 1;
		out[i * 32 +  8] = out[i * 32 +  9] = out[i * 32 + 10] = ( in[i] & 0x20 ) << 2;
		out[i * 32 + 12] = out[i * 32 + 13] = out[i * 32 + 14] = ( in[i] & 0x10 ) << 3;
		out[i * 32 + 16] = out[i * 32 + 17] = out[i * 32 + 18] = ( in[i] & 0x08 ) << 4;
		out[i * 32 + 20] = out[i * 32 + 21] = out[i * 32 + 22] = ( in[i] & 0x04 ) << 5;
		out[i * 32 + 24] = out[i * 32 + 25] = out[i * 32 + 26] = ( in[i] & 0x02 ) << 6;
		out[i * 32 + 28] = out[i * 32 + 29] = out[i * 32 + 30] = ( in[i] & 0x01 ) << 7;
		out[i * 32 +  3] = 255;
		out[i * 32 +  7] = 255;
		out[i * 32 + 11] = 255;
		out[i * 32 + 15] = 255;
		out[i * 32 + 19] = 255;
		out[i * 32 + 23] = 255;
		out[i * 32 + 27] = 255;
		out[i * 32 + 31] = 255;
	}
}


void nnet::Image::convertGray8bpp( unsigned char* out ) {
	int size = this_x * this_y;
	unsigned char* in = &this_data[0];
	for( int i = 0; i < size; i++ ) {
		out[i * 4 + 0] = out[i * 4 + 1] = out[i * 4 + 2] = Common::HdistLookup[ in[i] ] * 31;
		out[i * 4 + 3] = 255;
	}
}


void nnet::Image::convertColor8bpp( unsigned char* out ) {
	int size = this_x * this_y;
    unsigned char* in = &this_data[0];
    unsigned char byte;
    for( int i = 0; i < size; i++ ) {
        byte = in[i];
        out[i * 4 + 0] = Common::HdistLookup[( byte & 0xe0 )] * 36;
        out[i * 4 + 1] = Common::HdistLookup[( byte & 0x1c )] * 36;
        out[i * 4 + 2] = Common::HdistLookup[( byte & 0x03 )] * 85;
        out[i * 4 + 3] = 255;
    }
}


void nnet::Image::convertColor24bpp( unsigned char* out ) {
	int size = xPixels() * this_y;
    unsigned char* in = &this_data[0];
    for( int i = 0; i < size; i++ ) {
        out[i * 4 + 0] = Common::HdistLookup[ in[i * 3 + 0] ] * 31;
        out[i * 4 + 1] = Common::HdistLookup[ in[i * 3 + 1] ] * 31;
        out[i * 4 + 2] = Common::HdistLookup[ in[i * 3 + 2] ] * 31;
        out[i * 4 + 3] = 255;
    }
}


void nnet::Image::unconvertGray1bpp( unsigned char* in ) {
	int size = this_x * this_y;
	unsigned char* out = &this_data[0];
	unsigned char byte;
	for( int i = 0; i < size; i++ ) {
		byte = 0;
		byte |= ( (in[i * 32 +  0] + in[i * 32 +  1] + in[i * 32 +  2] ) / 3 ) & 0x80;
		byte |= ( (in[i * 32 +  4] + in[i * 32 +  5] + in[i * 32 +  6] ) / 3 ) & 0x80 >> 1;
		byte |= ( (in[i * 32 +  8] + in[i * 32 +  9] + in[i * 32 + 10] ) / 3 ) & 0x80 >> 2;
		byte |= ( (in[i * 32 + 12] + in[i * 32 + 13] + in[i * 32 + 14] ) / 3 ) & 0x80 >> 3;
		byte |= ( (in[i * 32 + 16] + in[i * 32 + 17] + in[i * 32 + 18] ) / 3 ) & 0x80 >> 4;
		byte |= ( (in[i * 32 + 20] + in[i * 32 + 21] + in[i * 32 + 22] ) / 3 ) & 0x80 >> 5;
		byte |= ( (in[i * 32 + 24] + in[i * 32 + 25] + in[i * 32 + 26] ) / 3 ) & 0x80 >> 6;
		byte |= ( (in[i * 32 + 28] + in[i * 32 + 29] + in[i * 32 + 30] ) / 3 ) & 0x80 >> 7;
		out[i] = byte;
	}
}


void nnet::Image::unconvertGray8bpp( unsigned char* in ) {
	int size = this_x * this_y;
    unsigned char* out = &this_data[0];
    for( int i = 0; i < size; i++ ) {
		out[i] = Common::BitGradient[ ( in[i * 4 + 1] + 
										in[i * 4 + 2] + 
										in[i * 4 + 3] ) / 93  ]; // 93 so that the result divides into 0 - 8 equal slots
    }
}


void nnet::Image::unconvertColor8bpp( unsigned char* in ) {
	int size = this_x * this_y;
    unsigned char* out = &this_data[0];
    for( int i = 0; i < size; i++ ) {
		out[i] = ( Common::BitGradient[ in[i * 4 + 1] / 36 ] << 5 ) |
				 ( Common::BitGradient[ in[i * 4 + 2] / 36 ] << 2 ) |
				 ( Common::BitGradient[ in[i * 4 + 3] / 85 ] );
    }
}


void nnet::Image::unconvertColor24bpp( unsigned char* in ) {
	int size = xPixels() * this_y;
    unsigned char* out = &this_data[0];
    for( int i = 0; i < size; i++ ) {
        out[i * 3 + 0] = Common::BitGradient[ in[i * 4 + 1] / 31 ];
        out[i * 3 + 1] = Common::BitGradient[ in[i * 4 + 2] / 31 ];
        out[i * 3 + 2] = Common::BitGradient[ in[i * 4 + 3] / 31 ];
    }
}



nnet::SerializablePtr nnet::Image::Create( std::string name ) {
	return RefCountPtr<Image>( new Image( name ) );
}

#ifndef BRAINLIB_IMAGE_H
#define BRAINLIB_IMAGE_H


#include <vector>
#include "Serializable.h"


namespace nnet {
	
	
	class Image : public Serializable {
		/// Object subdefinitions
	public:
		enum ColorEncoding {
			Gray1bpp,
			Gray8bpp,
			Color8bpp332,
			Color24bpp
		};
		
		/// Object methods
	public:
		Image( std::string name );
		std::string name();
		
		virtual std::string type();
		virtual void write( xmlNodePtr to );
		virtual void read( xmlNodePtr from );
		
		void create( int xres, int yres, ColorEncoding enc );
		void create( int xres, int yres, int zres, ColorEncoding enc );
		
		void convertToRGBA( unsigned char* out );
		void convertFromRGBA( unsigned char* in );
		
		unsigned char& at( int index );     // bounds checked array access
		ColorEncoding  encoding();
		int            x();               // x res in chars, not pixels
		int            y();
		int            z();
		int            size();
		int            xPixels();         // x res in pixels
		int            bpp();
		
		/// Object methods - private
	private:
		void convertGray1bpp( unsigned char* out );
		void convertGray8bpp( unsigned char* out );
		void convertColor8bpp( unsigned char* out );
		void convertColor24bpp( unsigned char* out );

		void unconvertGray1bpp( unsigned char* in );
		void unconvertGray8bpp( unsigned char* in );
		void unconvertColor8bpp( unsigned char* in );
		void unconvertColor24bpp( unsigned char* in );
		
		/// Object data
	private:
		std::string this_name;
		std::vector<unsigned char> this_data;
		ColorEncoding this_enc;
		int  this_size;
		int  this_x;
		int  this_y;
		int  this_z;
		
		
		/// Class methods
	public:
		static SerializablePtr Create( std::string name );
	};
	
	
	/// Inline functions
	inline unsigned char& Image::at( int index ) {
		return this_data.at( index );
	}
	
	
	typedef RefCountPtr<Image> ImagePtr;
	
}


#endif

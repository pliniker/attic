#ifndef BRAINLIB_COMMON_H
#define BRAINLIB_COMMON_H


#include <limits>


namespace nnet {


    class Common {
		/// Class methods
    public:
        typedef unsigned char Unit;
        static int UnitBits;
		static int UnitMax;

        static int IntMax;
        static int MaxResponse(int bits);
				
        static Unit HdistLookup[];
		static Unit BitGradient[];
    };


    typedef Common::Unit Unit;


} // end namespace


#endif

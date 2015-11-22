#ifndef BRAINLIB_RANDOMINT_H
#define BRAINLIB_RANDOMINT_H


#include <ctime>
#include <cstdlib>
#include <limits>


namespace nnet {


    class RandomInt {
		/// Class methods
    public:
        static void     Initialize();
        static size_t   Get();
        static size_t   Max;

        // each call to this function overwrites the internal array
        static size_t*  UniqueSequence( size_t size );
    };


} // end namespace


#endif


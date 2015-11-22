
#include <vector>
#include <algorithm>

#include "RandomInt.h"



void nnet::RandomInt::Initialize() 
{
    std::srand(std::time(NULL));
    Max = RAND_MAX;
}


size_t nnet::RandomInt::Get() 
{
    return rand();
}


size_t nnet::RandomInt::Max;


size_t* nnet::RandomInt::UniqueSequence( size_t size )
{
    static std::vector<size_t> randomSequence;

    randomSequence.resize( size );

    size_t count = 0;
    while( count < size )
    {
        randomSequence[ count ] = count;
        count++;
    }

    count = 0;
    while( count < size )
    {
        size_t first    = count;
        size_t second   = rand() % size;

        if( randomSequence[ first ] == first )
        {
            std::swap( randomSequence[ first ], randomSequence[ second ] );
        }

        count++;
    }

    return &randomSequence[ 0 ];
}


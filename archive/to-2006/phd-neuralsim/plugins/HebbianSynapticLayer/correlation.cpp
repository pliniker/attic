
#include <iostream>
#include <iomanip>
#include <cmath>

//#define NODUMP 1
#include "Correlation.hpp"


void correlate_binary()
{
    float a, b, c, d;

    std::cout << "Give me 4 numbers a b c and d in the form " << std::endl
        << " inputs/outputs 0 1" << std::endl
        << "              0 a b" << std::endl
        << "              1 c d" << std::endl;

    std::cin >> a >> b >> c >> d;

    std::cout << "You gave me" << std::endl
        << " inputs/outputs\t0\t1" << std::endl
        << "              0\t" << a << "\t" << b << std::endl
        << "              1\t" << c << "\t" << d << std::endl;

    float normi_a, normi_b, normi_c, normi_d;
    float normo_a, normo_b, normo_c, normo_d;

    normi_a = (a == b) ? 0.5f : ( a / (a + b) );
    normi_b = (a == b) ? 0.5f : ( b / (a + b) );
    normi_c = (c == d) ? 0.5f : ( c / (c + d) );
    normi_d = (c == d) ? 0.5f : ( d / (c + d) );

    normo_a = (a == c) ? 0.5f : ( a / (a + c) );
    normo_b = (b == d) ? 0.5f : ( b / (b + d) );
    normo_c = (a == c) ? 0.5f : ( c / (a + c) );
    normo_d = (b == d) ? 0.5f : ( d / (b + d) );

    std::cout << "Normalised input rows are" << std::endl
        << " inputs/outputs\t0\t1" << std::endl << std::setw(4)
        << "              0\t" << normi_a << "\t" << normi_b << std::endl
        << "              1\t" << normi_c << "\t" << normi_d << std::endl;    
    
    std::cout << "Normalised output columns are" << std::endl
        << " inputs/outputs\t0\t1" << std::endl << std::setw(4)
        << "              0\t" << normo_a << "\t" << normo_b << std::endl
        << "              1\t" << normo_c << "\t" << normo_d << std::endl;


    float icorrelation = 1.0f;

    float temp =    fmaxf( normi_a, normi_b ) - 
                    fminf( normi_a, normi_b );
    //temp *= temp;
    icorrelation *= temp;
    std::cout << "Input norm 0 = " << temp << std::endl;

    temp =          fmaxf( normi_c, normi_d ) -
                    fminf( normi_c, normi_d );
    //temp *= temp;
    icorrelation *= temp;
    std::cout << "Input norm 1 = " << temp << std::endl;

    float ocorrelation = 1.0f;

    temp =          fmaxf( normo_a, normo_c ) -
                    fminf( normo_a, normo_c );
    //temp *= temp;
    ocorrelation *= temp;
    std::cout << "Output norm 0 = " << temp << std::endl;
    
    temp =          fmaxf( normo_b, normo_d ) -
                    fminf( normo_b, normo_d );
    //temp *= temp;
    ocorrelation *= temp;
    std::cout << "Output norm 1 = " << temp << std::endl;

    float correlation = icorrelation * ocorrelation;
    correlation = std::sqrt( correlation );

    std::cout << "Correlation is " << correlation << std::endl;
}


int main( int argc, char* argv[] )
{
    correlation_t table;
    
    /*
    table.set_entry( 0, 0,  5 );
    table.set_entry( 1, 0,  5 );
    table.set_entry( 2, 0,  9 );

    table.set_entry( 0, 1,  5 );
    table.set_entry( 1, 1,  5 );
    table.set_entry( 2, 1,  9 );
    table.set_entry( 3, 3, 10 );
    table.set_entry( 4, 4, 10 );
    */
    /*
    table.set_entry( 0, 0, 1 );
    table.set_entry( 1, 1, 1 );
    table.set_entry( 2, 2, 1 );
    table.set_entry( 3, 3, 1 );
    */

    table.increment( 0, 0 );
    table.increment( 1, 1 );
    table.increment( 2, 2 );

    std::cout 
        << std::endl
        << "Correlation is " 
        << table.calculate() 
        << std::endl << std::endl;

#ifndef NODUMP
    table.dump();
#endif

    //correlate_binary();

    return 0;
}


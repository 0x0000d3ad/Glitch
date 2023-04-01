#include "RandomFunction.h"

static MathLib::Random::MersenneTwister mt;

unsigned int choose_rand( const unsigned int & size )
{
        return mt.Generate( 0, size );
}

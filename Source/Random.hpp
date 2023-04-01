///////////////////////////////////////////////////////////////////////////////////////////////////
// Random number generator class
// For mersenne twister implemtation, consulted the work of Makoto Matsumoto and Takuji Nishimura,
// Copyright (C) 1997 - 2002, 
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef KT_RANDOM_NUMBER_GENERATOR_H
#define KT_RANDOM_NUMBER_GENERATOR_H

#include <assert.h>
#include <cmath>

// parameters for Mersenne Twistor
#define SIZE_STATE_VECTOR 624
#define M 397
#define MATRIX_A         0x9908b0dfUL // constant vector a
#define BIT_MASK         0xffffffffUL
#define UPPER_MASK       0x80000000UL // most significant w-r bits
#define LOWER_MASK 	 0x7fffffffUL // least significant r bits
#define KNUTH_MULTIPLIER 1812433253UL // multiplier from Donald Knuth TAOCP vol 2, 3rd. Ed. p.106
#define MULTIPLIER_1     1664525UL
#define MULTIPLIER_2     1566083941UL

// base used for scaling
#define DEFAULT_BASE 100000

namespace MathLib
{

///////////////////////////////////////////////////////////////////////////////////////////////////
// KT implementation of random number generator class
///////////////////////////////////////////////////////////////////////////////////////////////////
namespace Random
{

///////////////////////////////////////////////////////////////////////////////////////////////////
// Define read time stamp counter for Windoze and Linux
// Define cursory random number generator
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef WIN32
        inline long long read_time_stamp()
        {
		register long long TSC asm("eax") = 0;
		asm volatile (".byte 15, 49" : : : "eax", "edx");
		return TSC;
        }

	inline unsigned int random_number_generator( void )
	{
		unsigned int output = 0;
		unsigned int r_seed = read_time_stamp();

		// if r_seed == 0, grap the time stamp counter,
		// else use r_seed value to seed	
		if ( ! r_seed )
		{
			register long long TSC asm("eax") = 0;
			__asm__ __volatile__  ( ".byte 15, 49;		\n"
						"imul  $13, %%eax;	\n"	// multiply by 13
						"movl  %%eax, %%edx;	\n"	// mov eax into edx
						"shl   $16, %%eax;	\n"	// shift left 16 bits
						"shr   $16, %%edx;	\n"	// shift left 16 bits
						"movw  %%dx, %%ax;	\n"	// mov dx to ax (swapping segments)
						"movl  %%eax, %0;	\n"	// load into result	   						   
						: "=r"(output)
						: 
						: "eax", "edx");
			return TSC;
		}
		else
		{
			__asm__ __volatile__ ( 	"movl  %1, %%eax;	\n" 
						"imul  $13, %%eax;	\n"	// multiply by 13
						"movl  %%eax, %%edx;	\n"	// mov eax into edx
						"shl   $16, %%eax;	\n"	// shift left 16 bits
						"shr   $16, %%edx;	\n"	// shift left 16 bits
						"movw  %%dx, %%ax;	\n"	// mov dx to ax (swapping segments)
						"movl  %%eax, %0;	\n"	// load into result	   						   
						: "=r"(output)
						: "r"(r_seed) 
						: "eax", "edx");
		}
		
		return output;
	}
	inline double random_number_generator( const double & min, const double & max )
	{
		return ( max - min ) * ( ( double ) ( ( random_number_generator() % ( DEFAULT_BASE + 1 ) ) / DEFAULT_BASE ) );
	}
#else
        __declspec( naked )
        inline long __cdecl read_time_stamp()
        {
                __asm
                {
                        rdtsc;
                        ret;
                }
        }

	inline unsigned int random_number_generator( void )
	{
		unsigned int output = 0;
		long r_seed = read_time_stamp();

		if ( ! r_seed )
		{
			__asm
			{
				rdtsc;
				imul eax, 0x0d;
				mov  eax, edx;
				shl  eax, 0x0F;
				shr  edx, 0x0F;
				mov  ax, dx;
				mov  output, eax;
			}
		}
		else
		{
			__asm
			{
				mov  eax, r_seed;
				imul eax, 0x0d;
				mov  eax, edx;
				shl  eax, 0x0F;
				shr  edx, 0x0F;
				mov  ax, dx;
				mov  output, eax;
			}
		}
		return output;
	}

	inline double random_number_generator( const double & min, const double & max )
	{
		return ( max - min ) * 
		( ( ( ( double ) ( random_number_generator() % ( DEFAULT_BASE + 1 ) ) )
			/ ( ( double ) DEFAULT_BASE ) ) );
	}
#endif 	// WIN32

///////////////////////////////////////////////////////////////////////////////////////////////////
// Parent class for random number generator
///////////////////////////////////////////////////////////////////////////////////////////////////
class ParentRandomNumberGenerator 
{
	public :

		ParentRandomNumberGenerator( void ) : seed_( ( unsigned int ) read_time_stamp() ) {}

		ParentRandomNumberGenerator( const unsigned int & seed ) : seed_( seed ) {};

		virtual ~ParentRandomNumberGenerator( void ) {}

		virtual inline void SetSeed( const unsigned int & seed ) { this->seed_ = seed; }

		virtual inline void SetSeed( void ) { this->seed_ = read_time_stamp(); }

		virtual inline unsigned int GetSeed( void ) { return this->seed_; }

		virtual inline unsigned int Generate( void ) { return 0; };

		virtual inline unsigned int operator () ( void ) = 0;

		virtual inline unsigned int operator () ( const unsigned int & min, const unsigned int & max ) = 0;

		virtual inline double operator () ( const double & min, const double & max ) = 0;

		// generates a random number in [ min, max ]
		virtual inline unsigned int Generate( const unsigned int & min, const unsigned int & max )
		{
			assert( max > min );
			return ( min + this->Generate() % ( ( max - min ) ) );
		}

		virtual inline double Generate( const double & min, const double & max )
		{
			assert( max > min );
			return ( max - min ) * 
			( ( ( ( double ) ( this->Generate() % ( DEFAULT_BASE + 1 ) ) )
				/ ( ( double ) DEFAULT_BASE ) ) ) + min;
		}

		virtual inline double Perturb( const double & number, const double & gran, 
						const double & min, const double & max )
		{
			double increment = gran * this->Generate( min, max );
			if ( ( number + increment ) > max )
			{
				return min + ( ( number + increment ) - max );
			}
			if ( ( number + increment ) < max )
			{
				return max + ( ( number + increment ) - min );
			}

			return increment;
		}

	protected :

		unsigned int seed_;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// class for cursory random number generator.  Some assembly math operations.  Fairly decent RNG,
// more designed for performance than efficacy
// random_number_generator doesn't need a seed, he uses the read_time_stamp function automatically
///////////////////////////////////////////////////////////////////////////////////////////////////
class CursoryRandomNumberGenerator : virtual public ParentRandomNumberGenerator
{
	public :

		CursoryRandomNumberGenerator( void ) 
			: ParentRandomNumberGenerator() {};

		~CursoryRandomNumberGenerator( void ) {}

		inline unsigned int Generate( void ) { return random_number_generator(); };

		inline unsigned int Generate( const unsigned int & min, const unsigned int & max )
		{ return ParentRandomNumberGenerator::Generate( ( unsigned int ) min, ( unsigned int ) max ); }

		virtual inline double Generate( const double & min, const double & max )
		{ return ParentRandomNumberGenerator::Generate( min, max ); }

		virtual inline double Perturb( const double & number, const double & gran, 
						const double & min, const double & max )
		{ return ParentRandomNumberGenerator::Perturb( number, gran, min, max ); }

	 	inline unsigned int operator () ( void ) { return this->Generate(); }

		inline unsigned int operator () ( const unsigned int & min, const unsigned int & max ) { return this->Generate( min, max ); }

		inline double operator () ( const double & min, const double & max ) { return this->Generate( min, max ); }
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// class for implementation of mersenne twister
///////////////////////////////////////////////////////////////////////////////////////////////////
class MersenneTwister : virtual protected ParentRandomNumberGenerator
{
	public :

		MersenneTwister( void ) : state_index_( 0 )
		{
			this->seed_ = random_number_generator();
			MersenneTwister::Init();
		};

		MersenneTwister( const unsigned int seed ) : state_index_( 0 )
		{
			this->seed_ = seed;
			MersenneTwister::Init();
		}

		~MersenneTwister( void ) {}

		inline void Init( void )
		{
			this->state_vector_array_[ 0 ] = this->seed_ & 0xffffffffUL;
			for ( unsigned int i = 1; i < SIZE_STATE_VECTOR; ++i ) 
			{
				this->state_vector_array_[ i ] = 
				( KNUTH_MULTIPLIER * ( this->state_vector_array_[ i - 1 ] ^ 
					( this->state_vector_array_[ i - 1 ] >> 30 ) ) + i ); 
				this->state_vector_array_[ i ] &= 0xffffffffUL;
			}
		}

		inline void Init( const unsigned int * init_key, const unsigned int & size_key )
		{
			// presumeably Init() has already been called in the constructor
			unsigned int k = ( SIZE_STATE_VECTOR > size_key ? SIZE_STATE_VECTOR : size_key );
			unsigned int i = 1; unsigned int j = 0;

			for (; k; --k, ++i, ++j )
			{
				if ( i >= SIZE_STATE_VECTOR )
				{
					this->state_vector_array_[ 0 ] = 
						this->state_vector_array_[ SIZE_STATE_VECTOR - 1];
					i = 1;
				}

				this->state_vector_array_[ i ] = 
				( this->state_vector_array_[ i ] ^ 
					( MULTIPLIER_1 * ( this->state_vector_array_[ i - 1 ] ^ 
					( this->state_vector_array_[ i - 1 ] >> 30 ) ) ) + 
					init_key[ j % size_key ] + j % size_key ); 
				this->state_vector_array_[ i ] &= BIT_MASK; 
			}

			for ( k = SIZE_STATE_VECTOR - 1; k; --k, ++i ) 
			{
				if ( i >= SIZE_STATE_VECTOR )
				{
					this->state_vector_array_[ 0 ] = 
						this->state_vector_array_[ SIZE_STATE_VECTOR - 1];
					i = 1;
				}

				this->state_vector_array_[ i ] = 
				( this->state_vector_array_[ i ] ^ 
					( MULTIPLIER_2 * ( this->state_vector_array_[ i - 1 ] ^ 
					( this->state_vector_array_[ i - 1 ] >> 30 ) ) ) - 1 ); 
				this->state_vector_array_[ i ] &= BIT_MASK; 
			}

			// this->state_vector_array_[ 0 ] = UPPER_MASK;
		}

		inline void SetSeed( const unsigned int & seed )
		{
			this->seed_ = seed;
			MersenneTwister::Init();
		}

		inline void SetSeed( void )
		{
			this->seed_ = random_number_generator();
			MersenneTwister::Init();
		}

		inline unsigned int Generate( void ) 
		{
			// seed should have been set in the constructor, or if re-seeded
			unsigned int return_value;
			unsigned int mag01[ 2 ] = { 0x0UL, MATRIX_A };

			// generate SIZE_STATE_VECTOR words at a time
			if ( this->state_index_ >= SIZE_STATE_VECTOR ) 
			{
				unsigned int i = 0;
				for ( i = 0; i < ( SIZE_STATE_VECTOR - M ); ++i ) 
				{
					return_value = 
					( ( this->state_vector_array_[ i ] & UPPER_MASK ) |
                                          ( this->state_vector_array_[ i + 1 ] & LOWER_MASK ) );

					this->state_vector_array_[ i ] = 
					( this->state_vector_array_[ i + M ] ^ 
						( return_value >> 1 ) ^ mag01[ return_value & 0x1UL ] );
				}

				for ( ; i < SIZE_STATE_VECTOR - 1; ++i ) 
				{
					return_value = 
					( ( this->state_vector_array_[ i ] & UPPER_MASK ) |
                                          ( this->state_vector_array_[ i + 1 ] & LOWER_MASK ) );

					this->state_vector_array_[ i ] = 
					( this->state_vector_array_[ i + ( SIZE_STATE_VECTOR - M ) ] ^ 
						( return_value >> 1 ) ^ mag01[ return_value & 0x1UL ] );
				}

				return_value = 
				( ( this->state_vector_array_[ SIZE_STATE_VECTOR - 1 ] & UPPER_MASK ) |
				  ( this->state_vector_array_[ 0 ] & LOWER_MASK ) );

				this->state_vector_array_[ SIZE_STATE_VECTOR - 1 ] = 
				this->state_vector_array_[ M - 1 ] ^ ( return_value >> 1 ) ^ 
				mag01[ return_value & 0x1UL ];

				this->state_index_ = 0;
			}

			return_value = this->state_vector_array_[ ++this->state_index_ ];

			// tempering
			return_value ^= ( return_value >> 11 );
			return_value ^= ( return_value << 7  ) & 0x9d2c5680UL;
			return_value ^= ( return_value << 15 ) & 0xefc60000UL;
			return_value ^= ( return_value >> 18 );

			return return_value ;
		} 

		inline unsigned int Generate( const unsigned int & min, const unsigned int & max )
		{ return ParentRandomNumberGenerator::Generate( min, max ); }

		virtual inline double Generate( const double & min, const double & max )
		{ return ParentRandomNumberGenerator::Generate( min, max ); }

		virtual inline double Perturb( const double & number, const double & gran, 
						const double & min, const double & max )
		{ return ParentRandomNumberGenerator::Perturb( number, gran, min, max ); }

	 	inline unsigned int operator () ( void ) { return this->Generate(); }

		inline unsigned int operator () ( const unsigned int & min, const unsigned int & max ) { return this->Generate( min, max ); }

		inline double operator () ( const double & min, const double & max ) { return this->Generate( min, max ); }

	private :

		unsigned int state_vector_array_[ SIZE_STATE_VECTOR ];

		unsigned int state_index_;
};

} // Random

} // MathLib

#endif 	// KT_RANDOM_NUMBER_GENERATOR_H

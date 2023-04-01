#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <new>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "RandomFunction.h"

#define MKV_SIZE 256  

namespace markov_fuzzing
{
	void zero_markov( unsigned int [ MKV_SIZE ][ MKV_SIZE ] );
	void save( unsigned int [ MKV_SIZE ][ MKV_SIZE ], const char*  );
	bool open( unsigned int [ MKV_SIZE ][ MKV_SIZE ], const char*  );
	void print(unsigned int [ MKV_SIZE ][ MKV_SIZE ], const char * );
	void aquire_markov( unsigned int [ MKV_SIZE ][ MKV_SIZE ], char * , std::ifstream::pos_type );
	unsigned int calculate_stddev( unsigned int [ MKV_SIZE ][ MKV_SIZE ]);
	unsigned int calculate_mean( unsigned int [ MKV_SIZE ][ MKV_SIZE ]);
	void find_benchmark( unsigned int &, unsigned int [ MKV_SIZE ][ MKV_SIZE ] );

	//corruption mechanisms
	//can take markov matrix over a set or over a single file

	void corrupt_common(unsigned int [ MKV_SIZE ][ MKV_SIZE ], char* , std::ifstream::pos_type, unsigned int);
	void corrupt_uncommon(unsigned int [ MKV_SIZE ][ MKV_SIZE ], char* , std::ifstream::pos_type, unsigned int);
	void corrupt_common_length(unsigned int [ MKV_SIZE ][ MKV_SIZE ], unsigned int , char* , std::ifstream::pos_type, unsigned int);
	void corrupt_uncommon_length(unsigned int [ MKV_SIZE ][ MKV_SIZE ], unsigned int , char* , std::ifstream::pos_type, unsigned int);

	void corrupt_common_length_most_prob(unsigned int [ MKV_SIZE ][ MKV_SIZE ], unsigned int , char* , std::ifstream::pos_type, unsigned int);
	void corrupt_uncommon_length_most_prob(unsigned int [ MKV_SIZE ][ MKV_SIZE ], unsigned int , char* , std::ifstream::pos_type, unsigned int);
	void corrupt_common_length_least_prob(unsigned int [ MKV_SIZE ][ MKV_SIZE ], unsigned int , char* , std::ifstream::pos_type, unsigned int);
	void corrupt_uncommon_length_least_prob(unsigned int [ MKV_SIZE ][ MKV_SIZE ], unsigned int , char* , std::ifstream::pos_type, unsigned int);

	void overwrite_with_mkv_mp(unsigned int [ MKV_SIZE ][ MKV_SIZE ], unsigned int , char* , std::ifstream::pos_type, unsigned int);
	void overwrite_with_mkv_lp(unsigned int [ MKV_SIZE ][ MKV_SIZE ], unsigned int , char* , std::ifstream::pos_type, unsigned int);

	void swap_common(unsigned int [ MKV_SIZE ][ MKV_SIZE ], char* , std::ifstream::pos_type, unsigned int);
	void swap_uncommon(unsigned int [ MKV_SIZE ][ MKV_SIZE ], char* , std::ifstream::pos_type, unsigned int);
	void corrupt_ascii(unsigned int [ MKV_SIZE ][ MKV_SIZE ], char* , std::ifstream::pos_type, unsigned int);
}

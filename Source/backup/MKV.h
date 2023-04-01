#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <new>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef WINDOWS
#include <windows.h>
#pragma once
#pragma warning(disable:4996)
#endif // WINDOWS

#pragma once

namespace MKV
{
	void save( unsigned int [256][256], const char*  );
	bool open( unsigned int [256][256], const char*  );
	void print_single(unsigned int [256][256]);
	void print_multiple( unsigned int [256][256] );
	void aquire_markov_single( unsigned int [256][256], char * , std::ifstream::pos_type );
	void aquire_markov_multiple( unsigned int [256][256] , char *, char *);	//datatype needs to be of the form "*.bmp"
	unsigned int calculate_stddev( unsigned int [256][256]);
	unsigned int calculate_mean( unsigned int [256][256]);
	void find_benchmark( unsigned int &, unsigned int [256][256] );

	//corruption mechanisms
	//can take markov matrix over a set or over a single file

	void corrupt_common(unsigned int [256][256], char* , std::ifstream::pos_type, unsigned int);
	void corrupt_uncommon(unsigned int [256][256], char* , std::ifstream::pos_type, unsigned int);
	void corrupt_common_length(unsigned int [256][256], unsigned int , char* , std::ifstream::pos_type, unsigned int);
	void corrupt_uncommon_length(unsigned int [256][256], unsigned int , char* , std::ifstream::pos_type, unsigned int);

	void corrupt_common_length_most_prob(unsigned int [256][256], unsigned int , char* , std::ifstream::pos_type, unsigned int);
	void corrupt_uncommon_length_most_prob(unsigned int [256][256], unsigned int , char* , std::ifstream::pos_type, unsigned int);
	void corrupt_common_length_least_prob(unsigned int [256][256], unsigned int , char* , std::ifstream::pos_type, unsigned int);
	void corrupt_uncommon_length_least_prob(unsigned int [256][256], unsigned int , char* , std::ifstream::pos_type, unsigned int);

	void overwrite_with_mkv_mp(unsigned int [256][256], unsigned int , char* , std::ifstream::pos_type, unsigned int);
	void overwrite_with_mkv_lp(unsigned int [256][256], unsigned int , char* , std::ifstream::pos_type, unsigned int);

	void swap_common(unsigned int [256][256], char* , std::ifstream::pos_type, unsigned int);
	void swap_uncommon(unsigned int [256][256], char* , std::ifstream::pos_type, unsigned int);
	void corrupt_ascii(unsigned int [256][256], char* , std::ifstream::pos_type, unsigned int);

}

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include <cstdlib>
#include <new>
#include <iomanip>
#include <vector>
#include "MKV.h"
#include "fuzzing_operations.h"
#include "parser.h"


#ifdef WINDOWS
#include <windows.h>
#pragma warning(disable:4996)
#endif // WINDOWS

using namespace std;
using namespace fuzzing_operations;
using namespace MKV;

char * memblock=NULL;

ifstream::pos_type size;

int main( int argc, char **argv )
{

	if(argc==1)
	{
		print_help();
		return(0);
	}
//srand(GetTickCount());
	fc_options oppies;
	oppies.ai_already = false;
	oppies.parsed_already = false;
	oppies.number = false;
	oppies.dir = NULL;
	oppies.ext = NULL;
	ifstream in;
	float density=0;
	unsigned int max_length=0;
	int choice=0;
	int iterations=0;
	int ai=0;
	bool markov_in = false;
	unsigned int markov[256][256];
	unsigned int benchmark=0;

	string line;
	ifstream file (*(argv + 1), ios::in|ios::binary|ios::ate);

	if (file.is_open())
	{
		size = file.tellg();
		memblock = (char*)malloc(size*sizeof(char));
		if(memblock==NULL)
		{
			cerr << "Memory could not be freed" << endl;
			exit(0);
		}
		file.seekg (0, ios::beg);
		file.read (memblock, size);
		file.close();
	}
	else 
	{
		cerr << "Unable to open file" << endl;
		return(1);
	}

	//apply parser to command line options
	parser( argc, argv, oppies );

	//set density based on command line options from parser
	density = oppies.dense;

	//set max_length based on density
	max_length = (unsigned int)(density*(float)size);

	//obtain number of iterations from command line options
	iterations = oppies.number_of_times;

	for( int c = 0; c < iterations; c++ )
	{

		//obtain the choice from the parser
		choice = parser( argc, argv, oppies );

		switch(choice)
		{
			case(1): shotgun( memblock, density, size ); break;							//shotgun blast of hex
			case(2): shotgun_shift( memblock, density, size ); break;					//shotgun blast of shifts
			case(3): replace( memblock, density, size ); break;							//replaces one byte with another from somewhere in the buffer
//replacement because case 4 is broken
			case(4): shotgun( memblock, density, size ); break;							//shotgun blast of hex
//			case(4): swap( memblock, max_length, size ); break;							//swaps two bit regions of a given size
			case(5): translate( memblock, max_length, size ); break;						//translates a string of bytes by a randomly chosen amount
			case(6): shift( memblock, max_length, size ); break;							//translate function but with shifts

			//markov corruptions using data from single file
			case(7): 
				aquire_markov_single(markov, memblock, size);
				find_benchmark( benchmark, markov );
				corrupt_common_length( markov,  max_length, memblock, size,  benchmark ); break;
			case(8): 
				aquire_markov_single(markov, memblock, size);
				find_benchmark( benchmark, markov );
				corrupt_uncommon_length( markov,  max_length, memblock, size,  benchmark ); break;

			case(9): 
				aquire_markov_single(markov, memblock, size);
				find_benchmark( benchmark, markov );
				corrupt_common_length_most_prob( markov,  max_length, memblock, size,  benchmark ); break;
			case(10): 
				aquire_markov_single(markov, memblock, size);
				find_benchmark( benchmark, markov );
				corrupt_uncommon_length_most_prob( markov,  max_length, memblock, size,  benchmark ); break;
			case(11): 
				aquire_markov_single(markov, memblock, size);
				find_benchmark( benchmark, markov );
				corrupt_common_length_least_prob( markov,  max_length, memblock, size,  benchmark ); break;
			case(12): 
				aquire_markov_single(markov, memblock, size);
				find_benchmark( benchmark, markov );
				corrupt_uncommon_length_least_prob( markov,  max_length, memblock, size,  benchmark ); break;

			case(13):  
				aquire_markov_single(markov, memblock, size);
				find_benchmark( benchmark, markov );
				overwrite_with_mkv_mp( markov,  max_length, memblock, size,  benchmark); break;
			case(14):  
				aquire_markov_single(markov, memblock, size);
				find_benchmark( benchmark, markov );
				overwrite_with_mkv_lp( markov,  max_length, memblock, size,  benchmark); break;

			/*	markov corruptions using data from many files - 
			 *	in each case, the we attempt to open the markov binary
			 *	if it is not there, then we collect the data and save the binary
			 *	if the directory and filename extension is NULL, then we revert to
			 *	the corresponding markov (single file) corruption so that we don't waste
			 *	a test case
			*/
			case(15): 
				markov_in = open(markov, "markov_data.dat");
				if(!markov_in)
				{
					if( oppies.dir!=NULL && oppies.ext!=NULL )
					{
						aquire_markov_multiple(markov, oppies.dir, oppies.ext);
						save(markov, "markov_data.dat");
						find_benchmark( benchmark, markov );
						corrupt_common_length( markov,  max_length, memblock, size,  benchmark ); 
					}
					else
					{
						aquire_markov_single(markov, memblock, size);
						find_benchmark( benchmark, markov );
						corrupt_common_length( markov,  max_length, memblock, size,  benchmark ); 
					}
				}
				find_benchmark( benchmark, markov );
				corrupt_common_length( markov,  max_length, memblock, size,  benchmark ); 
				break;
			case(16): 
				markov_in = open(markov, "markov_data.dat");
				if(!markov_in)
				{
					if( oppies.dir!=NULL && oppies.ext!=NULL )
					{
						aquire_markov_multiple(markov, oppies.dir, oppies.ext);
						save(markov, "markov_data.dat");
						find_benchmark( benchmark, markov );
						corrupt_uncommon_length( markov,  max_length, memblock, size,  benchmark ); 					
					}
					else
					{
						aquire_markov_single(markov, memblock, size);
						find_benchmark( benchmark, markov );
						corrupt_uncommon_length( markov,  max_length, memblock, size,  benchmark ); 					
					}
				}
				find_benchmark( benchmark, markov );
				corrupt_uncommon_length( markov,  max_length, memblock, size,  benchmark ); 					
				break;
			case(17): 
				markov_in = open(markov, "markov_data.dat");
				if(!markov_in)
				{
					if( oppies.dir!=NULL && oppies.ext!=NULL )
					{
						aquire_markov_multiple(markov, oppies.dir, oppies.ext);
						save(markov, "markov_data.dat");
						find_benchmark( benchmark, markov );
						corrupt_common_length_most_prob( markov,  max_length, memblock, size,  benchmark ); 
					}
					else
					{
						aquire_markov_single(markov, memblock, size);
						find_benchmark( benchmark, markov );
						corrupt_common_length_most_prob( markov,  max_length, memblock, size,  benchmark ); 
					}
				}
				find_benchmark( benchmark, markov );
				corrupt_common_length_most_prob( markov,  max_length, memblock, size,  benchmark ); 
				break;
			case(18): 
				markov_in = open(markov, "markov_data.dat");
				if(!markov_in)
				{
					if( oppies.dir!=NULL && oppies.ext!=NULL )
					{
						aquire_markov_multiple(markov, oppies.dir, oppies.ext);
						save(markov, "markov_data.dat");
						find_benchmark( benchmark, markov );
						corrupt_uncommon_length_most_prob( markov,  max_length, memblock, size,  benchmark ); 
					}
					else
					{
						aquire_markov_single(markov, memblock, size);
						find_benchmark( benchmark, markov );
						corrupt_uncommon_length_most_prob( markov,  max_length, memblock, size,  benchmark ); 
					}
				}
				find_benchmark( benchmark, markov );
				corrupt_uncommon_length_most_prob( markov,  max_length, memblock, size,  benchmark ); 
				break;				
			case(19): 
				markov_in = open(markov, "markov_data.dat");
				if(!markov_in)
				{
					if( oppies.dir!=NULL && oppies.ext!=NULL )
					{
						aquire_markov_multiple(markov, oppies.dir, oppies.ext);
						save(markov, "markov_data.dat");
						find_benchmark( benchmark, markov );
						corrupt_common_length_least_prob( markov,  max_length, memblock, size,  benchmark ); 
					}
					else
					{
						aquire_markov_single(markov, memblock, size);
						find_benchmark( benchmark, markov );
						corrupt_common_length_least_prob( markov,  max_length, memblock, size,  benchmark ); 
					}
				}
				find_benchmark( benchmark, markov );
				corrupt_common_length_least_prob( markov,  max_length, memblock, size,  benchmark ); 
				break;				
			case(20): 
				markov_in = open(markov, "markov_data.dat");
				if(!markov_in)
				{
					if( oppies.dir!=NULL && oppies.ext!=NULL )
					{
						aquire_markov_multiple(markov, oppies.dir, oppies.ext);
						save(markov, "markov_data.dat");
						find_benchmark( benchmark, markov );
						corrupt_uncommon_length_least_prob( markov,  max_length, memblock, size,  benchmark ); 
					}
					else
					{
						aquire_markov_single(markov, memblock, size);
						find_benchmark( benchmark, markov );
						corrupt_uncommon_length_least_prob( markov,  max_length, memblock, size,  benchmark ); 
					}
				}
				find_benchmark( benchmark, markov );
				corrupt_uncommon_length_least_prob( markov,  max_length, memblock, size,  benchmark ); 
				break;				

			case(21):  
				markov_in = open(markov, "markov_data.dat");
				if(!markov_in)
				{
					if( oppies.dir!=NULL && oppies.ext!=NULL )
					{
						aquire_markov_multiple(markov, oppies.dir, oppies.ext);
						save(markov, "markov_data.dat");
						find_benchmark( benchmark, markov );
						overwrite_with_mkv_mp( markov,  max_length, memblock, size,  benchmark); 
					}
					else
					{
						aquire_markov_single(markov, memblock, size);
						find_benchmark( benchmark, markov );
						overwrite_with_mkv_mp( markov,  max_length, memblock, size,  benchmark); 
					}
				}
				find_benchmark( benchmark, markov );
				overwrite_with_mkv_mp( markov,  max_length, memblock, size,  benchmark); 
				break;				
			case(22):  
				markov_in = open(markov, "markov_data.dat");
				if(!markov_in)
				{
					if( oppies.dir!=NULL && oppies.ext!=NULL )
					{
						aquire_markov_multiple(markov, oppies.dir, oppies.ext);
						save(markov, "markov_data.dat");
						find_benchmark( benchmark, markov );
						overwrite_with_mkv_lp( markov,  max_length, memblock, size,  benchmark);
					}
					else
					{
						aquire_markov_single(markov, memblock, size);
						find_benchmark( benchmark, markov );
						overwrite_with_mkv_lp( markov,  max_length, memblock, size,  benchmark);
					}
				}
				find_benchmark( benchmark, markov );
				overwrite_with_mkv_lp( markov,  max_length, memblock, size,  benchmark);
				break;				
		}
	}

	ofstream file1 (*(argv + 2), ios::out|ios::binary);
	if (file1.is_open())
	{
		file1.write (memblock, size);
		file1.close(); 
	}
	else
	{
		cerr << "Unable to open file" << endl;
	}

	free(memblock);

	return( 0 );

}


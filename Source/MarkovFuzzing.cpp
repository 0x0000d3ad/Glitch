#include "MarkovFuzzing.h"

using namespace std;

namespace markov_fuzzing
{
	void zero_markov( unsigned int markov[ MKV_SIZE ][ MKV_SIZE ] )
	{
		for ( unsigned int i = 0; i < MKV_SIZE; ++i )
		{
			for ( unsigned int j = 0; j < MKV_SIZE; ++j )
			{
				markov[ i ][ j ] = 0;
			}
		}
	}

	void save( unsigned int markov[ MKV_SIZE ][ MKV_SIZE ], const char* filename )
	{
		ofstream out( filename, ios::binary|ios::out|ios::ate );

		if( out )
		{
			for( unsigned int i = 0; i < MKV_SIZE; ++i )
			{
				out.write( ( char * ) & markov[ i ], MKV_SIZE * sizeof( unsigned int ) );
			}
			out.close();
		}

	}

	bool open( unsigned int markov[ MKV_SIZE ][ MKV_SIZE ], const char* filename )
	{
		ifstream in( filename, ios::binary );

		if( in )
		{
			for( unsigned int i = 0; i < MKV_SIZE; ++i )
			{
				in.read( ( char * ) & markov[ i ], MKV_SIZE * sizeof( unsigned int ) );
			}
			in.close();
			return true;
		}
		else
		{
			return false;
		}
	}

	void print( unsigned int markov[ MKV_SIZE ][ MKV_SIZE ], const char * filename )
	{
		ofstream out( filename, ios::out|ios::ate );
		for( unsigned int i = 0; i < MKV_SIZE ; ++i )
		{
			for( unsigned int j = 0; j <  MKV_SIZE ; ++j ) 
			{
				out << markov[i][j] << "\t";
			}
			out << endl;
		}
	}

	void aquire_markov( unsigned int markov[ MKV_SIZE ][ MKV_SIZE ], char * buffer, std::ifstream::pos_type size )
	{
		for( unsigned int k = 0; k < ( ( unsigned int ) size ) - 1; ++k )
		{
			++markov[ ( unsigned int ) ( unsigned char ) * ( buffer + k ) ]
                                [ ( unsigned int ) ( unsigned char ) * ( buffer + k + 1 ) ];
		}
	}

	unsigned int calculate_mean( unsigned int markov[ MKV_SIZE ][ MKV_SIZE ] )
	{

		unsigned int n =  MKV_SIZE ;
		unsigned int sum = 0;

		for( int i = 0; i <  MKV_SIZE ; i++ ) 
		{
			for( int j = 0; j <  MKV_SIZE ; j++ )
			{
				sum += markov[i][j];
			}
		}

		return( (unsigned int)(((double)sum)/(double)n) );

	}

	unsigned int calculate_stddev( unsigned int markov[ MKV_SIZE ][ MKV_SIZE ] )
	{

		unsigned int nminus1 =  MKV_SIZE  - 1;
		unsigned int diffsqd = 0;
		unsigned int mean = calculate_mean( markov );

		for( int i = 0; i <  MKV_SIZE ; i++ ) 
		{
			for( int j = 0; j <  MKV_SIZE ; j++ )
			{
				diffsqd += (mean - markov[i][j])*(mean - markov[i][j]);
			}
		}

		return( (unsigned int)sqrt( ((double)diffsqd)/(double)nminus1 ) );

	}

	void find_benchmark( unsigned int & benchmark, unsigned int markov[ MKV_SIZE ][ MKV_SIZE ] )
	{
		benchmark = calculate_mean( markov ) + calculate_stddev( markov );
	}

	void corrupt_common(unsigned int markov[ MKV_SIZE ][ MKV_SIZE ], char* filename, ifstream::pos_type size, unsigned int benchmark)
	{
		int position = choose_rand((int)size-1);		//make sure last value will not blow past the buffer
		int i = 0;

		//find common structure, prevent infinite loops
		while( i < (int)size && markov[(int)(unsigned char)*((filename+(i+position)%size))][(int)(unsigned char)*((filename+(i+position+1)%size))] < benchmark ) i++;

		//if no common structure found, exit
		if( i >=((int)size-1) ) 
		{
			cerr << "incrementing past buffer" << endl;
			return;
		}

		//apply corruption mechanism to structure
		//later replace this with other corruption mech's
		while(i < 2*(int)size && markov[(int)(unsigned char)*((filename+(i+position)%size))][(int)(unsigned char)*((filename+(i+position+1)%size))] > benchmark )
		{
			*(filename + (i+position)%size ) = choose_rand( MKV_SIZE );
			i++;
		}

	}

	void corrupt_uncommon(unsigned int markov[ MKV_SIZE ][ MKV_SIZE ], char* filename, ifstream::pos_type size, unsigned int benchmark)
	{

		unsigned int i = 0;
		unsigned int position = choose_rand((int)size-1);		//make sure last value will not blow past the buffer
		int count = 0;

		while( count < 100 && markov[(int)(unsigned char)*((filename+(position)%size))][(int)(unsigned char)*((filename+(position+1)%size))] > benchmark ) 
		{
			position = choose_rand((int)size-1);
			count++;
		}

		while(i < (unsigned int)size && markov[(int)(unsigned char)*((filename+(i+position)%size))][(int)(unsigned char)*((filename+(i+position+1)%size))] < benchmark )
		{

			*(filename + (i+position)%size ) = choose_rand( MKV_SIZE );
			i++;
		}

	}

	void corrupt_common_length(unsigned int markov[ MKV_SIZE ][ MKV_SIZE ], unsigned int max_length , char* filename, ifstream::pos_type size, unsigned int benchmark)
	{
		unsigned int i = 0;
		unsigned int position = choose_rand((int)size-1);		//make sure last value will not blow past the buffer
		int count = 0;

		while( count < 100 && markov[(int)(unsigned char)*((filename+(position)%size))][(int)(unsigned char)*((filename+(position+1)%size))] < benchmark ) 
		{
			position = choose_rand((int)size-1);
			count++;
		}

		do
		{
			if(markov[(int)(unsigned char)*((filename+(i+position)%size))][(int)(unsigned char)*((filename+(i+position+1)%size))] > benchmark)
			{
				*(filename + (i+position)%size ) = choose_rand( MKV_SIZE );
			}
			i++;
		}
		while(i < max_length && ((i+position)%size) != position);

	}

	void corrupt_uncommon_length(unsigned int markov[ MKV_SIZE ][ MKV_SIZE ], unsigned int max_length, char* filename, ifstream::pos_type size, unsigned int benchmark)
	{

	
		unsigned int i = 0;
		unsigned int position = choose_rand((unsigned int)size-1);		//make sure last value will not blow past the buffer
		int count = 0;

		while( count < 100 && markov[(int)(unsigned char)*((filename+(position)%size))][(int)(unsigned char)*((filename+(position+1)%size))] > benchmark ) 
		{
			position = choose_rand((int)size-1);
			count++;
		}

		do
		{
			if(markov[(int)(unsigned char)*((filename+(i+position)%size))][(int)(unsigned char)*((filename+(i+position+1)%size))] < benchmark)
			{
				*(filename + (i+position)%size ) = choose_rand( MKV_SIZE );
			}
			i++;
		}
		while(i < max_length && ((i+position)%size) != position);

	}


	void corrupt_common_length_most_prob(unsigned int markov[ MKV_SIZE ][ MKV_SIZE ], unsigned int max_length , char* filename, ifstream::pos_type size, unsigned int benchmark)
	{
		unsigned int i = 0;
		unsigned int position = choose_rand((int)size-1);		//make sure last value will not blow past the buffer
		int count = 0;
		unsigned int temp = 0;

		while( count < 100 && markov[(int)(unsigned char)*((filename+(position)%size))][(int)(unsigned char)*((filename+(position+1)%size))] < benchmark ) 
		{
			position = choose_rand((int)size-1);
			count++;
		}

		do
		{
			if(markov[(int)(unsigned char)*((filename+(i+position)%size))][(int)(unsigned char)*((filename+(i+position+1)%size))] > benchmark)
			{
				//find the max element in the future state row
				temp = markov[(int)(unsigned char)*(filename + (i+position)%size )][0];
				for(int k = 1; k <  MKV_SIZE ; k++)
				{
					if(markov[(int)(unsigned char)*(filename + (i+position)%size )][k] > temp)
					{
						temp = markov[(int)(unsigned char)*(filename + (i+position)%size )][k];
					}
				}
				*(filename + (i+position)%size ) = temp;
			}
			i++;
		}
		while(i < max_length && ((i+position)%size) != position);

	}

	void corrupt_uncommon_length_most_prob(unsigned int markov[ MKV_SIZE ][ MKV_SIZE ], unsigned int max_length, char* filename, ifstream::pos_type size, unsigned int benchmark)
	{
		unsigned int i = 0;
		unsigned int position = choose_rand((int)size-1);		//make sure last value will not blow past the buffer
		int count = 0;
		unsigned int temp = 0;

		while( count < 100 && markov[(int)(unsigned char)*((filename+(position)%size))][(int)(unsigned char)*((filename+(position+1)%size))] > benchmark ) 
		{
			position = choose_rand((int)size-1);
			count++;
		}

		do
		{
			if(markov[(int)(unsigned char)*((filename+(i+position)%size))][(int)(unsigned char)*((filename+(i+position+1)%size))] < benchmark)
			{
				//find the max element in the future state row
				temp = markov[(int)(unsigned char)*(filename + (i+position)%size )][0];
				for(int k = 1; k <  MKV_SIZE ; k++)
				{
					if(markov[(int)(unsigned char)*(filename + (i+position)%size )][k] > temp)
					{
						temp = markov[(int)(unsigned char)*(filename + (i+position)%size )][k];
					}
				}
				*(filename + (i+position)%size ) = temp;
			}
			i++;
		}
		while(i < max_length && ((i+position)%size) != position);

	}
	void corrupt_common_length_least_prob(unsigned int markov[ MKV_SIZE ][ MKV_SIZE ], unsigned int max_length , char* filename, ifstream::pos_type size, unsigned int benchmark)
	{
		unsigned int i = 0;
		unsigned int position = choose_rand((int)size-1);		//make sure last value will not blow past the buffer
		int count = 0;
		unsigned int temp = 0;

		while( count < 100 && markov[(int)(unsigned char)*((filename+(position)%size))][(int)(unsigned char)*((filename+(position+1)%size))] < benchmark ) 
		{
			position = choose_rand((int)size-1);
			count++;
		}

		do
		{
			if(markov[(int)(unsigned char)*((filename+(i+position)%size))][(int)(unsigned char)*((filename+(i+position+1)%size))] > benchmark)
			{
				//find the min element in the future state row
				temp = markov[(int)(unsigned char)*(filename + (i+position)%size )][0];
				for(int k = 1; k <  MKV_SIZE ; k++)
				{
					if(markov[(int)(unsigned char)*(filename + (i+position)%size )][k] < temp)
					{
						temp = markov[(int)(unsigned char)*(filename + (i+position)%size )][k];
						*(filename + (i+position)%size ) = temp;
					}
				}
				*(filename + (i+position)%size ) = temp;			
			}
			i++;
		}
		while(i < max_length && ((i+position)%size) != position);

	}

	void corrupt_uncommon_length_least_prob(unsigned int markov[ MKV_SIZE ][ MKV_SIZE ], unsigned int max_length, char* filename, ifstream::pos_type size, unsigned int benchmark)
	{
		unsigned int i = 0;
		unsigned int position = choose_rand((int)size-1);		//make sure last value will not blow past the buffer
		int count = 0;
		unsigned int temp = 0;

		while( count < 100 && markov[(int)(unsigned char)*((filename+(position)%size))][(int)(unsigned char)*((filename+(position+1)%size))] > benchmark ) 
		{
			position = choose_rand((int)size-1);
			count++;
		}

		do
		{
			if(markov[(int)(unsigned char)*((filename+(i+position)%size))][(int)(unsigned char)*((filename+(i+position+1)%size))] < benchmark)
			{
				//find the min element in the future state row
				temp = markov[(int)(unsigned char)*(filename + (i+position)%size )][0];
				for(int k = 1; k <  MKV_SIZE ; k++)
				{
					if(markov[(int)(unsigned char)*(filename + (i+position)%size )][k] < temp)
					{
						temp = markov[(int)(unsigned char)*(filename + (i+position)%size )][k];
		
					}
				}
				*(filename + (i+position)%size ) = temp;			
			}
			i++;
		}
		while(i < max_length && ((i+position)%size) != position);

	}

	void overwrite_with_mkv_mp(unsigned int markov[ MKV_SIZE ][ MKV_SIZE ], unsigned int max_length, char* filename, ifstream::pos_type size, unsigned int benchmark)
	{
		( void ) benchmark; // TODO: implement benchmark or remove the variable
		unsigned int i = 0;
		unsigned int position = 1+choose_rand((int)size-2);		//make sure last value will not blow past the buffer
		unsigned int temp = 0;

		for( i=0; i < max_length; i++)
		{
			//find the max element in the future state row
			temp = markov[(int)(unsigned char)*(filename + (i+position)%size )][0];
			for(int k = 1; k <  MKV_SIZE ; k++)
			{
				if(markov[(int)(unsigned char)*(filename + (i+position)%size )][k] > temp)
				{
					temp = markov[(int)(unsigned char)*(filename + (i+position)%size )][k];
				}
			}
			*(filename + (i+position)%size ) = temp;
		}
	}

	void overwrite_with_mkv_lp(unsigned int markov[ MKV_SIZE ][ MKV_SIZE ], unsigned int max_length, char* filename, ifstream::pos_type size, unsigned int benchmark)
	{
		( void ) benchmark; // TODO: implement benchmark or remove the variable
		unsigned int i = 0;
		unsigned int position = 1+choose_rand((int)size-2);		//make sure last value will not blow past the buffer
		unsigned int temp = 0;

		for( i=0; i < max_length; i++)
		{
			//find the min element in the future state row
			temp = markov[(int)(unsigned char)*(filename + (i+position)%size )][0];
			for(int k = 1; k <  MKV_SIZE ; k++)
			{
				if(markov[(int)(unsigned char)*(filename + (i+position)%size )][k] < temp)
				{
					temp = markov[(int)(unsigned char)*(filename + (i+position)%size )][k];
				}
			}
			*(filename + (i+position)%size ) = temp;
		}
	}

	void swap_common(unsigned int markov[ MKV_SIZE ][ MKV_SIZE ], char* filename, ifstream::pos_type size, unsigned int benchmark)
	{
		( void ) markov;
		( void ) filename;
		( void ) size;
		( void ) benchmark;
	}

	void swap_uncommon(unsigned int markov[ MKV_SIZE ][ MKV_SIZE ], char* filename, ifstream::pos_type size, unsigned int benchmark)
	{
		( void ) markov;
		( void ) filename;
		( void ) size;
		( void ) benchmark;
	}

	void corrupt_ascii(unsigned int markov[ MKV_SIZE ][ MKV_SIZE ], char* filename, ifstream::pos_type size, unsigned int benchmark)
	{
		( void ) markov;
		( void ) filename;
		( void ) size;
		( void ) benchmark;
	}

}

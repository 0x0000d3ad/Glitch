#include "MKV.h"

using namespace std;

static int ctr = 0;

unsigned int choose_rand( unsigned int size )
{

	ctr++;
	unsigned int output = (ctr%2==1)?rand()%size:(size-rand()%size);
	return( (output>=size)?(size-1):output );

}

namespace MKV
{

	void save( unsigned int markov[256][256], const char* filename )
	{
		int sizem = 256;
		ofstream out(filename, ios::binary|ios::out|ios::ate );

		if(out)
		{
			for(int i=0;i<sizem;i++)
			{
				out.write((char*)&markov[i], sizem*sizeof(unsigned int));
			}
			out.close();
		}

	}

	bool open( unsigned int markov[256][256], const char* filename )
	{
		int sizem = 256;
		ifstream in(filename, ios::binary );

		if(in)
		{
			for(int i=0;i<sizem;i++)
			{
				in.read((char*)&markov[i], sizem*sizeof(unsigned int));
			}
			in.close();
			return true;
		}
		else
		{
			return false;
		}

	}

	void print_single( unsigned int markov[256][256] )
	{

		ofstream out("markov_single.txt", ios::out|ios::ate);
		for( int i = 0; i < 256; i++ )
		{
			for( int j = 0; j < 256; j++)
			{
				out << markov[i][j] << "\t";
			}
			out << endl;
		}

	}

	void print_multiple( unsigned int markov[256][256] )
	{

		ofstream out("markov_multiple.txt", ios::out|ios::ate);
		for( int i = 0; i < 256; i++ )
		{
			for( int j = 0; j < 256; j++)
			{
				out << markov[i][j] << "\t";
			}
			out << endl;
		}

	}

	void aquire_markov_single(  unsigned int markov[256][256], char * buffer, std::ifstream::pos_type size )
	{

		const int sizem=256;
		unsigned int temp = 0;


		for( int i = 0; i < sizem; i++ ) for( int j = 0; j < sizem; j++ ) markov[i][j] = 0;


#ifdef WIN32
		srand((unsigned int)GetTickCount())
#endif // WIN32

		for( int k = 0; k < (int)size-1; k++ )
		{
			markov[(int)(unsigned char)*(buffer+k)][(int)(unsigned char)*(buffer+k+1)]++;
		}

/*		//Normalize each column of the markov matrix
		for( int i = 0; i<(int)sizem; i++)
		{
			for( int j = 0; j<(int)sizem; j++)
			{
				temp += markov[i][j];
			}
			for( int j = 0; j<(int)sizem; j++)
			{
				markov[i][j]=markov[i][j]/temp;
			}
			temp=0;
		}
*/
	}

	void aquire_markov_multiple( unsigned int markov[256][256], char * dir, char * ext)		//datatype needs to be of the form "*.bmp"
	{

#ifdef WIN32
		char * memblock = NULL;
		char * name = NULL;
		char * search_dir = NULL;

		unsigned int temp = 0;
		std::ifstream::pos_type size;
		int sizem = 256;
		for( int i = 0; i < sizem; i++ ) for( int j = 0; j < sizem; j++ ) markov[i][j] = 0;


		//create input for FindFirstFile, accounting for whether user has entered
		//the last "\" in the directory specification
		if(*(dir+strlen(dir)-1)=='\\')
		{
			search_dir = (char*)malloc(strlen(dir)+strlen("*.")+strlen(ext)+8);
			sprintf(search_dir, "%s*.%s", dir, ext );
		}
		else
		{
			search_dir = (char*)malloc(strlen(dir)+strlen("\\*.")+strlen(ext)+8);
			sprintf(search_dir, "%s\\*.%s", dir, ext );
		}

		WIN32_FIND_DATA FileData;
		HANDLE hSearch;

		hSearch = FindFirstFile(search_dir, &FileData);

		free(search_dir);

		//create file name for the ifstream, accounting for whether user has entered
		//the last "\" in the directory specification
		while(hSearch!=INVALID_HANDLE_VALUE)
		

			if(*(dir+strlen(dir)-1)=='\\')
			{
				name = (char*)malloc(strlen(dir)+strlen(FileData.cFileName)+8);
				sprintf(name, "%s%s", dir, FileData.cFileName );
			}
			else
			{
				name = (char*)malloc(strlen(dir)+strlen("\\")+strlen(FileData.cFileName)+8);
				sprintf(name, "%s\\%s", dir, FileData.cFileName );
			
			ifstream file(name, ios::in|ios::binary|ios::ate);
			if (file.is_open())
			{
				size = file.tellg();
				memblock = (char*)malloc(size*sizeof(char));
				if(memblock==NULL)
				{
					cerr << "Memory could not be freed" << endl;
					exit(0);
				}
				file.seekg(0, ios::beg);
				file.read (memblock, size);
				file.close();
			}
			else 
			{
				cerr << "Markov multiple acquisition error: unable to open file" << endl;
				return;
			}

			for( int k = 0; k < (int)size-1; k++ ) markov[ (int)(unsigned char)*(memblock+k)][ (int)(unsigned char)*(memblock+k+1)]++;

			free(memblock);
			free(name);

			if(FindNextFile(hSearch, &FileData)==0) break;
		}

		FindClose(hSearch);
/*
		//Normalize each column of the markov matrix
		for( int i = 0; i<(int)sizem; i++)
		{
			for( int j = 0; j<(int)sizem; j++)
			{
				temp += markov[i][j];
			}
			for( int j = 0; j<(int)sizem; j++)
			{
				markov[i][j]=markov[i][j]/temp;
			}
			temp=0;
		}
*/

		print_multiple(markov);

#endif // WIN32
	}

	unsigned int calculate_mean( unsigned int markov[256][256] )
	{

		unsigned int n = 256;
		unsigned int sum = 0;

		for( int i = 0; i < 256; i++ ) 
		{
			for( int j = 0; j < 256; j++ )
			{
				sum += markov[i][j];
			}
		}

		return( (unsigned int)(((double)sum)/(double)n) );

	}

	unsigned int calculate_stddev( unsigned int markov[256][256] )
	{

		unsigned int nminus1 = 256 - 1;
		unsigned int diffsqd = 0;
		unsigned int mean = calculate_mean( markov );

		for( int i = 0; i < 256; i++ ) 
		{
			for( int j = 0; j < 256; j++ )
			{
				diffsqd += (mean - markov[i][j])*(mean - markov[i][j]);
			}
		}

		return( (unsigned int)sqrt( ((double)diffsqd)/(double)nminus1 ) );

	}

	void find_benchmark( unsigned int & benchmark, unsigned int markov[256][256] )
	{
		benchmark = calculate_mean( markov ) + calculate_stddev( markov );
	}

	void corrupt_common(unsigned int markov[256][256], char* filename, ifstream::pos_type size, unsigned int benchmark)
	{
		int sizem=256;
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
			*(filename + (i+position)%size ) = choose_rand(256);
			i++;
		}

	}

	void corrupt_uncommon(unsigned int markov[256][256], char* filename, ifstream::pos_type size, unsigned int benchmark)
	{

		int sizem=256;
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

			*(filename + (i+position)%size ) = choose_rand(256);
			i++;
		}

	}

	void corrupt_common_length(unsigned int markov[256][256], unsigned int max_length , char* filename, ifstream::pos_type size, unsigned int benchmark)
	{
		int sizem=256;
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
				*(filename + (i+position)%size ) = choose_rand(256);
			}
			i++;
		}
		while(i < max_length && ((i+position)%size) != position);

	}

	void corrupt_uncommon_length(unsigned int markov[256][256], unsigned int max_length, char* filename, ifstream::pos_type size, unsigned int benchmark)
	{

	
		int sizem=256;
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
				*(filename + (i+position)%size ) = choose_rand(256);
			}
			i++;
		}
		while(i < max_length && ((i+position)%size) != position);

	}


	void corrupt_common_length_most_prob(unsigned int markov[256][256], unsigned int max_length , char* filename, ifstream::pos_type size, unsigned int benchmark)
	{
		int sizem=256;
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
				for(int k = 1; k < 256; k++)
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

	void corrupt_uncommon_length_most_prob(unsigned int markov[256][256], unsigned int max_length, char* filename, ifstream::pos_type size, unsigned int benchmark)
	{

		int sizem=256;
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
				for(int k = 1; k < 256; k++)
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
	void corrupt_common_length_least_prob(unsigned int markov[256][256], unsigned int max_length , char* filename, ifstream::pos_type size, unsigned int benchmark)
	{
		int sizem=256;
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
				for(int k = 1; k < 256; k++)
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

	void corrupt_uncommon_length_least_prob(unsigned int markov[256][256], unsigned int max_length, char* filename, ifstream::pos_type size, unsigned int benchmark)
	{

		int sizem=256;
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
				for(int k = 1; k < 256; k++)
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

	void overwrite_with_mkv_mp(unsigned int markov[256][256], unsigned int max_length, char* filename, ifstream::pos_type size, unsigned int benchmark)
	{
		int sizem=256;
		unsigned int i = 0;
		unsigned int position = 1+choose_rand((int)size-2);		//make sure last value will not blow past the buffer
		int count = 0;
		unsigned int temp = 0;

		for( i=0; i < max_length; i++)
		{
			//find the max element in the future state row
			temp = markov[(int)(unsigned char)*(filename + (i+position)%size )][0];
			for(int k = 1; k < 256; k++)
			{
				if(markov[(int)(unsigned char)*(filename + (i+position)%size )][k] > temp)
				{
					temp = markov[(int)(unsigned char)*(filename + (i+position)%size )][k];
				}
			}
			*(filename + (i+position)%size ) = temp;
		}
	}

	void overwrite_with_mkv_lp(unsigned int markov[256][256], unsigned int max_length, char* filename, ifstream::pos_type size, unsigned int benchmark)
	{
		int sizem=256;
		unsigned int i = 0;
		unsigned int position = 1+choose_rand((int)size-2);		//make sure last value will not blow past the buffer
		int count = 0;
		unsigned int temp = 0;

		for( i=0; i < max_length; i++)
		{
			//find the min element in the future state row
			temp = markov[(int)(unsigned char)*(filename + (i+position)%size )][0];
			for(int k = 1; k < 256; k++)
			{
				if(markov[(int)(unsigned char)*(filename + (i+position)%size )][k] < temp)
				{
					temp = markov[(int)(unsigned char)*(filename + (i+position)%size )][k];
				}
			}
			*(filename + (i+position)%size ) = temp;
		}
	}

	void swap_common(unsigned int markov[256][256], char* filename, ifstream::pos_type size, unsigned int benchmark)
	{

	}
	void swap_uncommon(unsigned int markov[256][256], char* filename, ifstream::pos_type size, unsigned int benchmark)
	{

	}
	void corrupt_ascii(unsigned int markov[256][256], char* filename, ifstream::pos_type size, unsigned int benchmark)
	{

	}

}

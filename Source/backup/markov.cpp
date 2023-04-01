#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include <cstdlib>
#include <new>

using namespace std;

ifstream::pos_type size;
char * memblock;

int main( int argc, char **argv )
{

	ofstream out("output.txt");
	const int sizem=256;
	float markov[sizem][sizem];
	for( int i = 0; i < size; i++ ) for( int j = 0; j < size; j++ ) markov[i][j] = 0;

	if(argc==1)
	{
		cerr << "Insufficient command line arguments" << endl;
		exit(0);
	}
	srand(time(0));
	string line;
	ifstream file (*(argv + 1), ios::in|ios::binary|ios::ate);
	if (file.is_open())
	{
		size = file.tellg();
		memblock = (char*)malloc(size*sizeof(char)+64);
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
		exit(0);
	}

	for( int k = 0; k < (int)size-1; k++ ) markov[ (int)*(memblock+k) + 128 ][ (int)*(memblock+k+1) + 128 ]++;

	for( int i = 0; i < sizem; i++ )
	{
		for( int j = 0; j < sizem; j++ )
		{
			out << markov[i][j] << "\t";
		}
		out << endl;
	}

	return 0;
}
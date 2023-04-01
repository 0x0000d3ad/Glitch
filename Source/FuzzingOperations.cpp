#include "MarkovFuzzing.h"
#include "FuzzingOperations.h"
#include "RandomFunction.h"

#define min(x,y) (x<y?x:y)

using namespace std;

namespace fuzzing_operations
{
	void shotgun( char *buffer, float density, ifstream::pos_type size)
	{
		int i = 0;
		while( i < density*size )
		{
			*(buffer + choose_rand( size )) = choose_rand( 256 );
			i++;
		}
	}

	void shotgun_shift( char *buffer, float density, ifstream::pos_type size)
	{
		int i = 0;
		while( i < density*size )
		{
			unsigned int shift_index = choose_rand( 8 );
			unsigned int buffer_index = choose_rand( size );
			if( shift_index >= 4 )		
			{
				*(buffer + buffer_index ) = ( ( *(buffer + buffer_index)<<shift_index ) % 256 );
			}
			else				
			{
				*(buffer + buffer_index ) = ( ( *(buffer + buffer_index)>>shift_index ) % 256 );
			}
			i++;
		}
	}

	void replace( char *buffer, float density, ifstream::pos_type size)
	{
		int i = 0;
		while( i < density*size )
		{
			*(buffer + choose_rand( size )) = *(buffer + choose_rand( size ));
			i++;
		}
	}

	void swap( char *buffer, unsigned int max_size_chunk, ifstream::pos_type size )
	{
		unsigned int i=0;

		unsigned int p2=(1+rand())%(((unsigned int)size));
		unsigned int p1=p2+(1+rand())%((unsigned int)size-p2);
		unsigned int s4=(1+rand())%(min(max_size_chunk, (unsigned int)abs((long)((unsigned int)size-p1)) ));
		unsigned int s2=(1+rand())%(min(max_size_chunk, (unsigned int)abs((long)(p2-p1))));

		unsigned int s1=p2;
		unsigned int s3=p1-(s1+s2);
		unsigned int s5=p2+s4+s3+s2;
		unsigned int s5max=(unsigned int)size-s5;

		char * buffer1;
		buffer1 = (char*)malloc(size*sizeof(char)+32);
		if(buffer1==NULL) return;

		for( i=0; i<s1; i++ )	
			*(buffer1+i) = *(buffer+i);
		for( i=0; i<s4; i++ )
			*(buffer1+p2+i)	= *(buffer+p1+i);
		for( i=0; i<s3; i++ )
			*(buffer1+p2+s4+i) = *(buffer+p2+s2+i);
		for( i=0; i<s2; i++ )
			*(buffer1+p2+s4+s3+i) = *(buffer+p2+i);
		for( i=0; i<s5max; i++ )
			*(buffer1+p2+s4+s3+s2+i) = *(buffer+p2+s4+s3+s2+i);
		for( i=0; i<(unsigned int)size; i++ )	
			*(buffer+i)=*(buffer1+i);

		free(buffer1);
	}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//! insert functions need work :( !
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	void insert_rand( char *buffer, unsigned int max_len, ifstream::pos_type size )
	{
		int i=0;
		unsigned int position=choose_rand( size );
		unsigned int size_i =choose_rand(min(max_len, ((unsigned int)size)-(position+max_len)));

		unsigned int size_T = ((unsigned int)size)+size_i;
		unsigned int size_r1 = position;
		unsigned int size_r2 = size_T-(position+size_i);
	cout << "size_T" <<"\t"<< size_T << endl;
	cout << "size_i" <<"\t"<< size_i << endl;
	cout << "size_r1" <<"\t"<< size_r1 << endl;
	cout << "size_r2" <<"\t"<< size_r2 << endl;
	cout << "size" <<"\t"<< size << endl;
	cout << "position" <<"\t"<< position << endl;

		char *buffer1;
		buffer1 = (char*)malloc(size*sizeof(char));
		if(buffer1==NULL) 
			return;

		for(i=0; i<size; i++)				
			*(buffer1+i)=*(buffer+i);	//create copy of buffer
		
		buffer=(char*)realloc(buffer, size_T*sizeof(char));			//reallocate more room to insert stuff

		if(buffer==NULL) return;

		for(i=0; i<(int)size_i; i++)	
		{
			*(buffer+position+i)=choose_rand( 256 );
		}
		for(i=0; i<(int)size_r2; i++)	
		{
			*(buffer+position+size_i+i)=*(buffer1+size_r1+i);
		}
		size=size_T;
	cout << "size" <<"\t"<< size << endl;

		free(buffer1);
	}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//! insert functions need work :( !
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	void insert_A( char *buffer, unsigned int max_len, ifstream::pos_type size )
	{

		unsigned int i=0;
		unsigned int stop=choose_rand( size );
		unsigned int stop2=choose_rand( max_len );

		while(stop+stop2>(unsigned int)size)
		{
			stop=choose_rand( size );
			stop2=choose_rand( max_len );
		}

		char *buffer1;
		buffer1 = (char*)malloc(size*sizeof(char)+32);
		if(buffer1==NULL) return;

		for(i=0; i<(unsigned int)size; i++)						
			*(buffer1+i)=*(buffer+i);
		for(i=stop; i<stop+stop2;i++)				
			*(buffer+i)=0x41;
		for(i=stop+stop2; i<stop+stop2+size; i++)	
			*(buffer+i)=*(buffer1+i-(stop+stop2));
		free(buffer1);
	}

	
	void translate( char *buffer, unsigned int max_len, ifstream::pos_type size )
	{

		unsigned int i=0;
		unsigned int stop=choose_rand( size );
		unsigned int stop2=choose_rand( max_len );
		unsigned char translate = choose_rand( 64 );

		while(stop+stop2>(unsigned int)size)
		{
			stop=choose_rand( size );
			stop2=choose_rand( max_len );
		}

		for(i=(unsigned int)stop; i<(unsigned int)(stop+stop2);i++)
			*(buffer+i)=(*(buffer+i)+translate)%256;

	}

	void shift( char *buffer, unsigned int max_len, ifstream::pos_type size )
	{

		unsigned int i=0;
		unsigned int stop=choose_rand( size );
		unsigned int stop2=choose_rand( max_len );
		int translate = choose_rand( 8 );
		//int direction = choose_rand( 2 );

		while(stop+stop2>(unsigned int)size)
		{
			stop=choose_rand( size );
			stop2=choose_rand( max_len );
		}

		if(translate==0)
			for(i=stop; i<(unsigned int)(stop+stop2);i++)	
				*(buffer+i)<<=translate;
		else				
			for(i=stop; i<(unsigned int)(stop+stop2);i++)	
				*(buffer+i)>>=translate;

	}
}

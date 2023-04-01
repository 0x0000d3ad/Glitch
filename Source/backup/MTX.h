#include <vector>
#include <iostream>

using namespace std;

#ifndef MTX_H
#define MTX_H

static char T = 'T';
static int ct = 0;
static int sig_p = 1;

#define inc(){ ct = ct++%100; }

class MTX
{

	friend class NN;
	friend void save_NN( NN &, const char * );
	friend bool open_NN( NN &, const char * );

	friend MTX operator*( double, MTX );
	friend MTX operator ^ ( const MTX &, char);
	friend MTX SIG( MTX );
	friend MTX DSIG( MTX );
	friend MTX MTX_ERROR( MTX, MTX );
	friend MTX COMP( MTX, MTX ); 			//componentwise mult.
	friend MTX KRO( MTX, MTX );				//kronecker prod. (1d arrays)
	friend MTX BLEND( MTX &, MTX &, int );		//blends two column matrices at n
	friend MTX UNBLEND( MTX &, MTX &, int );	//opposite of BLEND
	friend double DOT( MTX, MTX );			//dot product
	friend double SUM_MTX( MTX );			//sum all elements

	public:
		MTX();											//constructor
		void init(int, int);							//zero init	(M by N)	
		void rinit(int, int);							//random init (M by N)
		void print();									//std out
		
		MTX& operator = ( const MTX& );
		MTX& operator = ( const vector< vector< double > > & );
		MTX& operator = ( const vector< double > & );	//makes a column vector
		MTX& operator += ( const MTX& );
		MTX& operator -= ( const MTX& );
		bool operator == ( const MTX& );
		bool operator != ( const MTX& );
		MTX operator + ( const MTX& );
		MTX operator - ( const MTX& );
		MTX operator * ( const MTX& );
		vector<double> & operator [] (const int );
	
	private:
		int ii;											//number rows
		int jj;											//number columns
		vector< vector< double > > matrix;				//column of rows

};

#endif

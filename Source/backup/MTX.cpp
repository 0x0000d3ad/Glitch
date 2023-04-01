#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include "MTX.h"

using namespace std;

double sigmoid( double x, double parameter )				//sigmoid function R |-> {-1,1}
{ return (2/(1+exp(-parameter*x)) - 1); }

double dsigmoid( double x, double parameter )				//derivative of sigmoid function R |-> {-1,1}
{ return ( 2*parameter*exp(-parameter*x)/
			(pow((1+exp(-parameter*x)),2)) ); }
															//initialize vector with 0's
void v_init( vector< vector< double > > &vi, int iii, int jjj )
{

	vi.clear();
	vector< double > temp;
	
	for(int i = 0; i < iii; i++ )
	{
		for(int j = 0; j < jjj; j++ )
		{
			temp.push_back((double)0);
		}
	vi.push_back(temp);
	}		
}

MTX::MTX()													//constructor
{
	matrix.clear();		

}

void MTX::init(int aye, int jay )							//initialize MTX with 0's
{
	ii = aye;
	jj = jay;
	vector<double> temp;
	temp.clear();
	
	for(int j = 0; j < jj; j++ )
	{
		temp.push_back(0);
	}
	for(int i = 0; i < ii; i++ )
	{
		matrix.push_back(temp);
	}		
}

void MTX::rinit(int aye, int jay )							//initialize MTX with random values
{

	ii = aye;
	jj = jay;
	vector<double> temp;
	temp.clear();
	matrix.clear();
	
	double temp_d;
	for(int i = 0; i < ii; i++ )
	{
		for(int j = 0; j < jj; j++ )
		{

			temp_d = (double)(rand()%10000)/10000;			//push random number
			temp.push_back(2*temp_d - 1);
		}
	matrix.push_back(temp);                            
	temp.clear();
	}		
}

void MTX::print()											//print MTX elements
{
	for(int i = 0; i < ii; i++ )
	{
		for(int j = 0; j < jj; j++ )  
		{
			cout << setw(9) << matrix[i][j] << "\t";
		}
		cout << endl;
	}
	cout << endl;
}

															//assinment operator for vectors
MTX& MTX::operator = ( const vector< vector< double > > & right )
{
	matrix = right;
	ii = (int)matrix.size();
	jj = (int)matrix[0].size();
	return *this;
}

MTX& MTX::operator = ( const vector< double > & right )
{
	MTX temp;
	vector< vector< double > > push;
	push.clear();
	
	push.push_back( right );
	
	temp = push;
	*this = temp^T;

	return *this;
}
															//assinment operator for MTX's
MTX& MTX::operator = ( const MTX& right )
{
	matrix = right.matrix;
	ii = right.ii;
	jj = right.jj;
	return *this;
}
															//addition assinment operator for MTX's
MTX& MTX::operator += ( const MTX& right )
{
	*this = *this + right;
	return *this;	
}

															//subtraction assinment operator for MTX's
MTX& MTX::operator -= ( const MTX& right )
{
	*this = *this - right;
	return *this;
}

															//comparison operator for MTX's
bool MTX::operator == ( const MTX& right )
{
	bool output = true;
	bool temp = true;
	
	int i = 0;
	while( output && i < ii )
	{
		temp = (matrix[i] == right.matrix[i]);				//compare element by element
		output = output && temp;							//if one element differs, return false
		i++;
	}
	return output;
}

															//!comparison operator
bool MTX::operator != ( const MTX& right )					//! of == operator
{
	return !(*this == right);
}

MTX MTX::operator + ( const MTX& right )					//addition operator
{

	if( ii != right.ii || jj != right.jj )					//check that matrix indices have same bounds
		cerr << "Matrices of unequal size cannot be added" << endl;

	MTX output;
	output.init( right.ii, right.jj );
	
	vector< vector< double > > out;
	out.clear();
	v_init( out, right.ii, right.jj );

	for(int i = 0; i < ii; i++ )
	{
		for(int j = 0; j < jj; j++ )
		{													//add element-wise
			out[i][j] = matrix[i][j] + right.matrix[i][j];
		}
	}
	output = out;
	return output;			
}

MTX MTX::operator - ( const MTX& right )					//subtraction operator
{

	if( ii != right.ii || jj != right.jj )					//check that matrix indices have same bounds
		cerr << "Matrices of unequal size cannot be added" << endl;

	MTX output;
	output.init( right.ii, right.jj );

	vector< vector< double > > out;
	out.clear();
	v_init( out, right.ii, right.jj );

	for(int i = 0; i < ii; i++ )
	{
		for(int j = 0; j < jj; j++ )
		{
			out[i][j] = matrix[i][j] - right.matrix[i][j];	//subtract element-wise
		}
	}
	output = out;
	return output;			
}

MTX MTX::operator * ( const MTX& right )					//multiplication operator
{
															//check bounds
	if( jj != right.ii ) cerr << "incompatible array indices" << endl;

	MTX mout;
	mout.init(ii, right.jj);

	vector< vector< double > > output;
	v_init( output, ii, right.jj );

	double temp = 0;
															//multiplication algorithms
	for(int i = 0; i < ii; i++ )
	{	
		for(int k = 0; k < right.jj; k++ )
		{
			for(int j = 0; j < jj; j++ )
			{
				temp += matrix[i][j] * right.matrix[j][k];
			}
		output[i][k] = temp;
		temp = 0;
		}
	}

	mout = output;
	return mout;			
		
}

MTX operator*( double a, MTX right )						//scalar multiplication
{

	MTX output;
	output.init(right.ii, right.jj);

	vector< vector< double > > out;
	v_init( out, right.ii, right.jj );
	

	for(int i = 0; i < right.ii; i++ )
	{
		for(int j = 0; j < right.jj; j++ )
		{
			out[i][j] = a*right.matrix[i][j];
		}
	}

	output = out;
	return output;			

}

MTX operator ^ ( const MTX &left, char V)					//Transpose operator
{

	MTX output;
	if( V == T )											//transpose selected
	{ 
	
		output.init(left.jj, left.ii);

		vector< vector< double > > out;
		v_init( out, left.jj, left.ii );
	
		for(int i = 0; i < left.jj; i++ )
		{
			for(int j = 0; j < left.ii; j++ )
			{
				out[i][j] = left.matrix[j][i];				//swap rows and columns
			}
		}

		output.matrix = out;
	
		output.ii = left.jj;
		output.jj = left.ii;

	}
	return output;			

}

MTX SIG( MTX input )
{

	MTX output;
	output.init( input.ii, input.jj );
	
	for(int i = 0; i < input.ii; i++)
	{
		for(int j = 0; j < input.jj; j++)
		{
			output.matrix[i][j] = sigmoid( input.matrix[i][j], sig_p );
		}
	}

	return(output);

}

MTX DSIG( MTX input )
{

	MTX output;
	output.init( input.ii, input.jj );
	
	for(int i = 0; i < input.ii; i++)
	{
		for(int j = 0; j < input.jj; j++)
		{
			output.matrix[i][j] = dsigmoid( input.matrix[i][j], sig_p);
		}
	}

	return(output);

}

MTX MTX_ERROR( MTX out, MTX in )
{

	if( in.ii != out.ii || in.jj != out.jj )
	{
		cerr << "operands of unequal size" << endl;
	}
	
	MTX output;
	output.init( in.ii, in.jj ); // should be the same as out.ii, out.jj
	
	output = out - in;
	
	for( int i = 0; i < output.ii; i++ )
	{
		for( int j = 0; j < output.jj; j++ )
		{
			output[i][j] = abs(output[i][j]);	
		}
	}
	
	return( output );

}


MTX COMP( MTX left, MTX right )
{

	if( left.ii != right.ii || left.jj != right.jj )
	{
	
		cerr << "incompatible array sizes" << endl;
	
	}

	MTX output;
	output.init(right.ii, right.jj);  //should be equiv. to left.ii, left.jj

	vector< vector< double > > out;
	v_init( out, right.ii, right.jj );
	

	for(int i = 0; i < right.ii; i++ )
	{
		for(int j = 0; j < right.jj; j++ )
		{
			out[i][j] = left.matrix[i][j]*right.matrix[i][j];
		}
	}

	output = out;
	return output;			

}

MTX KRO( MTX left, MTX right )
{

	return left*right;			

}

double DOT( MTX left, MTX right )
{

	if( !(left.ii == 1 || left.jj == 1) )
	{
		cerr << "for linear arrays only" << endl;
	}
	if( !(right.ii == 1 || right.jj == 1) )
	{
		cerr << "for linear arrays only" << endl;
	}

	double output = 0;

	if( left.ii == 1 && right.ii == 1 && left.jj == right.jj )
	{

		for(int i = 0; i < right.jj; i++) output += left[0][i]*right[0][i];
	
	}	
	else if( left.jj == 1 && right.jj == 1 && left.ii == right.ii )
	{
	
		for(int i = 0; i < right.ii; i++) output += left[i][0]*right[i][0];
	
	}
	else if( left.jj != right.jj )
	{
	
		cerr << "Incompatible array sizes" << endl;
		
	}
	else if( left.ii != right.ii )
	{
	
		cerr << "Incompatible array sizes" << endl;
		
	}
	else cerr << "Something is wrong :(" << endl;
	
	return(output);

}

double SUM_MTX( MTX sum )
{

	double y = 0;
	for(int i = 0; i < sum.ii; i++) 
	{
		for( int j = 0; j < sum.jj; j++ )
		{
			y += sum[i][j];
		}
	}
	return(y);

}

MTX BLEND( MTX & m1, MTX & m2, int place )	//read:"blend m2 into m1 at place"
{

	//check to see that matrices are column
	if( m1.jj != 1 && m2.jj != 1 )
	{
		cerr << "Only column vectors may be blended" << endl;
		exit(0);
	}
	if( m1.ii == m2.ii )
	{
		return( m2 );
	}
	else if( m1.ii > m2.ii )
	{
		for( int i = 0; i < m2.ii; i++ ) 
		{								//mod-ed so no buffer overflow!
			m1[(i+place)%(m1.ii)][0] = m2[i][0];		
		}
		return(m1);
	}
	else if( m1.ii < m2.ii )
	{
		for( int i = 0; i < m1.ii; i++ ) 
		{								//mod-ed so no buffer overflow!
			m2[(i+place)%(m2.ii)][0] = m1[i][0];		
		}
		return(m2);
	}
	else
	{
		cout << "Something is wrong :(" << endl;
		exit(0);
	}

}

MTX UNBLEND( MTX & m1, MTX & m2, int place )	//read:"unblend m2 into m1 at place"
{

	//check to see that matrices are column
	if( m1.jj != 1 && m2.jj != 1 )
	{
		cerr << "Only column vectors may be blended" << endl;
		exit(0);
	}
	if( m1.ii == m2.ii )
	{
		return( m2 );
	}
	else if( m1.ii > m2.ii )
	{
		for( int i = 0; i < m2.ii; i++ ) 
		{								//mod-ed so no buffer overflow!
			m2[i][0]=m1[(i+place)%(m1.ii)][0];		
		}
		return(m2);
	}
	else if( m1.ii < m2.ii )
	{
		for( int i = 0; i < m1.ii; i++ ) 
		{								//mod-ed so no buffer overflow!
			m1[i][0]=m2[(i+place)%(m2.ii)][0];		
		}
		return(m1);
	}
	else
	{
		cout << "Something is wrong :(" << endl;
		exit(0);
	}

}

vector<double>& MTX::operator [] (const int index)			//array subscript operator
{
	return(matrix[index]);									//return index'th row
}

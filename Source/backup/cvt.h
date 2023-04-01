#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;

namespace cvt
{

	int convertchar2int( char * input );

	float convertchar2float( char* input );

	string convertint2string( int x );

	int convertstring2int( string input );

	string convertfloat2string( float x );

	float convertstring2float( string input );

	string convertsint2string( short int x );

	short int convertstring2sint( string input );

	string convertdouble2string( double x );

	double convertstring2double( string input );

	string convertuint2string( unsigned int x );

	unsigned int convertstring2uint( string input );

	string convertlint2string( long int x );

	long int convertstring2lint( string input );

	string convertsiint2string( signed int x );

	signed int convertstring2siint( string input );

	void getdoubledata( double a[] );

	void getfloatdata( float a[] );

	void getintdata( int a[] );

	void getlintdata( long int a[] );

	void getsintdata( short int a[] );

	void getuintdata( unsigned int a[] );

	void getsiintdata( signed int a[] );

	void getchardata( char a[] );

	void putdouble( double a[] );

	void putfloat( float a[] );

	void putint( int a[] );

	void putlint( long int a[] );

	void putsint( short int a[] );

	void putuint( unsigned int a[] );

	void putsiint( signed int a[] );

	void putchar( char a[] );

}

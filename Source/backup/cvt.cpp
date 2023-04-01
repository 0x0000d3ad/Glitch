#include "cvt.h"

using namespace std;

namespace cvt
{

	int convertchar2int( char * input )
	{

		string output = input;									//string for the output
		bool negative = false;									//logival variable for negative values
		if( output[0] == '-' )
		{
			negative = true;
		}
		int address = 0;
		int count = 0;
		int l = (negative?1:0);

		while(l < (int)strlen(input))								//converts string of the address into integer address
		{
			address += static_cast<int> ( ( static_cast<int>(output[l]) - 48 )*
				pow(10.0, static_cast<int>( output.length() - (l + 1) ) ) );
			l++;
		}

		if( negative == true ) address = -1*address;			//accounts for negative

		return( address );

	}

	float convertchar2float( char* input )
	{

		int position = 0;
		float result = 0;

		for( size_t i = 0; i < strlen(input); i++ )
		{
			if(input[i] == '.') position = (int)i;
		}

		for( size_t i = position-1; (int)i >= 0; i-- )
		{
			result += (float)((int)input[i]-48)*(pow((float)10.0, (int)(position-i-1)));
		}

		for( size_t i = position+1; i < strlen(input); i++ )
		{
			result += (float)((int)input[i]-48)/(pow((float)10.0, (int)(i-position)));
		}

		return(result);
	}

	string convertint2string( int x )
	{


		int x1 = x;
		bool negative = false;									//variable to account for negative values
		if( x < 0 )												//branch for negative values
		{
			x1 = -1*x;
			negative = true;
		}	
		string output;											//string for output
		int test = 1;
		int i = -1;
		int places = i;
		int temp1 = 0;
		int temp2 = x1;

		while( test >= 0 )										//counts how many decimal places are in the input
		{
			
			i++;
			places = i;
			test = x1 - static_cast<int>( pow( 10.0, i ) );

		}

		if(places == -1) output += '0';

		else													//constructs the string representing the number
		{
			places -= 1;
					
			for(int j = places; j >= 0; j--)
			{

				temp1 = temp2/static_cast<int>( pow( 10.0, j ) );
				output += static_cast<char>( temp1 + 48 );
				temp2 -= temp1*static_cast<int>( pow( 10.0, j ) );

			}

		}

		if( negative == true ) output = '-' + output;			//accounts for negative values
		if( x == 0 ) output = '0';								//accounts for zero

		return( output );

	}


	int convertstring2int( string input )
	{

		string output = input;									//string for the output
		bool negative = false;									//logival variable for negative values
		if( output[0] == '-' )
		{
			output = input.erase(0, 1);							//removes negative sign
			negative = true;
		}
		int address = 0;
		int count = 0;
		int l = 0;

		while(output[l] != '\0')								//converts string of the address into integer address
		{
			address += static_cast<int> ( ( static_cast<int>(output[l]) - 48 )*
				pow(10.0, static_cast<int>( output.length() - (l + 1) ) ) );
			l++;
		}

		if( negative == true ) address = -1*address;			//accounts for negative

		return( address );

	}


	string convertfloat2string( float x )
	{


		float x1 = x;
		bool negative = false;									//logical variable for negative values
		if( x < 0 )												//condition to deal with negatives
		{
			x1 = -1*x;											//removes negative sign
			negative = true;
		}
		const int setprecision = 6;								//sets numerical precision = 6 places
		string result = "0";
		string sinteger = "0";
		string point = ".";
		string sdecimal = "0";
		int integer = static_cast< int >( x1 );
		float decimal = x1 - integer;
		float pointmove = decimal*pow((float)10.0, setprecision);
		int idecimal = static_cast< int >( pointmove );
		sdecimal = convertint2string( idecimal );				//creates decimal part of the float
		sinteger = convertint2string( integer );				//creates integer part of the float

		while( sdecimal.length() < setprecision )				//adds in zeroes for the rest of the string
		{

			sdecimal = '0' + sdecimal;

		}
																//removes unneccessary zeroes
		for(int i = (int)sdecimal.length() - 1; i >= 0; i--)
		{

			if( sdecimal[i] != '0' ) break;

			else
			{

				sdecimal = sdecimal.erase( i, 1 );

			}

		}

		result = sinteger + point + sdecimal;					//result = integer part + "." + decimal part

		if( negative == true ) result = '-' + result;			//adds "-" if neccessary

		return( result );

	}


	float convertstring2float( string input )
	{

		string output = input;									//variable for output
		bool point = true;
		bool negative = false;									//logical variable for negative values
		if( output[0] == '-' )									//removes "-"
		{
			output = input.erase(0, 1);
			negative = true;
		}
		int place = 0;
																//first two conditionals check if input is +- integer
		if( output.find( ".", 0 ) == string::npos && !negative )		return( (float)convertstring2int( output ) );
		else if( output.find( ".", 0 ) == string::npos && negative )	return( (float)convertstring2int( '-' + output ) );
		else													//third conditional is for floats
		{

			place = (int)output.find( ".", 0 );					//finds decimal point
			float result = 0;
			string integer = output.substr( 0, place );			//creates integer part, next line creates decimal part
			string decimal = output.substr( place, output.length() - place );
			decimal.erase(0, 1);								//erases "." in decimal part
			int exponent = (int)decimal.length();
			int iinteger = convertstring2int( integer );
			int idecimal = convertstring2int( decimal );
			float finteger = static_cast< float >( iinteger );
			float fdecimal = static_cast< float >( idecimal );
			fdecimal = fdecimal*pow( (float)10.0, -1*exponent );
			result = finteger + fdecimal;						//result = integer + decimal

			if( negative == true ) result = -1*result;			//accounts for negative values

			return( result );

		}

	}

	string convertsint2string( short int x )
	{


		short int x1 = x;
		bool negative = false;									//variable to account for negative values
		if( x < 0 )												//branch for negative values
		{
			x1 = -1*x;
			negative = true;
		}	
		string output;											//string for output
		short int test = 1;
		int i = -1;
		int places = i;
		short int temp1 = 0;
		short int temp2 = x1;

		while( test >= 0 )										//counts how many decimal places are in the input
		{
			
			i++;
			places = i;
			test = x1 - static_cast<short int>( pow( 10.0, i ) );

		}

		if(places == -1) output += '0';

		else													//constructs the string representing the number
		{
			places -= 1;
					
			for(int j = places; j >= 0; j--)
			{

				temp1 = temp2/static_cast<short int>( pow( 10.0, j ) );
				output += static_cast<char>( temp1 + 48 );
				temp2 -= temp1*static_cast<short int>( pow( 10.0, j ) );

			}

		}

		if( negative == true ) output = '-' + output;			//accounts for negative values
		if( x == 0 ) output = '0';								//accounts for zero

		return( output );

	}

	short int convertstring2sint( string input )
	{

		string output = input;									//string for the output
		bool negative = false;									//logival variable for negative values
		if( output[0] == '-' )
		{
			output = input.erase(0, 1);							//removes negative sign
			negative = true;
		}
		short int address = 0;
		int count = 0;
		int l = 0;

		while(output[l] != '\0')								//converts string of the address into integer address
		{
			address += static_cast<short int> ( ( static_cast<short int>(output[l]) - 48 )*
				pow(10.0, static_cast<short int>( output.length() - (l + 1) ) ) );
			l++;
		}

		if( negative == true ) address = -1*address;			//accounts for negative

		return( address );

	}

	string convertdouble2string( double x )
	{


		double  x1 = x;
		bool negative = false;									//logical variable for negative values
		if( x < 0 )												//condition to deal with negatives
		{
			x1 = -1*x;											//removes negative sign
			negative = true;
		}
		const int setprecision = 6;								//sets numerical precision = 6 places
		string result = "0";
		string sinteger = "0";
		string point = ".";
		string sdecimal = "0";
		int integer = static_cast< int >( x1 );
		double decimal = x1 - integer;
		double pointmove = decimal*pow((double)10.0, setprecision);
		int idecimal = static_cast< int >( pointmove );
		sdecimal = convertint2string( idecimal );				//creates decimal part of the float
		sinteger = convertint2string( integer );				//creates integer part of the float

		while( sdecimal.length() < setprecision )				//adds in zeroes for the rest of the string
		{

			sdecimal = '0' + sdecimal;

		}
																//removes unneccessary zeroes
		for(int i = (int)sdecimal.length() - 1; i >= 0; i--)
		{

			if( sdecimal[i] != '0' ) break;

			else
			{

				sdecimal = sdecimal.erase( i, 1 );

			}

		}

		result = sinteger + point + sdecimal;					//result = integer part + "." + decimal part

		if( negative == true ) result = '-' + result;			//adds "-" if neccessary

		return( result );

	}

	double convertstring2double( string input )
	{

		string output = input;									//variable for output
		bool point = true;
		bool negative = false;									//logical variable for negative values
		if( output[0] == '-' )									//removes "-"
		{
			output = input.erase(0, 1);
			negative = true;
		}
		int place = 0;
																//first two conditionals check if input is +- integer
		if( output.find( ".", 0 ) == string::npos && !negative )		return( (double)convertstring2int( output ) );
		else if( output.find( ".", 0 ) == string::npos && negative )	return( (double)convertstring2int( '-' + output ) );
		else													//third conditional is for floats
		{

			place = (int)output.find( ".", 0 );					//finds decimal point
			double result = 0;
			string integer = output.substr( 0, place );			//creates integer part, next line creates decimal part
			string decimal = output.substr( place, output.length() - place );
			decimal.erase(0, 1);								//erases "." in decimal part
			int exponent = (int)decimal.length();
			int iinteger = convertstring2int( integer );
			int idecimal = convertstring2int( decimal );
			double finteger = static_cast< double >( iinteger );
			double fdecimal = static_cast< double >( idecimal );
			fdecimal = fdecimal*pow( (double)10.0, -1*exponent );
			result = finteger + fdecimal;						//result = integer + decimal

			if( negative == true ) result = -1*result;			//accounts for negative values

			return( result );

		}

	}

	string convertuint2string( unsigned int x )
	{


		unsigned int x1 = x;
		string output;											//string for output
		short int test = 1;
		int i = -1;
		int places = i;
		unsigned int temp1 = 0;
		unsigned int temp2 = x1;

		while( test >= 0 )										//counts how many decimal places are in the input
		{
			
			i++;
			places = i;
			test = x1 - static_cast<short int>( pow( 10.0, i ) );

		}

		if(places == -1) output += '0';

		else													//constructs the string representing the number
		{
			places -= 1;
					
			for(int j = places; j >= 0; j--)
			{

				temp1 = temp2/static_cast<unsigned int>( pow( 10.0, j ) );
				output += static_cast<char>( temp1 + 48 );
				temp2 -= temp1*static_cast<unsigned int>( pow( 10.0, j ) );

			}

		}

		if( x == 0 ) output = '0';								//accounts for zero

		return( output );

	}

	unsigned int convertstring2uint( string input )
	{

		string output = input;									//string for the output

		unsigned int address = 0;
		int count = 0;
		int l = 0;

		while(output[l] != '\0')								//converts string of the address into integer address
		{
			address += static_cast<unsigned int> ( ( static_cast<unsigned int>(output[l]) - 48 )*
				pow(10.0, static_cast<int>( output.length() - (l + 1) ) ) );
			l++;
		}

		return( address );

	}


	string convertlint2string( long int x )
	{


		long int x1 = x;
		bool negative = false;									//variable to account for negative values
		if( x < 0 )												//branch for negative values
		{
			x1 = -1*x;
			negative = true;
		}	
		string output;											//string for output
		long int test = 1;
		int i = -1;
		int places = i;
		long int temp1 = 0;
		long int temp2 = x1;

		while( test >= 0 )										//counts how many decimal places are in the input
		{
			
			i++;
			places = i;
			test = x1 - static_cast<long int>( pow( 10.0, i ) );

		}

		if(places == -1) output += '0';

		else													//constructs the string representing the number
		{
			places -= 1;
					
			for(int j = places; j >= 0; j--)
			{

				temp1 = temp2/static_cast<long int>( pow( 10.0, j ) );
				output += static_cast<char>( temp1 + 48 );
				temp2 -= temp1*static_cast<long int>( pow( 10.0, j ) );

			}

		}

		if( negative == true ) output = '-' + output;			//accounts for negative values
		if( x == 0 ) output = '0';								//accounts for zero

		return( output );

	}


	long int convertstring2lint( string input )
	{

		string output = input;									//string for the output
		bool negative = false;									//logival variable for negative values
		if( output[0] == '-' )
		{
			output = input.erase(0, 1);							//removes negative sign
			negative = true;
		}
		long int address = 0;
		int count = 0;
		int l = 0;

		while(output[l] != '\0')								//converts string of the address into integer address
		{
			address += static_cast<long int> ( ( static_cast<int>(output[l]) - 48 )*
				pow(10.0, static_cast<int>( output.length() - (l + 1) ) ) );
			l++;
		}

		if( negative == true ) address = -1*address;			//accounts for negative

		return( address );

	}

	string convertsiint2string( signed int x )
	{


		signed int x1 = x;
		bool negative = false;									//variable to account for negative values
		if( x < 0 )												//branch for negative values
		{
			x1 = -1*x;
			negative = true;
		}	
		string output;											//string for output
		signed int test = 1;
		signed int i = -1;
		int places = i;
		signed int temp1 = 0;
		signed int temp2 = x1;

		while( test >= 0 )										//counts how many decimal places are in the input
		{
			
			i++;
			places = i;
			test = x1 - static_cast<signed int>( pow( 10.0, i ) );

		}

		if(places == -1) output += '0';

		else													//constructs the string representing the number
		{
			places -= 1;
					
			for(int j = places; j >= 0; j--)
			{

				temp1 = temp2/static_cast<signed int>( pow( 10.0, j ) );
				output += static_cast<char>( temp1 + 48 );
				temp2 -= temp1*static_cast<signed int>( pow( 10.0, j ) );

			}

		}

		if( negative == true ) output = '-' + output;			//accounts for negative values
		if( x == 0 ) output = '0';								//accounts for zero

		return( output );

	}


	signed int convertstring2siint( string input )
	{

		string output = input;									//string for the output
		bool negative = false;									//logival variable for negative values
		if( output[0] == '-' )
		{
			output = input.erase(0, 1);							//removes negative sign
			negative = true;
		}
		signed int address = 0;
		int count = 0;
		int l = 0;

		while(output[l] != '\0')								//converts string of the address into integer address
		{
			address += static_cast<int> ( ( static_cast<int>(output[l]) - 48 )*
				pow(10.0, static_cast<int>( output.length() - (l + 1) ) ) );
			l++;
		}

		if( negative == true ) address = -1*address;			//accounts for negative

		return( address );

	}


	void getdoubledata( double a[] )
	{
		string line;
		ifstream data( "doubledata.txt" );

		int i = 0;
		while(!data.eof()) 
		{
			getline(data, line);
			a[i] = cvt::convertstring2double( line );
			i++;
		}
		data.close();
	}

	void getfloatdata( float a[] )
	{
		string line;
		ifstream data( "floatdata.txt" );

		int i = 0;
		while(!data.eof()) 
		{
			getline(data, line);
			a[i] = cvt::convertstring2float( line );
			i++;
		}
		data.close();
	}

	void getintdata( int a[] )
	{
		string line;
		ifstream data( "intdata.txt" );

		int i = 0;
		while(!data.eof()) 
		{
			getline(data, line);
			a[i] = cvt::convertstring2int( line );
			i++;
		}
		data.close();
	}

	void getlintdata( long int a[] )
	{
		string line;
		ifstream data( "lintdata.txt" );

		int i = 0;
		while(!data.eof()) 
		{
			getline(data, line);
			a[i] = cvt::convertstring2lint( line );
			i++;
		}
		data.close();
	}

	void getsintdata( short int a[] )
	{
		string line;
		ifstream data( "sintdata.txt" );

		int i = 0;
		while(!data.eof()) 
		{
			getline(data, line);
			a[i] = cvt::convertstring2sint( line );
			i++;
		}
		data.close();
	}

	void getuintdata( unsigned int a[] )
	{
		string line;
		ifstream data( "uintdata.txt" );

		int i = 0;
		while(!data.eof()) 
		{
			getline(data, line);
			a[i] = cvt::convertstring2uint( line );
			i++;
		}
		data.close();
	}

	void getsiintdata( signed int a[] )
	{
		string line;
		ifstream data( "siintdata.txt" );

		int i = 0;
		while(!data.eof()) 
		{
			getline(data, line);
			a[i] = cvt::convertstring2siint( line );
			i++;
		}
		data.close();
	}

	void getchardata( char a[] )
	{
		string line;
		ifstream data( "chardata.txt" );

		int i = 0;
		while(!data.eof()) 
		{
			getline(data, line);
			a[i] = line[0];
			i++;
		}
		data.close();
	}

	void putdouble( double a[] )
	{
		ofstream data( "outputdouble.txt" );
		for(int i = 0; i < 30; i++ ) data << a[i] << endl;
		data.close();
	}

	void putfloat( float a[] )
	{
		ofstream data( "outputfloat.txt" );
		for(int i = 0; i < 30; i++ ) data << a[i] << endl;
		data.close();
	}

	void putint( int a[] )
	{
		ofstream data( "outputint.txt" );
		for(int i = 0; i < 30; i++ ) data << a[i] << endl;
		data.close();
	}

	void putlint( long int a[] )
	{
		ofstream data( "outputlint.txt" );
		for(int i = 0; i < 30; i++ ) data << a[i] << endl;
		data.close();
	}

	void putsint( short int a[] )
	{
		ofstream data( "outputsint.txt" );
		for(int i = 0; i < 30; i++ ) data << a[i] << endl;
		data.close();
	}

	void putuint( unsigned int a[] )
	{
		ofstream data( "outputuint.txt" );
		for(int i = 0; i < 30; i++ ) data << a[i] << endl;
		data.close();
	}

	void putsiint( signed int a[] )
	{
		ofstream data( "outputsiint.txt" );
		for(int i = 0; i < 30; i++ ) data << a[i] << endl;
		data.close();
	}

	void putchar( char a[] )
	{
		ofstream data( "outputchar.txt" );
		for(int i = 0; i < 30; i++ ) data << a[i] << endl;
		data.close();
	}

}

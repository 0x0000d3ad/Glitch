#ifndef UTILITY_COMMAND_LINE_PARSER_H
#define UTILITY_COMMAND_LINE_PARSER_H

#include <stdlib.h>

#include <iomanip>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

namespace Utility
{

namespace CommandLineParser
{

	const unsigned int width = 4;
	const std::string long_prefix( "--" );
	const std::string short_prefix( 1, '-' );
	const std::string delimeter( "--> " );
	const std::string help1( "--help" );
	const std::string help2( "-h" );

	template < typename T >
	T ArgumentParser( const char * a );

	template <>
	int ArgumentParser< int >( const char * a ) { return atoi( a ); }

	template <>
	unsigned int ArgumentParser< unsigned int >( const char * a ) { return ( unsigned int ) atoi( a ); }

	template <>
	std::string ArgumentParser< std::string >( const char * a ) { return std::string( a ); }

	template <>
	float ArgumentParser< float >( const char * a ) { return ( float ) atof( a ); }

	template <>
	double ArgumentParser< double >( const char * a ) { return atof( a ); }

	class ParentArg
	{
		public :

			ParentArg( void ) : is_set_( false ), switch_arg_( false ), description_(), short_(), long_() {}

			ParentArg( const bool & is_set, const bool & switch_arg, const char * short_description, 
				const char * long_description, const char * description ) : 
				is_set_( is_set ), switch_arg_( switch_arg ), description_( description ), 
				short_( short_description ), long_( long_description ) {}

			ParentArg( const bool & is_set, const bool & switch_arg, const std::string & short_description, 
				const std::string & long_description, const std::string & description ) : 
				is_set_( is_set ), switch_arg_( switch_arg ), description_( description ), 
				short_( short_description ), long_( long_description ) {}

			ParentArg( const ParentArg & p ) : is_set_( p.is_set_ ), switch_arg_( p.switch_arg_ ), 
				description_( p.description_ ), short_( p.short_ ), long_( p.long_ ) {}

			virtual ~ParentArg( void ) {}

			inline virtual const std::string GetShort( void ) const { return this->short_; }

			inline virtual const std::string GetLong( void ) const { return this->long_; }

			inline virtual const std::string GetDescription( void ) const { return this->description_; }

			inline virtual void SetSwitch( void ) { this->is_set_ = true; }

			inline virtual void SetValue( const std::string & str ) { ( void ) str; this->SetSwitch(); }

			inline virtual void Set( void ) { this->is_set_ = true; }

			inline virtual bool isSet( void ) { return this->is_set_; }

			inline virtual bool IsSwitch( void ) { return this->switch_arg_; }

		protected :

			bool is_set_, switch_arg_;

			std::string description_, short_, long_;

	};

	class CmdLine
	{
		public :

			CmdLine( const char * name, const char & delimeter, const char * version ) :
				name_( name ), delimeter_( 1, delimeter ), version_( version ) {}

			~CmdLine( void ) {}

			inline const std::string OutputName( void ) { return ( this->name_ + this->delimeter_ + this->version_ ); }

			void operator () ( ParentArg * p ) { this->arguments_.push_back( p ); }

			void PrintTitle( void ) const
			{
				std::cout << this->name_;
				std::cout << this->delimeter_;
				std::cout << this->version_ << std::endl;
			}

			void PrintHelp( void ) const
			{
				this->PrintTitle();
				for ( std::vector< ParentArg * >::const_iterator it = this->arguments_.begin();
					it != this->arguments_.end();
					++it )
				{
					if ( !( * it )->GetShort().empty() ) 
						std::cout << short_prefix << ( * it )->GetShort() << std::setw( width );
					if ( !( * it )->GetLong().empty() ) 
						std::cout << long_prefix << ( * it )->GetLong() << std::endl;
					std::cout << delimeter << ( * it )->GetDescription() << std::endl << std::endl;
				}
			}

			void parse( int & argc, char ** argv )
			{
				for ( unsigned int i = 0; i < ( unsigned int ) argc; ++i )
				{
					for ( std::vector< ParentArg * >::iterator it = this->arguments_.begin();
						it != this->arguments_.end();
						++it )
					{
						std::string setting;
						std::string raw_arg = std::string( * ( argv + i ) );
						if ( i + 1 < ( unsigned int ) argc )
							setting = std::string( * ( argv + i + 1 ) );
						std::string long_string_name  = long_prefix  + ( * it )->GetLong();
						std::string short_string_name = short_prefix + ( * it )->GetShort();

						if ( raw_arg == help1 || raw_arg == help2 )
						{
							this->PrintHelp();
							exit( 0 );
						}

						if ( ( long_string_name == raw_arg ) || ( short_string_name == raw_arg ) )
						{
							if ( ( * it )->IsSwitch() )
							{
								( * it )->SetSwitch();
								break;
							}

							if ( ! setting.empty() )
							{
								( * it )->SetValue( setting );
								++i;
							}
							break;
						}
					}
				}
			}

		private :

			std::string name_, delimeter_, version_;

			std::vector< ParentArg * > arguments_;
	};

	class SwitchArg : virtual public ParentArg
	{
		public :

			SwitchArg( const SwitchArg & s ) : ParentArg( s.is_set_, s.switch_arg_, s.short_, s.long_, s.description_ ), b_( s.b_ ) {}

			SwitchArg( const char * short_switch, const char * long_switch, const char * description, CmdLine & cmd, const bool & b = false ) :
				ParentArg( false, true, short_switch, long_switch, description ), b_( b )
			{ cmd( this ); }

			inline bool getValue( void ) { return this->is_set_; }

			inline virtual void SetValue( const std::string & str ) { ( void ) str; }

			inline void SetSwitch( void ) { this->is_set_ = true; }

		private :

			bool b_;
	};

	template < typename T >
	class ValueArg : virtual public ParentArg
	{
		public :

			typedef T value_type;

			ValueArg( const ValueArg & v ) : ParentArg( v.is_set_, v.switch_arg_, v.short_, v.long_, v.description_ ), 
				b_( v.b_ ), value_( v.value_ ), desc_( v.desc_ ) {}

			ValueArg( const char * short_switch, const char * long_switch, const char * description, bool b, 
				const value_type & init, const char * desc2, CmdLine & cmd ) : 
					ParentArg( false, false, short_switch, long_switch, description ), 
					b_( b ), value_( init ), desc_( desc2 )
			{ cmd( this ); }

			inline const value_type getValue( void ) const { return this->value_; }

			inline virtual void SetValue( const std::string & str ) 
			{ 
				this->SetSwitch();
				this->value_ = ArgumentParser< value_type >( str.c_str() );
			}

		private :

			bool b_;
			value_type value_;
			std::string desc_;
	};

	template < typename T >
	class MultiArg : virtual public ParentArg
	{
		public :

			typedef T value_type;

			MultiArg( const MultiArg & m ) : ParentArg( m.is_set_, m.switch_arg_, m.short_, m.long_, m.description_ ), 
				b_( m.b_ ), desc_( m.desc_ ) {}

			MultiArg( const char * short_switch, const char * long_switch, const char * description, bool b, 
				const char * desc, CmdLine & cmd ) :
				ParentArg( false, false, short_switch, long_switch, description ), b_( b ), desc_( desc )
			{ cmd( this ); }

			inline const typename std::vector< value_type > getValue( void ) const { return this->values_; }

			inline virtual void SetValue( const std::string & str ) 
			{ 
				this->SetSwitch();
				this->values_.push_back( ArgumentParser< value_type >( str.c_str() ) );
			}

		private :

			typename std::vector< value_type > values_;
			bool b_;
			std::string desc_;
	};

}

} // Utility

#endif // UTILITY_COMMAND_LINE_PARSER_H

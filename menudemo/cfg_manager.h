#pragma once
#include <fstream>
#include <iostream>

class cfg_manager //just really simple shit for now, no need to check single options for now at least
{ //TODO: make std::string work with this (some pointer workaround so it parses the c_str itself?
public:


	template <class TT>
	void save( const std::string& file_name, TT& data ) 
	{
		std::ofstream out;
	
		out.open( file_name, std::ios::binary );
		out.write( reinterpret_cast<char*>( &data ), sizeof( TT ) );
		out.close( );
	};

	template <class TT>
	void load( const std::string& file_name, TT& data ) 
	{
		std::ifstream in;
		in.open( file_name, std::ios::binary );
		in.read( reinterpret_cast<char*>( &data ), sizeof( TT ) );
		in.close( );
	};
};
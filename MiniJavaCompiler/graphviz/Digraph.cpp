#include "Digraph.h"

#include <ostream>
#include <fstream>

CDigraph::CDigraph() :
	data( "digraph {\n" )
{
}

void CDigraph::AddEdge( std::string from, std::string to )
{
	decorateName( from );
	decorateName( to );
	data += from + " -> " + to + " ; \n";
}

void CDigraph::AddEdge( std::string from, std::string to, std::string edgeName )
{
	decorateName( from );
	decorateName( to );
	decorateName( edgeName );
	data += from + " -> " + to + " [ label=\"" + edgeName + "\"] ; \n";
}

void CDigraph::Flush( const std::string& filename )
{
	data += "\n}";
	std::ofstream out( filename );
	out << data;
	out.close();
}

void CDigraph::SetNodeLabel( std::string nodeName, std::string nodeLabel )
{
	decorateName( nodeName );
	decorateName( nodeLabel );
	data += nodeName + " [ label=\"" + nodeLabel + "\" ]; \n";
}


void CDigraph::decorateName( std::string& stringToDecorate ) const {
	for( int i = 0; i < stringToDecorate.size(); i++ ) {
		if( stringToDecorate[i] == '.' || stringToDecorate[i] == ':' || stringToDecorate[i] == '=' ) {
			stringToDecorate[i] = '_';
		}
	}
}
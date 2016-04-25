// Описание: Класс инкапсулирующий создание файла dot для graphviz

#pragma once

#include <string>

class CDigraph {
public:

	explicit CDigraph();

	void SetNodeLabel( std::string nodeName, std::string nodeLabel );

	void AddEdge( std::string from, std::string to );
	void AddEdge( std::string from, std::string to, std::string edgeName );
	void Flush( const std::string& filename );

private:
	std::string data;

	void decorateName( std::string& stringToDecorate ) const;
};
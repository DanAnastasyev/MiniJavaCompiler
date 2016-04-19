// Описание: Класс инкапсулирующий создание файла dot для graphviz

#pragma once

#include <string>

class CDigraph {
public:

	CDigraph( std::string _fileName );
	~CDigraph();

	void SetNodeLabel( std::string nodeName, std::string nodeLabel );

	void AddEdge( std::string from, std::string to );
	void AddEdge( std::string from, std::string to, std::string edgeName );
	void Flush();

private:

	bool isFlushed;
	
	std::string fileName;
	std::string data;

	void decorateName( std::string& stringToDecorate );

};
// ��������: ����� ��������������� �������� ����� dot ��� graphviz

#pragma once

#include <string>

class CDigraph {
public:

	CDigraph( std::wstring _fileName );
	~CDigraph();

	void SetNodeLabel( std::string nodeName, std::string nodeLabel );

	void AddEdge( std::string from, std::string to );
	void AddEdge( std::string from, std::string to, std::string edgeName );
	void Flush();

private:

	bool isFlushed;
	
	std::wstring fileName;
	std::string data;

	void decorateName( std::string& stringToDecorate );

};
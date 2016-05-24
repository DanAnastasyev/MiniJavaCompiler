#pragma once
#include "FlowGraph.h"
#include <vector>
#include "Frame/Temp.h"
#include "Frame/Frame.h"

class CInterferenceGraphEdge;
class CInterferenceGraphNode;
class CInterferenceGraph;

class CInterferenceGraphEdge {
public:
	CInterferenceGraphEdge( CInterferenceGraphNode *f, CInterferenceGraphNode *s, bool isMove_ );
	CInterferenceGraphNode* getFirst( );
	CInterferenceGraphNode* getSecond( );
	bool IsMove( );

private:
	CInterferenceGraphNode *first;
	CInterferenceGraphNode *second;
	bool isMove;
};

class CInterferenceGraphNode {
public:
	CInterferenceGraphNode( const Temp::CTemp *t, int _id );
	void AddEdge( CInterferenceGraphNode *n, bool isMove );
	void DeleteEdge( CInterferenceGraphNode *n, bool isMove );
	bool ExistEdge( CInterferenceGraphNode *n, bool isMove );
	void DeleteAllEdges( );
	const Temp::CTemp* GetTemp( );
	int GetId( );

	std::map<CInterferenceGraphNode*, CInterferenceGraphEdge*> GetEdgeMap( );
	bool HasMoveEdge( );
	bool IsFreezed( );
	void SetFreeze( bool f );

	int GetColor( );
	void SetColor( int c );

	void SetCandidate( bool c );
	bool IsCandidate( );

	std::vector<const Temp::CTemp*> innerTemps;
private:
	int id;

	int color; //для раскрашивания графа
	const Temp::CTemp *temp;
	bool freezed;
	bool candidate;

	std::map<CInterferenceGraphNode*, CInterferenceGraphEdge*> edgeMap;
};

class CInterferenceGraph {
public:
	CInterferenceGraph( CNodeList* flowNodes, AssemFlowGraph* flowGraph, const Frame::CFrame* fr, std::map<const Temp::CTemp*, bool> onStack_ );
	CInterferenceGraph( CInterferenceGraph* graph );
	bool isReg( CInterferenceGraphNode* n );
	void AddNode( const Temp::CTemp *t );
	void AddNode( CInterferenceGraphNode *n );
	void AddEdge( CInterferenceGraphEdge *e );

	CInterferenceGraphNode* PopEdge( CInterferenceGraphEdge *e, int k );

	void DeleteEdge( CInterferenceGraphEdge *e );
	void DeleteNode( CInterferenceGraphNode *n );

	bool existEdge( CInterferenceGraphEdge *e );

	CInterferenceGraphNode* getNode( const Temp::CTemp *t );

	int nodesCount( );

	void WriteGraph( std::string path, bool enableColors, int k );

	void SetColors( int K );

	Assembler::CBaseInstructionList* UpdateInstructionList( Assembler::CBaseInstructionList* instrList, int K, const Frame::CFrame* frame, AssemFlowGraph& );
	void CleanColor( );

	bool IsColored( int K );

	bool OnStack( const Temp::CTemp* t );
	std::map<const Temp::CTemp*, bool> GetOnStack( ) { return onStack; }

	std::map<const Temp::CTemp*, int> GetColorMap( );
	std::map<std::string, std::string> GetColorStringsMap( );

private:
	int nextId;

	const Frame::CFrame* frame;
	CInterferenceGraphNode* fpNode;

	std::map<const Temp::CTemp*, CInterferenceGraphNode*> nodeMap;
	std::map<const Temp::CTemp*, bool> onStack;
	std::vector<CInterferenceGraphNode*> regNodes;

	CInterferenceGraphNode* simplify( int K );
	CInterferenceGraphNode* coalesce( int k );
	bool freeze( int k );
	CInterferenceGraphNode* spill();
	void select();

	void addRegisterClique( int k );
};
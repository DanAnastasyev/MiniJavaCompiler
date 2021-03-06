// ��������: �������������� ������ �������������� ������������� � dot ������

#include "Digraph.h"
#include "Visitors/Visitor.h"

namespace IRTree {

class IStm;
class IExpr;


class CIRTreeToDigraphConverter : public IIRTreeVisitor {
public:
	explicit CIRTreeToDigraphConverter() : minId( 0 ) {}

	// ������ ��������� ������� ������� ��������� �������
	void LinkedVisit( const IStm* node );
	void LinkedVisit( const IExpr* node );

	virtual void Visit( const CMove* node ) override;

	virtual void Visit( const CExpr* node ) override;

	virtual void Visit( const CJump* node ) override;

	virtual void Visit( const CCondJump* node ) override;

	virtual void Visit( const CSeq* node ) override;

	virtual void Visit( const CConst* node ) override;

	virtual void Visit( const CName* node ) override;

	virtual void Visit( const CTemp* node ) override;

	virtual void Visit( const CBinop* node ) override;

	virtual void Visit( const CMem* node ) override;

	virtual void Visit( const CCall* node ) override;

	virtual void Visit( const CESeq* node ) override;

	virtual void Visit( const CLabel* node ) override;

	void Flush( const std::string& filename )
	{
		treeRepresentation.Flush( filename );
		treeRepresentation = CDigraph();
	}

private:
	int minId;

	std::string lastNodeName;

	CDigraph treeRepresentation;

	void nextNameWithId( std::string label );
};

} // namespace IRTree
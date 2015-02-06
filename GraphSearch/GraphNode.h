#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include <list>

template <typename NodeType, typename ArcType> class GraphArc;

template<class NodeType, class ArcType>
class GraphNode {
private:    
    typedef GraphArc<NodeType, ArcType> Arc;
    typedef GraphNode<NodeType, ArcType> Node;

    NodeType m_data;
    list<Arc> m_arcList;
    bool m_marked;	
	int heuristic;
	GraphNode<NodeType, ArcType>* previousNode;
public:
	GraphNode() {
		previousNode = NULL;
	}

    list<Arc> const & arcList() const {
        return m_arcList;              
    }

    bool marked() const {
        return m_marked;
    }

    NodeType & data()  {
        return m_data;
    }

    void setData(NodeType data) {
        m_data = data;
    }
    
    void setMarked(bool mark) {
        m_marked = mark;
    }

	GraphNode* getPreviousPointer() {
		return previousNode;
	}

	void setPreviousPointer(GraphNode* parent) {
		previousNode = parent;
	}
           
	int getHeuristic() {
		return heuristic;
	}

	void setHeuristic(int value) {
		heuristic = value;
	}

    Arc* getArc( Node* pNode );    
    void addArc( Node* pNode, ArcType pWeight );
    void removeArc( Node* pNode );
};

template<typename NodeType, typename ArcType>
GraphArc<NodeType, ArcType>* GraphNode<NodeType, ArcType>::getArc( Node* pNode ) {

     list<Arc>::iterator iter = m_arcList.begin();
     list<Arc>::iterator endIter = m_arcList.end();
     Arc* pArc = 0;
     
     for( ; iter != endIter && pArc == 0; ++iter ) {         
          if ( (*iter).node() == pNode) {
               pArc = &( (*iter) );
          }
     }

     return pArc;
}

template<typename NodeType, typename ArcType>
void GraphNode<NodeType, ArcType>::addArc( Node* pNode, ArcType weight ) {
   Arc a;
   a.setNode(pNode);
   a.setWeight(weight);   
   m_arcList.push_back( a );
}

template<typename NodeType, typename ArcType>
void GraphNode<NodeType, ArcType>::removeArc( Node* pNode ) {
     list<Arc>::iterator iter = m_arcList.begin();
     list<Arc>::iterator endIter = m_arcList.end();

     int size = m_arcList.size();
     for( ; iter != endIter && m_arcList.size() == size;  
                                                    ++iter ) {
          if ( (*iter).node() == pNode) {
             m_arcList.remove( (*iter) );
          }                           
     }
}

#include "GraphArc.h"

#endif

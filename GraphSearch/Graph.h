#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <queue>

using namespace std;

template <class NodeType, class ArcType> class GraphArc;
template <class NodeType, class ArcType> class GraphNode;

template<class NodeType, class ArcType>
class Graph {
private:
    typedef GraphArc<NodeType, ArcType> Arc;
    typedef GraphNode<NodeType, ArcType> Node;

    Node** m_pNodes;

    int m_maxNodes;
    int m_count;
public:           
    Graph( int size );
    ~Graph();

    Node** nodeArray() const {
       return m_pNodes;
    }

    bool addNode( NodeType data, int index );
    void removeNode( int index );
    bool addArc( int from, int to, ArcType weight );
    void removeArc( int from, int to );
    Arc* getArc( int from, int to );        
    void clearMarks();
	void resetUCSNodes();
    void depthFirst( Node* pNode, void (*pProcess)(Node*) );
    void breadthFirst( Node* pNode, void (*pProcess)(Node*) );
	void breadthFirstSearch( Node* pNode, void (*pProcess)(Node*), NodeType target  );
	void UniformCostSearch( Node* pNode, Node* target ,std::vector<Node*> &path ) ;
	void UCS( Node* pNode, Node* target ) ;
	void aStar( Node* pStart, Node* pDest,/* void (*pProcess)(Node*),*/ std::vector<Node*> &path );
};

template<class NodeType, class ArcType>
Graph<NodeType, ArcType>::Graph( int size ) : m_maxNodes( size ) {
   int i;
   m_pNodes = new Node * [m_maxNodes];
   for( i = 0; i < m_maxNodes; i++ ) {
        m_pNodes[i] = 0;
   }

   m_count = 0;
}

template<class NodeType, class ArcType>
Graph<NodeType, ArcType>::~Graph() {
   int index;
   for( index = 0; index < m_maxNodes; index++ ) {
        if( m_pNodes[index] != 0 ) {
            delete m_pNodes[index];
        }
   }
   delete m_pNodes;
}

template<class NodeType, class ArcType>
bool Graph<NodeType, ArcType>::addNode( NodeType data, int index ) {
   bool nodeNotPresent = false;
   if ( m_pNodes[index] == 0) {
      nodeNotPresent = true;
      m_pNodes[index] = new Node;
      m_pNodes[index]->setData(data);
      m_pNodes[index]->setMarked(false);
      m_count++;
    }
    return nodeNotPresent;
}

template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::removeNode( int index ) {
	if( m_pNodes[index] != 0 ) {
		int node;
		Arc* arc;
		for( node = 0; node < m_maxNodes; node++ ) {
			if( m_pNodes[node] != 0 ) 
				arc = m_pNodes[node]->getArc( m_pNodes[index] );
			
			if( arc != 0 ) 
				removeArc( node, index );
		}
		delete m_pNodes[index];
        m_pNodes[index] = 0;
        m_count--;
    }
}

template<class NodeType, class ArcType>
bool Graph<NodeType, ArcType>::addArc( int from, int to, ArcType weight ) {
     bool proceed = true; 
     if( m_pNodes[from] == 0 || m_pNodes[to] == 0 ) 
         proceed = false;

     if( m_pNodes[from]->getArc( m_pNodes[to] ) != 0 ) 
         proceed = false;
     
     if (proceed == true) 
        m_pNodes[from]->addArc( m_pNodes[to], weight );
    
     return proceed;
}

template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::removeArc( int from, int to ) {
     bool nodeExists = true;
     if( m_pNodes[from] == 0 || m_pNodes[to] == 0 ) 
         nodeExists = false;
     
     if (nodeExists == true) 
        m_pNodes[from]->removeArc( m_pNodes[to] );
     
}

template<class NodeType, class ArcType>
GraphArc<NodeType, ArcType>* Graph<NodeType, ArcType>::getArc( int from, int to ) {
     Arc* pArc = 0;
     if( m_pNodes[from] != 0 && m_pNodes[to] != 0 ) 
         pArc = m_pNodes[from]->getArc( m_pNodes[to] );
                
     return pArc;
}

template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::clearMarks() {
     int index;
     for( index = 0; index < m_maxNodes; index++ ) {
          if( m_pNodes[index] != 0 ) 
              m_pNodes[index]->setMarked(false);
     }
}

template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::resetUCSNodes() {
	for (int i = 0 ; i < m_maxNodes ; i ++ ) {
		if ( m_pNodes[i] != 0 ) {
			m_pNodes[i]->setData(pair<string, int>(m_pNodes[i]->data().first, INT_MAX));
			m_pNodes[i]->setMarked(false);
			m_pNodes[i]->setPreviousPointer(NULL);
		}
	}
}

template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::depthFirst( Node* pNode, void (*pProcess)(Node*) ) {
     if( pNode != 0 ) {
           pProcess( pNode );
           pNode->setMarked(true);
           list<Arc>::iterator iter = pNode->arcList().begin();
           list<Arc>::iterator endIter = pNode->arcList().end();
        
		   for( ; iter != endIter; ++iter) {
                if ( (*iter).node()->marked() == false ) 
                   depthFirst( (*iter).node(), pProcess);
           }
     }
}

template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::breadthFirst( Node* pNode, void (*pProcess)(Node*) ) {
   if( pNode != 0 ) {
	  queue<Node*> nodeQueue;        
      nodeQueue.push( pNode );
      pNode->setMarked(true);

      while( nodeQueue.size() != 0 ) {
         pProcess( nodeQueue.front() );
         list<Arc>::const_iterator iter = nodeQueue.front()->arcList().begin();
         list<Arc>::const_iterator endIter = nodeQueue.front()->arcList().end();
         
		 for( ; iter != endIter; iter++ ) {
              if ( (*iter).node()->marked() == false) {
                 (*iter).node()->setMarked(true);
                 nodeQueue.push( (*iter).node() );
              }
         }
         nodeQueue.pop();
      }
   }  
}

template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::breadthFirstSearch( Node* pNode, void (*pProcess)(Node*), NodeType target ) {
	GraphNode<NodeType, ArcType>* temp = NULL;
	bool found = false;
	if( pNode != 0 ) {
	  queue<Node*> nodeQueue;        
      nodeQueue.push( pNode );
      while( nodeQueue.size() != 0 && found == false ) {
         pProcess( nodeQueue.front() );
         list<Arc>::const_iterator iter = nodeQueue.front()->arcList().begin();
         list<Arc>::const_iterator endIter = nodeQueue.front()->arcList().end();
         
		 for( ; iter != endIter; iter++ ) {
              if ( (*iter).node()->marked() == false) {
                 (*iter).node()->setMarked(true);
				 (*iter).node()->setPreviousPointer(nodeQueue.front() );
                 nodeQueue.push( (*iter).node() );
              }
			  if ((*iter).node()->data() == target) {
				  temp = (*iter).node();
				  found = true;
				  break;
			  }
         }
         nodeQueue.pop();
      }
   } 
}

template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::UniformCostSearch( Node* pNode, Node* target, std::vector<Node*> &path ) {
	GraphNode<NodeType, ArcType>* targetNode;	
	priority_queue<Node*, vector<Node*>, NodeSearchCostComparer> priorityQueue;		
	priorityQueue.push( pNode );				
	priorityQueue.top()->data().second = 0;		
	priorityQueue.top()->setMarked(true);		

	while( priorityQueue.size() != 0 && priorityQueue.top() != target ) {			
		list<Arc>::const_iterator iter = priorityQueue.top()->arcList().begin();	
		list<Arc>::const_iterator endIter = priorityQueue.top()->arcList().end();	
		for( ; iter != endIter; iter++ ) {      
			 if((*iter).node() != priorityQueue.top()->getPreviousPointer()) {      
				 int childDist = (*iter).weight() + priorityQueue.top()->data().second;		
				 if ( childDist < (*iter).node()->data().second) {      
					 (*iter).node()->data().second = childDist;			
					 (*iter).node()->setPreviousPointer(priorityQueue.top());		
				 }
				 if ((*iter).node()->marked() == false ) {      
					 (*iter).node()->setMarked(true);			
					 priorityQueue.push( (*iter).node() );		
				 }
			 }
		}
		priorityQueue.pop();	
	}	
	targetNode = priorityQueue.top();	
	path.push_back(targetNode);			
	while (targetNode->getPreviousPointer() != NULL )  {    
		cout << targetNode->data().first << endl;			
		path.push_back(targetNode->getPreviousPointer());	
		targetNode = targetNode->getPreviousPointer();		
	}
	cout << pNode->data().first << endl;		
}

template<class NodeType, class ArcType>
void Graph<NodeType,ArcType>::UCS(Node* pNode, Node* target) {
	resetUCSNodes();
	
	GraphNode<NodeType, ArcType>* targetNode;
	priority_queue<Node*, vector<Node*>, NodeSearchCostComparer> priorityQueue;
	priorityQueue.push( pNode );				
	priorityQueue.top()->data().second = 0;		
	priorityQueue.top()->setMarked(true);		

	while( priorityQueue.size() != 0 && priorityQueue.top() != target ) {			
		list<Arc>::const_iterator iter = priorityQueue.top()->arcList().begin();	
		list<Arc>::const_iterator endIter = priorityQueue.top()->arcList().end();	
		for( ; iter != endIter; iter++ ) {    
			 if( (*iter).node() != priorityQueue.top()->getPreviousPointer() ) {      
				 int childDist = (*iter).weight() + priorityQueue.top()->data().second;		
				 if ( childDist < (*iter).node()->data().second) {     
					 (*iter).node()->data().second = childDist;			
					 (*iter).node()->setPreviousPointer(priorityQueue.top());		
				 }
				 else { } 
				 if ((*iter).node()->marked() == false ) {      
					 (*iter).node()->setMarked(true);			
					 priorityQueue.push( (*iter).node() );		
				 }
			 }
		}
		priorityQueue.pop();	
	}	
	targetNode = priorityQueue.top();	
	//path.push_back(targetNode);			

	/*while (target->getPreviousPointer() != NULL )  {   
		cout << target->data().first << endl;		
		path.push_back(target);			
		target= target->getPreviousPointer();			
	}
	cout << pNode->data().first << endl;		
	cout << "//************ " << endl;*/

	pNode->setHeuristic((targetNode->data().second * 90) / 100);
}

template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::aStar( Node* pStart, Node* pDest, /*void (*pProcess)(Node*),*/ std::vector<Node*> &path ) {
	GraphNode<NodeType, ArcType>* startNode, target;
	priority_queue<Node*, vector<Node*>, NodeSearchCostComparer> priorityQueue;
	
	for( int  i = 0; i < m_maxNodes; i ++ ) {
		UCS(m_pNodes[i], pDest);
		m_pNodes[i]->data().second = INT_MAX;
		resetUCSNodes();
	}
	pStart->data().second = 0;
	pStart->setMarked(true);
	priorityQueue.push( pStart );		
	while( priorityQueue.size() != 0 && priorityQueue.top() != pDest ) {
		list<Arc>::const_iterator iter = priorityQueue.top()->arcList().begin();
		list<Arc>::const_iterator endIter = priorityQueue.top()->arcList().end();
		for( ; iter != endIter; iter++ ) {
			if( (*iter).node() != priorityQueue.top() && (*iter).node()->marked() == false ) {
				int childDist = (*iter).weight() + priorityQueue.top()->data().second;
				if( childDist < (*iter).node()->data().second ) {
					(*iter).node()->data().second = childDist;
					(*iter).node()->setPreviousPointer( priorityQueue.top() );
				}
				if( (*iter).node()->marked() == false ) {
					(*iter).node()->setMarked(true);
					priorityQueue.push( (*iter).node() );
				}
			}
		}
		priorityQueue.pop();
	}
	
	path.push_back(pDest);			

	while (pDest->getPreviousPointer() != NULL )  {   
		cout << pDest->data().first << endl;		
		path.push_back(pDest);			
		pDest= pDest->getPreviousPointer();			
	}
	cout << pStart->data().first << endl;	
}

#include "GraphNode.h"
#include "GraphArc.h"

#endif

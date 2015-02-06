#include <iostream>
#include <fstream>
#include <string>

#include "Graph.h"

#include "stdafx.h"

#ifdef _DEBUG
#pragma comment(lib,"sfml-graphics-d.lib")
#pragma comment(lib,"sfml-audio-d.lib")
#pragma comment(lib,"sfml-system-d.lib")
#pragma comment(lib,"sfml-window-d.lib")
#pragma comment(lib,"sfml-network-d.lib")
#else
#pragma comment(lib,"sfml-graphics.lib")
#pragma comment(lib,"sfml-audio.lib")
#pragma comment(lib,"sfml-system.lib")
#pragma comment(lib,"sfml-window.lib")
#pragma comment(lib,"sfml-network.lib")
#endif
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#include "SFML/Graphics.hpp"
#include "SFML/OpenGL.hpp"

using namespace std;

typedef GraphArc<pair<string, int>, int> Arcs;
typedef GraphNode<pair<string, int>, int> Node;
/*
void visit( Node * pNode ) {
	cout << "Visiting: " << pNode->data() << endl;
}*/

int main(int argc, int *argv[]) {
   
    Graph<pair<string, int>, int> graph( 30 );
	string stringArray[30];
	string tempString;
	std::vector<Node*> path;
	ifstream myfile;
	myfile.open ("Nodeys.txt");

	int infinite = INT_MAX;

	pair<string, int> temp;
	temp.second = infinite;

	int i = 0;
	while (!myfile.eof()) {
		getline(myfile, tempString);
		stringArray[i] = tempString;		
		temp.first = stringArray[i];
		graph.addNode(temp, i);
		i++;
	}

	myfile.close();
	myfile.open("Arcys.txt");

	int from, to, weight;
	while ( myfile >> from >> to >> weight ) {
		graph.addArc(from, to, weight);
	}
    myfile.close();

	graph.aStar(graph.nodeArray()[0], graph.nodeArray()[29], path);
	system("PAUSE");
	
}

class NodeSearchCostComparer {
public:
	bool operator()(Node * n1, Node * n2) {
		pair<string, int> p1 = n1->data();
		pair<string, int> p2 = n2->data();
		return p1.second > p2.second; 
	}
};


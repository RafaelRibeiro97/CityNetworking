//ASA Project 1
//2016/2017 - 2st Semestre
//Instituto Superior Tecnico
//U.C. Análise e Síntese de Algoritmos
//Francisco Barros, nº 85069
//Rafael Ribeiro, nº 84758

//Must be compiled with "g++ -o3 -ansi -Wall -o citynetworking citynetworking.cpp"

#include <stdio.h>
#include <iostream>
#include <vector>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>

/*------------------------------------------------------------------------------
			CONSTANTS
------------------------------------------------------------------------------*/
using namespace std;
using std::pair;


/*------------------------------------------------------------------------------
			GLOBAL VARIABLES
------------------------------------------------------------------------------*/
// Abstract city to which all cities with airports connect to.
int skyCity;
int skyCityCost = 0;
// Counts how many edges MST obtains during execution.
int airwayMstEdgeCount = 0;
// Output variables.
int airwayNetworkCost = 0; 			// Total weight of the MST (Minimum Spanning Tree).
int airwayNetworkRoads = 0; 		// Number of roads in the MST.
int airwayNetworkAirports = 0; 	// Number of airports in the MST.

int roadMstEdgeCount = 0;
int roadNetworkCost = 0; 		// Total weight of the MST (Minimum Spanning Tree).
int roadNetworkRoads = 0; 		// Number of roads in the MST.

bool airportsWereUsed = false;
bool validRoadMst = false;
bool validAirwayMst = false;

/*------------------------------------------------------------------------------
			STRUCTS
------------------------------------------------------------------------------*/
// Connection represents a road or airway between city int:a to city int:b.
typedef pair<int, int> Connection;
// Edge is the pair of elements whose elements are a connection and the respective int:cost.
typedef pair<Connection, int> Edge;

/*------------------------------------------------------------------------------
			AUXILIAR FUNCTIONS
------------------------------------------------------------------------------*/

/**
* Auxiliar boolean function used on edgeWeightComparator for vector::sort
* Returns true if and only if edge is not an airway to skyCity and anotherEdge is.
*/
bool airwayComparator(Edge edge1, Edge edge2) {
	// Gets the second city in the Connection pair of edge1 and edge2
	int a = edge1.first.second;
	int b = edge2.first.second;
	if (a != skyCity && b == skyCity) {
		return true;
	}
	else {
		return false;
	}
}

/**
* Comparator used for sorting the vector of concrete edges with vector::sort
* Returns true (sort) if:
* Edge is lighter then anotherEdge
* Edges have the same weight but Edge isnt an airway and anotherEdge is.
* Returns false (dont sort) if:
* Edge is heavier than anotherEdge;
* Edges have same weight and both are airways;
* Edges have same weight, Edge is an airway but anotherEdge isnt;
*/
bool edgeWeightComparator(const Edge& edge, const Edge& anotherEdge) {
	if (edge.second < anotherEdge.second)
    	return true;
  else if (edge.second == anotherEdge.second) {
		if (airwayComparator(edge, anotherEdge)) {
			return true;
		}
 	}
	return false;
}

/** Prints the output when a proper newtwork is created. */
void outputMST() {
	cout << airwayNetworkCost << endl;
	cout << airwayNetworkAirports << " " << airwayNetworkRoads << endl;
}
void outputRoads() {
	cout << roadNetworkCost << endl;
	cout << "0" << " " << roadNetworkRoads << endl;
}

/*------------------------------------------------------------------------------
			CLASSES
------------------------------------------------------------------------------*/
class Graph {
	private:
		int graphVertices;
		int networkMaxAirports;
		int networkMaxRoads;
		int *predecessor;
		int *rank;
		int *predecessorRoads;
		int *rankRoads;
		// Vector holds all concrete Edges<Connection, cost> in this graph.
		vector<Edge> edgeVector;
		vector<Edge> roads;

	public:
		Graph(int vertices) {
			this->graphVertices = vertices;
			this->networkMaxAirports = 0;
			this->networkMaxRoads = 0;
		}

		~Graph() {
			delete[] this->predecessor;
			delete[] this->rank;
		}

		// Getters.
		int getGraphVertices() { return graphVertices; }
		int getMaxAirports() { return networkMaxAirports; }
		int getMaxRoads() { return networkMaxRoads; }

		// Setters.
		void setMaxAirports(int i) { networkMaxAirports = i; }
		void setMaxRoads(int i) { networkMaxRoads = i; }

		// Edge methods
		/** Creates a new edge, <connection, cost>. */
		Edge newEdge(int a, int b, int c) {
			Connection con = make_pair(a, b);
			Edge e = make_pair(con, c);
			return e;
		}

		/** Adds the new adge to the graph.*/
		void addEdge(Edge e) {
			edgeVector.push_back(e);
		}
		void addRoad (Edge e) {
			roads.push_back(e);
		}

		/** Prints to standard output the information of the given edge. */
		void printEdge(Edge e) {
			Connection connection = e.first;
			int connectionCost = e.second;
			int cityA = connection.first;
			int cityB = connection.second;
			cout << "city a: " << cityA << ", city b: " << cityB << ", cost: " << connectionCost << endl;
		}

		/** Prints to standart output the information of all edges in this graph. */
		void printEdgeList() {
			cout << endl << "graph.printEdgeList()" << endl;
			vector<Edge>::const_iterator ci;
			for (ci = edgeVector.begin(); ci != edgeVector.end(); ci++) {
				printEdge(*ci);
			}
		}

		/** Sorts the concrete edges in this graphs edgeVector */
		void sortEdges() {
			sort(edgeVector.begin(), edgeVector.end(), edgeWeightComparator);
		}

		void sortRoads() {
			sort(roads.begin(), roads.end(), edgeWeightComparator);
		}

		/** Calculates the MST for this graph, generating the desired network. */
		void kruskalMST() {
			int u, v, setU, setV;
			makeSet();
			sortEdges();

			vector<Edge>::const_iterator it;
			for (it = edgeVector.begin(); it != edgeVector.end(); it++) {
				u = (*it).first.first;
				v = (*it).first.second;
				setU = findSet(u);
				setV = findSet(v);
				if (setU != setV) {
					if (v == skyCity) {
						if (networkMaxAirports > 1) {
							airwayNetworkAirports++;
							airportsWereUsed = true;
							airwayNetworkCost += (*it).second;
							uniteSet(setU, setV);
							if (skyCityCost == 0) {
								skyCityCost = (*it).second;
							}
						}
					}
					else {
						airwayNetworkRoads++;
						airwayNetworkCost += (*it).second;
						uniteSet(setU, setV);
					}
				}
			}

			if (airwayNetworkAirports == 1) {
				airwayNetworkAirports = 0;
				airwayNetworkCost -= skyCityCost;
				airportsWereUsed = false;
			}
		}

		/** Finds the set to which given vertex(city) belongs to */
		int findSet(int u) {
			if (u != predecessor[u]) {
				int i = predecessor[u];
				predecessor[u] = findSet(i);
			}
			return predecessor[u];
		}

		/** Initializes one set each with one vertex equal to skyCity int value. */
		void makeSet() {
			predecessor = new int[skyCity];
			rank = new int[skyCity];
			for (int i = 0; i < skyCity; i++) {
				rank[i] = 0;
				predecessor[i] = i;
			}
		}

		/** If u and v don't belong to the same set, unite them into the growing MST */
		void uniteSet(int u, int v) {
			u = findSet(u);
			v = findSet(v);
			if (rank[u] > rank[v])
				predecessor[v] = u;
			else
				predecessor[u] = v;
			if (rank[u] == rank[v])
				(rank[v])++;
			airwayMstEdgeCount++;
		}

		void kruskalMSTRoads() {
			int u, v, setU, setV;
			makeSetRoads();
			sortRoads();

			vector<Edge>::const_iterator it;
			for (it = roads.begin(); it != roads.end(); it++) {
				u = (*it).first.first;
				v = (*it).first.second;
				setU = findSetRoads(u);
				setV = findSetRoads(v);
				if (setU != setV) {
					roadNetworkRoads++;
					roadNetworkCost += (*it).second;
					uniteSetRoads(setU, setV);
				}
			}
		}

		int findSetRoads(int u) {
			if (u != predecessorRoads[u]) {
				int i = predecessorRoads[u];
				predecessorRoads[u] = findSetRoads(i);
			}
			return predecessorRoads[u];
		}

		/** Initializes one set each with one vertex equal to skyCity int value. */
		void makeSetRoads() {
			predecessorRoads = new int[getMaxRoads()+1];
			rankRoads = new int[getMaxRoads()+1];
			for (int i = 0; i <= getMaxRoads(); i++) {
				rankRoads[i] = 0;
				predecessorRoads[i] = i;
			}
		}

		/** If u and v don't belong to the same set, unite them into the growing MST */
		void uniteSetRoads(int u, int v) {

			u = findSetRoads(u);
			v = findSetRoads(v);

			if (rankRoads[u] > rankRoads[v])
				predecessorRoads[v] = u;
			else
				predecessorRoads[u] = v;

			if(rankRoads[u] == rankRoads[v])
				(rankRoads[v])++;

			roadMstEdgeCount++;
		}
	};

/*------------------------------------------------------------------------------
			Application
------------------------------------------------------------------------------*/
int main() {
	/**
	* Input variables.
	* int:aux variable is used to fetch the numbers such as number of cities, etc.
	* int:a, int:b are both graphVertices, representing two cities.
	* int:c represents the cost of building an airport on city a or a road between a and b.
	*/
	int aux, a, b, c, i, vertices;
	Edge e; //Edge (auxiliar).

	// Gets the number of cities (graph vertices) meant to be connected.
	cin >> vertices;
	skyCity = vertices + 1;
	Graph graph(vertices);

	// Gets the max number of airports (networkMaxAirports).
	cin >> aux;
	graph.setMaxAirports(aux);

	// Creates all the airways, connecting all cities with airports to skyCity.
	for (i = 0; i < aux; i++) {
		cin >> a >> c;
		e = graph.newEdge(a, skyCity, c);
		graph.addEdge(e);
	}

	// Gets the max number of roads (networkMaxRoads).
	cin >> aux;
	graph.setMaxRoads(aux);

	// Creates all the roads connecting city a to city b, if a and b aren't the same
	for (i = 0; i < aux; i++) {
		cin >> a >> b >> c;
		if (a != b) {
			e = graph.newEdge(a, b, c);
			graph.addRoad(e);
			graph.addEdge(e);
		}
	}

	// Runs Kruskal's algorithms to find the MST.
	graph.kruskalMSTRoads();
	graph.kruskalMST();

	// Check sufficiency for the generated MSTs.
	if (roadMstEdgeCount == vertices - 1)
		validRoadMst = true;
	if ((airportsWereUsed) && (airwayMstEdgeCount == vertices))
		validAirwayMst = true;
	else if ((!airportsWereUsed) && (airwayMstEdgeCount == vertices - 1))
		validAirwayMst = true;

	// Print the proper output.
	if (!validAirwayMst && !validRoadMst)
		cout << "Insuficiente" << endl;
	else if (!validAirwayMst && validRoadMst)
		outputRoads();
	else if (validAirwayMst && !validRoadMst)
		outputMST();
	else if (validAirwayMst && validRoadMst) {
		if (airwayNetworkCost < roadNetworkCost)
			outputMST();
		else if (roadNetworkCost <= airwayNetworkCost)
			outputRoads();
	}

	return 0;
}

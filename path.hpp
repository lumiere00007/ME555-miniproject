// this file includes class definitions and getter and setter methods

//including required files
#include<string>
#include<vector>
#include<list>
#include<cmath>
#include<iterator>
#include<utility>
#include<set>
#include<iostream>
#include<iomanip>
#include<cstring>
#include<algorithm>

//defining infinity
#define Infinity INFINITY

//class point
class Point
{
    friend class Node;
    friend class Graph;

private:
    //x and y co-ordinates
    double x {0};
    double y {0};

public:

    //getter methods
    double GetPointX (Point Point)
    {
        return Point.x;
    }

    double GetPointY (Point Point)
    {
        return Point.y;
    }
};


//class edge
class Edge
{
    friend class Graph;
    friend class Node;
private:
    //node to which source node is connected and the associated weight
    int DestNode {0};
    double Weight{0};

public:

    //constructors

    //default constructor
    Edge()
    {
    }

    //Overloading the '<' operator for sort
    inline Edge(int DestNodeID, double NodeWeight) : DestNode(DestNodeID), Weight(NodeWeight)
    {
    }

    //Overloading the '<' operator for sort
    inline bool operator< (const Edge &EdgeObject) const
    {
        return DestNode < EdgeObject.DestNode;
    }

    // Setter Methods

    void SetEdge (int DestNodeIP , double WeightIP)
    {
        DestNode = DestNodeIP;
        Weight = WeightIP;
    }
    void SetWeight (double WeightIP)
    {
        Weight =  WeightIP;
    }

    //Getter Methods

    int GetDestNode()
    {
        return DestNode;
    }

    double GetWeight()
    {
        return Weight;
    }
};


//node class
class Node
{
    friend class Edge;
    friend class Graph;

private :
    //Node ID and the coordinates associated with it
    int ID;
    Point point;


public:

    std::list <Edge> EdgeList;

    //Constructor
    Node()
    {
        ID = 0;
        point.x = 0;
        point.y = 0;
    }

    //Getter methods

    int GetID() const

    {
        return ID;
    }

    Point GetPoint()
    {
        return point;
    }

    std::list<Edge> GetEdgeList() const
    {
        return EdgeList;
    }

    //Setter Methods

    void SetID(int IPID)
    {
        ID = IPID;
    }

    void SetPoint(double xIP, double yIP)
    {
        point.x = xIP;
        point.y = yIP;
    }


};


//Graph class
class Graph
{
private:
    //int to store number of vertices
    int v{1};


public:

    //getter methods
    int GetNumberofVertices () const
    {
        return v;
    }
    std::vector <Node> Nodes;

    //increment the number of vertices
    void IncrementVerticesNo()
    {
        v++;
    }

    //Checking if the node exists
    bool CheckDuplicateByNode (Node INode);


    //adding the edge in a graph operation
    void AddNode(Node NewNode);


    //Adding edge
    void AddEdge(int ID1, int ID2);


    //get node by ID
    Node GetNodeByID(int InputId);


    //function to print the graph
    void PrintGraph (Graph  Graph);

    //function to check for duplicate values of edges
    bool CheckIfEdgeExists (int ID1 , int ID2);


    //Get the cost matrix representation of the cost matrix
    std::vector<std::vector<double>> GetCostMatrix (Graph Graph);

    //function to sort the list
    static Graph SortList (Graph IGraph);

    //function to delete an edge if it exists
    static Graph DeleteEdge (Graph Graph, int ID1, int ID2);
};







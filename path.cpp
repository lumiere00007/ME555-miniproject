
//including required files

#include"path.hpp"
#include<fstream>
#include <string>
#include <iostream>
#include<ostream>


Graph GetData (std::string filename , Graph  Graph);
//function to retrieve the nearest unvisited node
int GetNearestNode(std::vector<bool> isvisited, int v, std::vector<double> total, std::vector<double> hcost)
{

    double min = Infinity;
    int minnode=0;

    //iterating over vertices
    for (int i=0;i<v;i++)
    {
        //if the node is not visited and the sum of the cost and the heuristic is the minimum, return that node
        if((total[i] + hcost[i]) < min && !isvisited[i]  )
        {
            //heuristic cost is zero for dijkstra
            min = total[i]+ hcost[i];
            minnode= i;
        }

    }

    return minnode;
}
//get the index of the node in the graph
int GetIndex (Graph Graph, int IdToFind)
{
    //variables
    int v = Graph.GetNumberofVertices();
    int ID=-1;


    for( int i = 0; i<v;i++)
    {
        if(Graph.Nodes.at(i).GetID() == IdToFind)
        {
            //if the Id to find is found, its index is returned
            ID =i;
            break;
        }

    }

    //if not error
    if (ID ==-1)
    {
        std::cerr<<"No Node found with the provided Index";
        exit(1);
    }
    return ID;

}

//algorithm for dijkstras algorithm
void DijsktrasAlgorithm (Graph Graph,  int src , int dest)
{
    int v = Graph.GetNumberofVertices();

    //variables
    std::vector< std::vector<double> > cost;
    std::vector<double>  hcost;


    //getting cost matrix representation
    cost = Graph.GetCostMatrix(Graph);

    std::vector<double> total;
    std::vector<bool> isvisited;
    std::vector<int> ParentNodes;

    //initializing
    for(int i=0; i<v ; i ++)
    {
        total.push_back(Infinity);
        isvisited.push_back(false);
        ParentNodes.push_back(i);
        hcost.push_back(0);
    }

    //getting index
    int SrcIndex = GetIndex(Graph,src);
    int DestIndex = GetIndex(Graph, dest);

    //assigning the distance of source node from itself as zero
    total.at(SrcIndex)=0;

    //iterate over nodes
    for (int i=0;i<v-1;i++)
    {
        //get the nearest unvisited node with the least cost
        int NearestNode = GetNearestNode(isvisited,v,total, hcost);

        //mark that code as visited
        isvisited[NearestNode] = true;

        for(int j=0;j<v-1;j++)
        {
            //iterating over distances and if the new dist is min, saving it in the totals vector
            if(cost[NearestNode][j] &&
               total[j]> (total[NearestNode]+cost[NearestNode][j])  &&
               total[NearestNode]!=Infinity &&
               !isvisited[j])
            {
                total[j]= total[NearestNode] +  cost[NearestNode][j];
                ParentNodes[j]= NearestNode;
            }
        }

    }

    //if the dist is infinity no path exists
    if (total.at(DestIndex)== Infinity)
    {
        std::cout<< "No Path Exists"<< std::endl;
    }
    else
    {
        //Get the parent nodes vector
        int k = dest;
        int CurrentParentNode = ParentNodes[k];
        std::vector<int> Parent;
        while (CurrentParentNode != src) {
            Parent.insert(Parent.begin(), CurrentParentNode);
            CurrentParentNode = ParentNodes[CurrentParentNode];
        }

        //printing the path and distance
        std::cout << src << " ";

        for (int i = 0; i < Parent.size(); i++) {
            std::cout << Parent[i] << " ";
        }
        std::cout << dest << " : ";
        std::cout << total.at(DestIndex) << std::endl;
    }
}

// check if two given line segments intersect

bool onSegment(Point p, Point q, Point r)
{
    if (q.GetPointX(q) <= std::max(p.GetPointX(p), r.GetPointX(r)) && q.GetPointX(q) >= std::min(p.GetPointX(p), r.GetPointX(r)) &&
        q.GetPointY(q) <= std::max(p.GetPointY(p), r.GetPointY(r)) && q.GetPointY(q) >= std::min(p.GetPointY(p), r.GetPointY(r)))
        return true;

    return false;
}

int orientation(Point p, Point q, Point r)
{
    // See https://www.geeksforgeeks.org/orientation-3-ordered-points/
    double val = (q.GetPointY(q) - p.GetPointY(p)) * (r.GetPointX(r) - q.GetPointX(q)) -
                 (q.GetPointX(q) - p.GetPointX(p)) * (r.GetPointY(r) - q.GetPointY(q));

    if (val == 0) return 0;  // collinear

    return (val > 0)? 1: 2; // clock or counterclock wise
}

bool doIntersect(Point p1, Point q1, Point p2, Point q2)
{
    //  four orientations
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases

    // p1, q1 and p2 are collinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;

    // p1, q1 and q2 are collinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;

    // p2, q2 and p1 are collinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;

    // p2, q2 and q1 are collinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false;
}

//Update the graph after deleting obstacles
Graph UpdateAfterObstacles(Graph Graph, std::vector < std::vector<int> > Obstacles)
{
    for (int k =0 ; k< Obstacles.size(); k++)
    {
        for (int j=0 ;j<Obstacles.at(k).size()-1;j++)
        {
            //getting points of obstacle fences
            Point A = Graph.GetNodeByID(Obstacles.at(k).at(j)).GetPoint();
            Point B = Graph.GetNodeByID(Obstacles.at(k).at(j+1)).GetPoint();

            //getting edges
            for (int i=0;i<Graph.Nodes.size();i++)
            {
                Point A1 = Graph.Nodes.at(i).GetPoint();
                std::list <Edge> e = Graph.Nodes.at(i).GetEdgeList();
                for (auto it = e.begin(); it!= e.end();it++)
                {
                    //if they intersect delete edges
                    int id = it->GetDestNode();
                    Point B1 = Graph.Nodes.at(id).GetPoint();

                    bool Intersect = false;
                    Intersect= doIntersect(A,B,A1,B1);

                    if (Intersect)
                    {
                        Graph = Graph.DeleteEdge(Graph,id,i);
                    }
                }

            }

        }

        //repeating the above operation for the line cconnecting the first and the last element in the obstacle line
        int m= Obstacles.at(k).size();
        Point A = Graph.GetNodeByID(Obstacles.at(k).at(m-1)).GetPoint();
        Point B = Graph.GetNodeByID(Obstacles.at(k).at(0)).GetPoint();
        for (int i=0;i<Graph.Nodes.size();i++)
        {
            Point A1 = Graph.Nodes.at(i).GetPoint();
            std::list <Edge> e = Graph.Nodes.at(i).GetEdgeList();
            for (auto it = e.begin(); it!= e.end();it++)
            {
                int id = it->GetDestNode();
                Point B1 = Graph.Nodes.at(id).GetPoint();

                bool Intersect = false;
                Intersect= doIntersect(A,B,A1,B1);

                if (Intersect)
                {
                    Graph = Graph.DeleteEdge(Graph,id,i);
                }
            }
        }
    }
    //return the updated graph
    return Graph;
}


//function to get the heuristic cost vector
std::vector<double> GetHeuristic (Graph Graph, int src, int dest)
{

    //src and dest are index
    int v = Graph.Nodes.size();

    std::vector<double> Heuristic;

    //initializer
    for (int i = 0; i < v; i++)
    {
        Heuristic.push_back(Infinity);
    }

    //getting the point coordinates of the destination node
    Point A = Graph.Nodes.at(dest).GetPoint();
    double x1 = Graph.Nodes.at(dest).GetPoint().GetPointX(A) ;
    double y1 = Graph.Nodes.at(dest).GetPoint().GetPointY(A) ;


    for (int i = 0; i < v; i++) //rows
    {
        //getting the i th point
        Point B = Graph.Nodes.at(i).GetPoint();
        double x2 = Graph.Nodes.at(i).GetPoint().GetPointX(B) ;
        double y2 = Graph.Nodes.at(i).GetPoint().GetPointY(B) ;

        //heuristic cost is the euclidian distance
        double hcost = sqrt((pow((x1 - x2),2))+(pow((y1 - y2),2)));
        Heuristic.at(i)= hcost;

    }

    //returning the heuristic cost vector
    return Heuristic;
}

//function for the A* algorithm
void AStar (Graph Graph, int src, int dest)
{
    int v = Graph.GetNumberofVertices();



    //getting cost matrix representation
    std::vector<std::vector<double>> cost;
    std::vector<double> hcost;
    cost = Graph.GetCostMatrix(Graph);

    //variables
    std::vector<double> total;
    std::vector<bool> isvisited;
    std::vector<int> ParentNodes;

    //initializing
    for(int i=0; i<v ; i ++)
    {
        total.push_back(Infinity);
        isvisited.push_back(false);
        ParentNodes.push_back(i);
    }

    //getting index
    int SrcIndex = GetIndex(Graph,src);
    int DestIndex = GetIndex(Graph, dest);
    hcost = GetHeuristic(Graph,SrcIndex,DestIndex);

    //assigning the distance of source node from itself as zero
    total.at(SrcIndex)=0 ;

    //iterate over nodes
    for (int i=0;i<v-1;i++)
    {
        //get the nearest unvisited node with the least cost
        int NearestNode = GetNearestNode(isvisited,v,total , hcost);

        //mark that code as visited
        isvisited[NearestNode] = true;

        for(int j=0;j<v-1;j++)
        {
            //iterating over distances and if the new dist is min, saving it in the totals vector
            if(cost[NearestNode][j]!=Infinity &&
               total[j]> (total[NearestNode]+cost[NearestNode][j])  &&
               total[NearestNode]!=Infinity &&
               !isvisited[j])
            {
                total[j]= total[NearestNode] +  cost[NearestNode][j];
                ParentNodes[j]= NearestNode;
            }
        }
    }

    //if the dist is infinity no path exists
    if (total.at(DestIndex)== Infinity)
    {
        std::cout<< "No Path Exists"<< std::endl;
    }
    else
    {
        //Get the parent nodes vector
        int k = dest;
        int CurrentParentNode = ParentNodes[k];
        std::vector<int> Parent;
        while (CurrentParentNode != src) {
            Parent.insert(Parent.begin(), CurrentParentNode);
            CurrentParentNode = ParentNodes[CurrentParentNode];
        }
        //printing the path and distance
        std::cout << src << " ";

        for (int i = 0; i < Parent.size(); i++) {
            std::cout << Parent[i] << " ";
        }
        std::cout << dest << " : ";
        std::cout << total.at(DestIndex) << std::endl;
    }
}

//function to check if string is an integer
bool isNumber (const std::string & s)
{
    //iterating over string
    for (char const &ch : s) {
        //if no digits found return false
        if (std::isdigit(ch) == 0)
            return false;
    }
    return true;
}


//to check if a string is a double
bool isnumberDouble(const std::string& s)
{
    double ld;

    //returning true if string is a double
    return((std::istringstream(s) >> ld >> std::ws).eof());
}


//function to check if graph nodes are sorted
void CheckGraph(Graph Graph)
{
    //iterating over the nodes
    for (int i =1; i < Graph.Nodes.size(); i++)
    {
        //if consecutive node IDs are not consecutive
        if (Graph.Nodes.at(i).GetID() != Graph.Nodes.at(i-1).GetID()+1)
        {
            std::cerr<<"Your Nodes are not sorted";
            std::exit(1);
        }
    }

}

//Count how many values are passed on the line
int CountInputs (std::string const& str)
{
    std::stringstream stream(str);
    //retuning the number
    return std::distance( std::istream_iterator<std::string>(stream) , std::istream_iterator<std::string>());
}

//parsing function for the grid map
Graph GetData (std::string filename , Graph  Graph)
{
    //declaring filestream
    std::ifstream file;
    file.open(filename);

    //in case file doesnt exist throw exception
    if (!file)
    {
        throw 0;
    }

    //temporary buffer
    std::string buffer;

    //flags to keep track of file sections
    bool NodeFlag{false};
    bool EdgeFlag {false};

    //counter to count how many flags were set
    int CounterFlag{0};

    //variables to store inputs
    int ip1{0};
    double ip2{2}, ip3{0};
    std::string i1, i2, i3;
    Node Node;
    int e1{0}, e2{0};
    std::string ep1, ep2;
    std::string NodeStr = "$node\r";
    std::string EdgeStr = "$edge\r";

    //counter
    int i{0};


    //getting line by line input from file
    while(std::getline(file,buffer))
    {
        //handle null line
        if (buffer == "")
        {
            std::cerr << "\n Warning Null line \n";
            continue;
        }

        //converting string to stringstream
        std::istringstream line{buffer};

        i = i+1;

        //setting flag if found the node section
        if((NodeStr.compare(buffer) == 0)||(buffer == "$node"))
        {
            NodeFlag = true;
            EdgeFlag = false;
            CounterFlag++;
            continue;
        }

        //setting flag if found edge section
        if ((EdgeStr.compare(buffer) == 0)|| (buffer == "$edge"))
        {
            EdgeFlag = true;
            NodeFlag = false;
            CounterFlag++;
            continue;
        }

        //if inside node section code
        if (NodeFlag==true && EdgeFlag==false)
        {

            //storing the line in strings
            line >> i1 >> i2 >> i3;

            //checking if the string is int (Nodeid) & handling
            if(!isNumber(i1))
            {
                throw 'e';
            }

            //checking if they are doubles & handling (coordinates)
            if(!isnumberDouble(i2) || !isnumberDouble(i3))
            {
                throw 'd';
            }

            int InputCount = CountInputs(buffer);
            if (InputCount!=3)
            {
                throw 'n';
            }
            //converting to int and doubles
            ip1 = stoi(i1);
            ip2 = stod(i2);
            ip3 = stod(i3);

            //setting NodeID
            Node.SetID(ip1);
            //setting point
            Node.SetPoint(ip2, ip3);

            //inserting node in the graph class
            Graph.AddNode(Node);
            Graph.IncrementVerticesNo();

            continue;
        }

        //if inside the edge section
        if (EdgeFlag)
        {
            //storing the line in strings
            line >> ep1 >> ep2;

            //checking and handling if the provided values are integers
            if(!isNumber(ep1) || !isNumber(ep2))
            {
                throw 'e';
            }
            int InputCount = CountInputs(buffer);

            //handling : if there are not required number of values
            if (InputCount!=2)
            {
                throw 'n';
            }

            //converting strings to ints
            e1= stoi(ep1);
            e2= stoi(ep2);

            //insertting the edge in the graph
            Graph.AddEdge(e1,e2);
        }

        //If no section headers were set, handling the case
        if (NodeFlag==false && EdgeFlag==false)
        {
            throw 1;
        }
    }

    //handling if not enough sections were found
    if (CounterFlag != 2)
    {
        throw 2;
    }

    //check if the graph nodes are in order
    CheckGraph(Graph);

    //close file
    file.close();

    //return graph
    return Graph;
}

//function to check if the NodeId is valid and exits
int ConvertAndCheckValidity( std::string Str, Graph Graph)
{
    //checking if string is an int
    bool check = isNumber(Str);

    //handling if not
    if (!check)
    {
        std::cerr<<"enter a Number argument";
        std::exit(1);
    }

    //converting to int
    int Integer = std::stoi(Str);
    int returnval =-1;


    //if value founf returning index
    for(int i=0; i < Graph.Nodes.size();i++)
    {
        if(Graph.Nodes.at(i).GetID() == Integer)
        {
            returnval = i;
            break;
        }

    }

    //if the value was not found
    if (returnval ==-1)
    {
        std::cerr<<"Entered NodeID not found in the records";
        std::exit(1);
    }

    //returning index
    return returnval;

}


//parsing function for the obstacles file
std::vector<std::vector<int> > GetObstacle ( std::string filename, Graph Graph)
{
    //opening file stream
    std::ifstream file;
    file.open(filename);


    //handling if file doesnt exist
    if(!file)
    {
        throw 0;
    }

    //varaibles declarations
    std::string buffer;
    std::string tempVariable;
    std::vector < std::vector<int> > Obstacles;
    bool Obstaclesflag {false};
    int linecount =-1;
    int i = -2;
    int temp=0;

    //getting line by line input from file
    while( std::getline(file,buffer))
    {
        //handling null lines
        if (buffer == "")
        {
            std::cerr << "\n Warning Null line \n";
            continue;
        }
        i++;

        // converting string to stringstream
        std::istringstream line{buffer};
        std::vector <int> tempvector;

        //checking if the obstacle variable
        if((buffer == "$obstacles\r") || (buffer == "$obstacles"))
        {
            //setting flag
            Obstaclesflag = true;
            continue;
        }

        //if inside the obstacles section
        if(Obstaclesflag==true)
        {
            //Checking number of arguements passed
            int InputCount = CountInputs(buffer);

            //number of inputs
            if (InputCount < 3 )
            {
                throw 1;
            }
            //counting lines
            linecount ++;

            //storing the variables in temp string
            while(line >> tempVariable)
            {
                // checking if its a number
                if(!isNumber(tempVariable))
                {
                    throw 4;
                }

                //converting to variable
                temp = stod(tempVariable);

                //inserting values in vector
                tempvector.push_back(temp);
            }
            //insert the vector in vector of obstacles
            Obstacles.push_back(tempvector);
        }
    }

    //if no label exists
    if(Obstaclesflag == false)
    {
        throw 3;
    }

    //close file
    file.close();

    //return the vector
    return Obstacles;

}

bool Graph::CheckDuplicateByNode(Node INode) {

    bool check {false};


    for(int i = 0;i<Nodes.size();i++)
    {
        //found return true
        if (Nodes.at(i).GetID()==INode.GetID())
        {
            check = true;
            break;
        }
        if(Nodes.at(i).GetPoint().x==INode.GetPoint().x && Nodes.at(i).GetPoint().y==INode.GetPoint().y)
        {
            check = true;
            break;
        }
    }
    return check;

}

void Graph::AddNode(Node NewNode) {
    //checking if the node exists
    bool check = CheckDuplicateByNode(NewNode);

    //if duplicate exists exit
    if (check)
    {
        std::cerr<<"Duplicate Points found. Correct your input file"<<std::endl;
        std::exit(1);
    }
    //else insert node
    Nodes.push_back(NewNode);
}

void Graph::AddEdge(int ID1, int ID2) {

    if (ID1 == ID2)
    {
        std::cerr<<("You Cannot Add edges between the same point")<<std::endl;
        std::exit(1);
    }

    // Need to check if provided input nodes exist
    // Need to Check if the edge is already provided
    Node N1 = GetNodeByID(ID1);
    Node N2 = GetNodeByID((ID2));

    //checkign if they exist
    bool check1 = CheckDuplicateByNode(N1);
    bool check2 = CheckDuplicateByNode(N2);

    //if they do not exit print error
    if (!(check1 && check2))
    {
        std::cerr<<"Invalid Edge Nodes in input file"<<std::endl;
        std::exit(1);
    }

    //checkin if the edge already exists
    bool check3 = CheckIfEdgeExists(ID1,ID2);
    if(check3)
    {
        std::cerr<<"Duplicate edges found"<<std::endl;
        std::exit(1);
    }


    //temporary variables
    double x1{0}, x2{0}, y1{0}, y2{0};
    double weight{0};

    x1 = Nodes.at(ID1).GetPoint().x ;
    x2 = Nodes.at(ID2).GetPoint().x ;
    y1 = Nodes.at(ID1).GetPoint().y ;
    y2 = Nodes.at(ID2).GetPoint().y ;

    //calculating weight by distance formula
    weight = sqrt((pow((x1 - x2),2))+(pow((y1 - y2),2)));

    //iterating over the nodes vector
    for(int i {0};i< Nodes.size();i++)
    {
        //when Id1 found, insert edge to id2
        if (Nodes.at(i).GetID()== ID1)
        {
            Edge e(ID2,weight);
            Nodes.at(i).EdgeList.push_back(e);

        }

            //when id2 found insert edge to id1
        else if (Nodes.at(i).GetID()== ID2)
        {
            Edge e(ID1,weight);
            Nodes.at(i).EdgeList.push_back(e);
        }
    }
}

Node Graph::GetNodeByID(int InputId) {
    int i=0;

    //check if it exists
    bool check = false;
    for ( i  = 0 ; i< Nodes.size(); i++)
    {
        if (Nodes.at(i).GetID() == InputId)
        {
            check = true;
            break;
        }

    }

    //if not error
    if (check == false)
    {
        std::cerr<< "Invalid Node Id inputted";
        std::exit(1);
    }

    //else return the index
    return Nodes.at(i);
}

void Graph::PrintGraph(Graph Graph) {
    //iterating over the nodes
    for( int i =0 ; i< Graph.Nodes.size() ; i++)
    {
        //getting x and y co-ordinates
        std::cout <<"("<< Graph.Nodes.at(i).GetPoint().x <<","<<Graph.Nodes.at(i).GetPoint().y  <<") ";
    }

    //iterating over nodes again
    for( int i =0 ; i< Graph.Nodes.size() ; i++)
    {
        //printing
        std::cout <<"\n"<< Graph.Nodes.at(i).GetID() <<": ";
        Node N = GetNodeByID(i);

        //sorting the edgelist
        std::list <Edge> e {N.GetEdgeList()};
        e.sort();

        //printing the Edgelist
        for (auto it = e.begin(); it!= e.end();it++)
        {
            std::cout<< it->GetDestNode() << "," << it->GetWeight() << " ";
        }
        std::cout<<std::endl;
    }
}

bool Graph::CheckIfEdgeExists(int ID1, int ID2) {
    //get node
    Node N1 = GetNodeByID(ID1);
    //temporary edgelist
    std::list <Edge> e;
    e = N1.GetEdgeList();

    //iterating over the list
    for (auto it = e.begin(); it!= e.end();it++)
    {
        //found the id? if yes return true, if not return false
        if (it->GetDestNode() == ID2)
        {
            return true;
            break;
        }
    }
    return false;
}

std::vector<std::vector<double>> Graph::GetCostMatrix(Graph Graph) {
    int v = Graph.Nodes.size();

    std::vector<std::vector<double>> cost;

    //initializer
    for (int i = 0; i < v; i++)
    {
        std::vector<double> temp;
        for (int j = 0; j < v; j++)
        {
            temp.push_back(Infinity);
        }
        cost.push_back(temp);
    }

    //iterating over rows
    for (int i = 0; i < v-1; i++)
    {
        int RowNodeID = Graph.Nodes.at(i).GetID();
        std::list <Edge> e {Graph.Nodes.at(i).GetEdgeList()};

        for (int j=0;j<v;j++) //columns
        {
            //if the column id and the node id match, enter the weight values

            int ColumnNodeID = Graph.Nodes.at(j).GetID();
            if (ColumnNodeID == RowNodeID )
            {
                //distance of node from itself is zero
                cost[i][j]=0;
            }
            else
            {
                //entering the respective weights
                for (auto it = e.begin();it!= e.end();it++)
                {
                    int CurrentID = it->GetDestNode();
                    double w = it->GetWeight();

                    if (CurrentID==ColumnNodeID)
                    {
                        cost[i][j]=w;
                    }

                }
            }
        }

    }

    return cost;
}

Graph Graph::SortList(Graph IGraph) {
    int v = IGraph.v;
    Graph OGraph;
    OGraph.v = IGraph.v;

    //temporary edgelist
    std::list<Edge> e;

    //calling sort
    for (int i =0; i<v-1;i++)
    {
        IGraph.Nodes.at(i).EdgeList.sort();
    }

    return IGraph;
}

Graph Graph::DeleteEdge(Graph Graph, int ID1, int ID2) {

    //checking if the edge exists
    bool check = Graph.CheckIfEdgeExists(ID1,ID2);
    if (!check)
    {
        std::cerr<<"No such edge exists";
        std::exit(1);
    }


    //iterating over the nodes
    for (int i=0;  i<Graph.Nodes.size() ;i++)
    {
        if (Graph.Nodes.at(i).GetID()==ID1)
        {
            for(auto it = Graph.Nodes.at(i).EdgeList.begin(); it!=Graph.Nodes.at(i).EdgeList.end();it++)
            {
                //when id2 is found
                if (it->GetDestNode()==ID2)
                {
                    //erase the element when found
                    Graph.Nodes.at(i).EdgeList.erase(it);
                    break;
                }
            }

        }
        else if (Graph.Nodes.at(i).GetID()== ID2)
        {
            for(auto it = Graph.Nodes.at(i).EdgeList.begin(); it!= Graph.Nodes.at(i).EdgeList.end();it++)
            {
                //when id1 is found
                if (it->GetDestNode()==ID1)
                {
                    //erase the element when found
                    Graph.Nodes.at(i).EdgeList.erase(it);
                    break;
                }
            }

        }
    }
    return Graph;

}


//main function
int main (int argc , char  * argv[] )
{

    //checking if the number of arguments are in bounds
    if (argc < 5 || argc > 6 )
    {
        std::cerr << "Check arguments"<<std::endl;
        std::exit(1);
    }

    //declaring graph
    Graph Graph;

    //storing the filename in string
    std::string filename {argv[1]};

    //try to get data
    try {
        Graph = GetData(filename, Graph);
    }

        //catching exceptions of type int
    catch (int e)
    {
        if (e==0) {
            std::cerr << "File could not be opened" << std::endl;
            std::exit(1);
        }
        if (e==1)
        {
            std::cerr<<"No Section header found";
            std::exit(1);
        }
        if (e==2)
        {
            std::cerr<<"Check your sections";
            std::exit(1);
        }
    }
        //catchign exceptions of type char
    catch (char e)
    {
        if (e == 'e')
        {
            std::cerr<<"Enter Integer Value";
            std::exit(1);
        }
        if (e == 'd')
        {
            std::cerr<<"Enter double Value";
            std::exit(1);
        }
        if (e == 'n')
        {
            std::cerr<<"Invalid Number of Inputs";
            std::exit(1);
        }

    }

    //print graph
    Graph.PrintGraph(Graph);

    //get the cost matrix represntation
    Graph.GetCostMatrix(Graph);

    //strings to store the arguments of source and destination nodes
    std::string Str1 = argv[3];
    std::string Str2 = argv[4];

    //converting and checking validity of of the entered nodes
    int src= ConvertAndCheckValidity(Str1,Graph);
    int dest = ConvertAndCheckValidity(Str2,Graph);

    //DijsktrasAlgorithm(Graph,src,dest);

    //try to parse the obstacles file and the update the graph by deleting the edges
    try {
        std::vector<std::vector<int> > Obstacles = GetObstacle(argv[2], Graph);
        Graph = UpdateAfterObstacles(Graph, Obstacles);
    }

        //handle exception
    catch (int e)
    {
        if (e== 0)
        {
            std::cerr << "File Could not be opened"<<std::endl;
            std::exit(1);

        }
        if (e==1)
        {
            std::cerr <<"Need at least 2 nodes to make a obstacle";
            std::exit(1);
        }
        if(e==3)
        {
            std::cerr<<"Have you submitted the right file? Couldnt find obstacles on the first line";
            std::exit(1);
        }
        if(e==4)
        {
            std::cerr<<"Enter Integers";
            std::exit(1);
        }
    }


    //executing code for optional -a arguement

    //if -a not provided
    if (argc == 5 )
    {
        //calling dijkstra
        DijsktrasAlgorithm(Graph,src,dest);
    }

    //if extra argument is provided
    if (argc == 6  )
    {
        std::string Str3 = argv[5];
        if( Str3 == "-a")
        {
            AStar(Graph,src,dest);
        }

            //if the extra argument is not -a calling dijkstras
        else
        {
            DijsktrasAlgorithm(Graph,src,dest);
        }
    }

    //exit the function
    return EXIT_SUCCESS;
}

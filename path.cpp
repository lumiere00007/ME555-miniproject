#include"path.hpp"
#include<fstream>
#include<sstream>
#include<stdbool.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include<ostream>

Graph GetData (std::string filename , Graph  Graph);

Graph GetData (std::string filename , Graph  Graph)
{
    std::ifstream file;
    file.open(filename);
    
    if (!file)
    {
        std::cerr << "File could not be opened"<< std::endl;
        std::exit(1);
    }

    std::string buffer;

    bool NodeFlag{false};
    bool EdgeFlag {false};

    int CounterFlag{0};
    std::size_t * sz;
    
    int ip1{0};
    double ip2{2}, ip3{0};
    Node Node;
    

    int e1{0}, e2{0};
    int i{0};
    

    
    while(std::getline(file,buffer))
    {

        //std::cout<<"\n"<<buffer<<std::endl;
        //std::cout << "\n" <<buffer;
        std::istringstream line{buffer};
        //std::cout<<line<<std::endl;
        //(NodeFlag==true)? printf("\n NodeFlag true"):printf("\nNodeflag false");
        //(EdgeFlag==true)? printf("\tEdgeflag true"):printf("\tEdgeflag false");
        
        //std::cout<<"\n"<<i;
        i = i+1;

            if(buffer == "$Node")
            {
                NodeFlag = true;
                EdgeFlag = false;
                //std::cout<<"set Nodeflag";
                CounterFlag++;
                continue;
            }
        
        
            if (buffer == "$Edge")
            {
                EdgeFlag = true;
                NodeFlag = false;
                //std::cout<<"set Edgeflag";
                CounterFlag++;
                continue;
            }

            if (NodeFlag==true && EdgeFlag==false)
            {
                line >> ip1 >> ip2 >> ip3;
                //std::cout<<"ip1 "<<ip1<<" ip2 "<<ip2<<" ip3 "<<ip3<<std::endl;
                
                //std::cout<<"\n"<<"Calling SetId"<<std::endl;
                Node.SetID(ip1);
                //std::cout<<"\n"<<"Calling Setpoint"<<std::endl;
                Node.SetPoint(ip2, ip3);
                //std::cout<<"\n"<<"Calling Addnode"<<std::endl;
                Graph.AddNode(Node);
                Graph.IncrementVerticesNo();

                continue;
            }
   
            if (EdgeFlag)
            {
        
            line >> e1 >> e2;
            //std::cout<<"\n"<<"Calling AddEdge"<<std::endl;
            Graph.AddEdge(e1,e2);
            //std::cout<<"e1 "<< e1<<" e2 "<<e2<<std::endl;
            }     
    }
    file.close();
    return Graph;
}

int main (int argc , char * argv[] )
{
    //std::cout<<"here 1"<<std::endl;
    if (argc != 2)
    {
        std::cerr << "Not enough input arguements"<<std::endl;
        std::exit(1);
    }
    
    //std::cout<<"here 2"<<std::endl;

    Graph Graph;

    //std::cout<<"here 3"<<std::endl;

    
    std::string filename {argv[1]};
    Graph = GetData(filename, Graph);

    //std::cout<<"here 4"<<std::endl;

    Graph.PrintGraph(Graph);

    std::cout<<"here 5"<<std::endl;

    Graph.GetCostMatrix(Graph);

    int src =0;
    int dest =5;

    DijsktrasAlgorithm(Graph,src,dest);


    return 0;
}
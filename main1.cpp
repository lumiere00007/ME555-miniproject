#include "path.cpp"

//main function
int main (int argc , char  * argv[] )
{

    //checking if the number of arguments are in bounds
    if (argc != 2)
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


    //exit the function
    return EXIT_SUCCESS;
}
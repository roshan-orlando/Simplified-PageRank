// Project 2: Simplified PageRank

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>


class AdjacenyList {
private: 
    struct Node {    //Our list will be a simple linked list held together with nodes
        int toID = 0; //The urlID of the vertex its connected
        float outputValue = 0; //The amount the vertex willoutput
        int numOfEdges = 0;
        Node* next = nullptr;
    };

    std::vector<Node*> dataStructure; // Represents our a Adjacency list

    //Function will just return a associative vector that represents each vertexs output
    std::vector<float> outputFromEachPage() {
        std::vector<float> out;
        for (int i = 0; i < dataStructure.size(); i++) {
            Node* head = dataStructure[i];
            out.push_back(head->outputValue);
        }
        return out; //This vector returns the amount of output coming from a specific vertex (related to its index)
    }
    
public:

    //Function will help us add an edge between two values 
    void addEdge(int fromVal, int toVal) {
        int size = (dataStructure.size() - 1);
        if (size < fromVal) { //In case our adjaceny list does not have the current url in it
            Node* head = new Node();
            head->toID = fromVal;
            dataStructure.push_back(head);
        }
        size = (dataStructure.size() - 1);
        if (size < toVal) { //Same thing but we are checking the to value
            Node* head = new Node();
            head->toID = toVal;
            dataStructure.push_back(head);
        }

        Node* head = dataStructure[fromVal]; //This is the head of the current list we are going to iterate through
        head->numOfEdges++; //Because we have a connection, we are going to increase the amount of edges we have 
        head->outputValue = 1.0 / head->numOfEdges;
        while (true) { //Iterating through the linked list
            if (head->next != nullptr) {
                head = head->next;
                continue;
            }
            else {
                Node* newConnection = new Node();
                newConnection->toID = toVal; //Making sure our from value points to our toVal
                head->next = newConnection;
                break;
            }
        };
    };


    std::vector<float> pageRank(int p, std::vector<std::string>& urlIDArray) {
        int verticiesNum = urlIDArray.size(); //Our V/the amount of total verticies within our graph
        std::vector<float> rank(verticiesNum, 1.0 / verticiesNum); //Our rank 0, we are using 1.0 to convert to float 
        

        for (int i = 0; i < p - 1; i++) {
            std::vector<float> output(verticiesNum, 0.0);
            for (int j = 0; j < dataStructure.size(); j++) {
                Node* head = dataStructure[j];
                float outputAmount = head->outputValue;
                int from = head->toID; //We need to track where the edge connects  from so we can calculate the correct rank
                while (true) { //Iterating through the linked list
                    if (head == nullptr) {
                        break;
                    }
                    else if (j == head->toID) { // If we are at the "head" of the linked list
                        head = head->next;
                        continue;
                    }
                    else {
                        output[head->toID] += outputAmount * rank[from]; //The output amount is the 1/(Vertecies leaving from the vertex), rank is corresponding to where it came from

                        head = head->next;
                        continue;
                    }
                };
            }
            rank = output;

        }

        //We use the number of connections/edges to each leaving each page to help us calculate the "matrix"


        return rank;
    };
};

int findURLVal(std::string url, std::vector<std::string> &urlIDArray) { // Function to help us find the associative url value for each url
    for (int i = 0; i < urlIDArray.size(); i++) { //Searching through our associative array/vector
        if (urlIDArray[i] == url) {
            return i;
        }
    } 

    urlIDArray.push_back(url); //If we haven't found the urlId we are going to add the Url
    return (urlIDArray.size() - 1);
};


int main()
{
    //Creating an array to keep track of the Urls
    std::vector<std::string> urlIDArray;

    AdjacenyList graph = AdjacenyList();

    //Input information vars
    int numOfLines = 0;
    int p = 0;

    //Reading in the input parameters so we know how much we need to loop
    std::cin >> numOfLines; 
    std::cin >> p;
   
    
    for (int i = 0; i < numOfLines; i++) {
        std::string from = "";
        std::string to = "";

        std::cin >> from; //Reading in the from and to
        std::cin >> to;
        
        //Finding the associative values of to and from
        int fromVal = findURLVal(from, urlIDArray);
        int toVal = findURLVal(to, urlIDArray);

        graph.addEdge(fromVal, toVal); //Adding an edge to our Data Structure 
    }
    
    std::vector<float> results = graph.pageRank(p, urlIDArray); //This will return us our results numbers  

    // Once we are done iterating through how many power iterations we must print in order
    std::vector<std::string> namesInorder;
    copy(urlIDArray.begin(), urlIDArray.end(), back_inserter(namesInorder)); //Copying to sort names in order
    sort(namesInorder.begin(), namesInorder.end()); //Sorting the names in order 

    for (int i = 0; i < namesInorder.size(); i++) {
        float out = results[findURLVal(namesInorder[i], urlIDArray)];
        std::cout << namesInorder[i] << " ";
        std::cout << std::fixed << std::showpoint;
        std::cout << std::setprecision(2) << out << std::endl;
    }
}



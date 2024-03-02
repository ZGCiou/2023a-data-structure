/*****************************************************************************
    Data Structure 2023 Autumn
    Assignment #3   : Stack & Queues
    Author          : Zheng-Gang, Ciou (nycu311511022.ee11@nycu.edu.tw)
    File name       : 311511022.cpp

    Contains the definition and implementation of class template Stack, 
    the definition of structure point,
    the definition and implementation of class Maze, 
    and the main function of problem in "Lab3_Stacks Queues.pdf".
*****************************************************************************/
#include <iostream>
using namespace std;

/*****************************************************************************
    Definition and implementation of class template Stack
*****************************************************************************/
template <typename dataType>
class Stack {
private:
    /***** Node Class *****/
    class Node {
    public:
        Node() : next(0) {;}
        Node(dataType idata) : data(idata), next(0) {;}
        dataType data;
        Node *next;
    };
    /***** Data Member *****/
    Node *stackTop;
public:
    /***** Constructor *****/
    Stack() : stackTop(0) {;}
    /***** Copy Constructor *****/
    Stack(const Stack &origStack) {
        stackTop = 0;
        if (!origStack.empty()) {                           // Do nothing if copy from a empty list
            stackTop = new Node(origStack.stackTop->data);  // Copy first node
            Node *lastPtr = top;                            // Last node is copyied to new list
            Node *origPtr = origStack.stackTop->next;       // Node to be copy
            while (origPtr != 0) {                          // Traverse all node on origList
                lastPtr->next = new Node(origPtr->data);    // Copy
                lastPtr = lastPtr->next;                    // Point to next node
                origPtr = origPtr->next;
            }
        }
    }
    /***** Destructor *****/
    ~Stack() {
        Node *delPtr = stackTop;
        Node *nextPtr;
        while (delPtr != 0) {
            nextPtr = delPtr->next;
            delete delPtr;
            delPtr = nextPtr;
        }
    }
    /***** Assignment operator *****/
    Stack& operator=(const Stack &origStack) {
        if (origStack.empty()) {                            // If copy from a empty list
            this->~Stack();                                 // Delete old nodes
            stackTop = 0;
            return *this;
        }
        if (this != &origStack) {                           // Rule out self assign
            this->~Stack();                                 // Delete old nodes
            stackTop = new Node(origStack.stackTop->data);  // Copy first node
            Node *lastPtr = stackTop;                       // Last node is copyied to new list
            Node *origPtr = origStack.stackTop->next;       // Node to be copy
            while (origPtr != 0) {                          // Traverse all node on origList
                lastPtr->next = new Node(origPtr->data);    // Copy
                lastPtr = lastPtr->next;                    // Point to next node
                origPtr = origPtr->next;
            }
        }
        return *this;
    }

    /***** Empty *****/
    bool empty() const {
        return (stackTop == 0);
    }

    /***** Top *****/
    dataType top() const {
        return stackTop->data;
    }

    /***** Pop *****/
    dataType pop() {
        dataType odata = top();
        Node *newTop = stackTop->next;
        delete stackTop;
        stackTop = newTop;

        return odata;
    }

    /***** Push *****/
    void push(dataType idata) {
        Node *newPtr = new Node(idata);
        if (!empty()) {
            newPtr->next = stackTop;
        }
        stackTop = newPtr;
    }
};

/*****************************************************************************
    Definition of structure point
*****************************************************************************/
struct point {
    int x;
    int y;
};

/*****************************************************************************
    Definition and implementation of class Maze
*****************************************************************************/
class Maze {
private:
    int size;
    char **map;
    char wall;
    char passage;
public:
    Maze(char iwall='#', char ipass='.') : wall(iwall), passage(ipass), size(0) {;}
    ~Maze() {
        if (size > 0) {
            for (int i=0; i<size; i++) {
                delete [] map[i];
            }
        }
    }

    //----- Definition of loadMaze()
    // Load maze from istream and store in a dynamic allocated array.
    Maze& loadMaze(istream &in) {
        in >> size;
        map = new char* [size];
        for (int i=0; i<size; i++) {
            map[i] = new char[size];
            for (int j=0; j<size; j++) {
                in >> map[i][j];
            }

        }
        return *this;
    }

    //----- Definition of printMaze()
    // Display the map array of the Maze object on screen. 
    void printMaze() {
        for (int i=0; i<size; i++) {
            for (int j=0; j<size; j++) {
                cout << map[i][j];
            }
            cout << endl;
        }
    }

    //----- Definition of slove()
    // Find the solution of the maze.
     Maze& solve(point startPt={0,1}) {  // Defalut start ponit at (0,1)
        // Copy map to avoid changing the orignal maze array.
        char **map_mark = new char* [size];
        for (int i=0; i<size; i++) {
            map_mark[i] = new char[size];
            for (int j=0; j<size; j++) {
                map_mark[i][j] = map[i][j];
            }
        }

        // Connect point
        point connectPt[4] = {{0,1}, {1,0}, {0,-1}, {-1,0}};    // right > down > left > up

        point curPt, destPt = {size-1, size-2}, n;
        Stack<point> ptStack;   // Store the path log
        Stack<int> dirStack;    // Store the direction log
        curPt = startPt;
        int backDir = -1;       // Avoid go back loop
        string dir;             // Direction

        // Push start point
        cout << "Push down " << startPt.x << " " << startPt.y << endl;
        ptStack.push(startPt);

        while (curPt.x != destPt.x || curPt.y != destPt.y) {
            if (ptStack.empty()) {
                cout << "Path not found." << endl;
                return *this;
            }

            for (int i=0; i<5; i++) {
                // No road
                if (i==4) {
                    // Fill up dead road
                    map_mark[ptStack.top().x][ptStack.top().y] = '#';
                    // Go back
                    ptStack.pop();
                    // Direction
                    if (dirStack.top() == 0) dir = "right";
                    else if (dirStack.top() == 1) dir = "down";
                    else if (dirStack.top() == 2) dir = "left";
                    else if (dirStack.top() == 3) dir = "up";
                    dirStack.pop();

                    cout << "Pop ";// << dir << " " << ptStack.top().x << " " << ptStack.top().y << endl;
                    break;
                }

                // Neighbor point
                n = {curPt.x + connectPt[i].x, curPt.y + connectPt[i].y};

                // Not allow go back unless no road
                if (i == backDir) {
                    continue;
                }

                // Find road
                if (map_mark[n.x][n.y] == passage) {
                    dirStack.push(i);
                    ptStack.push(n);
                    if (dirStack.top() == 0) dir = "right";
                    else if (dirStack.top() == 1) dir = "down";
                    else if (dirStack.top() == 2) dir = "left";
                    else if (dirStack.top() == 3) dir = "up";
                    cout << "Push ";// << dir << " " << ptStack.top().x << " " << ptStack.top().y << endl;
                    break;
                }
            }

            curPt = ptStack.top();
            cout << dir << " " << curPt.x << " " << curPt.y << endl;
            backDir = (dirStack.top() + 2) % 4;
        }
        
        // Mark the solution path.
        while (!ptStack.empty()) {
            n = ptStack.pop();
            map[n.x][n.y] = '+';
        }

        return *this;
    }
};

/*****************************************************************************
    Main function
*****************************************************************************/
int main() {
    Maze m1;
    m1.loadMaze(cin).solve().printMaze();

    return 0;
}

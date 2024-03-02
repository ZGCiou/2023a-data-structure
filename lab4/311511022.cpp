/*****************************************************************************
    Data Structure 2023 Autumn
    Assignment #4   : Tree
    Author          : Zheng-Gang, Ciou (nycu311511022.ee11@nycu.edu.tw)
    File name       : 311511022.cpp

    Contains the definition and implementation of class Kingdom, 
    and the main function of problem in "Lab4_Tree.pdf".
*****************************************************************************/
#include <iostream>
#include <vector>
using namespace std;

//===============================================================
// Kingdom.h
// A class contains
// Nested Class
//  - CityNode:     The Node of a tree structure, contains indexes of a parent and children.
// Data Members
//  - cityNum:      The number of cities in the kingdom.
//  - capIdx:       The index of capitial city.
//  - resIdx:       The index of resort city.
//  - connectTable: The table records the connection of the kingdom, the No.x city corresponding to index x-1.
//                  (e.g., the No.1 city corresponding to index 0)
//                  The row is the index of a city, and the cloumn is index of cities which connects to that city.
// Function Members
//  - inRoad:        Input the roads of the kingdom.
//  - buildNaviTree: Return a vector stores a tree that the root is the given index of city.
//  - connect:       A recursive function called by buildNaviTree, it builds the tree accroding the connectTable.
//  - query:         Solve the question in "Lab4_Tree.pdf".
//  - findpath:      Given a tree and a index, and return a path from the index to the root of the tree.
//
//----- Definition of Kingdom class
class Kingdom {
private:
    //----- Nested Class
    class CityNode {
        public:
        int parentIdx;
        vector<int> childIdx;
        CityNode();
    };
    int cityNum;
    int capIdx;
    int resIdx;
    vector<vector<int> > connectTable;

public:
    Kingdom(int, int, int);
    Kingdom& inRoad(istream&);
    vector<CityNode> buildNaviTree(int);
    void connect(vector<CityNode>&, int);
    void query(istream&, ostream&, int);
    vector<int> findpath(vector<CityNode>, int);
    void showTable();
};

//===============================================================
// Kingdom.cpp
//----- Definition of CityNode Function
// Constructor
Kingdom::CityNode::CityNode() : parentIdx(-1) {
    ;
}
//----- Definition of Kingdom Function
// Constructor
Kingdom::Kingdom(int inum, int cap, int res) : cityNum(inum), capIdx(cap), resIdx(res) {
    connectTable.reserve(cityNum);
}

// inRoad
// Input roads information from istream to connectTable
Kingdom& Kingdom::inRoad(istream &in) {
    int c1, c2, c1_idx, c2_idx;
    for (int i=0; i<cityNum-1; i++) {
        in >> c1 >> c2;
        c1_idx = c1 - 1;
        c2_idx = c2 - 1;
        connectTable[c1_idx].push_back(c2_idx);
        connectTable[c2_idx].push_back(c1_idx);
    }
    return *this;
}

// buildNaviTree
// Return a tree that the root is the given index of city
vector<Kingdom::CityNode> Kingdom::buildNaviTree(int target) {
    // cout << "BuildNaviTree Function" << endl;
    vector<Kingdom::CityNode> naviTree(cityNum);
    connect(naviTree, target);
    return naviTree;
}

// connect
// Connect the given node in the given tree according to the connectTable
void Kingdom::connect(vector<CityNode> &roadTree, int idx) {
    // cout << "Connect Function: idx=" << idx << endl;
    for (auto it : connectTable[idx]) {
        if (it != roadTree[idx].parentIdx) {
            roadTree[idx].childIdx.push_back(it);
            roadTree[it].parentIdx = idx;
            connect(roadTree, it);
        }
    }
    return;
}

// findpath
// Return a vector the stores the index of path from given node to root of given tree.
vector<int> Kingdom::findpath(vector<CityNode> naviTree, int start) {
    // cout << "Findpath Function" << endl;
    vector<int> path;
    int curIdx = start;
    while (curIdx != -1)
    {
        path.push_back(curIdx + 1);
        curIdx = naviTree[curIdx].parentIdx;
    }
    return path;
}

// query
// Input a No. of city from istream and output the answer to ostream for given times.
void Kingdom::query(istream &in, ostream &out, int times) {
    // cout << "Query Function" << endl;
    vector<Kingdom::CityNode> capNaviTree = buildNaviTree(capIdx);
    vector<Kingdom::CityNode> resNaviTree = buildNaviTree(resIdx);
    vector<int> capPath, resPath;
    int qIdx, x, city;
    for (int i=0; i<times; i++) {
        cin >> qIdx;
        qIdx -= 1;
        capPath = findpath(capNaviTree, qIdx);
        resPath = findpath(resNaviTree, qIdx);
        x = min(capPath.size(), resPath.size());
        for (int j=0; j<x; j++){
            if (capPath[j] != resPath[j]) {
                break;
            }
            city = capPath[j];
        }
        out << city << endl;
    }
}

void Kingdom::showTable() {
    // cout << "ShowTable Function" << endl;
    int n = 1;
    for (int i=0; i<cityNum; i++) {
        cout << "City " << i+1 << ": " << endl;
        for (auto it : connectTable[i]) {
            cout << it << " ";
        }
        cout << endl;
    }
}

//===============================================================
// main.cpp
int main() {
    int N, Q, S, R;
    cin >> N >> Q >> S >> R;
    Kingdom lbkingdom(N, S-1, R-1);
    lbkingdom.inRoad(cin);
    // lbkingdom.showTable();
    lbkingdom.query(cin, cout, Q);

    return 0;
}
/*****************************************************************************
    Data Structure 2023 Autumn
    Assignment #5   : Hashing
    Author          : Zheng-Gang, Ciou (nycu311511022.ee11@nycu.edu.tw)
    File name       : 311511022.cpp

    Contains the definition and implementation of class Kingdom, 
    and the main function of problem in "Lab4_Tree.pdf".
*****************************************************************************/
#include <iostream>
#include <fstream>
using namespace std;

//===============================================================
// HashTable.h
// A class contains
// Data Members
//  - table:    The hash table base on array.
//  - size:     The size of hash table array.
// Function Members
//  - hash:     Modulo by the size of hash table.
//  - write:    Write element to the hash table according to a given key, overwrite the old element when collision.
//  - read:     Return the element by a given key.
//----- Definition of HashTable class
template<typename T>
class HashTable {
private:
    T *table;
    int size;
public:
    HashTable(int);
    HashTable(int, T);
    HashTable(const HashTable &);
    ~HashTable();
    HashTable& operator=(const HashTable &);
    int hash(int);
    HashTable& write(int, T);
    T read(int);
};

//===============================================================
// HashTable.cpp
//----- Definition of HashTable Function
// Constructor
template<typename T>
HashTable<T>::HashTable(int tableSize) : size(tableSize) {
    table = new T[tableSize];
}

// Constructor with initial value
template<typename T>
HashTable<T>::HashTable(int tableSize, T initValue) : size(tableSize) {
    table = new T[tableSize];
    for (int i=0; i<size; i++) {
        table[i] = initValue;
    }
}

// Copy Constructure
template<typename T>
HashTable<T>::HashTable(const HashTable<T> &orig) {
    size = orig.size;
    table = new T[orig.size];
    for (int i=0; i<size; i++) {
        table[i] = orig.table[i];
    }
}

// Destructor
template<typename T>
HashTable<T>::~HashTable() {
    delete table;
}

// Assignment Operator
template<typename T>
HashTable<T>& HashTable<T>::operator=(const HashTable<T> &orig) {
    size = orig.size;
    delete table;
    table = new int[orig.size];
    for (int i=0; i<size; i++) {
        table[i] = orig.table[i];
    }
}

// Hash Function
template<typename T>
int HashTable<T>::hash(int key) {
    return key % size;
}

// Write Item
template<typename T>
HashTable<T>& HashTable<T>::write(int key, T value) {
    table[hash(key)] = value;
    return *this;
}

// Read Item
template<typename T>
T HashTable<T>::read(int key) {
    return table[hash(key)];
}

//===============================================================
// main.cpp
struct SerialNum
{
    int index;
    int num;
};

int main(int argc, char **argv)
{
    int i = 0, j = 0;
    int target, num;
    ifstream fin(argv[1]);
    fin >> target;
    // Store the divisor of target value into array with index.
    // Then put the index into hash table and the value as a key.
    SerialNum divisor[target];
    HashTable<int> htable(target, -1);
    while (!fin.eof())
    {
        fin >> num;
        if ((target % num) == 0)
        {
            divisor[j] = {i, num};
            htable.write(num, i);
            j++;
        }
        i++;
    }
    i = 0;
    // Search if a pair of divisor is in the hash table from the smallest index.
    for (int k=0; k<j; k++)
    {   
        num = target / divisor[i].num;
        // The hash table is initialized to -1, so the value -1 means no element
        if (htable.read(num) != -1)
        {
            cout << divisor[i].index << " " << htable.read(num) << endl;
            break;
        }
        i++;
    }

    return 0;
}
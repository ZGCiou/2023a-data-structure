#include <fstream>
#include <iostream>
#include <cmath>
using namespace std;

struct Term {
    int coef;
    int power;
};

class Polynomial {
private:
    class Node {
    public:
        Term data;
        Node *nextPtr;
        Node() : nextPtr(0) {;}
        Node(Term iterm) : data(iterm), nextPtr(0) {;}
    };
    Node *headPtr;
public:
    /***** Constructor *****/
    Polynomial() : headPtr(0) {;}
    /***** Copy Constructor *****/
    Polynomial(const Polynomial &origPoly) {
        headPtr = 0;
        if (origPoly.headPtr != 0) {                        // Do nothing if copy from a empty list
            headPtr = new Node(origPoly.headPtr->data);     // Copy first node

            Node *lastPtr = headPtr;                        // Last node is copyied to new list
            Node *origPtr = origPoly.headPtr->nextPtr;      // Node to be copy
            while (origPtr != 0) {                          // Traverse all node on origList
                lastPtr->nextPtr = new Node(origPtr->data); // Copy
                lastPtr = lastPtr->nextPtr;                 // Point to next node
                origPtr = origPtr->nextPtr;
            }
        }
    }
    /***** Destructor *****/
    ~Polynomial() {
        Node *delPtr = headPtr;
        Node *postPtr;
        while (delPtr != 0) {
            postPtr = delPtr->nextPtr;
            delete delPtr;
            delPtr = postPtr;
        }
    }
    /***** Assignment operator *****/
    Polynomial& operator=(const Polynomial &origPoly) {
        if (origPoly.headPtr == 0) {                        // If copy from a empty list
            this->~Polynomial();                            // Delete old nodes
            headPtr = 0;
            return *this;
        }
        if (this != &origPoly) {                            // Rule out self assign
            this->~Polynomial();                            // Delete old nodes
            headPtr = new Node(origPoly.headPtr->data);     // Copy first node

            Node *lastPtr = headPtr;                        // Last node is copyied to new list
            Node *origPtr = origPoly.headPtr->nextPtr;      // Node to be copy
            while (origPtr != 0) {                          // Traverse all node on origList
                lastPtr->nextPtr = new Node(origPtr->data); // Copy
                lastPtr = lastPtr->nextPtr;                 // Point to next node
                origPtr = origPtr->nextPtr;
            }
        }
        return *this;
    }
    /***** Insertion *****/
    Polynomial& insert(const Term &iterm) {
        Node *newPtr = new Node(iterm);                                 // Create a new node
        if (headPtr == 0) {
            headPtr = newPtr;
        }
        else if (headPtr->data.power < newPtr->data.power) {            // First term's order is smaller the new term
            newPtr->nextPtr = headPtr;                                  // Add new term in front of the old first trem
            headPtr = newPtr;
        }
        else {
            Node *prePtr = headPtr;
            while (prePtr->nextPtr != 0) {                              // Traverse all terms and stop at last term
                if (prePtr->nextPtr->data.power < newPtr->data.power) { // Find a term that order is smaller than new term
                    break;                                              // That term is behind prePtr
                }                                                       // So just need to insert the new term behind prePtr
                else {
                    prePtr = prePtr->nextPtr;
                }
            }
            if (prePtr->data.power == newPtr->data.power) {             // If find the same power term
                prePtr->data.coef += newPtr->data.coef;                 // Only add the coeff
                delete newPtr;
            }
            else {
                newPtr->nextPtr = prePtr->nextPtr;                      // Insert the new term behind prePtr
                prePtr->nextPtr = newPtr;}
        }
        return *this;
    }
    /***** Display *****/
    void display(ostream &out) const {
        Node *curPtr = headPtr;
        while (curPtr != 0) {
            if (curPtr->data.coef == 0) {   // Display nothing if coefficient is zero
                curPtr = curPtr->nextPtr;
                continue;
            }
            if (curPtr == headPtr) {
                out << curPtr->data.coef;
            }
            else {
                out << " " << ((curPtr->data.coef < 0) ? "- " : "+ ")
                    << ((curPtr->data.coef < 0) ? -(curPtr->data.coef) : curPtr->data.coef);
            }
            out << "x^" << curPtr->data.power;
            curPtr = curPtr->nextPtr;
        }
    }
    /***** Addition *****/
    Polynomial operator+(const Polynomial &iPoly) const {
        Polynomial newPoly(iPoly);
        Node *curPtr = headPtr;
        while (curPtr != 0) {
            newPoly.insert(curPtr->data);
            curPtr = curPtr->nextPtr;
        }
        return newPoly;
    }
    /***** Subtraction *****/
    Polynomial operator-(const Polynomial &iPoly) const {
        Polynomial newPoly(*this);
        Node *curPtr = iPoly.headPtr;
        while (curPtr != 0) {
            newPoly.insert(Term{-(curPtr->data.coef), curPtr->data.power});
            curPtr = curPtr->nextPtr;
        }
        return newPoly;
    }
    /***** Multiplication *****/
    Polynomial operator*(const Polynomial &iPoly) const {
        Polynomial newPoly;
        Node *curPtr1 = headPtr;
        Node *curPtr2 = iPoly.headPtr;
        while (curPtr1 != 0) {
            while (curPtr2 != 0) {
                newPoly.insert(Term{curPtr1->data.coef * curPtr2->data.coef, curPtr1->data.power + curPtr2->data.power});
                curPtr2 = curPtr2->nextPtr;
            }
            curPtr2 = iPoly.headPtr;
            curPtr1 = curPtr1->nextPtr;
        }
        return newPoly;
    }
    /***** Evalution *****/
    int evaluate(double x) {
        int value = 0;
        Node *curPtr = headPtr;
        while (curPtr != 0) {
            value += curPtr->data.coef * pow(x, curPtr->data.power);
            curPtr = curPtr->nextPtr;
        }
        return value;
    }
};
/***** Output operator *****/
ostream& operator<<(ostream &out, const Polynomial &poly) {
    poly.display(out);
    return out;
}

int main(int argc, char **argv) {
    /* Read Data */
    ifstream fin(argv[1]);
    string str;
    Polynomial poly1, poly2;
    int coef, pow, idx=0;
    double x;
    while (fin.peek() != EOF) {
        if (fin.peek() == 10 || fin.peek() == 13) { // Skip \r and \n
            fin.get();
        }
        else if (fin.peek() == 'P') {
            fin >> str;
            idx++;
        }
        else if (fin.peek() == 'x') {
            fin >> str >> x;
        }
        else {
            fin >> coef >> pow;
            Term t = {coef, pow};
            if (idx==1) {
                poly1.insert(t);
            }
            else {
                poly2.insert(t);
            }
        }
    }
 
    /* Display */
    cout << "Polynomial 1: " << poly1 << endl;
    cout << "Polynomial 2: " << poly2 << endl;
    cout << "Addition: " << poly1 + poly2 << endl;
    cout << "Subtraction: " << poly1 - poly2 << endl;
    cout << "Multiplication: " << poly1 * poly2 << endl;
    cout << "Evaluation of Polynomial 1 at x = " << x << " is: " << poly1.evaluate(x) << endl;
    cout << "Evaluation of Polynomial 2 at x = " << x << " is: " << poly2.evaluate(x) << endl;

    /* Write to file */
    // ofstream fout("output" + string(1, argv[1][14]) + ".txt");
    // cout << argv[1][14] << endl;
    // fout << "Polynomial 1: " << poly1 << endl;
    // fout << "Polynomial 2: " << poly2 << endl;
    // fout << "Addition: " << poly1 + poly2 << endl;
    // fout << "Subtraction: " << poly1 - poly2 << endl;
    // fout << "Multiplication: " << poly1 * poly2 << endl;
    // fout << "Evaluation of Polynomial 1 at x = " << x << " is: " << poly1.evaluate(x) << endl;
    // fout << "Evaluation of Polynomial 2 at x = " << x << " is: " << poly2.evaluate(x) << endl;

    return 0;
}
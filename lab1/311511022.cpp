#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <cmath>
using namespace std;

const string INPUT_DIR = "./open_case/";
const string OUTPUT_DIR = "./result/";
const int CASE_NUM = 10;

class BigInt {
    public:
    /* Constructor */
    BigInt(string inNumStr) {
        for (int i=0; i<inNumStr.length(); i++) {
            int inNumInt = stoi(inNumStr.substr(inNumStr.length()-1-i, 1));
            number.push_back(inNumInt);
        }
    }
    BigInt(const BigInt& bigInt) {
        number = bigInt.number;
    }
    /*Compare to 0*/
    bool isZero() {
        for (int x : number) {
            if (x != 0)
                return false;
        }
        return true;
    }

    bool isEven() {
        return (number[0]%2 == 0) ? true : false;
    }

    BigInt& mul2() {
        int carry = 0;
        vector<int>::iterator it;
        for (it=number.begin(); it!=number.end(); it++) {  
            int tmp = (*it * 2 + carry) % 10;
            carry = (*it * 2 + carry) / 10;
            *it = tmp;
        }
        if (carry == 1) {
            number.push_back(1);
        }
        return *this;
    }

    BigInt& div2() {
        int remain = 0;
        vector<int>::reverse_iterator it;
        for (it=number.rbegin(); it!=number.rend(); it++) {
            int tmp = (remain * 10 + *it) / 2;
            remain = (remain * 10 + *it) % 2;
            *it = tmp;
        }
        it = number.rbegin();
        if (*it == 0) {
            number.pop_back();
        }
        return *this;
    }

    void operator-=(const BigInt& bigInt) {
        if (number.size() < bigInt.number.size()) {
            number.clear();
            number.push_back(0);
        }
        else {
            int borrow = 0;
            for (int i=0; i<bigInt.number.size(); i++) {  
                if (number[i] - borrow < bigInt.number[i]) {
                    number[i] = 10 + number[i] - borrow - bigInt.number[i];
                    borrow = 1;
                }
                else {
                    number[i] = number[i] - borrow - bigInt.number[i];
                    borrow = 0;
                }
            }
            if (borrow != 0) {
                number[bigInt.number.size()] -= 1;
            }
        }
        vector<int>::reverse_iterator it;
        for (it=number.rbegin(); it!=number.rend(); it++) {
            if (*it == 0) {
                number.pop_back();
            }
            else {
                break;
            }
        }
    }

    bool operator>(const BigInt& bigInt) {
        if (number.size() > bigInt.number.size()) {
            return true;
        }
        else if (number.size() < bigInt.number.size()) {
            return false;
        }
        else {
            for (int i=number.size()-1; i>=0; i--) {
                if (number[i] > bigInt.number[i]) {
                    return true;
                }
                else if (number[i] < bigInt.number[i]) {
                    return false;
                }
            }
            return false;
        }
    }
    /*
    Binary Algorithm for Greatest Common Divisor
    Input: Two positive integers 𝑎 and 𝑏.
    Output: A positive integer 𝑎𝑛𝑠 representing greatest common divisor of 𝑎 and 𝑏.
    */
    BigInt gcd(BigInt bigInt) {
        /*
        step1: n = min(a,b), m = max(a,b), ans = 1
        */
        BigInt m(*this);
        BigInt n(bigInt);
        int double_times = 0;   // int ans = pow(2,0);
        if (bigInt > *this) {
            m.swap(n);
        }
        /*
        step2: loop if 𝑛 ≠ 0 and 𝑚 ≠ 0
        */
        while ((!n.isZero()) && (!m.isZero())) {
            if (n.isEven() && m.isEven()) {
                double_times++;
                n.div2();
                m.div2();
            }
            else if (n.isEven()) {
                n.div2();
            }
            else if (m.isEven()) {
                m.div2();
            }
            if (n > m) {
                n.swap(m);
            }
            m -= n;
        }
        for (int i=0; i<double_times; i++) {
            n.mul2();
        }
        return n;
    }

    void swap(BigInt& bigInt) {
        number.swap(bigInt.number);
    }

    /*Output*/
    friend ostream& operator<<(ostream& out, BigInt bigInt) {
        vector<int>::reverse_iterator it;
        for (it=bigInt.number.rbegin(); it!=bigInt.number.rend(); it++) {
            out << *it;
        }
        return out;
    }

    private:
    vector<int> number;
};

int main() {
    for (int i=0; i<CASE_NUM; i++) {
        /* File Read,Write */
        ifstream fin(INPUT_DIR + "case" + to_string(i));    // input case{i} file in ./open_case
        ofstream fout(OUTPUT_DIR + "res" + to_string(i));   // output res{i} file to ./result
        string bigintStr1, bigintStr2;
        fin >> bigintStr1 >> bigintStr2;
        BigInt bigint1(bigintStr1);
        BigInt bigint2(bigintStr2);
        fout << bigint1.gcd(bigint2) << endl;
        fin.close();
        fout.close();
    }
    return 0;
}
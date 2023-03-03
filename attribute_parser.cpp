/*
* HackerRank
* Challenge: Attribute Parser
* https://www.hackerrank.com/challenges/attribute-parser/
*
* Soluntion by: Ricardo Burgos (ricardoburgos.sv@gmail.com)
* March 2023
*
* Instructions to compile:
* g++ -Wall -std=c++17 -g attribute_parser.cpp -o ap
* or
* clang++ -Wall -std=c++17 -g attribute_parser.cpp -o ap
* Then, execute with:
* ./ap input0?.txt [ENTER]
*/

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>

using namespace std;


// Prints a vector for debugging purposes
void printVector(vector<string> v) {
    cout << "*** vector ==> [";
    if (v.size() == 0) {
        cout << " ]";
    }
    for (int i = 0; i < v.size(); i++) {
        if (i == v.size()-1) {
            cout << v[i] << "] ***";
        } else {
            cout << v[i] << ", ";
        }
    }
    cout << endl;
}


// split a string q by the given character c
// returns a vector with substrings
vector<string> split(string q, char c) {
    int position {0};
    vector<string> vec;
    for (int i = 0; i < q.length(); i++) {
        if (q.at(i) == c) {
            vec.push_back(q.substr(position, i-position));
            position = i+1;
        }
    }
    vec.push_back(q.substr(position));
    return vec;
        
}


// Remove angular brackets
string removeAngularBrackets(string q) {
    if (q.at(0) == '<') q.erase(0, 1);
    int s = q.size();
    if (q.at(s-1) == '>') q.erase(s-1, 1);
    return q;
}


// Strip a string from quotation marks ("")
string removeQuotes(string q) {
    if (q.at(0) == '"') q.erase(0, 1);
    int s = q.size();
    if (q.at(s-1) == '"') q.erase(s-1, 1);
    return q;
}


// Removes spaces before and after equal sign (=)
string fixEquals(string q) {
    for (int i = 0; i < q.length(); i++) {
        if (q.at(i) == '=') {
            if (q.at(i+1) == ' ') q.erase(i+1, 1);
            if (q.at(i-1) == ' ') q.erase(i-1, 1);
        }
    }
    return q;
}


// Extracts the tag name from a line
string extractTag(string q) {
    q = removeAngularBrackets(q);
    int pos = q.find(' ');
    return q.substr(0, pos);

}


// Process a single query
string parseQuery(vector<string> src, string q) {

    string result {"Not Found!"};
    const char tagChar {'.'};
    const char attChar {'~'};
    const char eqChar {'='};
    const char spcChar {' '};
    
    vector<string> openTags; // Keeps track of open tags
    vector<string> sq = split(q, tagChar);
    vector<string> sq2;
    
    string previousTag {};
    int nestLevel {0};
    if (sq.size() == 1) {
        // non-nested query
        sq2 = split(sq[0], attChar);         
    } else {
        // nested queries
        string last {sq[sq.size()-1]};
        previousTag  = sq[sq.size()-2];
        sq2 = split(last, attChar);
    }

    string tag  = sq2[0];
    string attr = sq2[1];
    string currentTag {}; // Current tag

    // Iterate on the input source code line by line
    for (unsigned int i = 0; i < src.size(); i++) {

        currentTag = extractTag(src[i]);
        if (currentTag.at(0) != '/') {
            // add tag to openTags vector
            openTags.push_back(currentTag);
        } else {
            // remove tag from openTags vector
            int delPosition {};
            string auxTag = currentTag.erase(0, 1); // closing tag without '/'
            for (int k = 0; k < openTags.size(); k++) {
                if (openTags[k] == auxTag) {
                    delPosition = k;
                    break;
                }
            }
            openTags.erase(openTags.begin()+delPosition);
        }
        // printVector(openTags);

        nestLevel = openTags.size();

        if (src[i].find(tag) != string::npos && src[i].find(attr) != string::npos) {
            src[i] = removeAngularBrackets(src[i]);
            src[i] = fixEquals(src[i]);
            vector<string> sq3 = split(src[i], spcChar);

            for (unsigned int j = 1; j < sq3.size(); j++) {
                if (sq3[j].find(attr) != string::npos) {
                    vector<string> sq4 = split(sq3[j], eqChar);
                    if (nestLevel == sq.size() &&  sq4[0] == attr) {
                        result = removeQuotes(sq4[1]);
                    }
                }
            }
        }

    }
    return result;
    
}


int main(int argc, char *argv[]) {

    int N {}; // Number of lines in HRML source
    int Q {}; // Number of queries

    string file_name {argv[1]};

    ifstream inFil(file_name);

    if (inFil) {
        inFil >> N >> Q;

        // read HRML Source, store lines in a vector
        vector<string> source;
        for (int i = 0; i <= N; i++) {
            string srcLine;
            getline(inFil, srcLine);
            if (i > 0) source.push_back(srcLine);
        }

        // read and process the queries
        for (int j = 0; j < Q; j++) {
            string qLine;
            getline(inFil, qLine);
            cout << parseQuery(source, qLine) << endl;
        }

    } else {
        cout << "Error opening the file" << endl;
    }
    return 0;

}

// EOF

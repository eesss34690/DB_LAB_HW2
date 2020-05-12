#ifndef INDEX_H_
#define INDEX_H_

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>

using namespace std;

class Index
{
private:
    vector<int> d;
    Index **child_ptr;
    bool l;
    int n;
public:
    Index(int num_row, vector<int> key, int value);
    int search_index(int key);
    void insert_data(int key, int value);
    void adjust();
    void key_query(vector<int> query_keys);
    void range_query(vector<pair<int, int>> query_pairs);
};




#endif // INDEX_H_INCLUDED

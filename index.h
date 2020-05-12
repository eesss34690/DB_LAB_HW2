#ifndef INDEX_H_
#define INDEX_H_
#define N 100
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>

using namespace std;

class IndexNode
{
private:
    int* ind;
    int* val;
    IndexNode **child_ptr;
    bool l;
    int n;
public:
    IndexNode(bool l);
    int search_index(int key);
    void insert_data(int ind, int val);
    void adjust(int i, IndexNode *y);
friend class Index;
};
class Index
{
    IndexNode *root;
public:
    Index(int num_rows, vector<int>key, vector<int> value);
    void insert_node(int ind, int val);
    void key_query(vector<int> query_keys);
    void range_query( vector<int> query_pairs);
};



#endif // INDEX_H_INCLUDED

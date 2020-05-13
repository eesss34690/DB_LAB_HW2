#ifndef INDEX_H_
#define INDEX_H_
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
    IndexNode* parent;
    int n;
public:
    IndexNode();
    void adjust(int i, int ind, int val, IndexNode *y);
friend class Index;
};
struct finding
{
    IndexNode* par;
    int val;
};
class Index
{
    IndexNode *root;
public:
    Index(int num_rows, vector<int>key, vector<int> value);
    finding search_index(int key);
    void insert_data(IndexNode* cur, int ind, int val);
    void insert_node(int ind, int val);
    void key_query(vector<int> query_keys);
    void range_query( vector<int> query_pairs);
};



#endif // INDEX_H_INCLUDED

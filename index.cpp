#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include "index.h"
#define N 100
using namespace std;
Index::Index()
{
    this->n= 0;
    this->child_ptr= new Index* [101];
    this->l= 1;
    for (int i= 0; i< 101; i++)
    {
        this->child_ptr[i]= NULL;
    }
}
Index::Index(int num_row, vector<int> key, vector<int> value)
{
    this->n= 0;
    this->child_ptr= new Index* [101];
    this->l= 1;
    for (int i= 0; i< 101; i++)
    {
        this->child_ptr[i]= NULL;
    }
    for(vector<int>::iterator itr1 = key.begin(), vector<int>::iterator itr2 = value.begin(); itr1 != key.end(); itr1++, itr2++)
    {
        insert_node(&itr1, &itr2);
    }
}
void Index::insert_node(int key, int value)
{
    Index temp= *this;
    if (temp!= NULL)
    {
        if (temp.l == 1 && temp.n == 100)
        {
            int t= split(temp, -1);

        }
    }

}
Node* Index::getroot()
{
    return this->root;
}
int Index::getmnc()
{
    return maxnumchild;
}
int Index::getmnn()
{
    return maxnumnode;
}
void Index::setroot(Node* a)
{
    this->root = a;
}
void Index::key_query(vector<int> query_keys)
{

}
void Index::range_query(vector<int> query_pairs)
{

}
void Index::insert_node(int key, int value)
{
    Node a;
    a.leaf = 1;
    a.
}
void Index::clear_index()
{

}
Node** Index::findparent(Node* look, Node* child)
{
    if (look->leaf|| look->)

}

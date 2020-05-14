#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include "index.h"
#define N 100
using namespace std;
IndexNode::IndexNode()
{
    this->n= 0;
    this->child_ptr= new IndexNode* [N+ 1];
    for (int i= 0; i< N+ 1; i++)
    {
        child_ptr[i]= NULL;
    }
    this->ind= new int [N];
    this->val= new int [N];
    this->parent= NULL;
}
Index::Index(int num_row, vector<int> key, vector<int> value)
{
    this->root= NULL;
    for(int i = 0; i< key.size(); i++)
    {
    cout<<i<<endl;
        insert_node(key.at(i), value.at(i));
    }
}
finding Index::search_index(int key)
{
    IndexNode* cur= new IndexNode();
    cur= root;
    IndexNode* par= new IndexNode();
    par= NULL;
    finding a;
    int i= 0;
    while (cur)
    {
        while (i < cur->n)
        {
            if (cur->ind[i] < key)
                ++i;
            else if (cur->ind[i] == key)
            {
                a.par= par;
                a.val= cur->val[i];
                delete cur;
                delete par;
                return a;
            }
            else
                break;
        }
        par= cur;
        cur = cur->child_ptr[i];

        i = 0;
    }
    a.par= par;
    a.val= -1;
    delete cur;
    delete par;
    return a;
}
void Index::insert_node(int ind, int val)
{
    if (root == NULL)
    {
        root= new IndexNode();
        root->ind[0]= ind;
        root->val[0]= val;
        root->n= 1;
    }
    else
    {
        finding a= search_index(ind);
        IndexNode* cur= new IndexNode();
        cur= a.par;
        IndexNode* parent= new IndexNode();
        parent= cur->parent;
        while(1)
        {
            insert_data(cur, ind, val);
            if (cur->n >= N)
            {
                int mid= (cur->n- 1)/ 2;
                IndexNode* right= new IndexNode();
                int i= 0;
                for (int j= mid+ 1; j< cur->n; j++)
                {
                    right->ind[i]= cur->ind[j];
                    right->val[i++]= cur->val[j];
                    ++(right->n);
                }
                i= 0;
                for (int j= 0; j< cur->n; ++j)
                {
                    right->child_ptr[i++]= cur->child_ptr[j];
                    if (cur->child_ptr[i])
                    {
                        cur->child_ptr[i]->parent= right;
                    }
                }
                int middlei= cur->ind[mid];
                int middlev= cur->val[mid];
                cur->n= (cur->n- 1)/ 2;
                if (parent== NULL)
                {
                    IndexNode* temp= new IndexNode();
                    temp->ind[0]= middlei;
                    temp->val[0]= middlev;
                    ++(temp->n);
                    temp->child_ptr[0]= cur;
                    temp->child_ptr[1]= right;
                    cur->parent= temp;
                    right->parent= temp;
                    root= temp;
                    delete temp;
                    break;
                }
                else
                {
                    right->parent= parent;
                    i= N- 1;
                    while(parent->child_ptr[i]!= cur)
                    {
                        parent->child_ptr[i+ 1]= parent->child_ptr[i];
                        --i;
                    }
                    parent->child_ptr[i+ 1]= right;
                    cur= parent;
                    parent= cur->parent;
                }
                delete right;
            }
        }
        delete cur;
        delete parent;
    }
    cout<<ind<<" "<<val<<endl;
}
void Index::insert_data(IndexNode* cur, int ind, int val)
{
    int i= cur->n- 1;
    while (i >= 0 && cur->ind[i] > ind)
    {
        cur->ind[i+1] = cur->ind[i];
        i--;
    }
        cur->ind[i+1] = ind;
        cur->val[i+1] = val;
        cur->n = cur->n+ 1;
}
void Index::key_query(vector<int> query_keys)
{

}
void Index::range_query(vector<int> query_pairs)
{

}/*
void Index::clear_index()
{

}*/

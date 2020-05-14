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
{/*
    this->child_ptr.resize(N+ 1);
    this->ind.resize(N);
    this->val.resize(N);
    this->child_ptr= new IndexNode* [N+ 1];
    for (int i= 0; i< N+ 1; i++)
    {
        child_ptr[i]= NULL;
    }*/
    //this->ind= new int [N];
    //this->val= new int [N];
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
    IndexNode* c= new IndexNode();
    c= root;
    IndexNode* par= new IndexNode();
    par= NULL;
    finding a;
    int i= 0;
    while (c)
    {
        while (i < c->ind.size())
        {
            if (c->ind[i] < key)
                ++i;
            else if (c->ind[i] == key)
            {
                a.par= par;
                a.val= c->val[i];
                delete c;
                delete par;
                return a;
            }
            else
                break;
        }
        par= c;
        //cout<<c->ind[i-1]<<" ";
        if (c->leaf) break;
        c = c->child_ptr[i];
        i = 0;
    }
    a.par= par;
    //cout<<endl;
    a.val= -1;
    delete c;
    delete par;
    return a;
}
void Index::insert_node(int ind, int val)
{
    if (root == NULL)
    {
        root= new IndexNode();
        root->ind.push_back(ind);
        root->val.push_back(val);
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
            if (cur->ind.size() >= N)
            {
                int mid= (cur->ind.size()- 1)/ 2;
                IndexNode* right= new IndexNode();
                //int i= 0;
                (right->ind).assign((cur->ind).begin()+ mid+ 1, (cur->ind).end());
                (right->val).assign((cur->val).begin()+ mid+ 1, (cur->val).end());
                /*for (int j= mid+ 1; j< cur->n; j++)
                {
                    right->ind[i]= cur->ind[j];
                    right->val[i++]= cur->val[j];
                    ++(right->n);
                }*/
                //i= 0;
                (right->child_ptr).assign((cur->child_ptr).begin()+ mid+ 1, (cur->child_ptr).end());
                /*for (int j= mid+ 1; j< cur->n; ++j)
                {
                    right->child_ptr[i++]= cur->child_ptr[j];
                    if (cur->child_ptr[j])
                    {
                        cur->child_ptr[j]->parent= right;
                    }
                }*/
                (cur->ind).erase((cur->ind).begin()+ mid, (cur->ind).end());
                (cur->val).erase((cur->val).begin()+ mid, (cur->val).end());
                (cur->ind).erase((cur->ind).begin()+ mid, (cur->ind).end());
                if (parent== NULL)
                {
                    IndexNode* temp= new IndexNode();
                    temp->ind.push_back(cur->ind[mid]);
                    temp->val.push_back(cur->val[mid]);
                    temp->child_ptr[0]= cur;
                    temp->child_ptr[1]= right;
                    temp->leaf= false;
                    cur->parent= temp;
                    right->parent= temp;
                    root= temp;
                    delete temp;
                    break;
                }
                else
                {
                    right->parent= parent;
                    int i= N- 2;
                    if (parent->child_ptr[i+1]!= cur)
                        parent->child_ptr.push_back(parent->child_ptr[i+1]);
                    while(parent->child_ptr[i]!= cur)
                    {
                        parent->child_ptr[i+ 1]= parent->child_ptr[i];
                        --i;
                    }
                    parent->child_ptr[i+ 1]= right;
                    parent->leaf= false;
                    cur= parent;
                    parent= cur->parent;
                }
                delete right;
            }
        }
        delete cur;
        delete parent;
    }
    cout<<root->ind[0]<<" "<<root->val[0]<<endl;
    cout<<ind<<" "<<val<<endl<<endl;
}
void Index::insert_data(IndexNode* cur, int ind, int val)
{
    int i= cur->ind.size()- 2;
    if (cur->ind[i+1] > ind)
    {
        cur->ind.push_back(cur->ind[i+ 1]);
    }
    while (i >= 0 && cur->ind[i] > ind)
    {
        cur->ind[i+1] = cur->ind[i];
        i--;
    }
    cur->ind[i+1] = ind;
    cur->val[i+1] = val;
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

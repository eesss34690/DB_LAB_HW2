#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include "index.h"
#define N 201
using namespace std;
IndexNode::IndexNode(bool l)
{
    this->n= 0;
    this->child_ptr= new IndexNode* [N+ 1];
    this->ind= new int [N];
    this->val= new int [N];
    this->l= l;
}
Index::Index(int num_row, vector<int> key, vector<int> value)
{
    this->root= NULL;
    for(vector<int>::iterator itr1 = key.begin(), vector<int>::iterator itr2 = value.begin(); itr1 != key.end(); itr1++, itr2++)
    {
        insert_node(&itr1, &itr2);
    }
}
int IndexNode::search_index(int key)
{
    int i= 0;
    while(i< n&& key> ind[i])
    {
        i++;
    }
    if (ind[i]== key)
    {
        return ind[i];
    }
    if (l)
    {
        return -1;
    }
    return child_ptr[i]->search_index(key);
}
void Index::insert_node(int ind, int val)
{
    if (root == NULL)
    {
        root = new IndexNode(1);
        root->ind[0]= ind;
        root->val[0]= val;
        root->n= 1;
    }
    else
    {
        if (root->n== N)
        {
            IndexNode *s = new IndexNode(false);
            s->child_ptr[0] = root;
            s->adjust(0, root);
            int i = 0;
            if (s->ind[0] < ind)
                i++;
            s->child_ptr[i]->insert_data(ind, val);
            root = s;
        }
        else
            root->insert_data(ind, val);
    }
}
void IndexNode::insert_data(int ind, int val)
{
    int i= n- 1;
    if (l)
    {
        while (i >= 0 && ind[i] > ind)
        {
            ind[i+1] = ind[i];
            i--;
        }
        ind[i+1] = ind;
        val[i+1] = val;
        n = n+1;
    }
    else
    {
        while (i >= 0 && ind[i] > ind)
            i--;

        if (child_ptr[i+1]->n== N)
        {
            adjust(i+1, child_ptr[i+1]);
            if (ind[i+1] < ind)
                i++;
        }
        child_ptr[i+1]->insert_data(k);
    }
}
void IndexNode::adjust(int i, IndexNode *y)
{
    IndexNode *z = new IndexNode(y-> l);
    z->n = N/ 2+ 1;
    for (int j = 0; j < N/ 2+ 1; j++)
    {
        z->keys[j] = y->keys[j+ N/ 2+ 2];
        z->val[j]= y->val[j+ N/ 2+ 2];
    }
    if (!(y->l))
    {
        for (int j = 0; j < N/ 2+ 2; j++)
            z->child_ptr[j]= y->child_ptr[j+ N/ 2+ 1];
    }

    y->n= N/ 2+ 1;
    for (int j = n; j >= i+ 1; j--)
        child_ptr[j+ 1] = child_ptr[j];
    child_ptr[i+ 1] = z;
    for (int j = n-1; j >= i; j--)
    {
        ind[j+ 1]= ind[j];
        val[j+ 1]= val[j];
    }
    keys[i] = y->keys[N/ 2+ 1];
    n = n + 1;
}
void Index::key_query(vector<int> query_keys)
{

}
void Index::range_query(vector<int> query_pairs)
{

}
void Index::clear_index()
{

}

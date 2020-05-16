#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include "index.h"
#define OUTPUT_FILE "key_query_out.txt"
#define OUT2 "range_query_out.txt"

Node :: Node()
{
	isLeaf = true;
	prev = this;
	next = this;
}


void Node :: Insertkn(int key, Node* r)
{
	vector<int>::iterator index = lower_bound(keys.begin(), keys.end(), key);
	keys.insert(index, key);

	index = lower_bound(keys.begin(), keys.end(), key);
	children.insert(children.begin() + (index - keys.begin() + 1), r);
}


void Node :: Insertknn(int key, Node* l, Node* r)
{
	keys.push_back(key);
	children.push_back(l);
	children.push_back(r);
}


Node* Node :: Split(int* kp)
{
    if (!this->isLeaf)
    {
        int length = keys.size();

        Node* r = new Node;
        r->isLeaf = false;
        *kp = keys[length/2];

        r->keys.assign(keys.begin() + (length/2 + 1), keys.end());
        r->children.assign(children.begin() + (length/2 + 1), children.end());
        keys.erase(keys.begin() + length/2, keys.end());
        children.erase(children.begin() + (length/2 + 1), children.end());

        return r;
    }
	Node* r = new Node;

	*kp = keys[keys.size()/2];

	r->keys.assign(keys.begin() + keys.size()/2, keys.end());
	r->values.assign(values.begin() + values.size()/2, values.end());
	keys.erase(keys.begin() + keys.size()/2, keys.end());
	values.erase(values.begin() + values.size()/2, values.end());

	r->next = next;
	r->prev = this;
	next = r;
	(r->next)->prev = r;

	return r;

}

void Node :: Insertkv(int key, int value)
{
	vector<int>::iterator index = lower_bound(keys.begin(), keys.end(), key);

    keys.insert(index, key);
    index = lower_bound(keys.begin(), keys.end(), key);
    values.insert(values.begin() + (index - keys.begin()), value);
}

void Index :: Search_Path(Node* node, int key, stack<Node*>* path)
{
    path->push(node);

	if(!node->isLeaf)
    {
        vector<int>::iterator index = lower_bound(node->keys.begin(), node->keys.end(), key);

		if(key == node->keys[index - node->keys.begin()])
		{
            Search_Path(node->children[(index - node->keys.begin()) + 1], key, path);
		}

		else
		{
			Search_Path(node->children[index - node->keys.begin()], key, path);
		}
    }
}

void Index :: Destroy(Node* node)
{
	if (!node->isLeaf)
	{
        for(vector<Node*>::iterator index = node->children.begin(); index != node->children.end(); index++)
        {
            Destroy(*index);
        }
	}
	delete(node);
}

Index :: Index(int num, vector<int> keys, vector<int> val)
{

    vector<int>::iterator iter2= val.begin();
    root = NULL;
    for (vector<int>::iterator iter1= keys.begin(); iter1!= keys.end(); iter1++)
    {
        if(NULL == root)
        {
            root = new Node;
            root->Insertkv(*iter1, *iter2);
        }

        else
        {
            Node* l = new Node;
            l= NULL;
            Node* r = new Node;
            r = NULL;
            int* kp =new int;
            bool _pop = false;

            stack<Node*>* path = new stack<Node*>;
            Search_Path(root, *iter1, path);
            path->top()->Insertkv(*iter1, *iter2);
            while(path->top()->keys.size() == order)
            {
                l = path->top();
                r = l->Split(kp);
                if (path->size()> 0)path->pop();
                if(!path->empty())
                {
                    path->top()->Insertkn(*kp, r);
                }

                else
                {
                    _pop = true;
                    break;
                }
            }

            if(_pop)
            {
                Node* temp = new Node;
                temp->isLeaf = false;
                temp->Insertknn(*kp, l, r);

                root = temp;
            }
            delete(kp);
            delete(path);
        }
        iter2++;
    }
}

void Index :: key_query(vector<int> query_keys)
{
    fstream output;
    vector<int> k, v;
    vector<int>::iterator index;
    Node* a= new Node;
    output.open(OUTPUT_FILE, ios::out | ios::trunc);
    for (int i= 0; i< query_keys.size(); i++)
    {
        if(NULL == root)
        {
            output<<"-1"<<endl;
            continue;
        }
        a= root;
        while(!a->isLeaf)
        {
            index = lower_bound(a->keys.begin(), a->keys.end(), query_keys[i]);
            if(query_keys[i] == a->keys[index - a->keys.begin()])
            {
                a= a->children[(index - a->keys.begin()) + 1];
            }

            else
            {
                a= a->children[index - a->keys.begin()];
            }
        }
        k = a->keys;
        v = a->values;
        index = lower_bound(k.begin(), k.end(), query_keys[i]);

        if(query_keys[i] == k[index - k.begin()])
        {
            output<<v[index - k.begin()]<<endl;
        }

        else
        {
            output<<"-1"<<endl;
        }

    }
    delete(a);
    output.close();
}
void Index :: range_query(vector<pair<int,int>> query_pairs)
{
    fstream output2;
    vector<int> k, v;
    vector<int>::iterator index;
    output2.open(OUT2, ios::out | ios::trunc);
    Node* n = new Node;
    Node* a1= new Node;
    int ans;
    for (int i= 0 ; i< query_pairs.size(); i++)
    {
        if(NULL == root)
        {
            output2<<"-1"<<endl;
            continue;
        }
        ans= -1;
        a1= root;
        while(!a1->isLeaf)
        {
            index = lower_bound(a1->keys.begin(), a1->keys.end(), query_pairs[i].first);
            if(query_pairs[i].first == a1->keys[index - a1->keys.begin()])
            {
                a1= a1->children[(index - a1->keys.begin()) + 1];
            }

            else
            {
                a1= a1->children[index - a1->keys.begin()];
            }
        }
        k = a1->keys;
        v = a1->values;
        index = lower_bound(k.begin(), k.end(), query_pairs[i].first);
		n = a1->next;
        while(1)
        {
            if((index - k.begin()) == k.size())
            {
                k = n->keys;
                v = n->values;
                n = n->next;
                index = k.begin();
            }
            if((query_pairs[i].first <= k[index - k.begin()]) && (k[index - k.begin()] <= query_pairs[i].second))
            {
                if (ans < v[index- k.begin()])
                    ans = v[index- k.begin()];
            }
            else
            {
                break;
            }
            index++;
        }
        output2<<ans<<endl;

    }
    delete(a1);
    delete(n);
	output2.close();
}

void Index :: clear_index()
{
    Destroy(root);
}



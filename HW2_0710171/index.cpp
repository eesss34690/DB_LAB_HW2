#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include "index.h"
#define OUTPUT_FILE "key_query_out.txt"
#define OUT2 "range_query_out.txt"

bool Node :: Get_IsLeaf()
{
	return isLeaf;
}


vector<int> Node :: Get_Keys()
{
	return keys;
}

InternalNode :: InternalNode()
{
	isLeaf = false;
}


void InternalNode :: Insertkn(int key, Node* r)
{
	vector<int>::iterator index = lower_bound(keys.begin(), keys.end(), key);
	keys.insert(index, key);

	index = lower_bound(keys.begin(), keys.end(), key);
	children.insert(children.begin() + (index - keys.begin() + 1), r);
}


void InternalNode :: Insertknn(int key, Node* l, Node* r)
{
	keys.push_back(key);
	children.push_back(l);
	children.push_back(r);
}


Node* InternalNode :: Split(int* kp)
{
	int length = keys.size();

	InternalNode* r = new InternalNode;

	*kp = keys[length/2];

	r->keys.assign(keys.begin() + (length/2 + 1), keys.end());
	r->children.assign(children.begin() + (length/2 + 1), children.end());
	keys.erase(keys.begin() + length/2, keys.end());
	children.erase(children.begin() + (length/2 + 1), children.end());

	return r;
}

vector<Node*> InternalNode :: Get_Children()
{
	return children;
}

LeafNode :: LeafNode()
{
	isLeaf = true;
	prev = this;
	next = this;
}

void LeafNode :: Insertkv(int key, int value)
{
	vector<int>::iterator index = lower_bound(keys.begin(), keys.end(), key);

    keys.insert(index, key);
    index = lower_bound(keys.begin(), keys.end(), key);
    values.insert(values.begin() + (index - keys.begin()), value);
}


Node* LeafNode :: Split(int* kp)
{
	LeafNode* r = new LeafNode;

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


vector<int> LeafNode :: Get_Values()
{
	return values;
}


Node* LeafNode :: Get_Next()
{
	return next;
}


void Index :: Search_Path(Node* node, int key, stack<Node*>* path)
{
    path->push(node);

	if(!node->Get_IsLeaf())
    {
		vector<int> keys = node->Get_Keys();
		vector<Node*> children = node->Get_Children();
        vector<int>::iterator index = lower_bound(keys.begin(), keys.end(), key);

		if(key == keys[index - keys.begin()])
		{
            Search_Path(children[(index - keys.begin()) + 1], key, path);
		}

		else
		{
			Search_Path(children[index - keys.begin()], key, path);
		}
    }
}

void Index :: Destroy(Node* node)
{
	if (!node->Get_IsLeaf())
	{
        vector<Node*> c = node->Get_Children();
        for(vector<Node*>::iterator index = c.begin(); index != c.end(); index++)
        {
            Destroy(*index);
        }
	}
	delete(node);
}

void Index :: Initialize()
{
	root = NULL;
}

Index :: Index(int num, vector<int> keys, vector<int> val)
{

    vector<int>::iterator iter2= val.begin();
    Initialize();
    for (vector<int>::iterator iter1= keys.begin(); iter1!= keys.end(); iter1++)
    {
        Insert(*iter1, *iter2);
        iter2++;
    }
}

void Index :: Insert(int key, int value)
{
	if(NULL == root)
	{
		root = new LeafNode;
		root->Insertkv(key, value);
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
		Search_Path(root, key, path);
		path->top()->Insertkv(key, value);
		while(path->top()->Get_Keys().size() == order)
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
			InternalNode* temp = new InternalNode;

			temp->Insertknn(*kp, l, r);

			root = temp;
		}
		delete(kp);
		delete(path);
	}
}


int Index :: Search1(int key)
{
	if(NULL == root)
	{
		return -1;
	}
    int i = 0;

    stack<Node*>* path = new stack<Node*>;
    Search_Path(root, key, path);

    vector<int> keys = path->top()->Get_Keys();
    vector<int> values = path->top()->Get_Values();
    vector<int>::iterator index = lower_bound(keys.begin(), keys.end(), key);

    if(key == keys[index - keys.begin()])
    {
        return values[index - keys.begin()];
    }

    else
    {
        return -1;
    }

    delete(path);
}


vector<int> Index :: Search2(int key1, int key2)
{
    vector<int> a;
	if(NULL == root)
	{
		a.push_back(-1);
	}

	else
	{
		int i = 0;

		stack<Node*>* path = new stack<Node*>;
		stack<Node*>* path2= new stack<Node*>;
		Search_Path(root, key1, path);
		Search_Path(root, key2, path2);

		vector<int> keys = path->top()->Get_Keys();
		vector<int> keys2 = path2->top()->Get_Keys();
		vector<int> values = path->top()->Get_Values();
		Node* next = new Node;
		next = path->top()->Get_Next();
		vector<int>::iterator index = lower_bound(keys.begin(), keys.end(), key1);
        vector<int>::iterator index2 = lower_bound(keys2.begin(), keys2.end(), key2);

            while(1)
            {
                if((index - keys.begin()) == keys.size())
                {
                    keys = next->Get_Keys();
                    values = next->Get_Values();
                    next = next->Get_Next();
                    index = keys.begin();
                }
                if((key1 <= keys[index - keys.begin()]) && (keys[index - keys.begin()] <= key2))
                {
                    a.push_back(values[index- keys.begin()]);
                }
                else
                {
                    break;
                }
                index++;
            }



		delete(path);
		delete(path2);
	}
	return a;
}
void Index :: key_query(vector<int> query_keys)
{
    fstream output;
    output.open(OUTPUT_FILE, ios::out | ios::trunc);
    for (int i= 0; i< query_keys.size(); i++)
    {
        output<<Search1(query_keys[i])<<endl;
    }
    output.close();
}
void Index :: range_query(vector<pair<int,int>> query_pairs)
{
    fstream output2;
    output2.open(OUT2, ios::out | ios::trunc);
    vector<int> a;
    for (int i= 0 ; i< query_pairs.size(); i++)
    {
        a = Search2(query_pairs[i].first, query_pairs[i].second);
        if (a.size()== 1&& a[0]== -1)
        {
            output2<<"-1\n";
        }
        else
        {
            output2<<*max_element(a.begin(), a.end())<<endl;
        }
        a.clear();

    }
	output2.close();
}

void Index :: clear_index()
{
    Destroy(root);
}



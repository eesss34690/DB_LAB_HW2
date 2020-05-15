#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include "index.h"
#define OUTPUT_FILE "key_query_out.txt"
#define OUT2 "range_query_out.txt"

// getter function for accessing isLeaf
bool Node :: Get_IsLeaf()
{
	// return whether leaf or internal node
	return isLeaf;
}


// getter function for accessing keys
vector<int> Node :: Get_Keys()
{
	// return the vector of keys
	return keys;
}

// constructor for internal node
InternalNode :: InternalNode()
{
	isLeaf = false;
}


// function for insertion in an internal node
void InternalNode :: Insertkn(int key, Node* rightChild)
{
	// insert key in to suitable position in the given internal node
	vector<int>::iterator index = lower_bound(keys.begin(), keys.end(), key);
	keys.insert(index, key);

	// insert right child in the immediately next index in the children vector
	index = lower_bound(keys.begin(), keys.end(), key);
	children.insert(children.begin() + (index - keys.begin() + 1), rightChild);
}


// function for insertion in a new internal root node
void InternalNode :: Insertknn(int key, Node* leftChild, Node* rightChild)
{
	// insert key, left child and right child
	keys.push_back(key);
	children.push_back(leftChild);
	children.push_back(rightChild);
}


// function for splitting an internal node
Node* InternalNode :: Split(int* keyToParent)
{
	int length = keys.size();

	// create a new right internal node
	InternalNode* rightNode = new InternalNode;

	// key to be moved up to the parent is the middle element in the current internal node
	*keyToParent = keys[length/2];

	// Copy the second half of the current internal node excluding the middle element to the
	// new right internal node. Erase the second half of the current internal node including
	// the middle element, and thus current internal node becomes the left internal node.
	rightNode->keys.assign(keys.begin() + (length/2 + 1), keys.end());
	rightNode->children.assign(children.begin() + (length/2 + 1), children.end());
	keys.erase(keys.begin() + length/2, keys.end());
	children.erase(children.begin() + (length/2 + 1), children.end());

	// return the new right internal node
	return rightNode;
}


// getter function for accessing children
vector<Node*> InternalNode :: Get_Children()
{
	// return the children vector
	return children;
}
// constructor for leaf node
LeafNode :: LeafNode()
{
	isLeaf = true;
	prev = this;
	next = this;
}


// function for insertion in a leaf node
void LeafNode :: Insertkv(int key, int value)
{
	// search for the key in the given leaf node
	vector<int>::iterator index = lower_bound(keys.begin(), keys.end(), key);

    // insert the new key
    keys.insert(index, key);
    //insert the corresponding value
    index = lower_bound(keys.begin(), keys.end(), key);
    values.insert(values.begin() + (index - keys.begin()), value);
}


// function for splitting a leaf node
Node* LeafNode :: Split(int* keyToParent)
{
	// create a new right leaf node
	LeafNode* rightNode = new LeafNode;

	// key to be moved up to the parent is the middle element in the current leaf node
	*keyToParent = keys[keys.size()/2];

	// Copy the second half of the current leaf node to the new right leaf node. Erase the second
	// half of the current leaf node, and thus the current leaf node becomes the left leaf node.
	rightNode->keys.assign(keys.begin() + keys.size()/2, keys.end());
	rightNode->values.assign(values.begin() + values.size()/2, values.end());
	keys.erase(keys.begin() + keys.size()/2, keys.end());
	values.erase(values.begin() + values.size()/2, values.end());

	// link the leaf nodes to form a doubly linked list
	rightNode->next = next;
	rightNode->prev = this;
	next = rightNode;
	(rightNode->next)->prev = rightNode;

	// return the right leaf node
	return rightNode;
}


// getter function for accessing values
vector<int> LeafNode :: Get_Values()
{
	// return the vector of values
	return values;
}


// getter function for accessing the next pointer
Node* LeafNode :: Get_Next()
{
	// return the pointer to the next leaf node
	return next;
}


// function for searching from root to leaf node and pushing on to a stack
void Index :: Search_Path(Node* node, int key, stack<Node*>* path)
{
	// push node to stack
        path->push(node);

	// check if the node pushed to stack is an internal node
	if(!node->Get_IsLeaf())
        {
		// search for the given key in the current node
		vector<int> keys = node->Get_Keys();
		vector<Node*> children = node->Get_Children();
        vector<int>::iterator index = lower_bound(keys.begin(), keys.end(), key);

		// check if key is found
		if(key == keys[index - keys.begin()])
		{
			// recursively repeat by searching the path through the corresponding right child index
            Search_Path(children[(index - keys.begin()) + 1], key, path);
		}

		// if key is not found
		else
		{
			// recursively repeat by searching the path through the corresponding left child index
			Search_Path(children[index - keys.begin()], key, path);
		}
        }
}


// function to destroy the tree
void Index :: Destroy(Node* node)
{
	// recursively repeat the function to delete all the nodes level by level, starting with the leaf nodes
	if (!node->Get_IsLeaf())
	{
        vector<Node*> children = node->Get_Children();
            for(vector<Node*>::iterator index = children.begin(); index != children.end(); index++)
            {
                Destroy(*index);
        	}
	}
	delete(node);
}


#ifdef DEBUG
// function to reveal the contents of the B+ tree
void Index :: Reveal_Tree(Node* node)
{
	// check if tree is empty
        if (NULL == node)
        {
                cout<<endl<<"Root Node: Null";
                return;
        }

	// check if current node is a leaf node
        if(node->Get_IsLeaf())
        {
                cout<<endl<<"Leaf Node: ";
        }

	// if current node is a internal node
        else
        {
                cout<<endl<<"Internal Node: ";
        }

	// display the keys
        vector<int> keys = node->Get_Keys();
        for(vector<int>::iterator index = keys.begin(); index != keys.end(); index++)
        {
                cout<<*index<<" ";
        }
        cout<<endl;

	// check if internal node to continue revelation of the next level
        if (!node->Get_IsLeaf())
        {
		// display the keys in the children of the current internal node
                vector<Node*> children = node->Get_Children();
                cout<<"children"<<endl<<"--------"<<endl;
                for(vector<Node*>::iterator index = children.begin(); index != children.end(); index++)
                {
                        vector<int> childKeys = (*index)->Get_Keys();
                        for(vector<int>::iterator i = childKeys.begin(); i != childKeys.end(); i++)
                        {
                                cout<<*i<<" ";
                        }
                        cout<<endl;
                }

		// recursively repeat revelation of the next level
                for(vector<Node*>::iterator index = children.begin(); index != children.end(); index++)
                {
                        Reveal_Tree(*index);
                }
        }
}
#endif


// operation: Initialize()
void Index :: Initialize()
{
	root = NULL;
}

Index :: Index(int num, vector<int> keys, vector<int> val)
{

    vector<int>::iterator iter2= val.begin();
    Initialize();/*
    for (int i= 0; i< keys.size(); i++)
    {
        cout<<&keys[i]<<" "<<&val[i]<<endl;
    }
    cout<<endl;*/
    for (vector<int>::iterator iter1= keys.begin(); iter1!= keys.end(); iter1++)
    {
    //cout<<*iter1<<" "<<*iter2<<endl;
        Insert(*iter1, *iter2);
        iter2++;
    }
}

// operation: Insert(key, value)
void Index :: Insert(int key, int value)
{
	// check if tree is empty
	if(NULL == root)
	{
		// Irrespective of the order, root is always a leaf node for
		// the first insertion. So, create a new leaf node.
		root = new LeafNode;
		root->Insertkv(key, value);
	}

	// if it is a subsequent insertion
	else
	{
		Node* leftNode = new Node;
		leftNode= NULL;
		Node* rightNode = new Node;
		rightNode = NULL;
		int* keyToParent =new int;
		bool rootPopped = false;

		// obtain the search path from the root to leaf node and push it on to a stack
		stack<Node*>* path = new stack<Node*>;
		Search_Path(root, key, path);
		// insert the key-value pair in the leaf node
		path->top()->Insertkv(key, value);
		// Split the current node and insert the middle key & children in to the parent. Perform
		// this as long as there is an imbalance in the tree, moving up the stack every iteration.
		while(path->top()->Get_Keys().size() == order)
		{
			// Update the current node as the left half and return the right half. Also
			// obtain the middle element, which is the key to be moved up to the parent.
			leftNode = path->top();
			rightNode = leftNode->Split(keyToParent);

			// check if currently split node is not the root node
			if (path->size()> 0)path->pop();
			if(!path->empty())
			{
				// Insert the middle key and the right half in to
				// the parent. The parent will be an internal node.
				path->top()->Insertkn(*keyToParent, rightNode);
			}

			// if currently split node is the root node
			else
			{
				// set flag indicating that the root has popped from the stack
				rootPopped = true;
				break;
			}
		}

		// new internal node needs to be created and assigned as the root
		if(rootPopped)
		{
			// create a new internal node
			InternalNode* tempRoot = new InternalNode;

			// insert the left and the right halves as the children of this new internal node
			tempRoot->Insertknn(*keyToParent, leftNode, rightNode);

			// mark this new internal node as the root of the tree
			root = tempRoot;
		}
		delete(keyToParent);
		delete(path);
	}
}


// operation: Search(key)
void Index :: Search(int key)
{
	// check if tree is empty
	if(NULL == root)
	{
		outputFile<<"-1"<<endl;
	}

	// if it is a vaild search
	else
	{
		int i = 0;

		// obtain the search path from root to leaf node and push it on to a stack
		stack<Node*>* path = new stack<Node*>;
		Search_Path(root, key, path);

		// search for the key in the leaf node, which is at the top of the stack
		vector<int> keys = path->top()->Get_Keys();
		vector<int> values = path->top()->Get_Values();
		vector<int>::iterator index = lower_bound(keys.begin(), keys.end(), key);

		// check if key is found
		if(key == keys[index - keys.begin()])
		{
			// display the values
			outputFile<<values[index - keys.begin()]<<endl;
		}

		// if key is not found
		else
		{
			outputFile<<"-1"<<endl;
		}

		delete(path);
	}
}


// operation: Search(key1, key2)
vector<int> Index :: Search(int key1, int key2)
{
    vector<int> a;
	// check if tree is empty
	if(NULL == root)
	{
		a.push_back(-1);
	}

	// if it is a valid range search
	else
	{
		int i = 0;
		bool firstPass = true;
		int firstKey = -1;

		// obtain the search path from root to leaf node and push it on to a stack
		stack<Node*>* path = new stack<Node*>;
		Search_Path(root, key1, path);

		// search for the key in the leaf node, which is at the top of the stack
		vector<int> keys = path->top()->Get_Keys();
		vector<int> values = path->top()->Get_Values();
		Node* next = new Node;
		next = path->top()->Get_Next();
		vector<int>::iterator index = lower_bound(keys.begin(), keys.end(), key1);

		// display all the keys in the search range, along with their corresponding values
		while(1)
		{
			// check if end of the current leaf node is reached
			if((index - keys.begin()) == keys.size())
			{
 				// go to the next leaf node
				keys = next->Get_Keys();
                values = next->Get_Values();
                next = next->Get_Next();
				index = keys.begin();
			}

			// save the smallest key in the given search range
			if(firstPass)
			{
				firstKey = keys[index - keys.begin()];
			}


			// check if already iterated through the doubly linked list once
			if(!(firstPass || (keys[index - keys.begin()] != firstKey)))
			{
				// exit the loop
				break;
			}

			// check if key is within the search range
			if((key1 <= keys[index - keys.begin()]) && (keys[index - keys.begin()] <= key2))
			{
                a.push_back(values[index- keys.begin()]);
			}

			// if key is not within the search range
			else
			{
                a.push_back(-1);
			}

			firstPass = false;
			index++;
		}

		delete(path);
	}
	return a;
}
void Index :: key_query(vector<int> query_keys)
{
    outputFile.open(OUTPUT_FILE, ios::out | ios::trunc);
    for (int i= 0; i< query_keys.size(); i++)
    {
        Search(query_keys[i]);
    }
    outputFile.close();
}
void Index :: range_query(vector<pair<int,int>> query_pairs)
{
    outputFile.open(OUT2, ios::out | ios::trunc);
    for (int i= 0 ; i< query_pairs.size(); i++)
    {
        vector<int> a = Search(query_pairs.first, query_pairs.second);

    }
    if (a.size()== 1&& a[0]== -1)
    {
        outputFile<<"-1\n";
    }
    else
    {
        outputFile<<*max_element(a.begin(), a.end())<<endl;
    }
	outputFile.close();
}


// destructor for tree
Index :: ~Index ()
{
	Destroy(root);
}


#ifdef DEBUG
// function to print the current state of the tree
void Index :: Print_Tree()
{
	Reveal_Tree(root);
}
#endif

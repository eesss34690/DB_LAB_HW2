#ifndef INDEX_H_
#define INDEX_H_
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <stack>

using namespace std;
class Node
{
	public:
		bool isLeaf;
		vector<int> keys;
		vector<Node*> children;
		Node* prev;
		Node* next;
		vector<int> values;

	public:
		Node();
		void Insertkn(int key, Node* rightChild);
		void Insertknn(int key, Node* leftChild, Node* rightChild);
		Node* Split(int* kp);
		void Insertkv(int key, int value);
};


// B+ tree
class Index
{
	public:
		int order= 40;
		Node* root;
		ofstream outputFile;
		void Search_Path(Node* node, int key, stack<Node*>* path);
		void Destroy(Node* node);

	public:
		Index(int num, vector<int> keys, vector<int> val);
		void key_query(vector<int> query_keys);
		void range_query(vector<pair<int,int>> query_pairs);
		void clear_index();

};

#endif // INDEX_H_INCLUDED

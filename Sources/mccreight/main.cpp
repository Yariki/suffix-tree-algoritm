#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <time.h>

using std::string;
using std::vector;

enum POS_TYPE
{
	EDGE,
	NODE
};


struct Node;

struct Edge
{
	Node* parent;
	Node* node;
	int first;
	int last;
};

struct Node
{
	Edge* owner;
	vector<Edge*> children;
	Node* suffix;
};

struct Label
{
	Label(int s, int e)
	{
		start = s;
		end = e;
	}
	int start;
	int end;
};

struct Pos
{
	void* element;
	POS_TYPE type;
	Label label;
};




Node* root;
Node* head;
string currenttext;
int lenght = -1;

void create_init_tree()
{

}

Node* get_parent(Node* node)
{
	return node->owner->parent;
}

Label get_label(Node*parent,Node* node)
{
	Label l(node->owner->first,node->owner->last);
	return l;
}

Node* get_suffix(Node* node)
{
	return node->suffix;
}


void set_suffix(Node* head, Node* suf)
{
	if(head != NULL)
		head->suffix = suf;
}

Pos* slowscan(Node* startNode, const Label& label)
{
	return nullptr;
}

Pos* fastscan(Node* endNode, const Label& label)
{
	return nullptr;
}

int main()
{
	currenttext = "abaab";
	currenttext += "$";
	lenght = currenttext.length();
	if(lenght == -1)
	{
		printf( "Wrong lenght...");
		system("pause");
	}
	create_init_tree();
	head = root;

	for(int i = 0; i < lenght;i++)
	{
		if(head == root)
		{
			Pos* pos = slowscan(root,Label(i+1,lenght-1));

			continue;
		}
		Node* u = get_parent(head);
		Label v = get_label(u,head);
		Pos* w = nullptr;
		if(u == root)
			w = fastscan(get_suffix(u),v);
		else
			w = fastscan(root,v);
		switch(w->type)
		{
		case POS_TYPE::EDGE:
			//
			break;
		case  POS_TYPE::NODE:
			//
			break;
		}
	}

	system("pause");
}
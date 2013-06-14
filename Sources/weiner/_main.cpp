
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <iostream>
#include <time.h>
#include <vector>

using std::string;
using std::vector;

struct wedge;
struct wnode;

enum typeobject
{
  tonone,
  node,
  edge
};

enum typeevent
{
  tenone,
  add,
  split
};

struct position
{
  wnode* node;
  wedge* edge;
  int startOld;
  int startNew;
  int end;
  typeevent eventtype;
};

struct wnode
{
  wedge* parent;
  wedge* child;
};

struct wedge
{
  wnode* parent;
  int start;
  int end;
  wedge* next;
  wnode* innerNode;
};

wedge* current;
wnode* root;
string currentStr;
int lastIndex;
int itemcount = 0;
char* format = "(%d:%s)\n";
char* formatN = "(%d:%s):leaf\n";



wnode* create_node(wedge* parent)
{
  auto node= new wnode();
  node->parent = parent;
  return node;
}

bool is_root(const wnode* node)
{
  return node->parent == nullptr;
}

bool is_leaf(const wedge* edge)
{
	return edge != NULL && edge->innerNode == NULL;
}

wedge* create_wedge(int& start, int& end)
{
  auto edge = new wedge();
  edge->start = start;
  edge->end = end;
  edge->innerNode = nullptr;
  return edge;
}

void add_edge(wnode* node, wedge* edge)
{
  auto child = node->child;
	
  if(!child)
  {
	node->child = edge;
	edge->parent = (wnode*)node;
  }
  else
  {
	edge->parent = (wnode*)node;
	while(child->next != nullptr)
		child = child->next;
	child->next = edge;
  }
}

position* create_position(const wnode* node, const wedge* edge,int& startOld,int& startNew, int& end,typeevent te)
{
  auto pos = new position();
  pos->node = (wnode*) node;
  pos->edge = (wedge*) edge;
  pos->startOld = startOld;
  pos->startNew = startNew;
  pos->end = end;
  pos->eventtype = te;
  return pos;
}

position* find_pos_recursive(const wnode* node, int start)
{
  wedge* child = node->child;
  while(child){
    if(currentStr[child->start] == currentStr[start])
      break;
    child = child->next;
  }
  if(!child)
	  return create_position(node, child,start,start,lastIndex,add);
  int f = child->start;
  int i = start;
  while( f <= child->end && i <= lastIndex)
	if(currentStr[f++] != currentStr[i++])
		break;
  if(child->innerNode && i <= lastIndex)
  {
	  find_pos_recursive(child->innerNode,i);
  }
  else
	  create_position(child->innerNode,child,--f,--i,lastIndex,i <= f ? split : add);
}

position* find_pos(int start)
{
  return find_pos_recursive(root, start);
}

void split_edge(const position* p){
	if(p->edge == nullptr) return;
	wedge* splitEdge1 = p->edge;
	int oldend = splitEdge1->end;
	splitEdge1->end = p->startOld - 1;
	int newEnd1 = p->startOld;
	auto splitEdge2 = create_wedge(newEnd1, oldend);
	auto currentEdge = create_wedge((int&)p->startNew,lastIndex);
	splitEdge1->innerNode = create_node(splitEdge1);
	add_edge(splitEdge1->innerNode,splitEdge2);
	add_edge(splitEdge1->innerNode,currentEdge);
}

void add_new_node(const position* p){
	int start = p->startNew;
	auto edge = create_wedge(start,lastIndex);
	add_edge(p->node,edge);

}

void show(wnode* src, int deep, string str)
{
	if(!src)
		return;
	wedge* edge = src->child;
	while(edge)
	{

		if(is_leaf(edge))
		{
			string temp = str;
			if(deep > 1)
			{
				for(int i = 0; i < 2;i++)temp += " ";
				printf("%s|\n",temp.c_str());
				printf("%s|->",temp.c_str());
			}
			else
			{
				printf("%s-->",temp.c_str());
			}
			int f = edge->start ? edge->start : 1;
			printf(formatN,deep,currentStr.substr(edge->start,edge->end - edge->start + 1).c_str());
			itemcount++;
		}
		else if(!is_leaf(edge))
		{
			string temp = str;
			string temp1 = str;
			for(int i = 0; i < 2;i++ )
			{
				temp += "-";
				temp1 += " ";
			}
			printf("%s>",temp.c_str());
			int f = edge->start ? edge->start : 1;
			printf(format,deep,currentStr.substr(edge->start,edge->end - edge->start + 1).c_str());
			itemcount++;
			temp1 += "|";
			show(edge->innerNode,deep + 1,temp1);
		}
		edge = edge->next;
	}
}

void show_tree()
{
	printf("Root\n");
	string str = "|";
	printf("|\n");
	show(root,1,str);
}

void delete_tree(wnode* node)
{
	if(!node)
		return;
	wedge* edge = node->child;
	vector<wedge*> vec;
	while(edge)
	{
		vec.push_back(edge);
		edge =  edge->next;
	}
	edge =  nullptr;

	for(int i = 0; i < vec.size();i++)
	{
		edge = vec[i];
		if(is_leaf(edge))
		{
			delete edge;
			edge = nullptr;
		}
		else
		{
			delete_tree(edge->innerNode);
		}
	}
	delete node;
	node = nullptr;
}

void add_first_edge()
{
	auto edge = create_wedge(lastIndex,lastIndex);
	add_edge(root,edge);
}

void show_info()
{
	printf("Rozrahunkovo-graphichna robota na temy 'Algoritmu pobydovu syffiksnuh derev'\n");
	printf("Algoritm - Weinera\n");
	printf("Stydentku grypu IS-22 - Simon Anastasii\n");
	printf("\n");
}

int main()
{
	show_info();
	char* row = new char[255];
	printf("Vvedit ryadok (pruklad -> 'ABRACADABRA'):");
	std::cin >> row;
  currentStr = string(row);
  root = create_node(nullptr);
  if(currentStr.length() > 0)
  {
    currentStr += "$";
  }
  lastIndex = currentStr.length() - 1;
  int startPos = lastIndex;
  add_first_edge();
  startPos--;
  for(; startPos >= 0; startPos--)
  {
	  position* pos = find_pos(startPos);
	  if(pos){
		  switch(pos->eventtype){
		  case split:
			  split_edge(pos);
			  break;
		  case add:
			  add_new_node(pos);
			  break;
		  }
	  }

  }
  show_tree();
  delete_tree(root);
  system("pause");
}

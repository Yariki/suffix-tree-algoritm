
#include <iostream>
#include <stdio.h>
#include <vector>

struct wedge;
struct wnode;

using std::vector;
using std::string;





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
};

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

wedge* create_wedge(int& start, int& end,wnode* parent)
{
  auto edge = new wedge();
  edge->start = start;
  edge->end = end;
  edge->parent = parent;
  return edge;
}

void add_edge(const wnode* node, wedge* edge)
{
  auto child = node->child;
  while(child->next != nullptr)
    child = child->next;
  child->next = edge;
}



int main()
{
  

  system("pause");
}

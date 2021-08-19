#include <iostream>
// using namespace std;

class LinkNode
{
public:
	int data;
	LinkNode *next;

	LinkNode(int data)
	{
		this->data = data;
		next = NULL;
	}
};

class LinkedList
{
private:
	LinkNode *head, *tail;

public:
	LinkedList()
	{
		head = NULL;
		tail = NULL;
	}

	LinkedList(int val)
	{
		head = tail = new LinkNode(val);
	}

	void insert(int val);
	void printList();
};

void LinkedList::insert(int val)
{
	LinkNode *node = new LinkNode(val);
	if (!head)
	{
		head = node;
		tail = node;
	}
	else
	{
		tail->next = node;
		tail = tail->next;
	}
}

void LinkedList::printList()
{
	LinkNode *node = head;
	while (node)
	{
		std::cout << node->data << " ";
		node = node->next;
	}
	std::cout << "\n";
}
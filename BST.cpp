#include <iostream>
#include <limits.h>
#include <fstream>
#include "LinkedList.h"
using namespace std;

class Node
{
public:
	Node *left, *right;
	bool lthread, rthread;
	int val, sizeOfRightSubtree;

public:
	Node(int val)
	{
		this->val = val;
		sizeOfRightSubtree = 0;
		left = right = NULL;
		lthread = rthread = false;
	}
};

class BST
{
public:
	Node *root;
	void deleteLeaf(Node *node, Node *par);
	void deleteNodeWithOneChild(Node *node, Node *par);
	void deleteNodeWithTwoChildren(Node *node, Node *par);
	void allElementsBetweenHelper(const Node *node, int k1, int k2, LinkedList *list);
	int kthElementHelper(Node *node, int k);

public:
	void inorder()
	{
		if (!root)
			return;

		Node *cur = root;

		while (!cur->lthread && cur->left)
			cur = cur->left;

		while (cur)
		{
			cout << cur->val << endl;
			if (cur->rthread)
				cur = cur->right;
			else
			{
				cur = cur->right;
				if (!cur)
					break;
				while (!cur->lthread && cur->left)
					cur = cur->left;
			}
		}
	}

	BST()
	{
		root = NULL;
	}

	// copy constructor
	BST(const BST *bst)
	{
		copyConstructorHelper(root, bst);
	}

	void copyConstructorHelper(Node *node, BST bst)
	{
		if (!node)
			return;
		bst.insert(node->val);
		copyConstructorHelper(node->left, bst);
		copyConstructorHelper(node->right, bst);
	}

	void insert(int x);
	Node *search(int x);
	void deleteKey(int x);
	Node *successor(Node *node);
	Node *predecessor(Node *node);
	LinkedList *reverseInorder();
	LinkedList *allElementsBetween(int k1, int k2);
	void kthElement(int k);
	void printTree();
	void printTreeHelper(const Node *node, ofstream &fout);
};

void BST::insert(int x)
{
	Node *cur = root;
	Node *par = NULL;

	if (search(x))
	{
		cout << "Duplicate Value!" << endl;
		return;
	}

	while (cur)
	{
		// if (x == cur->val)
		// {
		// 	cout << "Duplicate Value!" << endl;
		// 	return;
		// }

		par = cur;

		if (x < cur->val)
		{
			if (!cur->lthread)
				cur = cur->left;
			else
				break;
		}
		else
		{
			cur->sizeOfRightSubtree += 1;
			if (!cur->rthread)
				cur = cur->right;
			else
				break;
		}
	}

	Node *node = new Node(x);

	if (!par) // insert as root
	{
		root = node;
	}
	else if (x < par->val) // insert as left child
	{
		node->lthread = node->rthread = true;
		node->left = par->left;
		node->right = par;
		par->left = node;
		par->lthread = false;
	}
	else // insert as right child
	{
		node->lthread = node->rthread = true;
		node->right = par->right;
		node->left = par;
		par->right = node;
		par->rthread = false;
	}
}

Node *BST::search(int x)
{
	Node *cur = root;

	while (cur)
	{
		if (x == cur->val)
			return cur;
		else if (x < cur->val)
		{
			if (!cur->lthread)
				cur = cur->left;
			else
				break;
		}
		else
		{
			if (!cur->rthread)
				cur = cur->right;
			else
				break;
		}
	}
	return NULL;
}

void BST::deleteKey(int x)
{
	Node *cur = root, *par = NULL;

	if (!search(x))
	{
		throw "Key to be deleted is not present in the BST!";
	}

	while (cur)
	{
		if (x == cur->val)
		{
			break;
		}
		else if (x < cur->val)
		{
			par = cur;
			cur = cur->left;
		}
		else
		{
			cur->sizeOfRightSubtree -= 1;
			par = cur;
			cur = cur->right;
		}
	}

	if (cur->lthread && cur->rthread) // cur is leaf node
	{
		deleteLeaf(cur, par);
	}
	else if (!cur->lthread && !cur->rthread) // cur has two children
	{
		deleteNodeWithTwoChildren(cur, par);
	}
	else // cur has one child
	{
		deleteNodeWithOneChild(cur, par);
	}
}

void BST::deleteLeaf(Node *node, Node *par)
{
	// If Node to be deleted is root
	if (!par)
		root = NULL;

	// If Node to be deleted is left
	// of its parent
	else if (node == par->left)
	{
		par->lthread = true;
		par->left = node->left;
	}
	else
	{
		par->rthread = true;
		par->right = node->right;
	}

	// Free memory and return new root
	delete (node);
}

void BST::deleteNodeWithTwoChildren(Node *node, Node *par)
{
	// Find inorder successor and its parent.
	Node *parsucc = node;
	Node *succ = node->right;

	node->sizeOfRightSubtree -= 1;

	// Find leftmost child of successor
	while (!succ->lthread && succ->left != NULL)
	{
		parsucc = succ;
		succ = succ->left;
	}

	node->val = succ->val;

	if (succ->lthread == true && succ->rthread == true)
		deleteLeaf(succ, parsucc);
	else
		deleteNodeWithOneChild(succ, parsucc);
}

void BST::deleteNodeWithOneChild(Node *node, Node *par)
{
	Node *child;

	// Initialize child Node to be deleted has
	// left child.
	if (node->lthread == false)
		child = node->left;

	// Node to be deleted has right child.
	else
		child = node->right;

	// Node to be deleted is root Node.
	if (par == NULL)
		root = child;

	// Node is left child of its parent.
	else if (node == par->left)
		par->left = child;
	else
		par->right = child;

	// Find successor and predecessor
	Node *s = successor(node);
	Node *p = predecessor(node);

	// If node has left subtree.
	if (node->lthread == false)
		p->right = s;

	// If node has right subtree.
	else
	{
		if (node->rthread == false)
			s->left = p;
	}

	delete (node);
}

Node *BST::successor(Node *node)
{
	Node *cur = node;
	if (cur->rthread)
		return cur->right;

	cur = cur->right;
	if (!cur)
		return cur;

	while (!cur->lthread && cur->left)
		cur = cur->left;
	return cur;
}

Node *BST::predecessor(Node *node)
{
	Node *cur = node;
	if (cur->lthread)
		return cur->left;

	cur = cur->left;
	if (!cur)
		return cur;

	while (!cur->rthread && cur->right)
		cur = cur->right;
	return cur;
}

LinkedList *BST::reverseInorder()
{
	LinkedList *list = new LinkedList();
	if (!root)
		return list;
	Node *cur = root;
	while (!cur->rthread && cur->right)
		cur = cur->right;

	while (cur)
	{
		list->insert(cur->val);
		if (cur->lthread)
			cur = cur->left;
		else
		{
			cur = cur->left;
			if (!cur)
				break;
			while (!cur->rthread && cur->right)
				cur = cur->right;
		}
	}

	return list;
}

LinkedList *BST::allElementsBetween(int k1, int k2)
{
	if (k1 > k2)
		swap(k1, k2);
	LinkedList *list = new LinkedList();
	allElementsBetweenHelper(root, k1, k2, list);
	return list;
}

void BST::allElementsBetweenHelper(const Node *node, int k1, int k2, LinkedList *list)
{
	/* base case */
	if (!node)
		return;

	/* Since the desired o/p is sorted,
        recurse for left subtree first
        If node->data is greater than k1,
        then only we can get o/p keys
        in left subtree */
	if (!node->lthread && k1 < node->val)
		allElementsBetweenHelper(node->left, k1, k2, list);

	/* if node's val lies in range,
    then prints node's val */
	if (k1 <= node->val && node->val <= k2)
		list->insert(node->val);

	/* If node->val is smaller than k2,
        then only we can get o/p keys
        in right subtree */
	if (!node->rthread && k2 > node->val)
		allElementsBetweenHelper(node->right, k1, k2, list);
}

void BST::kthElement(int k)
{
	cout << "Kth (K = " << k << ") Largest Element is ";
	cout << kthElementHelper(root, k) << endl;
}

int BST::kthElementHelper(Node *node, int k)
{
	if (!node)
		return INT_MIN;

	Node *cur = node;

	while (cur)
	{
		if (cur->sizeOfRightSubtree + 1 == k)
			return cur->val;
		if (cur->sizeOfRightSubtree + 1 < k)
		{
			k -= (cur->sizeOfRightSubtree + 1);
			cur = cur->left;
		}
		else
			cur = cur->right;
	}
	return INT_MIN;
}

void BST::printTree()
{
	ofstream fout;

	fout.open("BST.dot");
	fout << "digraph g {\n";
	printTreeHelper(root, fout);
	fout << "}";
	fout.close();
	system("dot -Tpng BST.dot -o BST.png");
}

void BST::printTreeHelper(const Node *node, ofstream &fout)
{
	if (!node)
		return;

	if (node == root)
	{
		fout << "label = \" rooted at " << node->val << " \";\n";
		fout << node->val << " [root = true]\n";
	}
	if (!node->lthread)
	{
		fout << node->val << " -> " << node->left->val << "\n";
		printTreeHelper(node->left, fout);
	}
	else
	{
		if (node->left)
			fout << node->val << " -> " << node->left->val << "[style = dotted];\n";
	}

	if (!node->rthread)
	{
		fout << node->val << " -> " << node->right->val << "\n";
		printTreeHelper(node->right, fout);
	}
	else
	{
		if (node->right)
			fout << node->val << " -> " << node->right->val << "[style = dotted];\n";
	}
}

int main()
{
	BST *bst = new BST();
	int arr[] = {50, 30, 100, 5, 40, 90, 105, 0, 10, 35, 45, 85, 95};

	for (int num : arr)
	{
		bst->insert(num);
	}

	// bst->deleteKey(0);
	// bst->deleteKey(90);
	bst->insert(1);

	bst->printTree();

	// for (int i = 0; i < 13; ++i)
	// {
	// 	bst->kthElement(i + 1);
	// }

	// LinkedList *list = bst->allElementsBetween(1, 100);
	// list->printList();
}
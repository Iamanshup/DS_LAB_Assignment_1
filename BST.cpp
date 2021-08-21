#include <iostream>
#include <limits.h>
#include <fstream>
#include "LinkedList.h"
using namespace std;

// class for BST node
class Node
{
public:
	Node *left, *right;
	bool lthread, rthread;
	int val, sizeOfRightSubtree;

public:
	// constructor
	Node(int val)
	{
		this->val = val;
		sizeOfRightSubtree = 0;
		left = right = NULL;
		lthread = rthread = false;
	}
};

// class needed for PrintTree() function
class DoubleTrees
{
public:
	Node *left, *right;
	DoubleTrees()
	{
		left = NULL;
		right = NULL;
	}
};

// class for BST
class BST
{
public:
	Node *root;

	void copyConstructorHelper(Node *node, BST *bst);
	void deleteLeaf(Node *node, Node *par);
	void deleteNodeWithOneChild(Node *node, Node *par);
	void deleteNodeWithTwoChildren(Node *node, Node *par);
	void allElementsBetweenHelper(const Node *node, int k1, int k2, LinkedList *list);
	int kthElementHelper(Node *node, int k);

public:
	// void inorder()
	// {
	// 	if (!root)
	// 		return;

	// 	Node *cur = root;

	// 	while (!cur->lthread && cur->left)
	// 		cur = cur->left;

	// 	while (cur)
	// 	{
	// 		cout << cur->val << endl;
	// 		if (cur->rthread)
	// 			cur = cur->right;
	// 		else
	// 		{
	// 			cur = cur->right;
	// 			if (!cur)
	// 				break;
	// 			while (!cur->lthread && cur->left)
	// 				cur = cur->left;
	// 		}
	// 	}
	// }

	BST();
	BST(BST *bst);

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
	void split(int k);
	DoubleTrees *splitHelper(Node *node, int k);
};

// default constructor
BST::BST()
{
	root = NULL;
}

// copy constructor
BST::BST(BST *bst)
{
	bst = new BST();
	copyConstructorHelper(this->root, bst);
}

void BST::copyConstructorHelper(Node *node, BST *bst)
{
	if (!node)
		return;
	bst->insert(node->val);

	if (!node->lthread)
		copyConstructorHelper(node->left, bst);

	if (!node->rthread)
		copyConstructorHelper(node->right, bst);
}

// insert function
void BST::insert(int x)
{
	Node *cur = root;
	Node *par = NULL;

	// element s aleady present in tree
	// throw an exception
	if (search(x))
	{
		throw "Duplicate Value!";
		return;
	}

	// find position where element is to be inserted
	while (cur)
	{

		par = cur;

		if (x < cur->val)
		{
			// go to left child if exists
			if (!cur->lthread)
				cur = cur->left;
			else
				break;
		}
		else
		{
			// element is inserted in right subtree
			// increment size of right subtree
			cur->sizeOfRightSubtree += 1;

			// go to right child if exists
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

// search element in bst
Node *BST::search(int x)
{
	Node *cur = root;

	while (cur)
	{
		// return if found
		if (x == cur->val)
			return cur;

		else if (x < cur->val)
		{
			// go to left child if exists
			if (!cur->lthread)
				cur = cur->left;
			else
				break;
		}
		else
		{
			// go to right child if exists
			if (!cur->rthread)
				cur = cur->right;
			else
				break;
		}
	}

	// not found
	return NULL;
}

// deletekey function
void BST::deleteKey(int x)
{
	Node *cur = root, *par = NULL;

	// if element to be deleted not present in
	// the bst then throw an exception
	if (!search(x))
	{
		throw "Key to be deleted is not present in the BST!";
	}

	// find the element to be deleted
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
			// if deletion takes place from right subtree
			// decrement size of right subtree
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

	// Free memory
	delete (node);
}

void BST::deleteNodeWithTwoChildren(Node *node, Node *par)
{
	// Find inorder successor and its parent.
	Node *parsucc = node;
	Node *succ = node->right;

	// decrement size of right subtree
	// as successor node will be replaced
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

	// free memory
	delete (node);
}

// find successor of a node
Node *BST::successor(Node *node)
{
	Node *cur = node;

	// if right child does not exist
	// return successor stored as right child
	if (cur->rthread)
		return cur->right;

	// go to right child if exists
	cur = cur->right;
	if (!cur)
		return cur;

	// go to left most element
	while (!cur->lthread && cur->left)
		cur = cur->left;
	return cur;
}

Node *BST::predecessor(Node *node)
{
	// if left child does not exist
	// return successor stored as left child
	Node *cur = node;
	if (cur->lthread)
		return cur->left;

	// go to left child if exists
	cur = cur->left;
	if (!cur)
		return cur;

	// go to right most element
	while (!cur->rthread && cur->right)
		cur = cur->right;
	return cur;
}

// return the reverse inorder
// of bst in a linked list
LinkedList *BST::reverseInorder()
{
	LinkedList *list = new LinkedList();
	if (!root)
		return list;
	Node *cur = root;

	// go to right most element
	while (!cur->rthread && cur->right)
		cur = cur->right;

	while (cur)
	{
		// push current element to list
		list->insert(cur->val);
		// go to te predecessor
		cur = predecessor(cur);
	}

	// return the list
	return list;
}

// function to return all elements between k1 and k2
// in a lisnked list
LinkedList *BST::allElementsBetween(int k1, int k2)
{
	// k1 should be less or equal to that k2
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

// print the kth lasgest element
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
		if (cur->sizeOfRightSubtree + 1 == k) // cur element is kth largest
			return cur->val;

		if (cur->sizeOfRightSubtree + 1 < k) // kth largest is in the left subtree
		{
			k -= (cur->sizeOfRightSubtree + 1);
			cur = cur->left;
		}
		else // kth largest is in the right subtree
			cur = cur->right;
	}
	return INT_MIN;
}

// function to print the bst
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

	if (node == root) // add the label and root in the dot file
	{
		fout << "label = \" rooted at " << node->val << " \";\n";
		fout << node->val << " [root = true]\n";
	}

	if (!node->lthread) // if left child exits
	{
		fout << node->val << " -> " << node->left->val << "\n";
		printTreeHelper(node->left, fout);
	}
	else
	{
		if (node->left)
			fout << node->val << " -> " << node->left->val << "[style = dotted];\n";
	}

	if (!node->rthread) // if right child exists
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

void BST::split(int k)
{
	// find the two trees
	DoubleTrees *dt = splitHelper(root, k);
	// root = dt->left;

	// print the inorder of first tree
	Node *cur = dt->left;
	cout << "InOrder of T1: ";
	if (cur)
	{
		while (cur->left)
			cur = cur->left;

		while (cur)
		{
			cout << cur->val << " ";
			cur = successor(cur);
		}
	}
	cout << endl;

	// print the inorder of second tree
	cur = dt->right;
	cout << "InOrder of T2: ";
	if (cur)
	{
		while (cur->left)
			cur = cur->left;

		while (cur)
		{
			cout << cur->val << " ";
			cur = successor(cur);
		}
	}
	cout << endl;
}

// split a bst in two parts T1 and T2
DoubleTrees *BST::splitHelper(Node *node, int k)
{
	DoubleTrees *dt = new DoubleTrees();
	;

	if (!node)
		return dt;

	// if node->val <= k all elements in left subtree including root
	// will be in T1 therefore, recurse for the right subtree
	if (node->val <= k)
	{
		dt->left = node;
		if (!node->rthread)
		{
			DoubleTrees *dt1 = splitHelper(node->right, k);
			dt->left->right = dt1->left;
			dt->right = dt1->right;
		}
		else
		{
			DoubleTrees *dt1 = splitHelper(NULL, k);
			dt->left->right = dt1->left;
			dt->right = dt1->right;
		}
	}

	// if node->val > k all elements in right subtree including root
	// will be in T2 therefore, recurse for the left subtree
	else
	{
		dt->right = node;
		if (!node->lthread)
		{
			DoubleTrees *dt1 = splitHelper(node->left, k);
			dt->right->left = dt1->right;
			dt->left = dt1->left;
		}
		else
		{
			DoubleTrees *dt1 = splitHelper(NULL, k);
			dt->right->left = dt1->right;
			dt->left = dt1->left;
		}
	}
	return dt;
}

int main()
{
	BST *bst = new BST();
	int arr[] = {50, 30, 100, 5, 40, 90, 105, 0, 10, 35, 45, 85, 95};

	int c = 0;

	while (c != -1)
	{
		LinkedList *list;
		int k, k1, k2, num;
		cout << "1. Insert(x)\n2. Search(x)\n3. Delete(x)\n4. ReverseInorder\n5. Split(k)\n6. AllElementsBetween(k1, k2)\n7. KhElement\n8. Print Tree\n9. Exit\n";
		cin >> c;
		switch (c)
		{
		case 1:
			cout << "Enter a number: ";
			cin >> num;
			try
			{
				bst->insert(num);
			}
			catch (string e)
			{
				cout << e << "\n";
			}
			break;

		case 2:
			cout << "Enter a number: ";
			cin >> num;
			if (bst->search(num))
				cout << "Number is present in BST.";
			else
				cout << "Number is not present in BST.";
			break;

		case 3:
			cout << "Enter a number: ";
			cin >> num;
			try
			{
				bst->deleteKey(num);
			}
			catch (string e)
			{
				cout << e << "\n";
			}
			break;

		case 4:
			list = bst->reverseInorder();
			list->printList();
			break;

		case 5:
			cout << "Enter a number: ";
			cin >> k;
			// BST *newBST = bst;
			bst->split(k);
			break;

		case 6:
			cout << "Enter K1 and K2: ";
			cin >> k1 >> k2;
			list = bst->allElementsBetween(k1, k2);
			list->printList();
			break;

		case 7:
			cout << "Enter a number: ";
			cin >> k;
			bst->kthElement(k);
			break;

		case 8:
			bst->printTree();
			break;

		case 9:
			c = -1;
			break;

		default:
			cout << "Invalid Choice!\n Try again.";
			break;
		}
	}

	// for (int num : arr)
	// {
	// 	bst->insert(num);
	// }

	// BST bst2 = bst;

	// bst2.deleteKey(50);
	// bst->printTree();

	// bst->split(11);

	// bst->printTree();

	// for (int i = 0; i < 13; ++i)
	// {
	// 	bst->kthElement(i + 1);
	// }

	// LinkedList *list = bst->allElementsBetween(1, 100);
	// list->printList();
}
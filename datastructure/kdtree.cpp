/**
 * A C++ program to demonstrate operations of KD tree
 * geeksforgeeks.org/search-and-insertion-in-k-dimensional-tree
*/

#include <iostream>

const int k = 2;

// A structure to represent node of kd tree
struct Node
{
	int point[k];	// to store k dimensional point
	Node* left;
	Node* right;
};

// A method to create a node of KD tree
Node* new_node(int arr[])
{
	Node* temp = new Node;
	for (int i = 0; i < k; i++)
		temp->point[i] = arr[i];	
	
	temp->left = temp->right = NULL;
	return temp;
}

// Insert a new node and returns root of modified tree
// the parameter depth is used to decide axis of comparison
Node* insert_rec(Node* root, int point[], unsigned depth)
{
	// tree is empty?
	if (root == NULL)
		return new_node(point);
	
	// calculate current dimension (cd) of comparision
	unsigned cd = depth % k;

	// compare the new point with root on current dimension 'cd'
	// and decide the left or right
	if (point[cd] < (root->point[cd]))
		root->left = insert_rec(root->left, point, depth + 1);
	else
		root->right = insert_rec(root->right, point, depth + 1);

	return root;
}

// Function to insert a new point with given point in KD tree and return new root
// It mainly uses above recursive function "insert_rec"
Node* insert(Node* root, int point[])
{
	return insert_rec(root, point, 0);
}

// A utility method to determine if two points are same in K dimensional space
bool are_points_same(int point1[], int point2[])
{
	// compare individual pointinate values
	for (int i = 0; i < k; ++i)
		if (point1[i] != point2[i])
			return false;
	return true;
}

// Searches a point represented by "point[]" in the KD tree
// The parameter depth is used to determine current axis
bool search_rec(Node* root, int point[], unsigned depth)
{
	// base cases
	if (root == NULL)
		return false;
	if (are_points_same(root->point, point))
		return true;
	// current dimension is computed using current depth and total dimensions (k)
	unsigned cd = depth % k;

	// compare point with root with respect to cd (current dimension)
	if (point[cd] < root->point[cd])
		return search_rec(root->left, point, depth + 1);
	
	return search_rec(root->right, point, depth + 1);
}

// Search a point in the KD tree. It mainly uses search_rec()
bool search(Node* root, int point[])
{
	// pass current depth as 0
	return search_rec(root, point, 0);
}

// Driver program to test above functions
int main()
{
	Node* root = NULL;
	int points[][k] = {{3, 6}, {17, 15}, {13, 15}, {6,12}, {9, 1}, {2, 7}, {10, 19}};

	int n = sizeof(points) / sizeof(points[0]);

	for (int i = 0; i < n; i++)
		root = insert(root, points[i]);
	
	int point1[] = {10, 19};
	(search(root, point1)) ? std::cout << "Found\n" : std::cout << "Not found\n";

	int point2[] = {12, 19};
	(search(root, point2)) ? std::cout << "Found\n" : std::cout << "Not found\n";

	return 0;
}

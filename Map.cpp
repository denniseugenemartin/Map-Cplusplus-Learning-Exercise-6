#include <iostream>
#include <exception>
#include <vector>
#include <algorithm>


using namespace std;


// class Node takes a type K or key, and a type T or value. pointers leftChild
// and rightChild will point to keys inserted into the map that are less than
// the node or greater than the node, respectively.
template < typename K, typename T>
struct Node 
{
	T value;
	K key;
	Node<K, T>* leftChild = NULL;
	Node<K, T>* rightChild = NULL;

public:

	// destructor for Node class calls destructor on children as well, set pointers
	// to NULL if only single deletion is desired.
	~Node <K, T>()
	{

		if (leftChild)
		{
			delete leftChild;
		}
		if (rightChild)
		{
			delete rightChild;
		}
	}

	// default constructor for Node
	Node <K, T>()
	{
		leftChild = NULL;
		rightChild = NULL;
		value = T();
		key = K();
	}
	
	// copy constructor for Node class copies key and value pair, but not any pointers.
	Node<K, T>(const Node& nodeToBeCopied)
	{
		value = nodeToBeCopied.value;
		key = nodeToBeCopied.key;
	}

	// assignment operator for node copies key and value pair, but not any pointers.
	Node<K, T>* operator =(const Node& nodeToBeCopied)
	{
		key = nodeToBeCopied->key;
		value = nodeToBeCopied->value;
	}
};

// Map class consists of a collection of nodes.  The first node inserted into the
// map will be the root. 
template < typename K, typename T>
class Map
{
	// pointer for the root of the tree.
	Node<K, T>* root;

	// node pointer for use in member functions
	Node<K, T>* currentNode = root;

	// function called by map copy constructor that copys a given node and proceeds to
	// recursively copy child nodes until it has no more valid nodes into the target tree
	// using the put function.
	void copy(Node<K, T>* nodeToCopy)
	{
		put(nodeToCopy->key, nodeToCopy->value);
		if (nodeToCopy->leftChild)
		{
			copy(nodeToCopy->leftChild);
		}
		if (nodeToCopy->rightChild)
			copy(nodeToCopy->rightChild);
	}
	// findNodeLocation returns a pointer to a Node pointer. 
	Node<K, T>** findNodeLocation(const K& key)
	{
		//  start with the root node.
		currentNode = root;

		// loop will continue as long as the node being pointed at is not NULL (ie it exists).
		// if the map is empty, then this loop will not trigger.
		while (currentNode)
		{
			// if the key given is less than the current node's key then we will follow
			// the leftChild path. if there is not a node in this location we will return
			// the address of the leftChild pointer instead.
			if (key < currentNode->key)
			{
				if (!currentNode->leftChild)
				{
					return &currentNode->leftChild;

				}
				else
				{
					currentNode = currentNode->leftChild;
				}
			}

			// if the key given is greater than the current node's key then we will follow
			// the rightChild path. if there is not a node in this location we will return
			// the address of the rightChild pointer instead.
			else if (key > currentNode->key)
			{
				if (!currentNode->rightChild)
				{
					return &currentNode->rightChild;

				}
				else
				{
					currentNode = currentNode->rightChild;
				}
			}

			// if we reach here, the key given is equivalent to the current node's key.
			// we will return the address of the current node.
			else
			{
				return &currentNode;
			}
		}

		// if we reach here, the map is empty so we return a reference to the root pointer.
		return &root;
	}

	// function used to pass nodes into a vector for printing. used by printSorted function.
	// goes through list and pushes each node into the list unsorted following child pointers.
	void passNodeToVector(Node<K, T>* nodeToEnter, vector <Node<K, T>*>& vectorOfNodes)
	{
		vectorOfNodes.push_back(nodeToEnter);
		if (nodeToEnter->leftChild)
		{
			passNodeToVector(nodeToEnter->leftChild, vectorOfNodes);
		}
		if (nodeToEnter->rightChild)
		{
			passNodeToVector(nodeToEnter->rightChild, vectorOfNodes);
		}
	}

	// function sorts vector nodes by key value. used by printSorted function.
	void sortVector(vector<Node<K, T>*>& vectorOfNodes)
	{

		int currentPosition;

		//the vector starts sorting from the second element, comparing elements to the previous in sequence.
		for (int i = 1; i < vectorOfNodes.size(); i++)
		{
			//currentValue will hold the current value of the Node.  
			currentNode = vectorOfNodes[i];

			// currentPosition will hold the current position in the vector.
			currentPosition = i;

			//as long as we don't reach the first element in the vector and the
			// previous element in the vector is greater than the current
			//we will go through this loop. Essentially it keeps moving forward through
			//the vector until it reaches a value that is lower than
			//the previous value. When it gets such a value it will keep moving back
			// through the vector shifting each previous element forward one
			//until it finds the correct place in the vector and places the element there.  
			while (currentPosition > 0 && vectorOfNodes[currentPosition - 1]->key > currentNode->key)
			{
				vectorOfNodes[currentPosition] = vectorOfNodes[currentPosition - 1];
				currentPosition--;
			}
			vectorOfNodes[currentPosition] = currentNode;
		}
	}
public:
		
	// default constructor for Map class.
	Map<K, T>()
	{
		root = NULL;
	}
	
	// destructor for Map starts with the root (if one exists) and node destructor calls
	// destructor on every child throughout the tree until all nodes are deleted.
	~Map<K, T>()
	{
		if (root)
		{
			delete root;
		}

	}

	// copy constructor for Map class. if there is a valid root node in map to be copied,
	// it calls the copy function to copy each node in the tree following child nodes until
	// none are left.
	Map<K,T>(const Map& mapToBeCopied)
	{
		if (mapToBeCopied.root)
		{
			copy(mapToBeCopied.root);
		}
		else
		{
			return;
		}
	}

	// checks to see if a node exists. If it does it return the node's value, if
	// not it inserts a node with that key and a blank value.
	T& operator[](const K& key)
	{
		Node<K, T>** nodeToFind = findNodeLocation(key);
		if (!*nodeToFind)
		{
			put(key, T());
		}
		else
		{
			Node<K, T>* nodeToReturn = *nodeToFind;
			return nodeToReturn->value;
		}
	}

	// overloads the assignment operator to first clear the left Map, and then call the
	// copy function if there is a valid root node in map to be copied,
	// it calls the copy function to copy each node in the tree following child nodes until
	// none are left.
	Map<K, T>* operator =(const Map& mapToBeCopied)
	{
		clear();
		if (mapToBeCopied.root)
		{
			copy(mapToBeCopied.root);
		}
		return this;
	}
	// put function will either insert a new node into the map (if the key given is
	// unique) or it will change the value of a node that corresponds to a given key
	// if it is already present in the map.
	void put(const K& key, const T& value)
	{
		currentNode = root;

		// call the findNodeLocation function. this will search the map for a node with
		// the same key as the given key. if it finds an existing node, it will return
		// it's address. if there is no such node, then it will return the adress of 
		// where it would be located in the map instead.
		Node<K,T>** nodeToFind = findNodeLocation(key);

		// check to see if the node pointer returned is NULL.  if it is NULL, then no
		// node is present in the map with the given key.
		if (!*nodeToFind)
		{

			// create a new node and fill the value and key properties with the given
			// key/value pair.
			Node<K,T>* nodeToBeInserted = new Node<K,T>;
			nodeToBeInserted->value = value;
			nodeToBeInserted->key = key;
			
			// if the root is NULL, the map is empty so we will point the root pointer
			// at the new node.
			if (!root)
			{
				root = nodeToBeInserted;
			}

			// if the root is not NULL, dereference the nodeToFind pointer so that we 
			// get the pointer that it is pointing to (it will be the leftChild or 
			// rightChild pointer of some node in the map) and change that pointer from
			// NULL to point at the new node.
			else
			{
				*nodeToFind = nodeToBeInserted;
			}
		}

		// if we reach here, there is already a node in the map with the given key. we
		// will not create a node, but we will dereference the nodeToFind pointer to get
		// a pointer to the node with that key value.  then change the value property of
		// this node.
		else
		{
			currentNode = *nodeToFind;
			currentNode->value = value;
		}
	}

	// function looks to see if the map contains a given key. if it does, it returns
	// true, otherwise it returns false.
	bool contains(const K& key)
	{
		// call the findNodeLocation function and get a double pointer to where that node
		// is (or should be).  then dereference the nodeToFind pointer and check if that
		// pointer is pointing at a valid node or if it is pointing at NULL.
		Node<K,T>** nodeToFind = findNodeLocation(key);
		if (!*nodeToFind)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	// function gets a value from the node with the given key using the findNodeLocation
	// function.  If no such node exists, throws exception, otherwise returns node's
	// value.
	T& get(const K& key)
	{
		Node<K,T>** nodeToFind = findNodeLocation(key);
		if (!*nodeToFind)
		{
			throw runtime_error("No such key exists!");
		}
		else
		{
			Node<K,T>* nodeToReturn = *nodeToFind;
			return nodeToReturn->value;
		}
	}

	// checks to see if a node exists. If it does it return the node's value, if
	// not it inserts a node with that key and the passed value.
	T& get(const K& key, const T defaultValue)
	{
		Node<K, T>** nodeToFind = findNodeLocation(key);
		if (!*nodeToFind)
		{
			put(key,defaultValue);
		}
		else
		{
			Node<K, T>* nodeToReturn = *nodeToFind;
			return nodeToReturn->value;
		}
	}


	// function clears the value field of a given node by using findNodeLocation and
	// given key value to access node.
	void erase(const K& key)
	{
		Node<K, T>** nodeToFind = findNodeLocation(key);
		if (!*nodeToFind)
		{
			return;
		}
		else
		{
			Node<K, T>* nodeToErase = *nodeToFind;
			nodeToErase->value = T();
		}
	}

	// clear function empties the map by calling delete on root if it exists which
	// will recursively call delete on any child nodes until the tree is empty.
	void clear()
	{
		if (root)
		{
			delete root;
		}
	}

	// prints a list of nodes in the map sorted by key value.
	void printSorted()
	{
		// create a new vector of the same type of nodes as the map.
		vector<Node<K, T>*> vectorOfNodes;

		// if there is a root put it into the vector through the passNodeToVector function.
		// this function will recursively add all other nodes to the vector through child
		// pointers.
		if (root)
		{
		passNodeToVector(root, vectorOfNodes);
		}

		// call sortVector function which will sort the vector of nodes by their key value.
		sortVector(vectorOfNodes);

		// iterate through the sorted vector of nodes printing them in (key, value) pairs.
		for (int i = 0; i < vectorOfNodes.size(); ++i)
		{
			cout << "(" << vectorOfNodes[i]->key
				<< ", " << vectorOfNodes[i]->value
				<< ") ";
		}
		cout << endl;
		
	}
};

int main()
{
	Map <int, string> map1;
	map1.put(5, "Angus");
	map1.put(2, "Balthazar");
	map1.put(3, "Zachary");
	
	map1.put(4, "Yippy");
	map1.put(1, "Xaviar");
	map1.put(4, "Waggy");
	map1.put(6, "Viktor");
	
	map1.printSorted();
}

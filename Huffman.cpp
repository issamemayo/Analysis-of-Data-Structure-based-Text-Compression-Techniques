// Below is a program demonstrating the Huffman Coding lossless Data Compression Technique 
#include <bits/stdc++.h>
using namespace std;

//To define the node components of the Huffman Tree
struct HuffmanNode {

	// Data field for each character in the text input
	char data;

	// Data field for the number of occurences (frequency) for each char in text input
	unsigned freq;

	// Left and right child pointers for each node
	HuffmanNode *left, *right;

	HuffmanNode(char data, unsigned freq)

	{

		left = right = NULL;
		this->data = data;
		this->freq = freq;
	}
};

// Comparator function for the inbuilt sort() function to arrange HuffmanNodes according to their frequencies
struct compare {

	bool operator()(HuffmanNode* l, HuffmanNode* r)
    
	{
		return (l->freq > r->freq);
	}
};

// To print character and frequency of each HuffmanNode
void printCodes(struct HuffmanNode* root, string str)
{

	if (!root)
		return;

	if (root->data != '$')//
		cout << root->data << ": " << str << "\n";

	printCodes(root->left, str + "0");
	printCodes(root->right, str + "1");
}

// The main function that builds a Huffman Tree and print codes by calling the printCodes() function
void HuffmanCodes(char data[], int freq[], int size)
{
	struct HuffmanNode *left, *right, *top;

	// Create a min heap of HuffmanNodes & inserts all characters of data[]
	priority_queue<HuffmanNode*, vector<HuffmanNode*>,
				compare>
		minHeap;

	for (int i = 0; i < size; ++i)
		minHeap.push(new HuffmanNode(data[i], freq[i]));

	/* Iterate while size of heap doesn't become 1 -> which will
     represent root of the Huffman Tree */
    
	while (minHeap.size() != 1) {

		// Extract the two minimum freq items from min heap as left and right nodes
		left = minHeap.top();
		minHeap.pop();

		right = minHeap.top();
		minHeap.pop();

		/*Create a new internal node with frequency equal to the sum of the two nodes frequencies. Make the
		 two extracted node as left and right children
		 of this new node. Add this node
		 to the min heap and use '$' for all
         internal nodes.*/
		top = new HuffmanNode('$',
							left->freq + right->freq);

		top->left = left;
		top->right = right;

		minHeap.push(top);
	}

	//calling printCodes() function
	printCodes(minHeap.top(), "");
}

int main()
{

	char arr[] = { 'a', 'b', 'c', 'd', 'e', 'f' };
	int freq[] = { 5, 9, 12, 13, 16, 45 };

	int size = sizeof(arr) / sizeof(arr[0]);

	HuffmanCodes(arr, freq, size);

	return 0;
}


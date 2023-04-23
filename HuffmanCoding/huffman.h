#include <bits/stdc++.h>
#include<time.h>
using namespace std;
map<char,string>mp2;

//To define the node components of the Huffman Tree
struct HuffmanNode {

	// Data field for each character in the text input
	char data;
    //Data field representing code afterapplying algorithm
    string code;

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

priority_queue<HuffmanNode*, vector<HuffmanNode*>,
				compare> globalHeap;

// To print character and frequency of each HuffmanNode
void printCodes(struct HuffmanNode* root, string str,map<char,string>&mp2)
{

	if (!root)
		return;
    

	if (root->data != '$'){
		cout << root->data << ": " << str << "\n";
        root->code=str;
        mp2[root->data]=root->code;

    }


	printCodes(root->left, str + "0",mp2);
	printCodes(root->right, str + "1",mp2);
}

void printOccurences(map<char,int>mp){
	for(auto it:mp){
		cout<<it.first<<" : "<<it.second<<endl;
	}
	cout<<endl<<endl;
}

// The main function that builds a Huffman Tree and print codes by calling the printCodes() function
HuffmanNode* HuffmanCodes(map<char,int>mp)
{
	struct HuffmanNode *left, *right, *top;

	// Create a min heap of HuffmanNodes & inserts all characters of data[]
	priority_queue<HuffmanNode*, vector<HuffmanNode*>,
				compare>
		minHeap;

	for (auto it:mp){
		minHeap.push(new HuffmanNode(it.first, it.second));
    }

    globalHeap=minHeap;

        

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
    HuffmanNode* root = minHeap.top();
	//calling printCodes() function
	printCodes(root, "",mp2);
    return root;
}

void charFrequency(map<char,int>&mp,string text){
    //To find the frequency of each character in the text file
	for(int i=0;i<text.size();i++){
		if(mp.find(text[i])==mp.end()){
			mp[text[i]]=1;
		}
		else{
			mp[text[i]]+=1;
		}
	}
}

int binToDec(string inStr){
    int res = 0;
    for (auto c : inStr) {
        res = res * 2 + c - '0';
    }
    return res;
}

string decToBin(int inNum){
    string temp = "", res = "";
    while (inNum > 0) {
        temp += (inNum % 2 + '0');
        inNum /= 2;
    }
    res.append(8 - temp.length(), '0');//appends 0 to the remaining string to form a whole number of bytes
    for (int i = temp.length() - 1; i >= 0; i--) {
        res += temp[i];
    }
    return res;
}

void saveEncodedFile(HuffmanNode* root,map<char,string>mp2) {
    //Saving encoded (.huf) file
    fstream inFile,outFile;
    inFile.open("text.txt", ios::in);
    outFile.open("compressed.txt", ios::out | ios::binary);
    string in = "";
    string s = "";
    char id;

    //Saving the meta data (huffman tree)
    in += (char)globalHeap.size();
    priority_queue <HuffmanNode*, vector<HuffmanNode*>, compare> tempPQ(globalHeap);
    while (tempPQ.empty()) {
        HuffmanNode* curr = tempPQ.top();
        in += curr->data;
        //Saving 16 decimal values representing code of curr->data
        s.assign(127 - curr->code.length(), '0');
        s += '1';
        s += curr->code;
        //Saving decimal values of every 8-bit binary code 
        in += (char)binToDec(s.substr(0, 8));
        for (int i = 0; i < 15; i++) {
            s = s.substr(8);
            in += (char)binToDec(s.substr(0, 8));
        }
        tempPQ.pop();
    }
    s.clear();

    //Saving codes of every character appearing in the input file
    inFile.get(id);
    while (!inFile.eof()) {
        s += mp2[id];
        //Saving decimal values of every 8-bit binary code
        while (s.length() > 8) {
            in += (char)binToDec(s.substr(0, 8));
            s = s.substr(8);
        }
        inFile.get(id);
    }
    

    //Finally if bits remaining are less than 8, append 0's
    int count = 8 - s.length();
	if (s.length() < 8) {
		s.append(count, '0');
	}
	in += (char)binToDec(s);	
    //append count of appended 0's
    in += (char)count;

    //write the in string to the output file
	outFile.write(in.c_str(), in.size());
	inFile.close();
	outFile.close();
}

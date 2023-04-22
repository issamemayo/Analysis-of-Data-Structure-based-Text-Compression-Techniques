// Below is a program demonstrating the Huffman Coding lossless Data Compression Technique 
#include <bits/stdc++.h>
#include "huffman.h"



int main()
{
	string text;

	//Analysing text from text.txt file to encode it using Huffman Coding
	ifstream myfile ("text.txt");
	if(myfile.is_open()) { // to check whether the file is open
		ostringstream ss;
        ss << myfile.rdbuf(); // reading data
        text = ss.str();
	} 

	map<char,int>mp;
	
	charFrequency(mp,text);// calculating frequency of each character
	printOccurences(mp);// displaying char and number of occurences
	HuffmanNode* root=HuffmanCodes(mp);//displaying the binary format of the characters acc to Huffman Coding
	saveEncodedFile(root,mp2);

	return 0;
}


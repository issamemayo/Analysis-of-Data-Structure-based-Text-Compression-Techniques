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

	clock_t t =clock();	
	charFrequency(mp,text);// calculating frequency of each character
	printOccurences(mp);// displaying char and number of occurences
	HuffmanNode* root=HuffmanCodes(mp);//displaying the binary format of the characters acc to Huffman Coding
	saveEncodedFile(root,mp2);// Generating the binary file
	t=clock()-t;

	double time_taken=((double)t)/CLOCKS_PER_SEC;
	printf("Compression Time: %f seconds",time_taken);

	return 0;
}


#include <bits/stdc++.h>
#include<time.h>
using namespace std;

int val_total=0;
int val_found=0;
vector<int> encoding(string s1)
{
	cout << "Encoding\n";
	unordered_map<string, int> table;
	for (int i = 0; i <= 255; i++) {
		string ch = "";
		ch += char(i);
		table[ch] = i;
	}

	string p = "", c = "";
	p += s1[0];
	int code = 256;
	vector<int> output_code;
	cout << "String\tOutput_Code\tAddition\n";
	for (int i = 0; i < s1.length(); i++) {
		if (i != s1.length() - 1)
			c += s1[i + 1];
		if (table.find(p + c) != table.end()) {
			p = p + c;
			val_found++;
		}
		else {
			cout << p << "\t" << table[p] << "\t\t"
				<< p + c << "\t" << code << endl;
			output_code.push_back(table[p]);
			table[p + c] = code;
			code++;
			p = c;

		}
		c = "";
		val_total++;
	}
	cout << p << "\t" << table[p] << endl;
	
	
	output_code.push_back(table[p]);
	return output_code;
}

void decoding(vector<int> op)
{
	cout << "\nDecoding\n";
	unordered_map<int, string> table;
	for (int i = 0; i <= 255; i++) {
		string ch = "";
		ch += char(i);
		table[i] = ch;
	}
	int old = op[0], n;
	string s = table[old];
	string c = "";
	c += s[0];
	cout << s;
	int count = 256;
	for (int i = 0; i < op.size() - 1; i++) {
		n = op[i + 1];
		if (table.find(n) == table.end()) {
			s = table[old];
			s = s + c;
		}
		else {
			s = table[n];
		}
		cout << s;
		c = "";
		c += s[0];
		table[count] = table[old] + c;
		count++;
		old = n;
	}
}
int main()
{
	
    string s;
    ifstream myfile ("text.txt");
	if(myfile.is_open()) { // to check whether the file is open
		ostringstream ss;
        ss << myfile.rdbuf(); // reading data
        s = ss.str();
	} 
	
	clock_t t = clock();
	vector<int> output_code = encoding(s);
	t= clock()-t;
	cout << "Output Codes are: ";
	for (int i = 0; i < output_code.size(); i++) {
		cout << output_code[i] << " ";
	}
	double time_taken=((double)t)/CLOCKS_PER_SEC;
	printf("Compression Time: %f seconds ",time_taken);
	cout<<"Number of output codes used = "<<val_total-val_found+1;

	cout << endl;
	//decoding(output_code);
    return 0;
}

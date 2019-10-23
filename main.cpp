/*
Environment:
Programming language used: C++
IDE: Dev-C++ 5.11, Compiler: TDM-GCC 4.9.2 64-bit Release
OS: Windows 10 Pro Build 1809 64 bit
CPU: Intel Core 4th generation i7-4790, RAM: 16GB DDR3-1600
*/

#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>

using namespace std;

struct node {
	int occurance;
	char representative;
	node *left_child;
	node *right_child;
};

struct codes {
	char c;
	string code;
};

typedef struct codes Codes;

bool compare_node(node *a, node *b) {
	if(a->occurance == b->occurance) {
		return a->representative < b->representative;
	} else {
		return a->occurance < b->occurance;
	}
}

bool compare_encodes(codes a, codes b) {
	return a.c < b.c;
}

void calculate_code(node * root, string temp_code, string prev_code, vector<codes> &encode) {
	if(root->left_child != NULL) {
		prev_code = temp_code;
		temp_code += "0";
		calculate_code(root->left_child, temp_code, prev_code, encode);
		temp_code = prev_code; // reset
	}
	if(root->right_child != NULL) {
		prev_code = temp_code;
		temp_code += "1";
		calculate_code(root->right_child, temp_code, prev_code, encode);
		temp_code = prev_code; // reset
	}
	if(root->left_child == NULL && root->right_child == NULL) {
		Codes temp;
		temp.c = root->representative;
		temp.code = temp_code;
		encode.push_back(temp);
		temp_code = prev_code; // reset
	}
}

int main(int argc, char *argv[]) {
	
	ifstream input_file(argv[1]);
	
	vector<node *> forest; // for each character of "message"
	
	// store input file into string "message"
	string message;
	if(input_file.is_open()) {
		getline(input_file, message);
		input_file.close();
	}
	
	//insert each character into the vector "forest" and checked if already in forest or not
	for(int i = 0; i < message.length(); i++) {
		
		char temp_c = message[i];
		bool repeated = false;
		
		vector<node *>::iterator itr;
		for(itr = forest.begin(); itr != forest.end(); itr++) { // checked repeated in forest
			if((*itr)->representative == temp_c) {
				(*itr)->occurance++; // increase occurance only
				repeated = true;
				break;
			}
		}
		
		if(!repeated) { // not in forest
			node * temp_node = new node;
			temp_node->representative = temp_c;
			temp_node->occurance = 1;
			temp_node->left_child = NULL;
			temp_node->right_child = NULL;
			
			forest.push_back(temp_node);
		}
	}
	
	int size = forest.size();
	sort(forest.begin(), forest.end(), compare_node); // sort the forest
	
	// combine each node on the forest
	for(int i = 0; i < size - 1; i++) {
		node * temp_node = new node;
		temp_node->occurance = (*forest.begin())->occurance + (*(forest.begin()+1))->occurance;
		if((*forest.begin())->representative < (*(forest.begin()+1))->representative) {
			temp_node->representative = (*forest.begin())->representative;
			temp_node->left_child = (*forest.begin());
			temp_node->right_child = (*(forest.begin()+1));
		} else {
			temp_node->representative = (*(forest.begin()+1))->representative;
			temp_node->left_child = (*(forest.begin()+1));
			temp_node->right_child = (*forest.begin());
		}

        forest.push_back(temp_node);       
        forest.erase(forest.begin(),forest.begin()+2);
        
		sort(forest.begin(), forest.end(), compare_node); // sort the forest
	}

	// process code of each character
	node * root = *(forest.begin());
	string temp_code, prev_code;
	vector<codes> encodes;
	calculate_code(root, temp_code, prev_code, encodes);
	sort(encodes.begin(), encodes.end(), compare_encodes);
	
	vector<codes>::iterator itr;
	
	// output code (Part 1)
	ofstream output_code("code.txt");
	
	for(itr = encodes.begin(); itr != encodes.end(); itr++) {
		if(itr->c == ' ')
			output_code << "Space: " << itr->code << "\n";
		else
			output_code << itr->c << ": " << itr->code << "\n";	
	}

	// output encoded message
	ofstream output_encodemsg("encodemsg.txt");
	string encoded_message;
	for(int i = 0; i < message.length(); i++) {
		char target = message[i];
		itr = encodes.begin();
		while(itr->c != target)
			itr++;
		encoded_message += itr->code;
	}
	
	// output code (Part 2)
	output_code.precision(3);
	output_code << "Ave = " << (float)encoded_message.length()/message.length() << " bits per symbol";
	output_code.close();
	
	for(int i = 0; i < encoded_message.length()/80; i++) {
		encoded_message.insert((1+i)*80+i, "\n");
	}
	
	output_encodemsg << encoded_message;
	output_encodemsg.close();
	
	return 0;
}

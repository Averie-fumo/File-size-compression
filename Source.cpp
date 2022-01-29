// saleha muzammil
//20L-1192
// BSCS 3B
// assignment 6
#include <iostream>
#include <queue>
#include <fstream>
#include <string>
using namespace std;

// node class
class node
{
private:
	char ch;
	int freq;
	string code;
	node* left;
	node* right;
	friend class BST;
public:
	node()
	{
		ch = '\0';
		freq = 0;
		code = "";
		left = nullptr;
		right = nullptr;
	}
	char getch()
	{
		return ch;
	}
	int getfreq() const
	{
		return freq;
	}
	string getcode()
	{
		return code;
	}
	node(char ch, int freq)
	{
		left = nullptr;
		right = nullptr;
		code = "";
		this->ch = ch;
		this->freq = freq;
	}
};

//compare class for the priority queue
class compare
{
public:
	int operator() (const node* n1, const node* n2)
	{
		return n1->getfreq() > n2->getfreq();
	}
};

// bst class 
class BST
{
private:
	node* root;
public:
	//constructor 
	BST()
	{
		root = nullptr;
	}
	// insert function 


	void writeInFile(string fileName)
	{
		fstream fin;
		if (root == nullptr)
			return;

		fin.open(fileName);

		if (!fin.is_open())
		{
			cout << fileName << " could not be opened." << endl;
			return;
		}

		queue<node*> q;
		q.push(root);
		while (!q.empty())
		{
			node* curr = q.front();
			if (curr->ch == '\0')
				fin << '*' << " ";
			else
				fin << curr->ch << " ";
			fin << curr->freq << " ";
			if (curr == root)
				fin << "123" << " ";
			else
				fin << curr->code << " ";


			q.pop();

			if (curr->left != nullptr)
				q.push(curr->left);
			if (curr->right != nullptr)
				q.push(curr->right);
		}

		fin.close();
	}

	void insertFromFile(string filename)
	{
		char chr;
		string s;
		int freqi;
		Delete(root);

		fstream fin;
		fin.open(filename);

		if (!fin.is_open())
		{
			cout << filename << " could not be opened. " << endl;
			return;
		}
		else
		{
			while (!fin.eof())
			{
				fin >> chr;
				fin >> freqi;
				fin >> s;

				if (chr == '*')
					chr = '\0';

				if (s == "123")
				{
					root = new node;
					root->ch = chr;
					root->code = "";
					root->freq = freqi;
				}
				else
				{
					insertFromFile(chr, s, freqi, 0, root);
				}
			}
		}
	}
	void insertFromFile(char chr, string s, int freqi, int i, node*& curr)
	{
		if (!curr)
		{
			curr = new node;
			curr->ch = chr;
			curr->code = s;
			curr->freq = freqi;
			curr->left = nullptr;
			curr->right = nullptr;
			return;
		}
		if (i >= s.size())
			return;
		if (s[i] == '0')
		{
			insertFromFile(chr, s, freqi, i + 1, curr->left);
		}
		if (s[i] == '1')
		{
			insertFromFile(chr, s, freqi, i + 1, curr->right);

		}
	}
	void calcFreqInsert(char ch, int freq)
	{
		calcFreqInsert(ch, freq, root);
	}
	void calcFreqInsert(char ch, int freq, node*& curr)
	{
		node* parent = nullptr;
		node* check = search(ch, root, parent);
		// if data exist don't enter
		if (check)
		{
			check->freq += freq;
			return;
		}
		// if root is null
		if (curr == nullptr)
		{
			curr = new node;
			curr->ch = ch;
			curr->freq = freq;
			curr->left = nullptr;
			curr->right = nullptr;
		}
		// inserting in the BST
		else if (curr->ch == ch)
			return;
		else if (ch < curr->ch)
			calcFreqInsert(ch, freq, curr->left);
		else if (ch > curr->ch)
			calcFreqInsert(ch, freq, curr->right);
		return;
	}
	// search function
	node* search(char ch, node* curr, node*& parent)
	{
		if (!curr)
			return NULL;
		if (curr->ch == ch)
		{
			return curr;
		}
		else if (!curr->left && !curr->right)
			return NULL;
		else if (ch < curr->ch)
		{
			parent = curr;
			return search(ch, curr->left, parent);
		}
		else if (ch > curr->ch)
		{
			parent = curr;
			return search(ch, curr->right, parent);
		}
		else
			return NULL;
	}
	//function to fill the priority queue
	void populate_heap(priority_queue<node*, vector <node*>, compare>& minHeap)
	{
		if (!root)
			return;
		populate_heap(minHeap, root);
	}
	void populate_heap(priority_queue<node*, vector<node*>, compare>& minHeap, node* curr)
	{
		if (!curr)
			return;
		populate_heap(minHeap, curr->left);
		minHeap.push(curr);
		populate_heap(minHeap, curr->right);

	}
	//compression function
	void compression_func(priority_queue<node*, vector<node*>, compare>& minHeap)
	{
		while (minHeap.size() != 1)
		{
			node* n1 = minHeap.top();
			if (n1->ch != '\0')
			{
				n1->left = nullptr;
				n1->right = nullptr;
				delete n1->right;
				delete n1->left;
			}
			minHeap.pop();
			node* n2 = minHeap.top();
			if (n2->ch != '\0')
			{
				n2->left = nullptr;
				n2->right = nullptr;
				delete n2->right;
				delete n2->left;
			}
			minHeap.pop();
			node* newnode = new node;
			newnode->ch = '\0';
			newnode->freq = n1->freq + n2->freq;

			if (n1->freq < n2->freq)
			{
				newnode->left = n1;
				newnode->right = n2;
			}
			else
			{
				newnode->left = n2;
				newnode->right = n1;
			}

			minHeap.push(newnode);
		}

		Delete(root);
		root = minHeap.top();
		minHeap.pop();
	}
	// decompression function
	void decompression_func(string binary_code, fstream& decompress)
	{
		if (binary_code.size() == 0)
			return;
		node* curr = root;
		int i = 0;
		while (i < binary_code.size() - 1)
		{

			if (curr->ch != '\0')
			{
				decompress << curr->ch;
				curr = root;
			}
			if (binary_code[i] == '0')
			{
				curr = curr->left;
			}
			else if (binary_code[i] == '1')
			{
				curr = curr->right;
			}
			if (curr->ch != '\0')
			{
				decompress << curr->ch;
				curr = root;
			}
			i++;
		}
	}
	// assign_code function assigns binary codes to characters
	void assign_codes(node* array)
	{
		string code = "";
		int i = 0;
		assign_codes(root, code, array, i);
	}
	void assign_codes(node* curr, string code, node* array, int& i)
	{
		if (!curr)
			return;
		curr->code = code;
		if (curr->ch != '\0')
		{
			array[i] = *curr;
			i++;
		}
		assign_codes(curr->left, code + '0', array, i);
		assign_codes(curr->right, code + '1', array, i);
	}
	// delete function to delete all the nodes in the tree
	void Delete(node* curr)
	{
		if (!curr)
			return;
		Delete(curr->left);
		Delete(curr->right);
		curr = nullptr;
		delete curr;
	}
	~BST()
	{
		Delete(root);
	}
};

void compression_func(string s, int& count, int* array)
{
	for (int i = 0;i < s.size() / 8;i++)
	{
		array[i] = 0;
	}

	ofstream fin;
	fin.open("comp.bin");
	unsigned char ch;

	if (fin.is_open())
		for (int i = 0, track = 0, j; i < s.size();)
		{
			ch = 0x00;
			j = 0;
			for (;j < 8 && i < s.size();j++, i++)
			{
				ch = ch << 1;
				if (s[i] == '1')
					ch = ch | 1;
			}
			while (j < 8)
			{
				ch = ch << 1;
				count++;
				j++;
			}
			if (ch > 6 && ch < 32)
			{
				ch = ch + 40;
				array[track] = 1;
			}
			fin << ch;
			track++;
		}

	fin.close();
}
string decompression_func(int count, int* array)
{
	ifstream fin;
	fin.open("comp.bin");
	char a;
	string encoded = "", binary_code = "", spare = "";
	if (fin.is_open())
	{
		while (!fin.eof())
		{
			fin.get(a);

			if (a != -52)
				spare += a;

		}
	}
	fin.close();

	if (spare.size() == 0)
		return "";

	for (int i = 0; i < spare.size() - 1;i++)
		encoded += spare[i];

	unsigned char ch;
	int code[8];
	for (int i = 0, track = 0;i < encoded.size(); i++)
	{
		ch = encoded[i];
		if (array[track] == 1)
			ch = ch - 40;
		for (int j = 0;j < 8;j++)
		{
			code[7 - j] = (ch >> j) & 1;
		}

		for (int j = 0;j < 8;j++)
			binary_code += to_string(code[j]);
		track++;
	}
	string binary = "";

	for (int i = 0;i < binary_code.size() - count; i++)
	{
		binary += binary_code[i];
	}
	binary_code = binary;
	return binary_code;
}

int main()
{
	fstream compress, decompress;
	string cfile, dfile;
	string all_codes = "";
	cout << "Enter file name (to be compressed):" << endl;
	cin >> cfile;
	compress.open(cfile);

	if (!compress.is_open())
		cout << cfile << " could not be opened." << endl;
	else
	{
		// compression starts here 
		priority_queue< node*, vector<node*>, compare> minHeap;
		BST tree;
		char ch1, ch2;
		while (!compress.eof())
		{
			compress.get(ch1);
			tree.calcFreqInsert(ch1, 1);
		}
		// clsoing the file
		compress.clear();
		compress.close();
		// inserting values in priority queue 
		tree.populate_heap(minHeap);
		int size = minHeap.size();

		tree.compression_func(minHeap);
		node* code_array = new node[size];
		tree.assign_codes(code_array);

		decompress.open("comp.bin");
		if (!decompress.is_open())
		{
			cout << "Compression file could not be opened." << endl;
		}

		else
		{
			compress.open(cfile);
			while (!compress.eof())
			{
				compress.get(ch1);
				for (int i = 0;i < size;i++)
				{
					if (code_array[i].getch() == ch1)
					{
						all_codes += code_array[i].getcode();
					}
				}
			}
			int count = 0;
			cout << "The compressed version is in comp.bin " << endl;

			int* array = new int[all_codes.size()];
			compression_func(all_codes, count, array);
			compress.close();
			decompress.close();
			delete[] code_array;
			code_array = nullptr;

			tree.writeInFile("store.bin");
			cout << "Tree stored in store.bin ." << endl;

			dfile = "comp.bin";
			string binary_code = "";
			decompress.open(dfile);
			if (!decompress.is_open())
				cout << dfile << " could not be opened." << endl;
			else
			{
				while (!decompress.eof())
					decompress >> binary_code;
				decompress.close();

				compress.open("decomp.txt");
				if (!compress.is_open())
					cout << "decomp.txt could not be opened." << endl;
				else
				{
					string s = decompression_func(count, array);
					array = nullptr;
					delete[] array;
					tree.decompression_func(s, compress);
					compress.close();
					cout << "File decompressed in decomp.txt ." << endl;
				}

			}
		}
	}
	return 0;
}
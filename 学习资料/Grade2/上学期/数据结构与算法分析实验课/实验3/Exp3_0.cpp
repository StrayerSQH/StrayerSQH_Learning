#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

class TreeNode {
private:
	int value;
	TreeNode* leftChild = NULL; 
	TreeNode* rightChild = NULL;
public:
	TreeNode() {};

	TreeNode(int v) :value(v) {};

	int getValue()
	{ 
		return value; 
	}

	void setLeftChild(TreeNode* child) 
	{ 
		leftChild = child; 
	}

	void setRightChild(TreeNode* child) 
	{ 
		rightChild = child; 
	}

	TreeNode* getLeftChild()
	{ 
		return leftChild; 
	}

	TreeNode* getRightChild() 
	{ 
		return rightChild; 
	}
};

class Tree {
private:
	string filename, txt;
public:
	Tree() {};

	void ReadFile()
	{
		cout << "输入需要压缩的文件名: ";
		cin >> filename; 

		ifstream file(filename); 

		if (file.is_open()) 
		{
			stringstream buffer;
			buffer << file.rdbuf();
			txt = buffer.str(); 
			file.close(); 
		}
		else {
			cout << "无法打开该文件！ " << filename << endl;
		}
	}

	void CharacterMap()
	{
		cout << "各字符出现的次数统计为：" << endl;
		using CharCountPair = pair<char, int>;
		unordered_map<char, int> charCount;
		for (char ch : txt)
		{
			charCount[ch]++;
		}
		vector<CharCountPair> vec(charCount.begin(), charCount.end());
		sort(vec.begin(), vec.end(), [](const CharCountPair& a, const CharCountPair& b){
			return a.second < b.second || (a.second == b.second && a.first < b.first);
			});

		for (const auto& pair : vec) 
		{
			if (pair.first == '\n')
			{
				cout << "字符'" << "\\n" << "'出现了 " << pair.second << " 次。" << endl;
			}
			else
			{
				cout << "字符'" << pair.first << "'出现了 " << pair.second << " 次。" << endl;
			}
		}
	}
};

int main()
{
	Tree huffman;
	huffman.ReadFile();
	huffman.CharacterMap();
	return 0;
}

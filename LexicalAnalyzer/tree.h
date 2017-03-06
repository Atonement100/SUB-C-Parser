#pragma once
#include "token.h"

class BinaryTreeNode {
protected: 
	Token token;
	BinaryTreeNode* left = nullptr, right = nullptr;
public:
	BinaryTreeNode* getChild() { return this->left; }
	BinaryTreeNode* getSibling() { return this->right; }
	Token getToken() { return this->token; }

	BinaryTreeNode(Token token) {
		this->token = token;
	}
};

class LCRSTree {
protected:
	BinaryTreeNode* root;
public:
	LCRSTree(BinaryTreeNode* root);
	BinaryTreeNode* getRoot() { return this->root; }
};
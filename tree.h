#pragma once
#include "token.h"
#include <iostream>

class BinaryTreeNode {
protected: 
	Token token = Token("invalid", TokenType::INVALID);
	BinaryTreeNode *left = nullptr, *right = nullptr;
public:
	BinaryTreeNode* getChild() { return this->left; }
	BinaryTreeNode* getSibling() { return this->right; }
	Token getToken() { return this->token; }
	std::string getTokenText() { return this->token.getText(); }

	void PrintNode() {
		int numChildren = 0;
		BinaryTreeNode* child = this->getChild();

		while (child) {
			child = child->getSibling();
			numChildren++;
		}

		std::cout << token.getText() << "(" << numChildren << ")" << std::endl;
	}

	void PrintLeadingDots(int numDots) {
		while (numDots > 0) {
			numDots--;
			std::cout << ". ";
		}
	}

	void LeftToRightPreorder(int depth) {
		if (this) { 
			this->PrintLeadingDots(depth); 
			this->PrintNode(); 
		}
		if (this->left) { this->left->LeftToRightPreorder(depth+1); } 
		if (this->right) { this->right->LeftToRightPreorder(depth); }
	}

	BinaryTreeNode() {}

	BinaryTreeNode(Token &token) {
		this->token = token;
	}

	BinaryTreeNode* AddChild(BinaryTreeNode* newChild) {
		if (this->left) {
			return nullptr;
		}
		else {
			this->left = newChild;
			return this->left;
		}
	}

	BinaryTreeNode* AddSibling(BinaryTreeNode* newSibling) {
		if (this->right) {
			return nullptr;
		}
		else {
			this->right = newSibling;
			return this->right;
		}
	}
};

class LCRSTree {
protected:
	BinaryTreeNode* root;
public:
	LCRSTree(Token rootToken);
	BinaryTreeNode* getRoot() { return this->root; }
	void PrintTree();
};
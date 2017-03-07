#include "tree.h"

LCRSTree::LCRSTree(Token rootToken) {
	this->root = new BinaryTreeNode(rootToken);
}

void LCRSTree::PrintTree() {
	this->root->LeftToRightPreorder(0);
}
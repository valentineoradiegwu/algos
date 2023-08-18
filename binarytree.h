#pragma once
#include <memory>
struct BinaryTree
{
	int data;
	std::shared_ptr<BinaryTree> left;
	std::shared_ptr<BinaryTree> right;
};

struct BinarySearchTree
{
	BinarySearchTree(int data, std::shared_ptr<BinarySearchTree> left, std::shared_ptr<BinarySearchTree> right)
		:data{data},
		left{left},
		right{right}
	{}
	int data;
	std::shared_ptr<BinarySearchTree> left;
	std::shared_ptr<BinarySearchTree> right;
};

struct BinaryTreeP
{
	int data;
	std::shared_ptr<BinaryTreeP> parent;
	std::shared_ptr<BinaryTreeP> left;
	std::shared_ptr<BinaryTreeP> right;
};

#pragma once
#include<iostream>

template<typename T>
struct Node {
	T data;
	Node* left;
	Node* right;
};

template<typename T>
class Tree {
private:
	Node<T> * root;
	void copyTree(const Tree<T>& t);
	void copy(Node<T>*&, const Node<T>*) const; // po princip e const Node*&
	void deleteTree(Node<T>*&) const;
	void pr(Node<T>*) const;
	void createTree(Node<T>*&) const;
public:
	Tree() {
		root = nullptr;
	}
	~Tree() {
		deleteTree(root);
	}
	Tree(const Tree<T>& r) {
		copyTree(r);
	}
	Tree operator=(const Tree<T>& r) {
		if (this != &r) {
			deleteTree(root);
			copyTree(r);
		}
		return *this;
	}
	bool empty() const;
	Tree<T> leftTree() const;
	Tree<T> rightTree() const;
	void createTr(const T& x, const Tree<T>& l, const Tree<T>& r);
	Node<T>* getRoot() const;
	T rootTree() const;
	void print() const;
	void create();
};
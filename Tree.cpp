#include<iostream>
#include<assert.h>
#include"Tree.h"

template<typename T>
void Tree<T>::copy(Node<T>*& pos, const Node<T>* r) const {
	pos = nullptr;
	if (r) {
		pos = new Node<T>;
		assert(pos != nullptr);
		pos->data = r->data;
		copy(pos->left, r->left);
		copy(pos->right, r->right);
	}
}

template<typename T>
void Tree<T>::copyTree(const Tree<T>& t) {
	copy(this->root, t.root);
}

template<typename T>
void Tree<T>::deleteTree(Node<T>*& p) const {
	if (p) {
		deleteTree(p->left);
		deleteTree(p->right);
		delete p;
		p = nullptr;
	}
}

template<typename T>
void Tree<T>::pr(Node<T>* p) const {
	if (p) {
		pr(p->left);
		pr(p->right);
		std::cout << p->data << " ";
		//pr(p->right);
	}
}

template<typename T>
void Tree<T>::createTree(Node<T>*& pos) const {
	T x;
	char c;
	std::cout << "root: "; std::cin >> x;
	pos = new Node<T>; assert(pos != nullptr);
	pos->data = x; pos->left = nullptr; pos->right = nullptr;
	std::cout << "Enter y/n for LEFT Node: "; std::cin >> c;
	if (c == 'y') createTree(pos->left);
	std::cout << "Enter y/n for RIGHT Node: "; std::cin >> c;
	if (c == 'y') createTree(pos->right);
}

template<typename T>
bool Tree<T>::empty() const {
	return root == nullptr;
}

template<typename T>
Tree<T> Tree<T>::leftTree() const {
	Tree<T> t;
	copy(t.root, root->left);
	return t;
}

template<typename T>
Tree<T> Tree<T>::rightTree() const {
	Tree<T> t;
	copy(t.root, root->right);
	return t;
}

template<typename T>
void Tree<T>::createTr(const T& x, const Tree<T>& l, const Tree<T>& r) {
	if (root) deleteTree(root);
	root = new Node<T>; assert(root != nullptr);
	root->data = x;
	copy(root->left, l.root);
	copy(root->right, r.root);
}

template<typename T>
Node<T>* Tree<T>::getRoot() const {
	return root;
}

template<typename T>
T Tree<T>::rootTree() const {
	return root->data;
}

template<typename T>
void Tree<T>::create() {
	createTree(root);
}

template<typename T>
void Tree<T>::print() const {
	pr(root);
	std::cout << std::endl;
}
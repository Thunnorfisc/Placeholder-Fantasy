#pragma once
#include <iostream>
using namespace std;

template<class T>
class Stack
{
private:
	struct Node
	{
		T item;
		Node* next;
	};

	Node* topNode;
	size_t size;

public:
	Stack();
	~Stack();
	bool push(T item);
	bool pop();
	void clearStack();
	T getTop();
	bool isEmpty();
};

template<class T> Stack<T>::Stack()
{
	topNode = NULL;
	size = 0;
}

template<class T> Stack<T>::~Stack()
{
	for (size_t i = 0; i < size; i++)
	{
		pop();
	}
}

template<class T> bool Stack<T>::push(T item)
{
	Node* node = new Node;
	node->item = item;
	node->next = NULL;

	node->next = topNode;
	topNode = node;
	size++;
	return true;
}

template<class T> bool Stack<T>::pop()
{
	if (!isEmpty())
	{
		Node* temp = topNode;
		topNode = topNode->next;
		temp->next = NULL;
		delete temp;
		size--;
		return true;
	}
	return false;
}

template<class T> void Stack<T>::clearStack()
{
	for (size_t i = 0; i < size; i++)
	{
		pop();
	}
}

template<class T> T Stack<T>::getTop()
{
	if (!isEmpty())
	{
		return topNode->item;
	}
}

template<class T> bool Stack<T>::isEmpty()
{
	return size == 0;
}

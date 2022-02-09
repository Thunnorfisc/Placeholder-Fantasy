//============================================================================
//  Module:             Gameplay Programming
//  Assignment 2:       PlaceHolder Fantasy?
//  Student Name:       William Wibisana Dumanauw
//  Student Number:     S10195561A
//============================================================================
#ifndef _STACK_HPP             // prevent multiple definitions if this 
#define _STACK_HPP             // ..file is included in more than one place
#pragma once
#include <iostream>
using namespace std;

// Templates cannot be used in implementation files. HPP is used instead of H to differentiate file usage.
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
	// Constructor
	Stack();
	// Destructor
	~Stack();
	// Push item to top of stack
	bool push(T item);
	// Remove top of stack item
	bool pop();
	// Clear the entire stack
	void clearStack();
	// Get the top of the stack
	T getTop();
	// Check if stack is empty
	bool isEmpty();
};
#endif

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
	// Declare node and put item as node's item
	Node* node = new Node;
	node->item = item;
	node->next = NULL;
	// Point node next to previous top node
	node->next = topNode;
	// Make node top node
	topNode = node;
	// Increment size
	size++;
	return true;
}

template<class T> bool Stack<T>::pop()
{
	if (!isEmpty())
	{
		// Declare temp node
		Node* temp = topNode;
		// Set top node to be the next node in stack
		topNode = topNode->next;
		// Remove linkage and delete temp node
		temp->next = NULL;
		delete temp;
		// Decrement size
		size--;
		return true;
	}
	return false;
}

template<class T> void Stack<T>::clearStack()
{
	// Pop till stack is empty
	for (size_t i = 0; i < size; i++)
	{
		pop();
	}
}

template<class T> T Stack<T>::getTop()
{
	if (!isEmpty())
	{
		// Return top node's item
		return topNode->item;
	}
}

template<class T> bool Stack<T>::isEmpty()
{
	// If size is 0, return true. Otherwise, false.
	return size == 0;
}


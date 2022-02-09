//============================================================================<<
//  Module:             Gameplay Programming
//  Assignment 2:       PlaceHolder Fantasy?
//  Student Name:       William Wibisana Dumanauw
//  Student Number:     S10195561A
//============================================================================<>
#ifndef _POSTOFFICE_H
#define	_POSTOFFICE_H
#define WIN32_LEAN_AND_MEAN
#include "eventQueue.h"
#include "mail.h"

// THREAD SAFETY BOISSS
class PostOffice
{
private:
	EventQueue eventQueue;                // Event Queue
public:
	PostOffice();
	~PostOffice();

	void dispatchMessages();               // Event Queue Message Dispatcher
	void addMessages(Mail& mail);          // Event Queue Message Adder
};

#endif

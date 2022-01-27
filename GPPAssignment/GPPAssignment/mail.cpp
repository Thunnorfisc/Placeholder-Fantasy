#include "mail.h"

Mail::Mail(): sender(nullptr), receiver(nullptr), type((mailTypes)0), message(nullptr) 
{ };

Mail::Mail(MailSender& sender, MailReceiver& receiver, const mailTypes type, const void* msg):
	sender(&sender), receiver(&receiver), type(type), message(msg)
{ };

Mail::~Mail() { };

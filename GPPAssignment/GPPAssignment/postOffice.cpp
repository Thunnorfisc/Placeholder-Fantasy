#include "postOffice.h"

PostOffice::PostOffice() {  };
PostOffice::~PostOffice() {  };

//====>Event Queue<=====//
void PostOffice::dispatchMessages()
{
    while (!eventQueue.isEmpty())
    {
        // Get the front message
        Mail mail = eventQueue.dequeue();

        // Get receiver
        MailReceiver* receiver = mail.receiver;
        if (receiver)
        {
            receiver->onMessage(mail);
        }
    }
}

void PostOffice::addMessages(Mail& mail)
{
    eventQueue.enqueue(mail);
}
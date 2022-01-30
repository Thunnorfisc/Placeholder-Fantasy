#ifndef _MAIL_H
#define _MAIL_H
#define WIN32_LEAN_AND_MEAN

enum mailTypes { PlaySoundEvent, StopSoundEvent };

class MailSender;
class MailReceiver;

struct Mail {
	MailSender* const sender;     // Sender of message
	MailReceiver* const receiver; // Receiver of message
	const mailTypes type;
	const void* message;

	Mail();
	Mail(MailSender&, MailReceiver&, const mailTypes, const void*);
	~Mail();
};

class MailSender {};
class MailReceiver
{
public:
	virtual void onMessage(const Mail& mail) = 0;
};
#endif
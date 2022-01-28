#ifndef _AUDIO_H             // Prevent multiple definitions if this 
#define _AUDIO_H             // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

// Windows Media Foundation API
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfuuid")
// ^ KEEP THESE ON TOP, MMREG.H WILL SCOLD ME OTHERWISE
 
// Microsoft includes
#include <wrl.h>

// XAudio includes
#include <xaudio2.h>
#pragma comment(lib, "xaudio2.lib")
#include "mail.h"
#include "audioEngine.h"
#include "gameerror.h"
#include "constants.h"
#include <vector>

class AudioEngine;

class Sound
{
private:
	IXAudio2SourceVoice* sourceVoice;	// the XAudio2 source voice
	WAVEFORMATEX* waveFormat;			// the format of the audio file
	unsigned int waveLength;			// the length of the wave
	std::vector<BYTE> audioData;		// the audio data
	XAUDIO2_BUFFER audioBuffer;			// the actual buffer with the audio data

	float fallof;						// falloff distance
	unsigned int priority;				// music priority

	unsigned int index;					// the index of the actual sound to play

public:
	Sound();
	~Sound();

	friend class Audio;
};


// Feeder and message handler
class Audio: public MailReceiver
{
private:
	AudioEngine* audioEngine;          // XAudio2 Audio Engine

public:
	Audio();
	~Audio();

	void loadFile(const char* filename, Sound& sound);

	void playSound(const Sound& sound);
	void stopSound(const Sound& sound);

	void onMessage(const Mail& mail);

};
#endif

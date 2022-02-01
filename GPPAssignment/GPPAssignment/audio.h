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

enum audioTypes { Music, Sfx };
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
	AudioEngine* audioEngine;            // XAudio2 Audio Engine

	// A submix is a track that combines audio signals routed to it from specific audio tracks or track sends in the same sequence
	IXAudio2SubmixVoice* sfxSubmix;      // Collection of all SFX
	IXAudio2SubmixVoice* musicSubmix;    // Collection of all music files

	// Music type
	XAUDIO2_SEND_DESCRIPTOR sendMusic;
	XAUDIO2_VOICE_SENDS musicSendList;

	// Sound Effects type
	XAUDIO2_SEND_DESCRIPTOR sendSfx;
	XAUDIO2_VOICE_SENDS sfxSendList;

	// Volume
	float sfxVolume = 1.0f;
	float musicVolume = 1.0f;

public:
	Audio();
	~Audio();

	void loadFile(const char* filename, Sound& sound, const audioTypes& audioType);

	void playSound(const Sound& sound);
	void stopSound(const Sound& sound);

	void setVolume(const audioTypes& audioType, const float volume);
	const float getVolume(const audioTypes& audioType);

	void loadVolume();

	void onMessage(const Mail& mail);

};
#endif

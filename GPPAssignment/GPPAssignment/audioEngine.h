#ifndef _AUDIOENGINE_H             // Prevent multiple definitions if this 
#define _AUDIOENGINE_H             // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

// Windows Media Foundation API
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfuuid")
// ^ KEEP THESE ON TOP, MMREG.H WILL SCOLD ME OTHERWISE


#include <wrl.h>     // Windows Runtime Template Library

#include <XAudio2.h> // XAudio 2 Library
#pragma comment(lib, "xaudio2.lib")

#include <vector>
#include <string>
#include "audio.h"

class AudioEngine
{
private:
	// Audio Device for XAudio2 Engine
	Microsoft::WRL::ComPtr<IXAudio2> audioDevice;
	// Mastering Voice
	IXAudio2MasteringVoice* masterVoice;
	// Windows Media Foundation Source Reader Configuration
	Microsoft::WRL::ComPtr<IMFAttributes> sourceReaderConfig;

public:
	AudioEngine();
	~AudioEngine();

	// Initialize Audio
	void initialize();

	// Load Audio from file
	void loadAudio(const char* filename, std::vector<BYTE>& audioData, WAVEFORMATEX** waveFormat, unsigned int& waveLength);

	friend class Audio;
};
#endif

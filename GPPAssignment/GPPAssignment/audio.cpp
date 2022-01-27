#include "audio.h"
#include <stdexcept>

Sound::Sound() : sourceVoice(nullptr), waveFormat(nullptr)
{
	audioData.clear();
	// The RtlZeroMemory routine fills a block of memory with zeros.
	ZeroMemory(&audioBuffer, sizeof(XAUDIO2_BUFFER));
}

Sound::~Sound()
{
	// Dlear the buffers
	audioData.clear();
	ZeroMemory(&audioBuffer, sizeof(XAUDIO2_BUFFER));

	// Delete the wave format
	if (waveFormat)
		SAFE_DELETE(waveFormat);

	// Delete the source voice
	if (sourceVoice)
	{
		sourceVoice->DestroyVoice();
		sourceVoice;
	}
}


Audio::Audio()
{
	audioEngine = new AudioEngine();
}

Audio::~Audio()
{
	if (audioEngine)
	{
		SAFE_DELETE(audioEngine);
	}
}

void Audio::loadFile(const char* filename, Sound& sound)
{
	// Handle errors
	HRESULT hr = S_OK;
	audioEngine->loadAudio(filename, sound.audioData, &sound.waveFormat, sound.waveLength);

	// Create source voice
	hr = audioEngine->audioDevice->CreateSourceVoice(&sound.sourceVoice, sound.waveFormat);
	if (FAILED(hr))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to create source voice!"));

	ZeroMemory(&sound.audioBuffer, sizeof(XAUDIO2_BUFFER));
	sound.audioBuffer.AudioBytes = (UINT32)sound.audioData.size();
	sound.audioBuffer.pAudioData = (BYTE* const)&sound.audioData[0];
	sound.audioBuffer.pContext = nullptr;
}

void Audio::onMessage(const Mail& mail)
{
	// Play the sound
	if (mail.type == mailTypes::PlaySoundEvent)
	{
		// Error Handling
		HRESULT hr = S_OK;

		// Submit the audio buffer to the source voice
		hr = ((Sound*)mail.message)->sourceVoice->SubmitSourceBuffer(&((Sound*)mail.message)->audioBuffer);
		if (FAILED(hr))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to submit audio buffer!"));

		// Start the source voice
		((Sound*)mail.message)->sourceVoice->Start();
	}

	// Stop the sound
	else if (mail.type == mailTypes::StopSoundEvent)
	{
		((Sound*)mail.message)->sourceVoice->Stop();
	}
}


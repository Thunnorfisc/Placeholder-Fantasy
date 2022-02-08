#include "audio.h"
#include <stdexcept>

Sound::Sound(const char* filename, audioTypes type) : filename(filename), type(type), sourceVoice(nullptr), waveFormat(nullptr)
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

Stream::Stream(const char* filename, const bool loop, const audioTypes type) : filename(filename), loop(loop), type(type)
{

}

Stream::~Stream() { };

Audio::Audio()
{
	audioEngine = new AudioEngine();
	// Error Handling
	HRESULT hr = S_OK;

	// Create submix voices
	// Second Param is number of input channels
	// 44.1 kHz is the standard used
	hr = audioEngine->audioDevice->CreateSubmixVoice(&musicSubmix, 1, 44100, 0, 0, 0, 0);
	if (FAILED(hr))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to create music submix voice!"));

	hr = audioEngine->audioDevice->CreateSubmixVoice(&sfxSubmix, 1, 44100, 0, 0, 0, 0);
	if (FAILED(hr))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to create sound effect submix voice!"));
	// The first flag, set to 0, means that we do not want to use a filter.
	// The second param specifies the submix voice the voices should be send to.

	sendSfx = { 0, sfxSubmix };
	sfxSendList = { 1, &sendSfx };
	sendMusic = { 0, musicSubmix };
	musicSendList = { 1, &sendMusic };

	musicSubmix->SetVolume(musicVolume);
	sfxSubmix->SetVolume(sfxVolume);
}

Audio::~Audio()
{
	ZeroMemory(&sfxSendList, sizeof(sfxSendList));
	ZeroMemory(&musicSendList, sizeof(musicSendList));

	ZeroMemory(&sendSfx, sizeof(sendSfx));
	ZeroMemory(&sendMusic, sizeof(sendMusic));

	musicSubmix->DestroyVoice();
	sfxSubmix->DestroyVoice();

	audioEngine->audioDevice->StopEngine();

	SAFE_DELETE(audioEngine);

	if (streamingThread->joinable())
		streamingThread->join();

	if (streamingThread)
		delete streamingThread;

}

void Audio::loadFile(Sound& sound)
{
	// Handle errors
	HRESULT hr = S_OK;
	audioEngine->loadAudio(sound.filename, sound.audioData, &sound.waveFormat, sound.waveLength);

	// Create source voice
	if (sound.type == audioTypes::Music)
		hr = audioEngine->audioDevice->CreateSourceVoice(&sound.sourceVoice, sound.waveFormat, 0, XAUDIO2_DEFAULT_FREQ_RATIO, nullptr, &musicSendList, NULL);
	else if (sound.type == audioTypes::Sfx)
		hr = audioEngine->audioDevice->CreateSourceVoice(&sound.sourceVoice, sound.waveFormat, 0, XAUDIO2_DEFAULT_FREQ_RATIO, nullptr, &sfxSendList, NULL);
	if (FAILED(hr))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to create source voice!"));

	ZeroMemory(&sound.audioBuffer, sizeof(XAUDIO2_BUFFER));
	sound.audioBuffer.AudioBytes = (UINT32)sound.audioData.size();
	sound.audioBuffer.pAudioData = (BYTE* const)&sound.audioData[0];
	sound.audioBuffer.pContext = nullptr;
}

void Audio::playSound(const Sound& sound)
{
	// Handle Error
	HRESULT hr = S_OK;
	hr = sound.sourceVoice->SubmitSourceBuffer(&sound.audioBuffer);
	if (FAILED(hr))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to submit source buffer!"));

	sound.sourceVoice->Start();
}

void Audio::stopSound(const Sound& sound)
{
	sound.sourceVoice->Stop();
}

void Audio::streamFile(const char* filename, const bool loop, const audioTypes type)
{
	audioEngine->streamStopped = false;
	if (type == audioTypes::Music)
		streamingThread = new std::thread(&AudioEngine::streamFile, audioEngine, filename, musicSendList, loop);
	else if (type == audioTypes::Sfx)
		streamingThread = new std::thread(&AudioEngine::streamFile, audioEngine, filename, sfxSendList, loop);
}

void Audio::endStream()
{
	audioEngine->streamStopped = true;
	// Check if active thread of execution
	if (streamingThread->joinable())
		streamingThread->join();
}



void Audio::setVolume(const audioTypes& audioType, const float volume)
{
	if (audioType == audioTypes::Music)
	{
		musicVolume = volume;
		musicSubmix->SetVolume(volume);
	}
	else if (audioType == audioTypes::Sfx)
	{
		sfxVolume = volume;
		sfxSubmix->SetVolume(volume);
	}
}

const float Audio::getVolume(const audioTypes& audioType)
{
	if (audioType == audioTypes::Music)
	{
		return musicVolume;
	}
	else if (audioType == audioTypes::Sfx)
	{
		return sfxVolume;
	}
	return 1.0f;
}

void Audio::loadVolume()
{
	
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

	else if (mail.type == mailTypes::BeginStream)
	{
		if (mail.message == nullptr)
		{
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Mail is empty!"));
		}
		streamFile(((Stream*)mail.message)->filename, ((Stream*)mail.message)->loop, ((Stream*)mail.message)->type);
	}
	else if (mail.type == mailTypes::EndStream)
	{
		endStream();
	}
}


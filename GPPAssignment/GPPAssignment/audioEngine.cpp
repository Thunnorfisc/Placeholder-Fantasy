#include "audioEngine.h"
#include "gameerror.h"

AudioEngine::AudioEngine(): audioDevice(NULL), masterVoice(NULL) 
{
	initialize();
}

AudioEngine::~AudioEngine()
{
	// Shut down Windows Media Foundation
	MFShutdown();

	// Destroy the master voice
	masterVoice->DestroyVoice();

	// Stop the audio device engine
	audioDevice->StopEngine();
}

//==>Initialization<==//
void AudioEngine::initialize()
{
	// For errors
	HRESULT hr = S_OK;

	hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(hr))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Cannot initialize COM library!"));

	// Start up Media Foundation
	hr = MFStartup(MF_VERSION);
	if (FAILED(hr))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Starting up Windows Media Foundation Failed!"));

	// Create MF Source Reader Configuration
	hr = MFCreateAttributes(sourceReaderConfig.GetAddressOf(), 1);
	if (FAILED(hr))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Creating Media Foundation Source Reader Configuration Failed!"));

	// Set up low latency for source reader
	hr = sourceReaderConfig->SetUINT32(MF_LOW_LATENCY, true);
	if (FAILED(hr))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to set Source Reader Configuration!"));

	// Create XAudio2 engine
	hr = XAudio2Create(audioDevice.GetAddressOf());
	if (FAILED(hr))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to create XAudio2 engine!"));

	// Create XAudio2 mastering device
	hr = audioDevice->CreateMasteringVoice(&masterVoice);
	if (FAILED(hr))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to create XAudio2 mastering voice!"));
}

//===> Load Audio <===//
// !!-> To be used by Audio! <-!! //
void AudioEngine::loadAudio(const char* filename, std::vector<BYTE>& audioData, WAVEFORMATEX** waveFormat, unsigned int& waveLength)
{
	// Check for Errors
	HRESULT hr = S_OK;
	// stream index
	DWORD streamIndex = (DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM;
	
	//====> Conversion from Char* to wstring <=====//
	size_t sizeOfChar = strlen(filename) + 1;

	// The following creates a buffer large enough to contain
	// the exact number of characters in the original string
	// in the new format. If you want to add more characters
	// to the end of the string, increase the value of newsize
	// to increase the size of the buffer.
	wchar_t* wcstring = new wchar_t[sizeOfChar];

	// Convert char* string to a wchar_t* string.
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, sizeOfChar, filename, _TRUNCATE);

	// Convert wchar_t* to wstring
	std::wstring wstr;
	wstr.assign(wcstring);
	//====> Conversion Finished <=====//

	// Create the source reader
	Microsoft::WRL::ComPtr<IMFSourceReader> sourceReader;
	hr = MFCreateSourceReaderFromURL(wstr.c_str(), sourceReaderConfig.Get(), sourceReader.GetAddressOf());
	if (FAILED(hr))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to create source reader from URL!"));

	// Select the first audio stream, and deselect all other streams
	hr = sourceReader->SetStreamSelection((DWORD)MF_SOURCE_READER_ALL_STREAMS, false);
	if (FAILED(hr))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to disable streams!"));

	hr = sourceReader->SetStreamSelection(streamIndex, true);
	if (FAILED(hr))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to enable first audio stream!"));

    // Query information aboout the media file
	Microsoft::WRL::ComPtr<IMFMediaType> nativeMediaType;
	hr = sourceReader->GetNativeMediaType(streamIndex, 0, nativeMediaType.GetAddressOf());
	if (FAILED(hr))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to query native media type!"));

	// Ensure the file requested is actually an audio file
	GUID majorType{}; // GUIDs are the Microsoft implementation of the distributed computing environment (DCE) universally unique identifier (UUID)
	hr = nativeMediaType->GetGUID(MF_MT_MAJOR_TYPE, &majorType);
	if (majorType != MFMediaType_Audio)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error: File requested is NOT an audio file!"));

	// Check for whether the audio file is compressed
	// MFAudioFormat_PCM = Uncompressed Pulse Code Modulation(PCM) audio
	GUID subType{};
	hr = nativeMediaType->GetGUID(MF_MT_SUBTYPE, &subType);
	if (!(subType == MFAudioFormat_Float || subType == MFAudioFormat_PCM))
	{
		// Audio is compressed, decompress audio
		// Tell source reader that audio must be uncompressed; 
		// Source reader will look for decoders to decompress audio
		Microsoft::WRL::ComPtr<IMFMediaType> partialType = nullptr;
		hr = MFCreateMediaType(partialType.GetAddressOf());
		if (FAILED(hr))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to create media type!"));

		// Set the media type to "audio"
		hr = partialType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
		if (FAILED(hr))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to set media type to audio!"));

		// Request uncompressed audio data
		hr = partialType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
		if (FAILED(hr))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to set GUID of media type to uncompressed!"));

		// Set the current media type to submit request to source reader
		hr = sourceReader->SetCurrentMediaType(streamIndex, NULL, partialType.Get());
		if (FAILED(hr))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to set current media type!"));
	}

	// Load it into an XAudio2 buffer
	Microsoft::WRL::ComPtr<IMFMediaType> uncompressedAudioType = nullptr;
	hr = sourceReader->GetCurrentMediaType(streamIndex, uncompressedAudioType.GetAddressOf());
	if (FAILED(hr))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to get current media type!"));

	// Create wave format;  XAudio2 natively works with audio files in the Resource Interchange File Format (RIFF)
	hr = MFCreateWaveFormatExFromMFMediaType(uncompressedAudioType.Get(), waveFormat, &waveLength);
	if (FAILED(hr))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to create wave format!"));

	// Ensure stream is selected
	hr = sourceReader->SetStreamSelection(streamIndex, true);
	if (FAILED(hr))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to select audio stream!"));

	// Copy data into byte vector
	Microsoft::WRL::ComPtr<IMFSample> sample = nullptr;
	Microsoft::WRL::ComPtr<IMFMediaBuffer> buffer = nullptr;
	BYTE* localAudioData = NULL;
	DWORD localAudioDataLength = 0;

	while (true)
	{
		DWORD flags = 0;
		hr = sourceReader->ReadSample(streamIndex, 0, nullptr, &flags, nullptr, sample.GetAddressOf());
		if (FAILED(hr))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to read audio sample!"));

		// Check whether the data is still valid
		if (flags & MF_SOURCE_READERF_CURRENTMEDIATYPECHANGED)
			break;

		// Check for end of stream
		if (flags & MF_SOURCE_READERF_ENDOFSTREAM)
			break;
		
		if (sample == nullptr)
			continue;

		// Convert data into contiguous buffer
		hr = sample->ConvertToContiguousBuffer(buffer.GetAddressOf());
		if (FAILED(hr))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to convert audio sample to contiguous buffer!"));

		// Lock the buffer and copy the data into local memory
		hr = buffer->Lock(&localAudioData, nullptr, &localAudioDataLength);
		if (FAILED(hr))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to lock the audio buffer!"));

		for (size_t i = 0; i < localAudioDataLength; i++)
		{
			audioData.push_back(localAudioData[i]);
		}

		// Unlock the buffer
		hr = buffer->Unlock();
		localAudioData = nullptr;
		if (FAILED(hr))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error has occured while unlocking audio buffer!"));
	}
}
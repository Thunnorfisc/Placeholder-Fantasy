#include "audioEngine.h"
#include "gameerror.h"

// Helper Function, convert char* to wstring
std::wstring convertCharArrayToWString(const char* charArray)
{
	//====> Conversion from Char* to wstring <=====//
	size_t sizeOfChar = strlen(charArray) + 1;

	// The following creates a buffer large enough to contain
	// the exact number of characters in the original string
	// in the new format. If you want to add more characters
	// to the end of the string, increase the value of newsize
	// to increase the size of the buffer.
	wchar_t* wcstring = new wchar_t[sizeOfChar];

	// Convert char* string to a wchar_t* string.
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, sizeOfChar, charArray, _TRUNCATE);

	// Convert wchar_t* to wstring
	std::wstring wstr;
	wstr.assign(wcstring);
	//====> Conversion Finished <=====//

	return wstr;
}

StreamingVoiceCallback::StreamingVoiceCallback() : hBufferEndEvent(CreateEventEx(nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE))
{ }

StreamingVoiceCallback::~StreamingVoiceCallback()
{
	CloseHandle(hBufferEndEvent);
}

SourceReaderCallback::SourceReaderCallback() : hReadSample(CreateEventEx(nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE)), 
endOfStream(false), status(S_OK)
{ }
SourceReaderCallback::~SourceReaderCallback()
{
	CloseHandle(hReadSample);
}

// Restart the callback
void SourceReaderCallback::restart()
{
	std::lock_guard<std::mutex> lock(guard);
	endOfStream = false;
	sample = nullptr;
}

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
	// Stream index
	DWORD streamIndex = (DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM;
	
	std::wstring wsFilename = convertCharArrayToWString(filename);

	// Create the source reader
	Microsoft::WRL::ComPtr<IMFSourceReader> sourceReader;

	// Set the source reader to synchronous mode
	hr = sourceReaderConfig->SetUnknown(MF_SOURCE_READER_ASYNC_CALLBACK, nullptr);
	if (FAILED(hr))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to set the source reader to synchronous mode!"));

	hr = MFCreateSourceReaderFromURL(wsFilename.c_str(), sourceReaderConfig.Get(), sourceReader.GetAddressOf());
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
	Microsoft::WRL::ComPtr <IMFSample> sample = nullptr;
	Microsoft::WRL::ComPtr <IMFMediaBuffer> buffer = nullptr;
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

//===> Create Async Reader <===//
// !!-> To be used by Stream! <-!! //
void AudioEngine::createAsyncReader(const char* filename, IMFSourceReader** sourceReader, WAVEFORMATEX* wfx, size_t wfxSize)
{
	// Handle Errors
	HRESULT hr = S_OK;

	// Set source reader to async mode
	hr = sourceReaderConfig->SetUnknown(MF_SOURCE_READER_ASYNC_CALLBACK, &sourceReaderCallback);
	if (FAILED(hr))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to set the source reader to async mode!"));

	// Stream index
	DWORD streamIndex = (DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM;

	std::wstring wsFilename = convertCharArrayToWString(filename);

	// Create the source reader
	hr = MFCreateSourceReaderFromURL(wsFilename.c_str(), sourceReaderConfig.Get(), sourceReader);
	if (FAILED(hr))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to create source reader from URL!"));

	// Select the first audio stream, and deselect all other streams
	hr = (*sourceReader)->SetStreamSelection((DWORD)MF_SOURCE_READER_ALL_STREAMS, false);
	if (FAILED(hr))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to disable streams!"));

	hr = (*sourceReader)->SetStreamSelection(streamIndex, true);
	if (FAILED(hr))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to enable first audio stream!"));

	// Query information aboout the media file
	Microsoft::WRL::ComPtr<IMFMediaType> nativeMediaType;
	hr = (*sourceReader)->GetNativeMediaType(streamIndex, 0, nativeMediaType.GetAddressOf());
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
		hr = (*sourceReader)->SetCurrentMediaType(streamIndex, NULL, partialType.Get());
		if (FAILED(hr))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to set current media type!"));
	}

	// Load it into an XAudio2 buffer
	Microsoft::WRL::ComPtr<IMFMediaType> uncompressedAudioType = nullptr;
	hr = (*sourceReader)->GetCurrentMediaType(streamIndex, uncompressedAudioType.GetAddressOf());
	if (FAILED(hr))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to get current media type!"));

	UINT32 waveFormatSize = 0;
	WAVEFORMATEX* waveFormat = nullptr;
	// Create wave format;  XAudio2 natively works with audio files in the Resource Interchange File Format (RIFF)
	hr = MFCreateWaveFormatExFromMFMediaType(uncompressedAudioType.Get(), &waveFormat, &waveFormatSize);
	if (FAILED(hr))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to create wave format!"));

	// Ensure stream is selected
	hr = (*sourceReader)->SetStreamSelection(streamIndex, true);
	if (FAILED(hr))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to select audio stream!"));

	// Copy data
	memcpy_s(wfx, wfxSize, waveFormat, waveFormatSize);
	CoTaskMemFree(waveFormat);
}

void AudioEngine::streamFile(const char* filename, XAUDIO2_VOICE_SENDS sendList, const bool loop)
{
	// Handle Errors
	HRESULT hr = S_OK;
	// Stream Index
	DWORD streamIndex = (DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM;

	// Create async source reader
	Microsoft::WRL::ComPtr<IMFSourceReader> sourceReader;
	WAVEFORMATEX waveFormat;
	createAsyncReader(filename, sourceReader.GetAddressOf(), &waveFormat, sizeof(waveFormat));

	// Create the source voice
	IXAudio2SourceVoice* sourceVoice;
	hr = audioDevice->CreateSourceVoice(&sourceVoice, &waveFormat, 0, XAUDIO2_DEFAULT_FREQ_RATIO, &streamVoiceCallback, &sendList, NULL);
	if (FAILED(hr))
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to create source voice for streaming!"));
	}
	sourceVoice->Start();

	// Create the loop
	loopStream(sourceReader.Get(), sourceVoice, loop);

	sourceReader->Flush(streamIndex);
	sourceVoice->DestroyVoice();
	sourceReader = nullptr;
}

void AudioEngine::loopStream(IMFSourceReader* const sourceReader, IXAudio2SourceVoice* const sourceVoice, const bool loop)
{
	// Handle Errors
	HRESULT hr = S_OK;
	// Stream Index
	DWORD streamIndex = (DWORD)MF_SOURCE_READER_FIRST_AUDIO_STREAM;
	// Current Stream Buffer
	DWORD currentStreamBuffer = 0;
	// Buffer size
	size_t bufferSize[maxBufferCount] = { 0 };
	// Pointer to array of buffers
	uint8_t* buffers[maxBufferCount];

	ZeroMemory(buffers, sizeof(uint8_t*) * maxBufferCount);

	// Infinite loop
	for (;;)
	{
		// If the stream is stopped, break
		if (streamStopped)
			break;

		// Read source sample
		hr = sourceReader->ReadSample(streamIndex, 0, nullptr, nullptr, nullptr, nullptr);
		if (FAILED(hr))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to read source samples!"));

		// Waits until hReadSample is in the signaled state. Infinite means it cannot time out.
		WaitForSingleObject(sourceReaderCallback.hReadSample, INFINITE);

		// If it is the end of the stream
		if (sourceReaderCallback.endOfStream)
		{
			if (loop)
			{
				// Restart the stream
				sourceReaderCallback.restart();
				// Set properties of object
				PROPVARIANT var = { 0 };
				// Type of the property is an 8-byte signed integer in the little-endian byte order format
				var.vt = VT_I8;
				// Reset the position of the source reader to the beginning of the audio file
				hr = sourceReader->SetCurrentPosition(GUID_NULL, var);
				if (SUCCEEDED(hr))
					continue;
				else
					throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to reset source reader position!"));
			}
			else
				break;
		}

		// Set media buffer to contiguous buffer
		Microsoft::WRL::ComPtr<IMFMediaBuffer> mediaBuffer;
		hr = sourceReaderCallback.sample->ConvertToContiguousBuffer(mediaBuffer.GetAddressOf());
		if (FAILED(hr))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to convert media buffer to contiguous buffer!"));

		BYTE* audioData = nullptr;
		DWORD sampleBufferLength = 0;

		// Lock the media buffer
		hr = mediaBuffer->Lock(&audioData, nullptr, &sampleBufferLength);
		if (FAILED(hr))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to lock the media buffer!"));

		// If the sample buffer length is larger than size of the current buffer
		if (bufferSize[currentStreamBuffer] < sampleBufferLength)
		{
			// Set buffer to new pointer, buffer size to sample buffer length
			buffers[currentStreamBuffer] = new uint8_t[sampleBufferLength];
			bufferSize[currentStreamBuffer] = sampleBufferLength;
		}

		memcpy_s(buffers[currentStreamBuffer], sampleBufferLength, audioData, sampleBufferLength);

		hr = mediaBuffer->Unlock();
		if (FAILED(hr))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to unlock media buffer!"));

		// Wait until XAudio2 has played enough data
		/*  We want to have only maxBufferCount - 1 buffers on the queue
			to make sure that there is always one free buffer for the Media Foundation streamer */
		XAUDIO2_VOICE_STATE state;
		for (;;)
		{
			sourceVoice->GetState(&state);
			if (state.BuffersQueued < maxBufferCount - 1)
				break;

			WaitForSingleObject(streamVoiceCallback.hBufferEndEvent, INFINITE);
		}

		XAUDIO2_BUFFER buffer = { 0 };
		buffer.AudioBytes = sampleBufferLength;
		buffer.pAudioData = buffers[currentStreamBuffer];
		sourceVoice->SubmitSourceBuffer(&buffer);

		// Example: 1/3 = 1, 2/3 = 2, 3/3 = 0(Reset), loop around
		currentStreamBuffer++;
		// current stream buffer modulo max buffer count
		currentStreamBuffer %= maxBufferCount;
	}
	if (FAILED(hr))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error: Unable to loop trhough the media stream!"));
	// For the love of memory management, thou shalt delete this pointer...
	for (size_t i = 0; i < maxBufferCount; i++)
	{
		SAFE_DELETE(buffers[i]);
	}
}
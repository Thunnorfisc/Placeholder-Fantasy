//============================================================================<<
//  Module:             Gameplay Programming
//  Assignment 2:       PlaceHolder Fantasy?
//  Student Name:       William Wibisana Dumanauw
//  Student Number:     S10195561A
//============================================================================<>
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
#include <mutex>

// Callback structure for XAudio2 voices
struct StreamingVoiceCallback : public IXAudio2VoiceCallback
{
	HANDLE hBufferEndEvent;
	// virtual void __stdcall OnVoiceProcessingPassStart(UINT32)
	STDMETHOD_(void, OnVoiceProcessingPassStart)(UINT32) override { };
	// virtual void __stdcall OnVoiceProcessingPassEnd()
	STDMETHOD_(void, OnVoiceProcessingPassEnd)() override { };
	// virtual void __stdcall OnStreamEnd()
	STDMETHOD_(void, OnStreamEnd)() override { };
	// virtual void* __stdcall OnBufferStart()
	STDMETHOD_(void, OnBufferStart)(void*) { };
	// virtual void* __stdcall OnBufferEnd()
	STDMETHOD_(void, OnBufferEnd)(void*) override { SetEvent(hBufferEndEvent); };
	// virtual void* __stdcall OnLoopEnd
	STDMETHOD_(void, OnLoopEnd)(void*) override { };
	// virtual void __stdcall OnVoiceError(void*, HRESULT)
	STDMETHOD_(void, OnVoiceError)(void*, HRESULT) override { };

	StreamingVoiceCallback();
	virtual ~StreamingVoiceCallback();
};

// Callback structure for the WMF Source Reader
struct SourceReaderCallback : public IMFSourceReaderCallback
{
	Microsoft::WRL::ComPtr<IMFSample> sample;  // The sample that was read in
	HANDLE hReadSample;                        // Handle to the read sample
	bool endOfStream;                          // Check if the end of the stream has been reached
	HRESULT status;                            // The status of the stream
	std::mutex guard;                          // Thread Safety

	/*If the interface that you are annotating is COM, use the COM form of these annotations. 
	Don't use the COM annotations with any other type interface.*/

	// Marcro: STDMETHOD(method) -> virtual HRESULT __stdcall method
	// typedef IID& REFIID, COM Output Pointer
	// A void ** is just a pointer to a pointer to memory with an unspecified type. (since you can't dereference a void*)
	// IID = The GUID you are specifying for the interface.
	STDMETHOD(QueryInterface) (REFIID iid, _COM_Outptr_ void** ppv) override
	{
		// Invalid Pointer
		if (!ppv)
			return E_POINTER;

		// Check if GUID of source reader has the same iid
		if (__uuidof(IMFSourceReaderCallback) == iid)
		{
			*ppv = this;
			return S_OK;
		}
		// No interface supported
		*ppv = nullptr;
		return E_NOINTERFACE;
	}
	// virtual ULONG __stdcall AddRef()
	STDMETHOD_(ULONG, AddRef)() override { return 1; }
	// virtual ULONG __stdcall Release()
	STDMETHOD_(ULONG, Release)() override { return 1; }
	// Called after IMFSourceReader::OnReadSample completes
	STDMETHOD(OnReadSample)(_In_ HRESULT hrStatus, _In_ DWORD dwStreamIndex, _In_ DWORD dwStreamFlags, 
		_In_ LONGLONG timestamp, _In_opt_ IMFSample* pSample) override
	{
		// For parameters that are unreferenced, avoid W4 warnings
		UNREFERENCED_PARAMETER(dwStreamIndex);
		UNREFERENCED_PARAMETER(timestamp);

		// Lock the thread
		std::lock_guard<std::mutex> lock(guard);

		if (SUCCEEDED(hrStatus))
		{
			if (pSample)
			{
				sample = pSample;
			}
		}

		if (dwStreamFlags & MF_SOURCE_READERF_ENDOFSTREAM)
		{
			endOfStream = true;
		}

		status = hrStatus;
		SetEvent(hReadSample);

		return S_OK;
	}
	// Called after IMFSourceReader::OnFLush completes
	STDMETHOD(OnFlush)(_In_ DWORD) override { return S_OK; }
	// Called after source reader receives certain events from media source
	STDMETHOD(OnEvent)(_In_ DWORD, _In_ IMFMediaEvent*) override { return S_OK; };

	void restart();

	SourceReaderCallback();
	virtual ~SourceReaderCallback();
};

class AudioEngine
{
private:
	// Audio Device for XAudio2 Engine
	Microsoft::WRL::ComPtr<IXAudio2> audioDevice;
	// Mastering Voice
	IXAudio2MasteringVoice* masterVoice;
	// Windows Media Foundation Source Reader Configuration
	Microsoft::WRL::ComPtr<IMFAttributes> sourceReaderConfig;

	// Streaming
	SourceReaderCallback sourceReaderCallback;    // Callback for the source reader
	StreamingVoiceCallback streamVoiceCallback;   // Callback for the source voice
	static const int maxBufferCount = 3;          // Maximum number of buffers used when streaming
	bool streamStopped = false;                   // Breaks streaming thread

public:
	AudioEngine();
	~AudioEngine();

	// Initialize Audio
	void initialize();

	// Load Audio from file
	void loadAudio(const char* filename, std::vector<BYTE>& audioData, WAVEFORMATEX** waveFormat, unsigned int& waveLength);

	// Create source reader in async mode
	void createAsyncReader(const char* filename, IMFSourceReader** sourceReader, WAVEFORMATEX* wfx, size_t wfxSize);
	// Streams a file
	void streamFile(const char* filename, XAUDIO2_VOICE_SENDS sendList, const bool loop = false);
	// The loop of the streaming function
	void loopStream(IMFSourceReader* const sourceReader, IXAudio2SourceVoice* const sourceVoice, const bool loop = false);

	friend class Audio;
};
#endif

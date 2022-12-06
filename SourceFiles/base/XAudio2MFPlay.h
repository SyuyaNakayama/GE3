#pragma once

#include <string>
#include <thread>
#include <vector>

#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>

#pragma comment(lib, "Mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "Mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")

#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")

namespace XAudio2MFSample
{
    class XAudio2MFPlay
    {
    public:
        void Play(std::wstring path)
        {
            CoInitializeEx(nullptr, COINIT_MULTITHREADED);

            MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);

            IMFSourceReader* pMFSourceReader{ nullptr };
            MFCreateSourceReaderFromURL(path.c_str(), NULL, &pMFSourceReader);

            IMFMediaType* pMFMediaType{ nullptr };
            MFCreateMediaType(&pMFMediaType);
            pMFMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
            pMFMediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
            pMFSourceReader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, pMFMediaType);

            pMFMediaType->Release();
            pMFMediaType = nullptr;
            pMFSourceReader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &pMFMediaType);

            WAVEFORMATEX* waveFormat{ nullptr };
            MFCreateWaveFormatExFromMFMediaType(pMFMediaType, &waveFormat, nullptr);

            std::vector<BYTE> mediaData;
            while (true)
            {
                IMFSample* pMFSample{ nullptr };
                DWORD dwStreamFlags{ 0 };
                pMFSourceReader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &dwStreamFlags, nullptr, &pMFSample);

                if (dwStreamFlags & MF_SOURCE_READERF_ENDOFSTREAM)
                {
                    break;
                }

                IMFMediaBuffer* pMFMediaBuffer{ nullptr };
                pMFSample->ConvertToContiguousBuffer(&pMFMediaBuffer);

                BYTE* pBuffer{ nullptr };
                DWORD cbCurrentLength{ 0 };
                pMFMediaBuffer->Lock(&pBuffer, nullptr, &cbCurrentLength);

                mediaData.resize(mediaData.size() + cbCurrentLength);
                memcpy(mediaData.data() + mediaData.size() - cbCurrentLength, pBuffer, cbCurrentLength);

                pMFMediaBuffer->Unlock();

                pMFMediaBuffer->Release();
                pMFSample->Release();
            }


            IXAudio2* pXAudio2{ nullptr };
            XAudio2Create(&pXAudio2, 0);

#if defined(_DEBUG)
            XAUDIO2_DEBUG_CONFIGURATION debug{ 0 };
            debug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
            debug.BreakMask = XAUDIO2_LOG_ERRORS;
            pXAudio2->SetDebugConfiguration(&debug, 0);
#endif

            IXAudio2MasteringVoice* pMasteringVoice{ nullptr };
            pXAudio2->CreateMasteringVoice(&pMasteringVoice);

            IXAudio2SourceVoice* pSourceVoice{ nullptr };
            pXAudio2->CreateSourceVoice(&pSourceVoice, waveFormat);

            XAUDIO2_BUFFER buffer{ 0 };
            buffer.pAudioData = mediaData.data();
            buffer.Flags = XAUDIO2_END_OF_STREAM;
            buffer.AudioBytes = sizeof(BYTE) * static_cast<UINT32>(mediaData.size());
            pSourceVoice->SubmitSourceBuffer(&buffer);

            pSourceVoice->Start(0);

            auto quit{ true };
            std::thread t([&quit]()
                {
                    while (true)
                    {
                        auto c{ getchar() };
                        if (c == 'q')
                        {
                            quit = false;
                        }
                    }
                });
            while (true)
            {
                if (!quit)
                {
                    break;
                }

                XAUDIO2_VOICE_STATE state{ 0 };
                pSourceVoice->GetState(&state);
                if (state.BuffersQueued == 0)
                {
                    break;
                }
                Sleep(10);
            }
            t.detach();

            pSourceVoice->DestroyVoice();
            pMasteringVoice->DestroyVoice();
            pXAudio2->Release();

            CoTaskMemFree(waveFormat);
            pMFMediaType->Release();
            pMFSourceReader->Release();
            MFShutdown();

            CoUninitialize();
        }
    };
}

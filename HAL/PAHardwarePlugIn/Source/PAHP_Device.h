#ifndef _PAHP_DEVICE_H_
#define _PAHP_DEVICE_H_

#include "HP_Device.h"
#include <IOKit/IOKitLib.h>
#include <CarbonCore/Multiprocessing.h>
#include <pulse/pulseaudio.h>
#include <pulse/mainloop.h>

class HP_DeviceControlProperty;
class HP_HogMode;
class HP_IOProc;
class HP_IOThread;
class PAHP_PlugIn;
class PAHP_Stream;

typedef struct AudioPosition {
	UInt32 bytePos;
	UInt64 cycleCount;
	CAMutex *mutex;
} AudioPosition;

#pragma mark ### PAHP_Device ###

class PAHP_Device : public HP_Device
{
public:
						PAHP_Device(AudioDeviceID inAudioDeviceID, PAHP_PlugIn *inPlugIn);
	virtual					~PAHP_Device();

	virtual void				Initialize();
	virtual void				Teardown();
	virtual void				Finalize();

protected:
	PAHP_PlugIn *				plugin;
	
public:
	PAHP_PlugIn *				GetSHPPlugIn() const { return plugin; }
	virtual CFStringRef			CopyDeviceName() const;
	virtual CFStringRef			CopyDeviceManufacturerName() const;
	virtual CFStringRef			CopyDeviceUID() const;
	virtual bool				HogModeIsOwnedBySelf() const;
	virtual bool				HogModeIsOwnedBySelfOrIsFree() const;
	virtual void				HogModeStateChanged();


private:
	HP_HogMode *				hogMode;

public:
	virtual bool				HasProperty(const AudioObjectPropertyAddress &inAddress) const;
	virtual bool				IsPropertySettable(const AudioObjectPropertyAddress &inAddress) const;
	virtual UInt32				GetPropertyDataSize(const AudioObjectPropertyAddress &inAddress,
								    UInt32 inQualifierDataSize,
								    const void *inQualifierData) const;
	virtual void				GetPropertyData(const AudioObjectPropertyAddress &inAddress,
								UInt32 inQualifierDataSize,
								const void *inQualifierData,
								UInt32 &ioDataSize,
								void *outData) const;
	virtual void				SetPropertyData(const AudioObjectPropertyAddress &inAddress,
								UInt32 inQualifierDataSize,
								const void *inQualifierData,
								UInt32 inDataSize,
								const void *inData,
								const AudioTimeStamp *inWhen);

protected:
	virtual void				PropertyListenerAdded(const AudioObjectPropertyAddress& inAddress);

protected:
	virtual bool				IsSafeToExecuteCommand();
	virtual bool				StartCommandExecution(void** outSavedCommandState);
	virtual void				FinishCommandExecution(void* inSavedCommandState);

public:
	virtual void				Do_StartIOProc(AudioDeviceIOProc inProc);
	virtual void				Do_StartIOProcAtTime(AudioDeviceIOProc inProc,
								     AudioTimeStamp &ioStartTime,
								     UInt32 inStartTimeFlags);

public:
	virtual CAGuard *			GetIOGuard();
	virtual bool				CallIOProcs(const AudioTimeStamp &inCurrentTime,
							    const AudioTimeStamp &inInputTime,
							    const AudioTimeStamp &inOutputTime);

protected:
	virtual void				StartIOEngine();
	virtual void				StartIOEngineAtTime(const AudioTimeStamp& inStartTime, UInt32 inStartTimeFlags);
	virtual void				StopIOEngine();
	
	virtual void				StartHardware();
	virtual void				StopHardware();

	void					StartIOCycle();
	void					PreProcessInputData(const AudioTimeStamp& inInputTime);
	bool					ReadInputData(const AudioTimeStamp& inStartTime, UInt32 inBufferSetID);
	void					PostProcessInputData(const AudioTimeStamp& inInputTime);
	void					PreProcessOutputData(const AudioTimeStamp& inOuputTime, HP_IOProc& inIOProc);
	bool					WriteOutputData(const AudioTimeStamp& inStartTime, UInt32 inBufferSetID);
	void					FinishIOCycle();
	
	HP_IOThread *				IOThread;

public:
	virtual UInt32				GetIOCycleNumber() const;

public:
	virtual void				GetCurrentTime(AudioTimeStamp& outTime);
	virtual void				SafeGetCurrentTime(AudioTimeStamp& outTime);
	virtual void				TranslateTime(const AudioTimeStamp& inTime, AudioTimeStamp& outTime);
	virtual void				GetNearestStartTime(AudioTimeStamp& ioRequestedStartTime, UInt32 inFlags);
	
	virtual void				StartIOCycleTimingServices();
	virtual bool				UpdateIOCycleTimingServices();
	virtual void				StopIOCycleTimingServices();

private:
	UInt64					anchorHostTime;
	
private:
	void					CreateStreams();
	void					ReleaseStreams();
	void					RefreshAvailableStreamFormats();

protected:
	void					CreateControls();
	void					ReleaseControls();
	
	static bool				IsControlRelatedProperty(AudioObjectPropertySelector inSelector);

private:
	bool					controlsInitialized;
	HP_DeviceControlProperty *		controlProperty;

	char					procname[MAXCOMLEN+1];

	pa_context *				PAContext;
	pa_stream *				PARecordStream;
	pa_stream *				PAPlaybackStream;
	
	unsigned char *				inputBuffer;
	unsigned char *				outputBuffer;

	AudioPosition				recordBufferReadPos;
	AudioPosition				recordBufferWritePos;
	AudioPosition				playbackBufferReadPos;
	AudioPosition				playbackBufferWritePos;
	
	bool					PAConnected;
	MPSemaphoreID				PAContextSemaphore;
	
	pa_threaded_mainloop *			PAMainLoop;
	pa_buffer_attr				buf_attr;
	
	CFRunLoopTimerRef			timer;
	Float64					actualSampleRate;
	UInt64					lastTime;
	pa_timing_info				lastTimingInfo;
		
	struct {
		Float64 x_next, x;
		Float64 P_next, P;
		Float64 K, Q, R, z;
	} k1;
	
	Float64 rateScalar;
	UInt64 underrunCount;
	
public:
	void					ContextStateCallback();
	void					StreamStartedCallback(pa_stream *stream);

	void					DeviceReadCallback(pa_stream *stream, size_t nbytes);
	void					DeviceWriteCallback(pa_stream *stream, size_t nbytes);
	void					StreamOverflowCallback(pa_stream *stream);
	void					StreamUnderflowCallback(pa_stream *stream);	
	void					StreamVolumeChanged(CFStringRef name, CFDictionaryRef userInfo);
	void					StreamMuteChanged(CFStringRef name, CFDictionaryRef userInfo);

	int					GetProcessName();
	
	void					ZeroAudioPosition(AudioPosition *p);
	void					IncAudioPosition(AudioPosition *a, UInt32 bytes);
	SInt64					DiffAudioPositions(const AudioPosition *a,
								   const AudioPosition *b) const;
	void					debugTimer();
	void					InitKalman(Float64 startValue);
	Float64					UpdateKalman(Float64);
	void					UpdateTiming();
	
	Float64					GetCurrentActualSampleRate() const;

public:
	

};

#endif /* _PAHP_DEVICE_H_ */
/***
 This file is part of the PulseAudio HAL plugin project
 
 Copyright 2010,2011 Daniel Mack <pulseaudio@zonque.de>
 
 The PulseAudio HAL plugin project is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 
 The PulseAudio HAL plugin project is distributed in the hope that it will be useful, but
 WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with PulseAudio; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 USA.
 ***/

#ifndef PA_STREAM_H_
#define PA_STREAM_H_

#include <CoreFoundation/CoreFoundation.h>
#include <CoreAudio/AudioHardware.h>

#include "PA_Object.h"
#include "PA_Plugin.h"
#include "PA_Device.h"

class PA_Plugin;
class PA_Device;
class PA_MuteControl;
class PA_VolumeControl;

class PA_Stream : public PA_Object
{
private:
	PA_Device *device;
	PA_MuteControl *muteControl;
	PA_VolumeControl *volumeControl;
	PA_Plugin *plugin;
	Boolean isInput;
	UInt32 startingChannel;
	
public:
	PA_Stream(PA_Plugin *inPlugIn,
		  PA_Device *inOwningDevice,
		  bool inIsInput,
		  UInt32 inStartingDeviceChannelNumber);
	~PA_Stream();
	
	void Initialize();
	void Teardown();
	
#pragma mark ### plugin interface ###

	OSStatus GetPropertyInfo(UInt32 inChannel,
				 AudioDevicePropertyID inPropertyID,
				 UInt32 *outSize,
				 Boolean *outWritable);
	
	OSStatus GetProperty(UInt32 inChannel,
			     AudioDevicePropertyID inPropertyID,
			     UInt32 *ioPropertyDataSize,
			     void *outPropertyData);
	
	OSStatus SetProperty(const AudioTimeStamp *inWhen,
			     UInt32 inChannel,
			     AudioDevicePropertyID inPropertyID,
			     UInt32 inPropertyDataSize,
			     const void *inPropertyData);

#pragma mark ### properties ###
	
	virtual Boolean	HasProperty(const AudioObjectPropertyAddress *inAddress);
	
	virtual OSStatus IsPropertySettable(const AudioObjectPropertyAddress *inAddress,
					    Boolean *outIsSettable);
	
	virtual OSStatus GetPropertyDataSize(const AudioObjectPropertyAddress *inAddress,
					     UInt32 inQualifierDataSize,
					     const void *inQualifierData,
					     UInt32 *outDataSize);
	
	virtual OSStatus GetPropertyData(const AudioObjectPropertyAddress *inAddress,
					 UInt32 inQualifierDataSize,
					 const void *inQualifierData,
					 UInt32 *ioDataSize,
					 void *outData);
	
	virtual OSStatus SetPropertyData(const AudioObjectPropertyAddress *inAddress,
					 UInt32 inQualifierDataSize,
					 const void *inQualifierData,
					 UInt32 inDataSize,
					 const void *inData);
	
	
	virtual void ReportOwnedObjects(std::vector<AudioObjectID> &arr);
	virtual PA_Object *FindObjectByID(AudioObjectID searchID);
	virtual const char *ClassName();
	
	OSStatus PublishObjects(Boolean active);
};

#endif // PA_STREAM_H_

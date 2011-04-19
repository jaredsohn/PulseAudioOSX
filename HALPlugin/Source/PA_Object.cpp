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

#define CLASS_NAME "PA_Object"

#include <sys/param.h>
#include "PA_Object.h"

AudioObjectID
PA_Object::GetObjectID()
{
	return objectID;
}

void
PA_Object::SetObjectID(AudioObjectID i)
{
	objectID = i;
}

void
PA_Object::ReportOwnedObjects(std::vector<AudioObjectID> & /* arr */)
{
}

const char *
PA_Object::ClassName() {
	return CLASS_NAME;
}

Boolean
PA_Object::HasProperty(const AudioObjectPropertyAddress *inAddress)
{
	switch (inAddress->mSelector) {
		case kAudioObjectPropertyOwnedObjects:
		case kAudioObjectPropertyListenerAdded:
		case kAudioObjectPropertyListenerRemoved:
			return true;
	}

	return false;
}

OSStatus
PA_Object::IsPropertySettable(const AudioObjectPropertyAddress *inAddress,
			      Boolean *outIsSettable)
{
	switch (inAddress->mSelector) {
		case kAudioObjectPropertyListenerAdded:
		case kAudioObjectPropertyListenerRemoved:
			*outIsSettable = true;
			return kAudioHardwareNoError;
	}

	*outIsSettable = false;
	return kAudioHardwareNoError;
}

OSStatus
PA_Object::GetPropertyDataSize(const AudioObjectPropertyAddress *inAddress,
			       UInt32 /* inQualifierDataSize */,
			       const void * /* inQualifierData */,
			       UInt32 *outDataSize)
{
	switch (inAddress->mSelector) {
		case kAudioObjectPropertyOwnedObjects: {
			std::vector<AudioObjectID> arr;
			ReportOwnedObjects(arr);
			*outDataSize = sizeof(AudioObjectID) * arr.size();
			return kAudioHardwareNoError;
		}
		case kAudioObjectPropertyListenerAdded:
		case kAudioObjectPropertyListenerRemoved:
			*outDataSize = sizeof(AudioObjectPropertyAddress);
			return kAudioHardwareNoError;
	}

	*outDataSize = 0;
	
	DebugLog("Unhandled property for id %d: '%c%c%c%c'",
		 (int) GetObjectID(),
		 ((int) inAddress->mSelector >> 24) & 0xff,
		 ((int) inAddress->mSelector >> 16) & 0xff,
		 ((int) inAddress->mSelector >> 8) & 0xff,
		 ((int) inAddress->mSelector >> 0) & 0xff);
	
	return kAudioHardwareUnknownPropertyError;
}

OSStatus
PA_Object::GetPropertyData(const AudioObjectPropertyAddress *inAddress,
			   UInt32 /* inQualifierDataSize */,
			   const void * /* inQualifierData */,
			   UInt32 *ioDataSize,
			   void *outData)
{
	switch (inAddress->mSelector) {
		case kAudioObjectPropertyOwnedObjects: {
			std::vector<AudioObjectID> arr;
			ReportOwnedObjects(arr);
			*ioDataSize = MIN(*ioDataSize, sizeof(AudioObjectID) * arr.size());
			memcpy(outData, &(arr.front()), *ioDataSize);
			return kAudioHardwareNoError;
		}			
		case kAudioObjectPropertyListenerAdded:
		case kAudioObjectPropertyListenerRemoved:
			//ASSERT
			memset(outData, 0, *ioDataSize);
			return kAudioHardwareNoError;
	}

	DebugLog("Unhandled property for id %d: '%c%c%c%c'",
		 (int) GetObjectID(),
		 ((int) inAddress->mSelector >> 24) & 0xff,
		 ((int) inAddress->mSelector >> 16) & 0xff,
		 ((int) inAddress->mSelector >> 8) & 0xff,
		 ((int) inAddress->mSelector >> 0) & 0xff);
	
	return kAudioHardwareUnknownPropertyError;
}

OSStatus
PA_Object::SetPropertyData(const AudioObjectPropertyAddress *inAddress,
			   UInt32 /* inQualifierDataSize */,
			   const void * /* inQualifierData */,
			   UInt32 /* inDataSize */,
			   const void *inData)
{
	switch (inAddress->mSelector) {
		case kAudioObjectPropertyListenerAdded: {
			const AudioObjectPropertyAddress *listen =
				static_cast<const AudioObjectPropertyAddress *> (inData);
			DebugLog("Added listener for property '%c%c%c%c'",
				 ((int) listen->mSelector >> 24) & 0xff,
				 ((int) listen->mSelector >> 16) & 0xff,
				 ((int) listen->mSelector >> 8) & 0xff,
				 ((int) listen->mSelector >> 0) & 0xff);
			return kAudioHardwareNoError;
		}
		case kAudioObjectPropertyListenerRemoved:
			return kAudioHardwareNoError;
	}

	DebugLog("Unhandled property for id %d: '%c%c%c%c'",
		 (int) GetObjectID(),
		 ((int) inAddress->mSelector >> 24) & 0xff,
		 ((int) inAddress->mSelector >> 16) & 0xff,
		 ((int) inAddress->mSelector >> 8) & 0xff,
		 ((int) inAddress->mSelector >> 0) & 0xff);
	
	return kAudioHardwareUnknownPropertyError;
}

void
PA_Object::Show()
{
	// implement me
}

void
PA_Object::Lock()
{
	pthread_mutex_lock(&mutex);
}

void
PA_Object::Unlock()
{
	pthread_mutex_unlock(&mutex);
}

PA_Object::PA_Object()
{
	pthread_mutex_init(&mutex, NULL);
}

PA_Object::~PA_Object()
{
	pthread_mutex_destroy(&mutex);
}

/***
 This file is part of PulseAudioKext

 Copyright (c) 2010,2011 Daniel Mack <pulseaudio@zonque.de>

 PulseAudioKext is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 ***/

#include "PAEngine.h"
#include "PADevice.h"
#include "PALog.h"

#define super IOAudioDevice

OSDefineMetaClassAndStructors(PADevice, IOAudioDevice)

bool
PADevice::init(OSDictionary *dictionary)
{
	OSDictionary *newDict = NULL;
	
	if (!dictionary)
		newDict = dictionary = OSDictionary::withCapacity(1);
	
	dictionary->setObject("IOUserClientClass", OSString::withCString(XStr(PADeviceUserClient)));
	
	bool ret = super::init(dictionary);
	
	if (newDict)
		newDict->release();
	
	return ret;
}

bool
PADevice::initHardware(IOService *provider)
{
	debugFunctionEnter();

	PADriver *driver = OSDynamicCast(PADriver, provider);
	if (!driver)
		return false;

	if (!super::initHardware(provider))
		return false;

	setDeviceName(deviceInfo.name);
	setDeviceShortName(deviceInfo.name);
	setManufacturerName("pulseaudio.org");
	setDeviceModelName("virtual audio device");
	setDeviceTransportType('virt');

	PAEngine *audioEngine = new PAEngine;

	if (!audioEngine)
		return false;

	if (!audioEngine->init(NULL) ||
	    !audioEngine->setDeviceInfo(&deviceInfo)) {
		audioEngine->release();
		audioEngine = NULL;
		return false;
	}

	activateAudioEngine(audioEngine);

	// the core is holding a reference now, so we can drop ours
	audioEngine->release();

	return true;
}

void
PADevice::setInfo(const struct PAVirtualDeviceInfo *info)
{
	debugFunctionEnter();
	memcpy(&deviceInfo, info, sizeof(deviceInfo));
}

void
PADevice::getInfo(struct PAVirtualDeviceInfo *info)
{
	debugFunctionEnter();
	memcpy(info, &deviceInfo, sizeof(deviceInfo));
}

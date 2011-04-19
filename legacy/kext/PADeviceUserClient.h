/***
 This file is part of PulseAudioKext
 
 Copyright (c) 2010,2011 Daniel Mack <pulseaudio@zonque.de>
 
 PulseAudioKext is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 ***/

#ifndef PADEVICEUSERCLIENT_H
#define PADEVICEUSERCLIENT_H

#include <IOKit/IOUserClient.h>

#include "PADevice.h"
#include "BuildNames.h"

class PADevice;

class PADeviceUserClient : public IOUserClient
{
	OSDeclareDefaultStructors(PADeviceUserClient)
	
private:
	PADevice	*device;
	UInt		currentDispatchSelector;
	task_t		clientTask;
	
	/* IOMethodDispatchers */
	static IOReturn	genericMethodDispatchAction(PADeviceUserClient *target, void *reference, IOExternalMethodArguments *args);
	
	IOReturn	getDeviceInfo(IOExternalMethodArguments *args);
	
	// IOUserClient interface
public:
	IOReturn	externalMethod(uint32_t selector, IOExternalMethodArguments *arguments,
				       IOExternalMethodDispatch *dispatch, OSObject *target, void *reference);
	IOReturn	clientClose(void);
	
	void		stop(IOService * provider);
	bool		start(IOService * provider);
	bool		initWithTask(task_t owningTask, void * securityID, UInt32 type);
	bool		terminate(IOOptionBits options);
};

#endif /* PADEVICEUSERCLIENT_H */


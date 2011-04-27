/***
 This file is part of PulseAudioOSX
 
 Copyright 2010,2011 Daniel Mack <pulseaudio@zonque.de>
 
 PulseAudioOSX is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 ***/

#import <Foundation/Foundation.h>


@interface PAServerConnectionAudio : NSObject
{
	PAServerConnection	*serverConnection;
	pa_context		*PAContext;
	
	pa_stream		*PARecordStream;
	pa_stream		*PAPlaybackStream;
	
	pa_buffer_attr		 bufAttr;
	pa_sample_spec		 sampleSpec;
	Float32			 sampleRate;
	UInt32			 ioBufferFrameSize;
	
	char			*inputDummyBuffer;
	char			*outputDummyBuffer;
}

@property (readonly) Float32 sampleRate;
@property (readonly) UInt32 ioBufferFrameSize;

- (id) initWithPAServerConnection: (PAServerConnection *) serverConnection
			  context: (pa_context *) context;

@end
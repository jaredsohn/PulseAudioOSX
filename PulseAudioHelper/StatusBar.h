/***
 This file is part of PulseAudioOSX
 
 Copyright 2010,2011 Daniel Mack <pulseaudio@zonque.de>
 
 PulseAudioOSX is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 ***/

#import <Cocoa/Cocoa.h>
#import "Preferences.h"

@interface StatusBar : NSObject<NSApplicationDelegate> {
    NSStatusItem *statusItem;
    NSImage *icon;
    Preferences *preferences;
}

@property (nonatomic, retain) Preferences *preferences;

- (NSMenu *) createMenu;

/* NSApplicationDelegate */
- (void) applicationDidFinishLaunching: (NSNotification *) aNotification;

@end

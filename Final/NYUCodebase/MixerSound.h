//
//  MixerSound.h
//  NYUCodebase
//
//  Created by Lyle True on 3/6/16.
//  Copyright (c) 2016 Ivan Safrin. All rights reserved.
//

#pragma once

class MixerSound
{    
public:
    Uint32 offset;
    Uint32 length;
    Uint8 *buffer;
    float volume;
    SDL_AudioFormat format;
    bool loaded;
    bool playing;
    bool loop;

};

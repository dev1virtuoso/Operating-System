#ifndef AUDIO_H
#define AUDIO_H

typedef struct {
    
} AudioModule;

void AudioModule_initialize(AudioModule* module);
void AudioModule_playSound(AudioModule* module, const char* soundFile);
void AudioModule_stopSound(AudioModule* module);

#endif
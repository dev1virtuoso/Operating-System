#ifndef VISION_H
#define VISION_H

typedef struct {
} VisionModule;

void VisionModule_initialize(VisionModule* module);
void VisionModule_processImage(VisionModule* module, const unsigned char* image);
void VisionModule_detectObjects(VisionModule* module);

#endif
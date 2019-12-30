#ifndef global_h
#define global_h
#include "lights.h"
#include "cobject.h"
#include "perspectiveCamera.h"

#define IMAGE_WIDTH       100
#define IMAGE_HEIGHT    100

extern lightUnion lights;
extern Union objects;
extern perspectiveCamera camera;
extern myBvhNode* headNode;
#endif /* global_h */

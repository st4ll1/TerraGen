#ifndef NOISE_H
#define NOISE_H


#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <math.h>
#include "ofMain.h"


class Noise {

private:
    GLuint permTextureID;
    GLuint gradTextureID;
public:
    Noise();
    void initPermTexture(GLuint *texID);
    void initGradTexture(GLuint *texID);


};

#endif 
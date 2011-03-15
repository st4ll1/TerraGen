#ifndef TEXTURE_H
#define TEXTURE_H

#include "ofMain.h"

class Texture {
public:
	Texture();
	~Texture();

	void load(ofImage *image, int wrapS, int wrapT);

	void bind();
	void unbind();

	int getId();

protected:
	unsigned int textureId;
};

#endif

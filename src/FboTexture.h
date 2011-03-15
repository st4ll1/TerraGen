#ifndef FBO_TEXTURE_H
#define FBO_TEXTURE_H

#include "Texture.h"

class FboTexture: public Texture {
public:
	// Allocate all resources necessary to use this frame buffer object
	// as a rendering target with a color buffer and a depth buffer
	// of the given size.
	// The method returns true if the allocation was successful.
	bool allocate(int width, int height);

	// Use this frame buffer object as the rendering target for subsequent
	// drawing operations to the color and depth buffer.
	void bindRenderTarget();
	// Use the standard screen again as the rendering target for subsequent
	// drawing operations
	void unbindRenderTarget();

	// Copy the entire content of the color buffer of this frame buffer
	// object to entire color buffer of the given destination frame buffer
	// object, scaling the image if necessary.
	void blitTo(FboTexture *destination);

	// Return the allocated width of the frame buffer.
	int getWidth();
	// Return the allocated height of the frame buffer.
	int getHeight();

protected:
	// allocated size of the frame buffer
	int width, height;
	// internal handle to the frame buffer, which consists of a
	// color buffer and a depth buffer.
	unsigned int frameBuffer;
	// internal handle to the depth buffer
	unsigned int depthBuffer;

	// Note that the super class Texture contains the handle
	// to the color buffer.
};

#endif

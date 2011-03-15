#include "FboTexture.h"

bool FboTexture::allocate(int width, int height) {
	bool result = true;

	this->width = width;
	this->height = height;
	// generate and new frame buffer object
	glGenFramebuffersEXT(1, &frameBuffer);
	// bind all subsequent frame buffer operations onto this one
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBuffer);

	// generate a depth buffer
	glGenRenderbuffersEXT(1, &depthBuffer);
	// bind the subsequent render buffer operations to this depth buffer
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBuffer);
	// this will actually only be used by the next operation:
	// declare the size of the depth buffer
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);

	// generate a texture as the color buffer for our frame buffer.
	// this is nearly the same code that defines a texture.
	glGenTextures(1, &textureId);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId);
	// declare the size of the color buffer
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		width, height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		0 // no pixels are loaded here...
	);
	// specify the texture parameters used for drawing the color buffer
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// color and depth buffer are created and ready to be attached...

	// attach the created texture as color buffer 0 of our frame buffer
	// note: OpenGL allows more color buffers
	glFramebufferTexture2DEXT(
		GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
		GL_TEXTURE_2D, textureId, 0
	);

	// attach the created depth buffer to our frame buffer
	glFramebufferRenderbufferEXT(
		GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
		GL_RENDERBUFFER_EXT, depthBuffer
	);

	// finally: check if everything worked out
	if (
		glCheckFramebufferStatusEXT(
			GL_FRAMEBUFFER_EXT
		) != GL_FRAMEBUFFER_COMPLETE_EXT
	) {
		result = false;
		ofLog(OF_LOG_ERROR, "Failed to allocate frame buffer object.");
	}

	// for now we still want to draw to the standard screen, so we
	// unbind our frame buffer.
	// use the method bindRenderTarget.
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glDisable(GL_TEXTURE_2D);

	return result;
}

void FboTexture::bindRenderTarget() {
	// bind all subsequent drawing operations to this frame buffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBuffer);
}

void FboTexture::unbindRenderTarget() {
	// bind all subsequent drawing operations to the standard screen again
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void FboTexture::blitTo(FboTexture *destination) {
	// note that we have to bind one of the two frame buffers involved in the
	// block image transfer operation
	destination->bindRenderTarget();
	// specify source and destination of the blit operation
	glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, frameBuffer);
	glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, destination->frameBuffer);
	// blit the entire color and depth buffer to the destination buffer
	glBlitFramebufferEXT(
		0, 0, width, height,
		0, 0, destination->width, destination->height,
		GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT,
		GL_NEAREST
	);
	destination->unbindRenderTarget();
}

int FboTexture::getWidth() {
	return width;
}

int FboTexture::getHeight() {
	return height;
}

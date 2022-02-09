#include "Image.h"

Image::Image() {
	initialized = false;
	spriteData.width = 2;
	spriteData.height = 2;
	spriteData.x = 0.0;
	spriteData.y = 0.0;
	spriteData.oriX = 0.0;
	spriteData.oriY = 0.0;
	spriteData.scaleX = 1.0;
	spriteData.scaleY = 1.0;
	spriteData.angle = 0.0;
	spriteData.rect.left = 0;	//used to select one frame from multi-frame image

	spriteData.rect.top = 0;
	spriteData.rect.right = spriteData.width;
	spriteData.rect.bottom = spriteData.height;
	spriteData.texture = NULL;			//Sprite texture(picture)
	spriteData.flipHorizontal = false;
	spriteData.flipVertical = false;

	cols = 1;
	textureManager = NULL;
	startFrame = 0;
	endFrame = 0;
	currentFrame = 0;
	frameDelay = 1.0;			//default to 1 second per frame of animation
	animTimer = 0.0;
	visible = true;						//the image is visible
	loop = true;						//loop frames
	animComplete = false;
	graphics = NULL;					//link to graphics system
	colorFilter = graphicsNS::WHITE;	//White for no change

}

Image::~Image() {

}

bool Image::initialize(Graphics* g, int width, int height, int ncols, TextureManager* textureM) {
	try {
		graphics = g;
		textureManager = textureM;

		spriteData.texture = textureManager->getTexture();
		if (width == 0)width = textureManager->getWidth();
		spriteData.width = width;
		if (height == 0) height = textureManager->getHeight();
		spriteData.height = height;
		cols = ncols;
		if (cols == 0) cols = 1;

		spriteData.rect.left = (currentFrame % cols) * spriteData.width;
		spriteData.rect.right = spriteData.rect.left + spriteData.width;
		spriteData.rect.top = (currentFrame / cols) * spriteData.height;
		spriteData.rect.bottom = spriteData.rect.top + spriteData.height;

	}
	catch (...) { return false; }
	initialized = true;
	return true;
}

void Image::draw(COLOR_ARGB color) {
	if (!visible || graphics == NULL) return;

	spriteData.texture = textureManager->getTexture();

	if (color == graphicsNS::FILTER) graphics->drawSprite(spriteData, colorFilter);
	else graphics->drawSprite(spriteData, color);

}

void Image::draw(SpriteData sd, COLOR_ARGB color) {
	if (!visible || graphics == NULL) return;
	sd.rect = spriteData.rect;
	sd.texture = textureManager->getTexture();
	if (color == graphicsNS::FILTER) graphics->drawSprite(sd, colorFilter);
	else graphics->drawSprite(sd, color);
}

inline void Image::setRect() {
	//configure spriteData.rect to draw currentFrame
	spriteData.rect.left = (currentFrame % cols) * spriteData.width;
	//right edge + 1
	spriteData.rect.right = spriteData.rect.left + spriteData.width;
	spriteData.rect.top = (currentFrame / cols) * spriteData.height;

	//bottom edge + 1
	spriteData.rect.bottom = spriteData.rect.top + spriteData.height;
}

void Image::update(float frameTime) {
	if (endFrame - startFrame > 0) {
		animTimer += frameTime;

		if (animTimer > frameDelay) {

			animTimer -= frameDelay;
			currentFrame++;
			if (currentFrame<startFrame || currentFrame > endFrame)
			{
				if (loop == true)
				{
					currentFrame = startFrame;
				}
				else {
					currentFrame = endFrame;
					animComplete = true;
				}
			}
			setRect();
		}
	}
}

void Image::setCurrentFrame(int c) {
	if (c >= 0) {
		currentFrame = c;
		animComplete = false;
		setRect();
	}
}


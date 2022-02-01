#ifndef _IMAGE_H
#define _IMAGE_H
#define WIN32_LEAN_AND_MEAN

#include "texturemanager.h"
#include "constants.h"

class Image {
protected:
	Graphics* graphics;
	TextureManager* textureManager;
	// spriteData contains the data required to draw the imge by Graphics::drawSprite()
	SpriteData spriteData;
	COLOR_ARGB colorFilter;
	int		cols;			//numbder of cols (1 to n) in multi-frame sprites
	int		startFrame;		//first frame of current animation
	int		endFrame;		//end frame of current animation
	int		currentFrame;	//current frame of current animation
	float	frameDelay;		//how long between frames of animation
	float	animTimer;		//animation timer
	HRESULT	hr;				//started return type
	bool	loop;			//true to loop frame
	bool	visible;		//true when visible
	bool	initialized;		//true when successfully initialized
	bool	animComplete;	//true when the loop is false and the animation is finished

public:
	Image();
	virtual ~Image();
	virtual bool initialize(Graphics* g, int width, int height, int ncols, TextureManager* textureM);
	virtual void flipHorizontal(bool flip) { spriteData.flipHorizontal = flip; }
	virtual void flipVertical(bool flip) { spriteData.flipVertical = flip; }
	virtual void draw(COLOR_ARGB color);
	virtual void draw(SpriteData sd, COLOR_ARGB color);
	virtual void update(float frameTime);
	const virtual SpriteData& getSpriteInfo() { return spriteData; }
	virtual bool getVisible() { return visible; }
	virtual float getX() { return spriteData.x; }
	virtual float getY() { return spriteData.y; }
	virtual float getScaleX() { return spriteData.scaleX; }
	//virtual float getScaleX2DP() {

	//	float value = (int)(spriteData.scaleX * 100);
	//	return (float)value / 100;
	//}
	//virtual int test() {
	//	float value = (int)(spriteData.scaleX * 100);
	//	float decimals = spriteData.scaleX * 100 - value;
	//	int finalValue = spriteData.scaleX * 100 - decimals;
	//	return finalValue;
	//}
	virtual float getScaleY() { return spriteData.scaleY; }
	virtual float getWidth() { return spriteData.width * spriteData.scaleX; }
	virtual float getHeight() { return spriteData.height * spriteData.scaleY; }
	virtual float getCenterX() { return spriteData.x + spriteData.width / 2 * getScaleX(); }
	virtual float getCenterY() { return spriteData.y + spriteData.height / 2 * getScaleY(); }
	virtual float getDegrees() { return spriteData.angle * (180.0f / (float)PI); }
	virtual float getRadian() { return spriteData.angle; }
	virtual int getFrameDelay() { return startFrame; }
	virtual int getStartFrame() { return endFrame; }
	virtual int getEndFrame() { return currentFrame; }
	virtual int setCurrentFrame() { return currentFrame; }
	virtual RECT getSpriteDataRect() { return spriteData.rect; }
	virtual bool getAnimationComplete() { return animComplete; }
	virtual COLOR_ARGB getColorFilter() { return colorFilter; }
	virtual void setX(float newX) { spriteData.x = newX; }
	virtual void setY(float newY) { spriteData.y = newY; }
	virtual void setScale(float scaleX, float scaleY) { spriteData.scaleX = scaleX; spriteData.scaleY = scaleY; }
	virtual void setScaleX(float scaleX) { spriteData.scaleX = scaleX; }
	virtual void setScaleY(float scaleY) { spriteData.scaleY = scaleY; }
	virtual void setDegrees(float deg) { spriteData.angle = deg * ((float)PI / 180.0f); }
	virtual void setRadians(float rad) { spriteData.angle = rad; }
	virtual void setVisible(bool v) { visible = v; }
	virtual void setFrameDelay(float d) { frameDelay = d; }
	virtual void setFrame(int s, int e) { startFrame = s; endFrame = e; }
	virtual void setCurrentFrame(int c);
	virtual void setRect();
	virtual void setSpriteDataRect(RECT r) { spriteData.rect = r; }
	virtual void setLoop(bool lp) { loop = lp; }
	virtual void setAnimationComplete(bool a) { animComplete = a; }
	virtual void setColorFilter(COLOR_ARGB color) { colorFilter = color; }
	virtual void setTextureManager(TextureManager* textureM) { textureManager = textureM; }
};
#endif // !_IMAGE_H

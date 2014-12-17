
#ifndef _ZEQ_INPUT_H
#define _ZEQ_INPUT_H

#include <irrlicht.h>

#include "types.h"
#include "exception.h"

using namespace irr;

class Input : public IEventReceiver
{
private:
	bool mSuspendInput;
	int8 mMoveDirection;
	int8 mTurnDirection;
	bool mRightMouseDown;
	int32 mMouseX;
	int32 mMouseY;
	float mRelativeMouseX;
	float mRelativeMouseY;

public:
	enum Movement
	{
		MOVE_FORWARD = -1,
		MOVE_NONE,
		MOVE_BACKWARD
	};

	enum Turn
	{
		TURN_LEFT = -1,
		TURN_NONE,
		TURN_RIGHT
	};

private:
	bool handleKeyboardEvent(const SEvent::SKeyInput& ev);
	bool handleMouseEvent(const SEvent::SMouseInput& ev);

public:
	Input();

	void suspendInput(bool toggle) { mSuspendInput = toggle; }

	int8 getMoveDirection() { return mMoveDirection; }
	int8 getTurnDirection() { return mTurnDirection; }
	bool getRightMouseDown() { return mRightMouseDown; }
	float getRelativeMouseXMovement() { return mRelativeMouseX; }
	float getRelativeMouseYMovement() { return mRelativeMouseY; }

	bool isMoving() { return mMoveDirection || mTurnDirection || mRelativeMouseX != 0.0f || mRelativeMouseY != 0.0f; }
	void resetMoved() { mRelativeMouseX = 0; mRelativeMouseY = 0; }

	virtual bool OnEvent(const SEvent& ev) override;
};

#endif

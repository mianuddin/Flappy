#include <iostream>
#include <sstream>
#include <cmath>
#include "CMUgraphics.h"
#include "Rando.h"
#include "Dice.h"

using namespace std;

class Bird
{
	int positionX, positionY, gravityRate; // gravityRate: How many pixels the bird should fall at a time.
	int state; // Wings up(1), down(2), or normal(0).
	int upCount; // How many times the "up" image has been drawn.
	bool flapAnimation; // If the flapAnimation is running.

	public:
	int getPositionX() { return positionX; };
	int getFrontPositionX() { return (positionX+34); }; // X Coordinate of front of bird.
	int getPositionY() { return positionY; };
	int getBottomPositionY() { return (positionY+24); }; // Y Coordinate of bottom of bird.
	void setup(int, int, int);
	void draw(window &inputWindow);
	void setState(int st) { state = st; };
	void flap();
	void pullDown() { positionY += gravityRate; };
	//bool detectHit();
};

void Bird::setup(int posX, int posY, int gravRate)
{
	positionX = posX;
	positionY = posY;
	gravityRate = gravRate;
	state = 2;
	upCount = 0;
	flapAnimation = false;
}

void Bird::draw(window &inputWindow)
{
	if(state == 0)
	{
		image bird0Image("assets\\bird.png", PNG);
		inputWindow.DrawImage(bird0Image, positionX, positionY);
		if(upCount == 4)
		{
			upCount = 0;
			state = 2;
			flapAnimation = false;
		}
	}
	else if(state == 1)
	{
		image bird1Image("assets\\birdUp.png", PNG);
		inputWindow.DrawImage(bird1Image, positionX, positionY);
		if(upCount == 3)
		{
			state = 0;
		}
		upCount++;
	}
	else if(state == 2)
	{
		image bird2Image("assets\\birdDown.png", PNG);
		inputWindow.DrawImage(bird2Image, positionX, positionY);
	}
}

void Bird::flap()
{
	positionY -= 100;
	if(flapAnimation == false)
	{
		flapAnimation = true;
		state = 1;
	}

}

void WaitNClear(window &inputWindow);

int main()
{
	int iX, iY;
	window gameWindow(451, 800, 5, 5);
	gameWindow.ChangeTitle("Flappy++");

	// Make sure main menu isnt erased.
	gameWindow.SetPen(BLACK);
    gameWindow.SetFont(16, BOLD, SWISS);
    gameWindow.DrawString(gameWindow.GetWidth()/2 - 100, gameWindow.GetHeight() - 25, "Click mouse to continue...");
    WaitNClear(gameWindow);
    
    // -- Title Screen --
    image backgroundImage("assets\\Background.png", PNG);
    gameWindow.DrawImage(backgroundImage, 0, 0);
    image startImage("assets\\splash.png", PNG);
    gameWindow.DrawImage(startImage, (225-147), (400-134));
    WaitNClear(gameWindow);

    gameWindow.SetBuffering(true);
    
    bool bQuit = false;
    keytype ktInput;
    clicktype ctInput;
    char cKeyData;
    
    gameWindow.FlushMouseQueue();
    gameWindow.FlushKeyQueue();

    gameWindow.SetFont(32, BOLD, BY_NAME, "Arial");

    Bird Joe;
    Joe.setup(125, 400, 10);

    do
    {
        gameWindow.DrawImage(backgroundImage, 0, 0);
        gameWindow.SetPen(BLACK);   
        ktInput = gameWindow.GetKeyPress(cKeyData);
        ctInput = gameWindow.GetMouseClick(iX, iY);
        Joe.draw(gameWindow);
        gameWindow.UpdateBuffer();
        if(ctInput == LEFT_CLICK)
        {
        	Joe.flap();
        }
        Joe.pullDown();
        Pause(50);
    } while(Joe.getBottomPositionY() <= 650);

	gameWindow.SetBuffering(false);
	return 0;
}


void WaitNClear(window &inputWindow)
{
	int iX, iY;

	// Flush the mouse queue
    inputWindow.FlushMouseQueue();
    // Ignore return value since we don't care what type of click it was
    inputWindow.WaitMouseClick(iX, iY);
    // Set the brush and pen white so we can clear the background
    inputWindow.SetPen(WHITE, 0);    
    inputWindow.SetBrush(WHITE);
    // Draw a rectangle that covers the entire window
    inputWindow.DrawRectangle(0, 0, inputWindow.GetWidth(), inputWindow.GetHeight());
}
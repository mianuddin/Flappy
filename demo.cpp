#include <iostream>
#include <sstream>
#include <cmath>
#include "CMUgraphics.h"
#include "Rando.h"
#include "Dice.h"

using namespace std;

int score = 0;

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
	if(positionY < 100)
	{
		positionY = 0;
	}
	else
	{
		positionY -= 70;
	}
	if(flapAnimation == false)
	{
		flapAnimation = true;
		state = 1;
	}
}

class Pipes
{
	int topX1, topY1, lowX1, lowY1; // Top left corner and bottom right corner.
	int topX2, topY2, lowX2, lowY2; // Top left corner and bottom right corner.

	public:
	int getTX1() { return topX1; };
	int getTY1() { return topY1; };
	int getLX1() { return lowX1; };
	int getLY1() { return lowY1; };
	int getTX2() { return topX2; };
	int getTY2() { return topY2; };
	int getLX2() { return lowX2; };
	int getLY2() { return lowY2; };
	void generate();
	void draw(window &inputWindow);
	void move();
	bool detectHit(Bird);
};

void Pipes::generate()
{
	Dice d(508); // Randomly choose start point for the gap inbetween the pipes.
	int startPosition = d.Roll();

	// Top Pipe
	topX1 = 451;
	topY1 = 0;
	lowX1 = (451+52); // Width of pipe is 52px
	lowY1 = startPosition;

	// Bottom Pipe
	topX2 = 451;
	topY2 = (startPosition+135); // Set Gap
	lowX2 = (451+52);
	lowY2 = 650;
}

void Pipes::draw(window &inputWindow)
{
	image pipeImage("assets\\pipe.png", PNG);

	inputWindow.SetPen(GREEN);
	inputWindow.SetBrush(GREEN);

	// Draw top pipe.
	inputWindow.DrawRectangle(topX1, topY1, lowX1, lowY1);

	// Draw bottom pipe.
	inputWindow.DrawRectangle(topX2, topY2, lowX2, lowY2);
}

void Pipes::move()
{
	topX1 -= 8;
	lowX1 -= 8;
	topX2 -= 8;
	lowX2 -= 8;
}

bool Pipes::detectHit(Bird inputBird)
{
	if(inputBird.getFrontPositionX() >= topX1 && inputBird.getFrontPositionX() <= lowX1 && inputBird.getPositionY() >= topY1 && inputBird.getPositionY() <= lowY1)
	{
		return true;
	}
	else if(inputBird.getFrontPositionX() >= topX2 && inputBird.getFrontPositionX() <= lowX2 && inputBird.getBottomPositionY() >= topY2 && inputBird.getBottomPositionY() <= lowY2)
	{
		return true;
	}
	else if(inputBird.getBottomPositionY() >= 650)
	{
		return true;
	}
}

void WaitNClear(window &inputWindow);

int main()
{
	int iX, iY;
	window gameWindow(451, 800, 5, 5);
	gameWindow.ChangeTitle("Flappy++");

	image logoImage("assets\\Logo.png", PNG);
	gameWindow.DrawImage(logoImage, gameWindow.GetWidth()/2 - 153, gameWindow.GetHeight()/2 - 37);
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

	Bird Joe; // Create new bird named Joe.
	Joe.setup(125, 325, 10); // Set his x, y, and gravity rate.

	Pipes Alex; // Create a new pipe named Alex.
	Alex.generate();
	bool scoreTrip = false; // Ensures you only get one point per pair of pipes.

	do
	{
		gameWindow.DrawImage(backgroundImage, 0, 0);
		gameWindow.SetPen(BLACK);   
		ktInput = gameWindow.GetKeyPress(cKeyData);
		ctInput = gameWindow.GetMouseClick(iX, iY);

		if(Alex.getLX1() < Joe.getPositionX() && scoreTrip == false)
		{
			score++;
			scoreTrip = true;
		}
		if(Alex.getLX1() < 0)
		{
			Alex.generate(); // Make new pipes.
			scoreTrip = false;
		}

		// Draw Bird
		Joe.draw(gameWindow);

		// Draw Pipes
		Alex.draw(gameWindow);

		// Draw Score
		gameWindow.SetFont(54, BOLD, BY_NAME, "Arial");
		gameWindow.SetPen(WHITE);
		ostringstream currentScore;
		currentScore << score;
		gameWindow.DrawString(gameWindow.GetWidth()/2 - 27, 20, currentScore.str());
		gameWindow.UpdateBuffer();

		if(ctInput == LEFT_CLICK || ktInput == 1)
		{
			Joe.flap();
		}
		Joe.pullDown();
		Alex.move();
		Pause(50);
	} while(Alex.detectHit(Joe) != true);

	gameWindow.SetBuffering(false);

	image endImage("assets\\scoreboard.png", PNG);
	gameWindow.DrawImage(endImage, (225-179), (400-129));
	// Print Score
	gameWindow.SetFont(36, BOLD, BY_NAME, "Arial");
	gameWindow.SetPen(BLACK);
	ostringstream printScore;
	printScore << score;
	gameWindow.DrawString(295, 413, printScore.str());

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
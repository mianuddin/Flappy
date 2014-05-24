#include <iostream>
#include <sstream>
#include <cmath>
#include "CMUgraphics.h"
#include "Rando.h"
#include "Dice.h"

using namespace std;

int score = 0;

const int kGravityRate = 10; // How many pixels the bird is pulled down.
const int kFlapDistance = 70; // How far the bird goes up per flap.
const int kPipeGap = 135; // The gap inbetween the top and bottom pipe.
const int kPipeMoveRate = 8; // How many pixels the pipes move.

class Bird
{
  int positionX, positionY;
  int state; // Wings up(1), down(2), or normal(0).
  int upCount; // How many times the "up" image has been drawn.
  bool flapAnimation; // If the flapAnimation is running.

  public:
  int getLeftX() { return positionX; };
  int getRightX() { return (positionX+34); };
  int getTopY() { return positionY; };
  int getBottomY() { return (positionY+24); };
  void setup(int, int);
  void draw(window &inputWindow);
  void flap();
  void pullDown() { positionY += kGravityRate; };
  void setY(int input) { positionY = input; };
};

void Bird::setup(int posX, int posY)
{
  positionX = posX;
  positionY = posY;
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
    positionY -= kFlapDistance;
  }
  if(flapAnimation == false)
  {
    flapAnimation = true;
    state = 1;
  }
}

class Pipes
{
  int leftX, rightX, bottomOfPipe1Y, topOfPipe2Y, bottomOfPipe2Y;

  public:
  int getRightX() { return rightX; };
  void generate();
  void draw(window &inputWindow);
  void move();
  bool detectHit(Bird);
};

void Pipes::generate()
{
  Dice d(508); // Randomly choose start point for the gap inbetween the pipes.
  int startPosition = d.Roll();
  int pipeWidth = 52;
  leftX = 451;
  rightX = (451+pipeWidth);
  // Top Pipe
  bottomOfPipe1Y = startPosition;
  // Bottom Pipe
  topOfPipe2Y = (startPosition+kPipeGap);
  bottomOfPipe2Y = 650;
}

void Pipes::draw(window &inputWindow)
{
  image pipe1Image("assets\\pipe-down.png", PNG);
  image pipe2Image("assets\\pipe-up.png", PNG);
  inputWindow.SetPen(DARKGREEN);
  inputWindow.SetBrush(DARKGREEN);

  // HI SPEC
  inputWindow.DrawImage(pipe1Image, leftX, bottomOfPipe1Y-500);
  inputWindow.DrawImage(pipe2Image, leftX, topOfPipe2Y);
  image floorImage("assets\\bottom.png", PNG);
  inputWindow.DrawImage(floorImage, 0, 650);

  // LOW SPEC
  //inputWindow.DrawRectangle(leftX, 0, rightX, bottomOfPipe1Y);
  //inputWindow.DrawRectangle(leftX, topOfPipe2Y, rightX, bottomOfPipe2Y);

}

void Pipes::move()
{
  leftX -= kPipeMoveRate;
  rightX -= kPipeMoveRate;
}

bool Pipes::detectHit(Bird inputBird)
{
  bool hitTopPipeC1 = inputBird.getRightX() >= leftX && inputBird.getRightX() <= rightX && inputBird.getTopY() >= 0 && inputBird.getTopY() <= bottomOfPipe1Y; // Top Corner 1
  bool hitTopPipeC2 = inputBird.getLeftX() >= leftX && inputBird.getLeftX() <= rightX && inputBird.getTopY() >= 0 && inputBird.getTopY() <= bottomOfPipe1Y; // Top Corner 2
  bool hitBottomPipeC1 = inputBird.getRightX() >= leftX && inputBird.getRightX() <= rightX && inputBird.getBottomY() >= topOfPipe2Y && inputBird.getBottomY() <= bottomOfPipe2Y; // Bottom Corner 1
  bool hitBottomPipeC2 = inputBird.getLeftX() >= leftX && inputBird.getLeftX() <= rightX && inputBird.getBottomY() >= topOfPipe2Y && inputBird.getBottomY() <= bottomOfPipe2Y; // Bottom Corner 2
  bool hitFloor = inputBird.getBottomY() >= 650;

  if(hitTopPipeC1 || hitTopPipeC2 || hitBottomPipeC1 || hitBottomPipeC2 || hitFloor)
  {
    return true;
  }
}

void WaitNClear(window &inputWindow);

int main()
{
  window gameWindow(451, 800, 0, 0);
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
  WaitNClear(gameWindow); // End Title Screen
  gameWindow.SetBuffering(true);
  gameWindow.FlushMouseQueue();
  gameWindow.FlushKeyQueue();
  keytype ktInput;
  clicktype ctInput;
  char cKeyData;
  int iX, iY;
  Bird Joe; // Create new bird named Joe.
  Joe.setup(125, 325); // Set his x, y, and gravity rate.
  Pipes Alex; // Create a new pipe named Alex.
  Alex.generate(); // Generate Pipes
  bool scoreTrip = false; // Ensures you only get one point per pair of pipes.
  do
  {
    gameWindow.DrawImage(backgroundImage, 0, 0);
    gameWindow.SetPen(BLACK);
    ktInput = gameWindow.GetKeyPress(cKeyData);
    ctInput = gameWindow.GetMouseClick(iX, iY);
    if(Alex.getRightX() < Joe.getLeftX() && scoreTrip == false)
    {
      score++;
      scoreTrip = true;
    }
    if(Alex.getRightX() < 0)
    {
      Alex.generate();
      scoreTrip = false;
    }
    // If the user clicks or presses a key, flap.
    if(ctInput == LEFT_CLICK || ktInput == 1)
    {
      Joe.flap();
    }
    Joe.pullDown();
    Alex.move();
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
    Pause(50);
  } while(Alex.detectHit(Joe) != true);

  while(Joe.getBottomY() < 650)
  {
    gameWindow.DrawImage(backgroundImage, 0, 0);
    Joe.pullDown();
    Alex.draw(gameWindow);
    Joe.draw(gameWindow);
    gameWindow.UpdateBuffer();
    // Pause(10);
  }

  if(Joe.getBottomY() != 650)
  {
    gameWindow.DrawImage(backgroundImage, 0, 0);
    Joe.setY(626);
    Alex.draw(gameWindow);
    Joe.draw(gameWindow);
    gameWindow.UpdateBuffer();
  }

  // End Screen
  gameWindow.SetBuffering(false);
  image endImage("assets\\scoreboard.png", PNG);
  gameWindow.DrawImage(endImage, (225-179), (400-129));
  // Print Score
  gameWindow.SetFont(36, BOLD, BY_NAME, "Arial");
  gameWindow.SetPen(BLACK);
  ostringstream printScore;
  printScore << score;
  gameWindow.DrawString(295, 413, printScore.str());
  Pause(2500);

  return 0;
}

void WaitNClear(window &inputWindow)
{
  int iX, iY;
  inputWindow.FlushMouseQueue();
  inputWindow.WaitMouseClick(iX, iY);
  inputWindow.SetPen(WHITE, 0);    
  inputWindow.SetBrush(WHITE);
  inputWindow.DrawRectangle(0, 0, inputWindow.GetWidth(), inputWindow.GetHeight());
}
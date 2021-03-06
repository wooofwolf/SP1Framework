﻿// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include "entity.h"
#include "npc.h"
#include "trap.h"
#include "Ftrap.h"
#include "WBtrap.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <fstream>


// Customizable Options
std::string fileName = "Map 1.txt";

// Map Colours
int settingsOption = 1;
int colourOption = 1;
std::string wallTxt = "White";
std::string groundTxt = "Blue";
WORD wall = 0xF6;
WORD ground = 0x1B;

// Tutorial Related
bool showFcontrols = true;
bool showFnpc = false;
bool showFnpc2 = false;
bool showFability = false;
bool showFobjective = false;
bool showWcontrols = false;
bool shownWcontrols = false;
bool showWnpc = false;
bool showWnpc2 = false;
bool showWability = false;
bool shownWability = false;
bool showWobjective = false;

int FBLives = 3;
int dead = 0;
bool fbwin = false;
double  g_dElapsedTime;
double  g_dDeltaTime;
int lastMove;
int lastMove2;
int doneShoot = 0;
int rOrC;
int tOrP;
int whichMap = 1;
int mapNum = 0;
int Wbtrap = 0;
bool mapSel = false;
bool fA = false;
bool wA = false;
bool WBTraptriggered = false;
int FT = 0;
int trapID;
int TrappedID;
bool FTrapTriggered = false;
double FTsecs[3] = { 0 };
SKeyEvent g_skKeyEvent[K_COUNT];
SMouseEvent g_mouseEvent;
char mapArray[81][26];
// NPC related stopwatch
double fsecsPassed[10] = { 0 };
double wsecsPassed[10] = { 0 };

// Game specific variables here
SGameChar   g_sPjtl;
SGameChar   g_sPjtl2;
SGameChar   g_sChar;
SGameChar   g_sChar2;
SGameChar   FTrap;
SGameChar   WBTrap;
EGAMESTATES g_eGameState = S_SPLASHSCREEN; // initial state
entity* npcPtr[10];
entity* WBTraps[3];
entity* FtrapPtr[3];

// Console object
Console g_Console(80, 25, "Arcane Ignition");

//--------------------------------------------------------------
// Purpose  : Initialisation function
//            Initialize variables, allocate memory, load data from file, etc. 
//            This is called once before entering into your main loop
// Input    : void
// Output   : void
//--------------------------------------------------------------
void init(void)
{
    npcPtr[0] = new npc;
    npcPtr[1] = new npc;
    npcPtr[2] = new npc;
    npcPtr[3] = new npc;
    npcPtr[4] = new npc;
    npcPtr[5] = new npc;
    npcPtr[6] = new npc;
    npcPtr[7] = new npc;
    npcPtr[8] = new npc;
    npcPtr[9] = new npc;
    FtrapPtr[0] = new Ftrap;
    FtrapPtr[1] = new Ftrap;
    FtrapPtr[2] = new Ftrap;
    WBTraps[0] = new WBtrap;
    WBTraps[1] = new WBtrap;
    WBTraps[2] = new WBtrap;


    // Set precision for floating point output
    g_dElapsedTime = 0.0;

    // sets the initial state for the game
    g_eGameState = S_SPLASHSCREEN;

    // sets the width, height and the font name to use in the console
    g_Console.setConsoleFont(0, 16, L"Consolas");

    // remember to set your keyboard handler, so that your functions can be notified of input events
    g_Console.setKeyboardHandler(keyboardHandler);
    g_Console.setMouseHandler(mouseHandler);
}

//--------------------------------------------------------------
// Purpose  : Reset before exiting the program
//            Do your clean up of memory here
//            This is called once just before the game exits
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void shutdown(void)
{
    // Deletes all used pointers
    for (int n = 0; n < 10; n++)
    {
        delete npcPtr[n];
    }

    for (int t = 0; t < 3; t++)
    {
        delete FtrapPtr[t];
    }

    for (int t = 0; t < 3; t++)
    {
        delete WBTraps[t];
    }

    // Reset to white text on black background
    colour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

    g_Console.clearBuffer();
}

//--------------------------------------------------------------
// Purpose  : Get all the console input events
//            This function sets up the keyboard and mouse input from the console.
//            We will need to reset all the keyboard status, because no events will be sent if no keys are pressed.
//
//            Remember to set the handlers for keyboard and mouse events.
//            The function prototype of the handler is a function that takes in a const reference to the event struct
//            and returns nothing. 
//            void pfKeyboardHandler(const KEY_EVENT_RECORD&);
//            void pfMouseHandlerconst MOUSE_EVENT_RECORD&);
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void getInput(void)
{
    // resets all the keyboard events
    memset(g_skKeyEvent, 0, K_COUNT * sizeof(*g_skKeyEvent));
    // then call the console to detect input from user
    g_Console.readConsoleInput();
}

//--------------------------------------------------------------
// Purpose  : This is the handler for the keyboard input. Whenever there is a keyboard event, this function will be called.
//            Ideally, you should pass the key event to a specific function to handle the event.
//            This is because in some states, some keys would be disabled. Hence, to reduce your code complexity, 
//            it would be wise to split your keyboard input handlers separately.
//            
//            The KEY_EVENT_RECORD struct has more attributes that you can use, if you are adventurous enough.
//
//            In this case, we are not handling any keyboard event in the Splashscreen state
//            
// Input    : const KEY_EVENT_RECORD& keyboardEvent - reference to a key event struct
// Output   : void
//--------------------------------------------------------------
void keyboardHandler(const KEY_EVENT_RECORD& keyboardEvent)
{
    switch (g_eGameState)
    {
    case S_SPLASHSCREEN: // don't handle anything for the splash screen
        break;
    case S_GAME: gameplayKBHandler(keyboardEvent); // handle gameplay keyboard event 
        break;
    }
}

//--------------------------------------------------------------
// Purpose  : This is the handler for the mouse input. Whenever there is a mouse event, this function will be called.
//            Ideally, you should pass the key event to a specific function to handle the event.
//            This is because in some states, some keys would be disabled. Hence, to reduce your code complexity, 
//            it would be wise to split your keyboard input handlers separately.
//            
//            For the mouse event, if the mouse is not moved, no event will be sent, hence you should not reset the mouse status.
//            However, if the mouse goes out of the window, you would not be able to know either. 
//
//            The MOUSE_EVENT_RECORD struct has more attributes that you can use, if you are adventurous enough.
//
//            In this case, we are not handling any mouse event in the Splashscreen state
//            
// Input    : const MOUSE_EVENT_RECORD& mouseEvent - reference to a mouse event struct
// Output   : void
//--------------------------------------------------------------
void mouseHandler(const MOUSE_EVENT_RECORD& mouseEvent)
{
    switch (g_eGameState)
    {
    case S_SPLASHSCREEN: // don't handle anything for the splash screen
        break;
    case S_GAME: gameplayMouseHandler(mouseEvent); // handle gameplay mouse event
        break;
    }
}

//--------------------------------------------------------------
// Purpose  : This is the keyboard handler in the game state. Whenever there is a keyboard event in the game state, this function will be called.
//            
//            Add more keys to the enum in game.h if you need to detect more keys
//            To get other VK key defines, right click on the VK define (e.g. VK_UP) and choose "Go To Definition" 
//            For Alphanumeric keys, the values are their ascii values (uppercase).
// Input    : const KEY_EVENT_RECORD& keyboardEvent
// Output   : void
//--------------------------------------------------------------
void gameplayKBHandler(const KEY_EVENT_RECORD& keyboardEvent)
{
    // here, we map the key to our enums
    EKEYS key = K_COUNT;
    switch (keyboardEvent.wVirtualKeyCode)
    {
    case 87: key = K_W; break;
    case 65: key = K_A; break;
    case 83: key = K_S; break;
    case 68: key = K_D; break;
    case 0x52: key = K_R; break;
    case 0x54: key = K_T; break;
    case 0x46: key = K_F; break;
    case VK_UP: key = K_UP; break;
    case VK_DOWN: key = K_DOWN; break;
    case VK_LEFT: key = K_LEFT; break;
    case VK_RIGHT: key = K_RIGHT; break;
    case VK_OEM_COMMA: key = K_COMMA; break;
    case VK_OEM_PERIOD: key = K_PERIOD; break;
    case VK_OEM_2: key = K_DIVIDE; break;
    case 0x31: key = K_1; break;
    case 0x32: key = K_2; break;
    case 0x33: key = K_3; break;
    case VK_SPACE: key = K_SPACE; break;
    case VK_ESCAPE: key = K_ESCAPE; break;
    }
    // a key pressed event would be one with bKeyDown == true
    // a key released event would be one with bKeyDown == false
    // if no key is pressed, no event would be fired.
    // so we are tracking if a key is either pressed, or released
    if (key != K_COUNT)
    {
        g_skKeyEvent[key].keyDown = keyboardEvent.bKeyDown;
        g_skKeyEvent[key].keyReleased = !keyboardEvent.bKeyDown;
    }
}

//--------------------------------------------------------------
// Purpose  : This is the mouse handler in the game state. Whenever there is a mouse event in the game state, this function will be called.
//            
//            If mouse clicks are detected, the corresponding bit for that mouse button will be set.
//            mouse wheel, 
//            
// Input    : const KEY_EVENT_RECORD& keyboardEvent
// Output   : void
//--------------------------------------------------------------
void gameplayMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent)
{
    if (mouseEvent.dwEventFlags & MOUSE_MOVED) // update the mouse position if there are no events
    {
        g_mouseEvent.mousePosition = mouseEvent.dwMousePosition;
    }
    g_mouseEvent.buttonState = mouseEvent.dwButtonState;
    g_mouseEvent.eventFlags = mouseEvent.dwEventFlags;
}

//--------------------------------------------------------------
// Purpose  : Update function
//            This is the update function
//            double dt - This is the amount of time in seconds since the previous call was made
//
//            Game logic should be done here.
//            Such as collision checks, determining the position of your game characters, status updates, etc
//            If there are any calls to write to the console here, then you are doing it wrong.
//
//            If your game has multiple states, you should determine the current state, and call the relevant function here.
//
// Input    : dt = deltatime
// Output   : void
//--------------------------------------------------------------
void update(double dt)
{
    // get the delta time
    g_dElapsedTime += dt;
    g_dDeltaTime = dt;

    switch (g_eGameState)
    {
    case S_SPLASHSCREEN: splashScreenWait(); // game logic for the splash screen
        break;
    case S_GAME: updateGame(); // gameplay logic when we are in the game
        break;
    }
}

void splashScreenWait()    // waits for time to pass in splash screen
{
    //if (g_dElapsedTime > 3.0) // wait for 3 seconds to switch to game mode, else do nothing
    g_eGameState = S_GAME;
}

void updateGame()       // gameplay logic
{
    // Start of game
    if (fbwin == false && FBLives > 0)
    {
        moveCharacter();    // moves the character, collision detection, physics, etc
        charAbility();
        moveNPC();          // moves NPC
        if (dead == 5 && mapNum == 1)
        {
            fbwin = true;
        }
    }
    processUserInput();
}

void moveCharacter()
{
    // Check how long Fireboy's Trap has been triggered, if already triggered
    if (FTrapTriggered == true)
    {
        FTsecs[trapID] += static_cast<Ftrap*>(FtrapPtr[trapID])->getFTSecsPassed();
        // Breaks trap if already triggered for 3 seconds
        if (FTsecs[trapID] >= 3)
        {
            FTrapTriggered = false;
            FtrapPtr[trapID]->setAlive(false);
            FTsecs[trapID] = 0;
        }
    }
   
    // Updating the location of the character based on the key release
    // Fire Boy moving up
    if (g_skKeyEvent[K_W].keyReleased && Collision(g_sChar.m_cLocation, 'U') == false)
    {
        g_sChar.m_cLocation.Y--;
        if (doneShoot == 0)
        {
            tpProj1();
            lastMove = 1;

        }

        for (int t = 0; t < 3; t++)//for number of traps
        {
            if (WBTraps[t]->getCoords().X == g_sChar.m_cLocation.X && WBTraps[t]->getCoords().Y == g_sChar.m_cLocation.Y && WBTraps[t]->getAlive() == true)//if fireboyy steps on waterboy trap and the trap is active
            {
                WBTraptriggered = true;//trigger the trap
                TrappedID = t;//that traps id = t
            }

        }
    }
    // Fire Boy moving left
    if (g_skKeyEvent[K_A].keyReleased && g_sChar.m_cLocation.X > 0 && Collision(g_sChar.m_cLocation, 'L') == false)
    {
        g_sChar.m_cLocation.X--;
        if (doneShoot == 0)
        {
            tpProj1();
            lastMove = 2;
        }

        for (int t = 0; t < 3; t++)//for loop for the number of traps
        {
            if (WBTraps[t]->getCoords().X == g_sChar.m_cLocation.X && WBTraps[t]->getCoords().Y == g_sChar.m_cLocation.Y && WBTraps[t]->getAlive() == true)//if fireboyy steps on waterboy trap and the trap is active
            {
                WBTraptriggered = true;//trigger the trap
                TrappedID = t;//that traps id = t
                
            }

        }
    }
    // Fire Boy moving down
    if (g_skKeyEvent[K_S].keyReleased && g_sChar.m_cLocation.Y < g_Console.getConsoleSize().Y - 1 && Collision(g_sChar.m_cLocation, 'D') == false)
    {
        g_sChar.m_cLocation.Y++;
        if (doneShoot == 0)
        {
            tpProj1();
            lastMove = 3;
        }

        for (int t = 0; t < 3; t++)//for loop for the number of traps
        {
            if (WBTraps[t]->getCoords().X == g_sChar.m_cLocation.X && WBTraps[t]->getCoords().Y == g_sChar.m_cLocation.Y && WBTraps[t]->getAlive() == true)//if fireboyy steps on waterboy trap and the trap is active
            {
                WBTraptriggered = true;//trigger the trap
                TrappedID = t;//that traps id = t
            }

        }
    }
    // Fire Boy moving right
    if (g_skKeyEvent[K_D].keyReleased && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 1 && Collision(g_sChar.m_cLocation, 'R') == false)
    {
        g_sChar.m_cLocation.X++;
        if (doneShoot == 0)
        {
            tpProj1();
            lastMove = 4;

        }

        for (int t = 0; t < 3; t++)//for loop for the number of traps
        {
            if (WBTraps[t]->getCoords().X == g_sChar.m_cLocation.X && WBTraps[t]->getCoords().Y == g_sChar.m_cLocation.Y && WBTraps[t]->getAlive() == true)//if fireboyy steps on waterboy trap and the trap is active
            {
                WBTraptriggered = true;//trigger the trap
                TrappedID = t;//that traps id = t
            }

        }
    }
    if (WBTraptriggered == true)//if the trap is triggered
    {
        FBLives--;//fireboy loses a life
        
        for (int t = 0; t < 3; t++) {//for loop for the traps
            for (int n = 0; n < 10; n++)//for loop for the NPCs
            {
                if (pow(npcPtr[n]->getCoords().X - WBTraps[t]->getCoords().X, 2) + pow(npcPtr[n]->getCoords().Y - WBTraps[t]->getCoords().Y, 2) * 2 <= 25)//if npc is withing a radius of 5 of the trap
                {
                    for (int y = 0; y < 26; y++)//for loop for the y axis of the map
                    {
                        for (int x = 0; x < 81; x++)//for loop for the x axis of the map
                        {
                            if (pow(npcPtr[n]->getCoords().X - WBTraps[t]->getCoords().X, 2) + pow(npcPtr[n]->getCoords().Y - WBTraps[t]->getCoords().Y, 2) * 2 <= 25 && npcPtr[n]->getAlive() == true)//if NPC is within the circle and its alive
                            {
                                for (int w = 0; w < 10; w++)//number of npcs that can be drenched
                                {
                                    g_Console.writeToBuffer(x, y, ' ', 0x90);//explosion animation
                                }
                            }
                        }   
                    }
                    for (int nw = 0; nw < 10; nw++)//npcs
                    {
                        if (pow(npcPtr[n]->getCoords().X - WBTraps[t]->getCoords().X, 2) + pow(npcPtr[n]->getCoords().Y - WBTraps[t]->getCoords().Y, 2) * 2 <= 25 && npcPtr[nw]->getAlive() == true && static_cast<npc*>(npcPtr[nw])->getSecsOnFire() >= 0)//if NPCS on fire and within range
                        {
                            drenchNpc(nw);//drench them
                        }
                    }
                }
            }
        }
        WBTraptriggered = false;//then set that trap to triggered = false
        WBTraps[TrappedID]->setAlive(false);//and set it so that its not alive.
    }
    
    // Water Boy moving up
    if (g_skKeyEvent[K_UP].keyReleased && g_sChar2.m_cLocation.Y > 0 && Collision(g_sChar2.m_cLocation, 'U') == false && FTrapTriggered == false)
    {
        g_sChar2.m_cLocation.Y--;
        if (doneShoot == 0)
        {
            tpProj2();
            lastMove2 = 1;
        }
        // Check if Fireboy's trap will be triggered
        for (int t = 0; t < 3; t++)
        {
            // Triggers trap
            if (g_sChar2.m_cLocation.X == FtrapPtr[t]->getCoords().X && g_sChar2.m_cLocation.Y == FtrapPtr[t]->getCoords().Y && FtrapPtr[t]->getAlive() == true)
            {
                FTrapTriggered = true;
                static_cast<Ftrap*>(FtrapPtr[t])->startFTWatch();
                trapID = t;
            }
        }
    }
    // Water Boy moving left
    if (g_skKeyEvent[K_LEFT].keyReleased && g_sChar2.m_cLocation.X > 0 && Collision(g_sChar2.m_cLocation, 'L') == false && FTrapTriggered == false)
    {
        g_sChar2.m_cLocation.X--;
        if (doneShoot == 0)
        {
            tpProj2();
            lastMove2 = 2;
        }
        // Check if Fireboy's trap will be triggered
        for (int t = 0; t < 3; t++)
        {
            // Triggers trap
            if (g_sChar2.m_cLocation.X == FtrapPtr[t]->getCoords().X && g_sChar2.m_cLocation.Y == FtrapPtr[t]->getCoords().Y && FtrapPtr[t]->getAlive() == true)
            {
                FTrapTriggered = true;
                static_cast<Ftrap*>(FtrapPtr[t])->startFTWatch();
                trapID = t;
            }
        }
    }
    // Water Boy moving down
    if (g_skKeyEvent[K_DOWN].keyReleased && g_sChar2.m_cLocation.Y < g_Console.getConsoleSize().Y - 1 && Collision(g_sChar2.m_cLocation, 'D') == false && FTrapTriggered == false)
    {
        g_sChar2.m_cLocation.Y++;
        if (doneShoot == 0)
        {
            tpProj2();
            lastMove2 = 3;
        }
        // Check if Fireboy's trap will be triggered
        for (int t = 0; t < 3; t++)
        {
            // Triggers trap
            if (g_sChar2.m_cLocation.X == FtrapPtr[t]->getCoords().X && g_sChar2.m_cLocation.Y == FtrapPtr[t]->getCoords().Y && FtrapPtr[t]->getAlive() == true)
            {
                FTrapTriggered = true;
                static_cast<Ftrap*>(FtrapPtr[t])->startFTWatch();
                trapID = t;
            }
        }
    }
    // Water Boy moving right
    if (g_skKeyEvent[K_RIGHT].keyReleased && g_sChar2.m_cLocation.X < g_Console.getConsoleSize().X - 1 && Collision(g_sChar2.m_cLocation, 'R') == false && FTrapTriggered == false)
    {
        g_sChar2.m_cLocation.X++;
        if (doneShoot == 0)
        {
            tpProj2();
            lastMove2 = 4;
        }
        // Check if Fireboy's trap will be triggered
        for (int t = 0; t < 3; t++)
        {
            // Triggers trap
            if (g_sChar2.m_cLocation.X == FtrapPtr[t]->getCoords().X && g_sChar2.m_cLocation.Y == FtrapPtr[t]->getCoords().Y && FtrapPtr[t]->getAlive() == true)
            {
                FTrapTriggered = true;
                static_cast<Ftrap*>(FtrapPtr[t])->startFTWatch();
                trapID = t;
            }
        }
    }    
}

// Making projectile go to Fire Boy
void tpProj1()
{
    g_sPjtl.m_cLocation.X = g_sChar.m_cLocation.X;
    g_sPjtl.m_cLocation.Y = g_sChar.m_cLocation.Y;
}

// Making projectile go to Water Boy
void tpProj2()
{
    g_sPjtl2.m_cLocation.X = g_sChar2.m_cLocation.X;
    g_sPjtl2.m_cLocation.Y = g_sChar2.m_cLocation.Y;
}

// Keys for their projectile and skill and projectile animation 
void charAbility()
{
    int pjtlRange = 6;
    double pjtlSpeed = 0.05;
    // Projectile timer
    CStopWatch pjtlTimer;
    double pSecsPassed = 0;

    if (doneShoot == 0)
    {
        // Fire boy projectile
        if (g_skKeyEvent[K_R].keyReleased)
        {
            rOrC = 1;
            tOrP = 1;
            doneShoot++;
            pjtlTimer.startTimer();
        }
        // Fire boy ability
        if (g_skKeyEvent[K_T].keyReleased)
        {
            fA = true;
        }
        // Fire boy Trap
        if (g_skKeyEvent[K_F].keyReleased)
        {
            if (FT <= 2)
            {
                FtrapPtr[FT]->setAlive(true);
                FtrapPtr[FT]->setCoords(g_sChar.m_cLocation);
                FT++;
            }
        }

        // Water boy projectile
        if (g_skKeyEvent[K_COMMA].keyReleased)
        {
            rOrC = 0;
            tOrP = 0;
            doneShoot++;
            pjtlTimer.startTimer();
        }
        // Water boy ability
        if (g_skKeyEvent[K_PERIOD].keyReleased)
        {
            wA = true;
        }
        //Waterboy trap
        if (g_skKeyEvent[K_DIVIDE].keyReleased)//if / is pressed
        {
            if (Wbtrap <= 2) {//if number of traps is lessthan or = 2
                WBTraps[Wbtrap]->setAlive(true);//make the trap alive so its steppable 
                WBTraps[Wbtrap]->setCoords(g_sChar2.m_cLocation);//sets the coordinates at wherever waterboy was at when setting the trap
                Wbtrap++;//int waterboy trap + 1
            }
        }
    }
    else if (doneShoot > 0 && doneShoot <= pjtlRange)
    {
        pSecsPassed += pjtlTimer.getElapsedTime();
        if (pSecsPassed > pjtlSpeed)
        {
            // Fire boy Shooting
            if (rOrC == 1)
            {
                if (lastMove == 1 && Collision(g_sPjtl.m_cLocation, 'U') == false) 
                { // checks which direction was last inputted and sees if there will be collision
                    g_sPjtl.m_cLocation.Y -= 1; // if not then shoot
                    if (g_sPjtl.m_cLocation.Y == g_sChar2.m_cLocation.Y && g_sPjtl.m_cLocation.X == g_sChar2.m_cLocation.X) //if the projectile position is = to  Waterboy's position
                    {
                        doneShoot = pjtlRange; //finishes the projectile animation and returns the projectile to fireboy position
                    }
                    if (doneShoot == pjtlRange - 2) // checks if the projectile goes out into the darkness for shooting upwards
                    {
                        doneShoot += 2;//reduces the range 
                    }
                   
                }
                else if (lastMove == 2 && Collision(g_sPjtl.m_cLocation, 'L') == false)// checks which direction was last inputted and sees if there will be collision
                {
                    g_sPjtl.m_cLocation.X -= 1;// if not then shoot
                    if (g_sPjtl.m_cLocation.X == g_sChar2.m_cLocation.X && g_sPjtl.m_cLocation.Y == g_sChar2.m_cLocation.Y)//if the projectile position is = to  Waterboy's position
                    {
                        doneShoot = pjtlRange; //finishes the projectile animation and returns the projectile to fireboy position
                    }
                }
                else if (lastMove == 3 && Collision(g_sPjtl.m_cLocation, 'D') == false)// checks which direction was last inputted and sees if there will be collision
                {
                    g_sPjtl.m_cLocation.Y += 1;// if not then shoot
                    if (g_sPjtl.m_cLocation.Y == g_sChar2.m_cLocation.Y && g_sPjtl.m_cLocation.X == g_sChar2.m_cLocation.X)//if the projectile position is = to  Waterboy's position
                    {
                        doneShoot = pjtlRange; //finishes the projectile animation and returns the projectile to fireboy position
                    }
                    if (doneShoot == pjtlRange - 2)// checks if the projectile goes out into the darkness for shooting downwards
                    {
                        doneShoot += 2;//reduces the range 
                    }
                }
                else if (lastMove == 4 && Collision(g_sPjtl.m_cLocation, 'R') == false)// checks which direction was last inputted and sees if there will be collision
                {
                    g_sPjtl.m_cLocation.X += 1;// if not then shoot
                    if (g_sPjtl.m_cLocation.X == g_sChar2.m_cLocation.X && g_sPjtl.m_cLocation.Y == g_sChar2.m_cLocation.Y)//if the projectile position is = to  Waterboy's position
                    {
                        doneShoot = pjtlRange; //finishes the projectile animation and returns the projectile to fireboy position
                    }
                }
            }
            // Water boy Shooting
            else if (rOrC == 0)
            {
                if (lastMove2 == 1 && Collision(g_sPjtl2.m_cLocation, 'U') == false)// checks which direction was last inputted and sees if there will be collision
                {
                    g_sPjtl2.m_cLocation.Y -= 1;// if not then shoot 
                    for (int n = 0; n < 10; n++) // for loop for NPC
                    {
                        if ((g_sPjtl2.m_cLocation.Y == npcPtr[n]->getCoords().Y && g_sPjtl2.m_cLocation.X == npcPtr[n]->getCoords().X)) // checks if NPC gets shot by the projectile
                        {
                            doneShoot = pjtlRange;//finishes animation and returns projectile to Waterboy position
                        }
                        if (g_sPjtl2.m_cLocation.Y == g_sChar.m_cLocation.Y && g_sPjtl2.m_cLocation.X == g_sChar.m_cLocation.X)//checks if fireboy has been hit by waterboys projectile
                        {
                            FBLives--; // if he does he loses 1 life
                            doneShoot = pjtlRange;//finishes the projectile animation
                            break;//breaks out of the for loop
                        }
                    }
                    if (doneShoot == pjtlRange - 2) //checks if projectile will go out of the vision
                    {
                        doneShoot += 2;//reduces the range 
                    }
                }
                else if (lastMove2 == 2 && Collision(g_sPjtl2.m_cLocation, 'L') == false)// checks which direction was last inputted and sees if there will be collision
                {
                    g_sPjtl2.m_cLocation.X -= 1;// if not then shoot
                    for (int n = 0; n < 10; n++) // for loop for NPC
                    {
                        if (g_sPjtl2.m_cLocation.Y == npcPtr[n]->getCoords().Y && g_sPjtl2.m_cLocation.X == npcPtr[n]->getCoords().X)// checks if NPC gets shot by the projectile
                        {
                            doneShoot = pjtlRange;//finishes the projectile animation
                        }
                        if (g_sPjtl2.m_cLocation.Y == g_sChar.m_cLocation.Y && g_sPjtl2.m_cLocation.X == g_sChar.m_cLocation.X)//checks if fireboy has been hit by waterboys projectile
                        {
                            FBLives--;// if he does he loses 1 life
                            doneShoot = pjtlRange;//finishes the projectile animation
                            break;//breaks out of the for loop
                        }
                    }
                }
                else if (lastMove2 == 3 && Collision(g_sPjtl2.m_cLocation, 'D') == false)// checks which direction was last inputted and sees if there will be collision
                {
                    g_sPjtl2.m_cLocation.Y += 1;// if not then shoot
                    for (int n = 0; n < 10; n++) // for loop for NPC
                    {
                        if (g_sPjtl2.m_cLocation.Y == npcPtr[n]->getCoords().Y && g_sPjtl2.m_cLocation.X == npcPtr[n]->getCoords().X)// checks if NPC gets shot by the projectile
                        {
                            doneShoot = pjtlRange;//finishes the projectile animation
                        }
                        if (g_sPjtl2.m_cLocation.Y == g_sChar.m_cLocation.Y && g_sPjtl2.m_cLocation.X == g_sChar.m_cLocation.X) //checks if fireboy has been hit by waterboys projectile
                        {
                            FBLives--;// if he does he loses 1 life
                            doneShoot = pjtlRange;//finishes the projectile animation
                            break;//breaks out of the for loop
                        }
                    }
                    if (doneShoot == pjtlRange - 2)//checks if projectile will go out of the vision
                    {
                        doneShoot += 2;//reduces the range  
                    }
                }
                else if (lastMove2 == 4 && Collision(g_sPjtl2.m_cLocation, 'R') == false)// checks which direction was last inputted and sees if there will be collision
                {
                    g_sPjtl2.m_cLocation.X += 1;// if not then shoot
                    for (int n = 0; n < 10; n++)
                    {
                        if (g_sPjtl2.m_cLocation.Y == npcPtr[n]->getCoords().Y && g_sPjtl2.m_cLocation.X == npcPtr[n]->getCoords().X)// checks if NPC gets shot by the projectile
                        {
                            doneShoot = pjtlRange;//finishes the projectile animation
                        }
                        if (g_sPjtl2.m_cLocation.Y == g_sChar.m_cLocation.Y && g_sPjtl2.m_cLocation.X == g_sChar.m_cLocation.X)//checks if fireboy has been hit by waterboys projectile
                        {
                            FBLives--;// if he does he loses 1 life
                            doneShoot = pjtlRange;//finishes the projectile animation
                            break;//breaks out of the for loop
                        }
                    }
                }
            }
            doneShoot++;
            pSecsPassed = 0;
        }
    }
    else if (doneShoot > pjtlRange)
    {
        // after Fire Boy shoots with ability on
        if (fA == true && tOrP == 1)
        {
            fA = false;
        }
        // after Water Boy shoots with ability on
        if (wA == true && tOrP == 0)
        {
            wA = false;
        }
        // teleporting projectile to owners
        tpProj1();
        tpProj2();
        // reset animation
        doneShoot = 0;
    }
    
}

// Drenching NPCs
void drenchNpc(int sd)
{
    static_cast<npc*>(npcPtr[sd])->setSecsOnFire(0);
    static_cast<npc*>(npcPtr[sd])->setCol(0x90);
    static_cast<npc*>(npcPtr[sd])->setDrenched(true);

    static_cast<npc*>(npcPtr[sd])->startWTimer();
}

// Moves every NPC
void moveNPC()
{
    for (int n = 0; n < 10; n++)
    {
        // NPC on fire
        if (static_cast<npc*>(npcPtr[n])->getSecsOnFire() > 0)
        {
            fsecsPassed[n] += static_cast<npc*>(npcPtr[n])->getFsecsPassed();

            if (fsecsPassed[n] > 0.33)
            {
                // Chooses a random direction to move
                int randomInt = rand() % 4 + 1;
                if (randomInt == 1) // Up
                {
                    if (npcPtr[n]->getCoords().Y - 1 >= 0 && Collision(npcPtr[n]->getCoords(), 'U') == false)
                    {
                        npcPtr[n]->setCoords(npcPtr[n]->getCoords().X, npcPtr[n]->getCoords().Y - 1);
                    }
                }
                else if (randomInt == 2) // Down
                {
                    if (npcPtr[n]->getCoords().Y + 1 <= g_Console.getMaxConsoleSize().Y - 1 && Collision(npcPtr[n]->getCoords(), 'D') == false)
                    {
                        npcPtr[n]->setCoords(npcPtr[n]->getCoords().X, npcPtr[n]->getCoords().Y + 1);
                    }
                }
                else if (randomInt == 3) // Left
                {
                    if (npcPtr[n]->getCoords().X - 1 >= 0 && Collision(npcPtr[n]->getCoords(), 'L') == false)
                    {
                        npcPtr[n]->setCoords(npcPtr[n]->getCoords().X - 1, npcPtr[n]->getCoords().Y);
                    }
                }
                else // Right
                {
                    if (npcPtr[n]->getCoords().X + 1 >= 0 && Collision(npcPtr[n]->getCoords(), 'R') == false)
                    {
                        npcPtr[n]->setCoords(npcPtr[n]->getCoords().X + 1, npcPtr[n]->getCoords().Y);
                    }
                }
                static_cast<npc*>(npcPtr[n])->setSecsOnFire(static_cast<npc*>(npcPtr[n])->getSecsOnFire() - 0.33);
                // Kills NPC if it burned for 3 secs or more
                if (static_cast<npc*>(npcPtr[n])->getSecsOnFire() <= 0)
                {
                    npcPtr[n]->setAlive(false);
                    dead++;
                }
                fsecsPassed[n] = 0;
            }
        }

        // NPC is alive
        else if (npcPtr[n]->getAlive() == true)
        {
            if (static_cast<npc*>(npcPtr[n])->getDrenched() == true)
            {
                wsecsPassed[n] += static_cast<npc*>(npcPtr[n])->getWsecsPassed();
                if (wsecsPassed[n] >= 5)
                {
                    static_cast<npc*>(npcPtr[n])->setDrenched(false);
                    static_cast<npc*>(npcPtr[n])->setCol(0xA0);
                    wsecsPassed[n] = 0;
                }
            }
            // check if player is in range of NPC
            if ((pow(g_sChar.m_cLocation.X - npcPtr[n]->getCoords().X, 2) + pow(g_sChar.m_cLocation.Y - npcPtr[n]->getCoords().Y, 2) * 2) <= 25)
            {
                int npcU, npcD, npcL, npcR;
                // Check every movement option
                npcU = pow(pow(g_sChar.m_cLocation.X - npcPtr[n]->getCoords().X, 2) + pow(g_sChar.m_cLocation.Y - npcPtr[n]->getCoords().Y - 1, 2), 0.5);
                npcD = pow(pow(g_sChar.m_cLocation.X - npcPtr[n]->getCoords().X, 2) + pow(g_sChar.m_cLocation.Y - npcPtr[n]->getCoords().Y + 1, 2), 0.5);
                npcL = pow(pow(g_sChar.m_cLocation.X - npcPtr[n]->getCoords().X - 1, 2) + pow(g_sChar.m_cLocation.Y - npcPtr[n]->getCoords().Y, 2), 0.5);
                npcR = pow(pow(g_sChar.m_cLocation.X - npcPtr[n]->getCoords().X + 1, 2) + pow(g_sChar.m_cLocation.Y - npcPtr[n]->getCoords().Y , 2), 0.5);
                // Check which movement option is the best
                if (Collision(npcPtr[n]->getCoords(), 'U') == false) // Up is not blocked
                {
                    if (Collision(npcPtr[n]->getCoords(), 'D') == false) // Down is not blocked
                    {
                        if (Collision(npcPtr[n]->getCoords(), 'L') == false) // Left is not blocked
                        {
                            if (Collision(npcPtr[n]->getCoords(), 'R') == false) // Right is not blocked
                            {
                                if (npcU <= npcD && npcU <= npcL && npcU <= npcR)
                                {
                                    npcPtr[n]->setCoords(npcPtr[n]->getCoords().X, npcPtr[n]->getCoords().Y - 1);
                                }
                                else if (npcD <= npcU && npcD <= npcL && npcD <= npcR)
                                {
                                    npcPtr[n]->setCoords(npcPtr[n]->getCoords().X, npcPtr[n]->getCoords().Y + 1);
                                }
                                else if (npcL <= npcU && npcL <= npcD && npcL <= npcR)
                                {
                                    npcPtr[n]->setCoords(npcPtr[n]->getCoords().X - 1, npcPtr[n]->getCoords().Y);
                                }
                                else if (npcR <= npcU && npcR <= npcD && npcR <= npcL)
                                {
                                    npcPtr[n]->setCoords(npcPtr[n]->getCoords().X + 1, npcPtr[n]->getCoords().Y);
                                }
                            }

                            else if (Collision(npcPtr[n]->getCoords(), 'R') == true) // Right is blocked
                            {
                                if (npcU <= npcD && npcU <= npcL && npcU <= npcR)
                                {
                                    npcPtr[n]->setCoords(npcPtr[n]->getCoords().X, npcPtr[n]->getCoords().Y - 1);
                                }
                                else if (npcD <= npcU && npcD <= npcL && npcD <= npcR)
                                {
                                    npcPtr[n]->setCoords(npcPtr[n]->getCoords().X, npcPtr[n]->getCoords().Y + 1);
                                }
                                else if (npcL <= npcU && npcL <= npcD && npcL <= npcR)
                                {
                                    npcPtr[n]->setCoords(npcPtr[n]->getCoords().X - 1, npcPtr[n]->getCoords().Y);
                                }
                            }
                        }

                        else if (Collision(npcPtr[n]->getCoords(), 'L') == true) // Left is blocked
                        {
                            if (Collision(npcPtr[n]->getCoords(), 'R') == false) // Right is not blocked
                            {
                                if (npcU <= npcD && npcU <= npcR)
                                {
                                    npcPtr[n]->setCoords(npcPtr[n]->getCoords().X, npcPtr[n]->getCoords().Y - 1);
                                }
                                else if (npcD <= npcU && npcD <= npcR)
                                {
                                    npcPtr[n]->setCoords(npcPtr[n]->getCoords().X, npcPtr[n]->getCoords().Y + 1);
                                }
                                else if (npcR <= npcU && npcR <= npcD)
                                {
                                    npcPtr[n]->setCoords(npcPtr[n]->getCoords().X + 1, npcPtr[n]->getCoords().Y);
                                }
                            }
                            else if (Collision(npcPtr[n]->getCoords(), 'R') == true) // Right is blocked
                            {
                                if (npcU <= npcD)
                                {
                                    npcPtr[n]->setCoords(npcPtr[n]->getCoords().X, npcPtr[n]->getCoords().Y - 1);
                                }
                                else if (npcD <= npcU)
                                {
                                    npcPtr[n]->setCoords(npcPtr[n]->getCoords().X, npcPtr[n]->getCoords().Y + 1);
                                }
                            }
                        }
                    }

                    else if (Collision(npcPtr[n]->getCoords(), 'D') == true) // Down is blocked
                    {
                        if (Collision(npcPtr[n]->getCoords(), 'L') == false) // Left is not blocked
                        {
                            if (Collision(npcPtr[n]->getCoords(), 'R') == false) // Right is not blocked
                            {
                                if (npcU <= npcL && npcU <= npcR)
                                {
                                    npcPtr[n]->setCoords(npcPtr[n]->getCoords().X, npcPtr[n]->getCoords().Y - 1);
                                }
                                else if (npcL <= npcU && npcL <= npcR)
                                {
                                    npcPtr[n]->setCoords(npcPtr[n]->getCoords().X - 1, npcPtr[n]->getCoords().Y);
                                }
                                else if (npcR <= npcU && npcR <= npcL)
                                {
                                    npcPtr[n]->setCoords(npcPtr[n]->getCoords().X + 1, npcPtr[n]->getCoords().Y);
                                }
                            }

                            else if (Collision(npcPtr[n]->getCoords(), 'R') == true) // Right is blocked
                            {
                                if (npcU <= npcL)
                                {
                                    npcPtr[n]->setCoords(npcPtr[n]->getCoords().X, npcPtr[n]->getCoords().Y - 1);
                                }
                                else if (npcL <= npcU)
                                {
                                    npcPtr[n]->setCoords(npcPtr[n]->getCoords().X - 1, npcPtr[n]->getCoords().Y);
                                }
                            }
                        }

                        else if (Collision(npcPtr[n]->getCoords(), 'L') == true) // Left is blocked
                        {
                            if (Collision(npcPtr[n]->getCoords(), 'R') == false) // Right is not blocked
                            {
                                if (npcU <= npcR)
                                {
                                    npcPtr[n]->setCoords(npcPtr[n]->getCoords().X, npcPtr[n]->getCoords().Y - 1);
                                }
                                else if (npcR <= npcU)
                                {
                                    npcPtr[n]->setCoords(npcPtr[n]->getCoords().X + 1, npcPtr[n]->getCoords().Y);
                                }
                            }
                        }
                    }
                }

                else if (Collision(npcPtr[n]->getCoords(), 'U') == true) // Up is blocked
                {
                    if (Collision(npcPtr[n]->getCoords(), 'D') == false) // Down is not blocked
                    {
                        if (Collision(npcPtr[n]->getCoords(), 'L') == false) // Left is not blocked
                        {
                            if (Collision(npcPtr[n]->getCoords(), 'R') == false) // Right is not blocked
                            {
                                if (npcD <= npcL && npcD <= npcR)
                                {
                                    npcPtr[n]->setCoords(npcPtr[n]->getCoords().X, npcPtr[n]->getCoords().Y + 1);
                                }
                                else if (npcL <= npcD && npcL <= npcR)
                                {
                                    npcPtr[n]->setCoords(npcPtr[n]->getCoords().X - 1, npcPtr[n]->getCoords().Y);
                                }
                                else if (npcR <= npcD && npcR <= npcL)
                                {
                                    npcPtr[n]->setCoords(npcPtr[n]->getCoords().X + 1, npcPtr[n]->getCoords().Y);
                                }
                            }

                            else if (Collision(npcPtr[n]->getCoords(), 'R') == true) // Right is blocked
                            {
                                if (npcD <= npcL)
                                {
                                    npcPtr[n]->setCoords(npcPtr[n]->getCoords().X, npcPtr[n]->getCoords().Y + 1);
                                }
                                else if (npcL <= npcD)
                                {
                                    npcPtr[n]->setCoords(npcPtr[n]->getCoords().X - 1, npcPtr[n]->getCoords().Y);
                                }
                            }
                        }

                        else if (Collision(npcPtr[n]->getCoords(), 'L') == true) // Left is blocked
                        {
                            if (Collision(npcPtr[n]->getCoords(), 'R') == false) // Right is not blocked
                            {
                                if (npcD <= npcU && npcD <= npcR)
                                {
                                    npcPtr[n]->setCoords(npcPtr[n]->getCoords().X, npcPtr[n]->getCoords().Y + 1);
                                }
                                else if (npcR <= npcU && npcR <= npcD)
                                {
                                    npcPtr[n]->setCoords(npcPtr[n]->getCoords().X + 1, npcPtr[n]->getCoords().Y);
                                }
                            }
                        }
                    }

                    else if (Collision(npcPtr[n]->getCoords(), 'D') == true) // Down is blocked
                    {
                        if (Collision(npcPtr[n]->getCoords(), 'L') == false) // Left is not blocked
                        {
                            if (Collision(npcPtr[n]->getCoords(), 'R') == false) // Right is not blocked
                            {
                                if (npcL <= npcR)
                                {
                                    npcPtr[n]->setCoords(npcPtr[n]->getCoords().X - 1, npcPtr[n]->getCoords().Y);
                                }
                                else if (npcR <= npcL)
                                {
                                    npcPtr[n]->setCoords(npcPtr[n]->getCoords().X + 1, npcPtr[n]->getCoords().Y);
                                }
                            }
                        }

                        else if (Collision(npcPtr[n]->getCoords(), 'L') == true) // Left is blocked
                        {
                            if (Collision(npcPtr[n]->getCoords(), 'R') == false) // Right is not blocked
                            {
                                npcPtr[n]->setCoords(npcPtr[n]->getCoords().X + 1, npcPtr[n]->getCoords().Y);
                            }
                        }
                    }
                }
            }
        }
    }
}

// Events when player hits the escape key
void processUserInput()
{
    if (g_skKeyEvent[K_ESCAPE].keyReleased)
    {
        // If in a map than go to menu
        if (mapNum == 1 || mapNum == 2 || mapNum == 3 || fbwin == true || FBLives == 0)
        {
            // Reset everything
            mapNum = 0;
            mapSel = false;
            fA = false;
            wA = false;
            fbwin = false;
            FBLives = 3;
            dead = 0;
            for (int t = 0; t < 3; t++)
            {
                WBTraps[t]->setAlive(false);
                Wbtrap = 0;
            }
            for (int t = 0; t < 3; t++)
            {
                FtrapPtr[t]->setAlive(false);
                FT = 0;
            }
            for (int n = 0; n < 10; n++)
            {
                npcPtr[n]->setAlive(false);
                npcPtr[n]->setCoords(-1, -1);
            }
        }
        // If in menu than quit
        else
        {
            g_bQuitGame = true;
        }
    }
}

// Making NPCs on fire or drench with Fire Boy and Water Boy abilities animation
void updateNPC(int n)
{
    CStopWatch explosionTimer;
    double esecsPassed = 0;
    int wbARange = 9;
    // NPC on Fire
    if (g_sPjtl.m_cLocation.X == npcPtr[n]->getCoords().X && g_sPjtl.m_cLocation.Y == npcPtr[n]->getCoords().Y && npcPtr[n]->getAlive() == true && static_cast<npc*>(npcPtr[n])->getSecsOnFire() <= 0 && static_cast<npc*>(npcPtr[n])->getDrenched() == false)
    {
        static_cast<npc*>(npcPtr[n])->setSecsOnFire(2);
        static_cast<npc*>(npcPtr[n])->setCol(0x4C);

        static_cast<npc*>(npcPtr[n])->startFTimer();
    }

    //Fire Boy ability
    // Checks if the ability button was pressed when fire boy's projectile was fired
    if (g_sPjtl.m_cLocation.X == npcPtr[n]->getCoords().X && g_sPjtl.m_cLocation.Y == npcPtr[n]->getCoords().Y && fA == true && tOrP == 1)
    {
        explosionTimer.startTimer();
        for (int y = 0; y < 26; y++)
        {
            for (int x = 0; x < 81; x++)

            {   //Checking if the NPCs are in range of one another
                if ((pow(x - npcPtr[n]->getCoords().X, 2) + pow(y - npcPtr[n]->getCoords().Y, 2) * 2 <= 9) && npcPtr[n]->getAlive() == true)
                {
                    esecsPassed += explosionTimer.getElapsedTime();
                    if (esecsPassed < 1)
                    {   //Changes the colour for NPCs who start burning because of the ability
                        g_Console.writeToBuffer(x, y, ' ', 0x4C);
                    }
                }
            }
        }
        for (int n1 = 0; n1 < 10; n1++)
        {   //Using a circle of radius of 3 to check if a burning NPC is near a non-burning NPC
            if ((pow(npcPtr[n1]->getCoords().X - npcPtr[n]->getCoords().X, 2) + pow(npcPtr[n1]->getCoords().Y - npcPtr[n]->getCoords().Y, 2) * 2 <= 9) && npcPtr[n1]->getAlive() == true && static_cast<npc*>(npcPtr[n1])->getDrenched() == false && static_cast<npc*>(npcPtr[n1])->getSecsOnFire() <= 0)
            {   //Setting how long NPCs stay burning for and the colour they change to when on fire
                static_cast<npc*>(npcPtr[n1])->setSecsOnFire(2);
                static_cast<npc*>(npcPtr[n1])->setCol(0x4C);
                //Starts the timer for burning NPCs
                static_cast<npc*>(npcPtr[n])->startFTimer();
                static_cast<npc*>(npcPtr[n1])->startFTimer();
            }
        }
    }

    // NPC drench
    if (g_sPjtl2.m_cLocation.X == npcPtr[n]->getCoords().X && g_sPjtl2.m_cLocation.Y == npcPtr[n]->getCoords().Y && npcPtr[n]->getAlive() == true && (static_cast<npc*>(npcPtr[n])->getSecsOnFire() >= 0 || static_cast<npc*>(npcPtr[n])->getCol() == 0x4C))
    {
        drenchNpc(n);
    }
    // Water Boy ability
    if (g_sPjtl2.m_cLocation.X == npcPtr[n]->getCoords().X && g_sPjtl2.m_cLocation.Y == npcPtr[n]->getCoords().Y && wA == true && tOrP == 0)
    {
        wA = false;
        for (int y = 0; y < 26; y++)
        {
            for (int x = 0; x < 81; x++)
            {
                if ((pow(x - npcPtr[n]->getCoords().X, 2) + pow(y - npcPtr[n]->getCoords().Y, 2) * 2 <= wbARange) && npcPtr[n]->getAlive() == true)
                {
                    for (int w = 0; w < 10; w++)
                    {
                        g_Console.writeToBuffer(x, y, ' ', 0x90);
                    }
                }
            }
        }
        for (int nw = 0; nw < 10; nw++)
        {
            if ((pow(npcPtr[nw]->getCoords().X - npcPtr[n]->getCoords().X, 2) + pow(npcPtr[nw]->getCoords().Y - npcPtr[n]->getCoords().Y, 2) * 2 <= wbARange) && npcPtr[nw]->getAlive() == true && static_cast<npc*>(npcPtr[nw])->getSecsOnFire() >= 0)
            {
                drenchNpc(nw);
            }
        }
    }
}

//--------------------------------------------------------------
// Purpose  : Render function is to update the console screen
//            At this point, you should know exactly what to draw onto the screen.
//            Just draw it!
//            To get an idea of the values for colours, look at console.h and the URL listed there
// Input    : void
// Output   : void
//--------------------------------------------------------------
void render()
{
    clearScreen();      // clears the current screen and draw from scratch 
    switch (g_eGameState)
    {
    case S_GAME: renderGame();
        break;
    }
    renderToScreen();       // dump the contents of the buffer to the screen, one frame worth of game
}

void clearScreen()
{
    // Clears the buffer with this colour attribute
    g_Console.clearBuffer(0xBF);
}

void renderToScreen()
{
    // Writes the buffer to the console, hence you will see what you have written
    g_Console.flushBufferToConsole();
}

// Loading the choosen map, Fire Boy, Water boy and NPCs
void renderGame()
{
    renderMap();        // renders the map to the buffer first
    if (mapNum == 1 || mapNum == 3)
    {
        renderCharacter(); // renders the character into the buffer
        renderNPC();
    }
}

// Pressing keys to choose a option in menu
void renderMap()
{
    int totalMaps = 3;

    if (whichMap == 1)
    {
        fileName = "Map 1.txt"; //Map 1 textfile to be opened if selected
    }
    else if (whichMap == 2)
    {
        fileName = "Map 2.txt";//Map 2 textfile to be opened if selected
    }
    else if (whichMap == 3)
    {
        fileName = "Map 3.txt"; // map 3 textfile to be opened if selected
    }

    /* SET SPAWN */
    // Map
    if (g_skKeyEvent[K_1].keyReleased && mapSel == false)
    {
        mapNum = 1;
        mapSel = true;

        // Set players spawn below
        g_sChar.m_cLocation.X = g_Console.getConsoleSize().X / 44;//sets fireboy X spawning location
        g_sChar.m_cLocation.Y = g_Console.getConsoleSize().Y / 1.05;//sets fireboy Y spwawning location
        tpProj1();
        g_sChar2.m_cLocation.X = g_Console.getConsoleSize().X / 1.025;//sets waterboy X spawning location
        g_sChar2.m_cLocation.Y = g_Console.getConsoleSize().Y / 15;//sets waterboy X spawning location
        tpProj2();

        // Set npcs spawn below
        int n = 0;//declaration of n = 0;
        std::ifstream mapFile; //decalre var mapfile using ifstream 
        mapFile.open(fileName, std::ifstream::in);//opens a file 
        for (int y = 0; y < 26; y++)//for loop for the Y axis of the map
        {
            for (int x = 0; x < 81; x++)//for loop for the X axis of the map
            {
                char c = mapFile.get(); //reads each line to find a char
                if (c == '2')//if char = '2'
                {
                    npcPtr[n]->setAlive(true);//sets the npcs alive
                    npcPtr[n]->setCoords(x, y);//at the position of '2'
                    static_cast<npc*>(npcPtr[n])->setSecsOnFire(0);//makes them not on fire
                    static_cast<npc*>(npcPtr[n])->setDrenched(false);//and makes them dry
                    static_cast<npc*>(npcPtr[n])->setCol(0xA0); // makes their color green
                    n++;
                }
            }
        }
    }

    // Settings
    if (g_skKeyEvent[K_2].keyReleased && mapSel == false)
    {
        mapNum = 2;
        mapSel = true;
    }

    // Tutorial
    else if (g_skKeyEvent[K_3].keyReleased && mapSel == false)
    {
        mapNum = 3;
        mapSel = true;

        // Tutorial related bools
        shownWability = false;
        shownWcontrols = false;
        showWcontrols = false;
        showWobjective = false;
        showFobjective = false;
        showFcontrols = true;

        // Set players spawn below
        g_sChar.m_cLocation.X = 2;
        g_sChar.m_cLocation.Y = 5;
        tpProj1();
        g_sChar2.m_cLocation.X = 2;
        g_sChar2.m_cLocation.Y = 19;
        tpProj2();

        // Set npcs spawn below
        int n = 0;
        std::ifstream mapFile;
        mapFile.open("TutorialMap.txt", std::ifstream::in);
        for (int y = 0; y < 26; y++)
        {
            for (int x = 0; x < 81; x++)
            {
                char c = mapFile.get();
                if (c == '2')
                {
                    npcPtr[n]->setAlive(true);//sets the npcs alive
                    npcPtr[n]->setCoords(x, y);//at the position of '2'
                    static_cast<npc*>(npcPtr[n])->setSecsOnFire(0);//makes them not on fire
                    static_cast<npc*>(npcPtr[n])->setDrenched(false);//and makes them dry
                    static_cast<npc*>(npcPtr[n])->setCol(0xA0); // makes their color green
                    n++;
                }

            }
        }
        for (int n = 4; n < 9; n++)
        {
            static_cast<npc*>(npcPtr[n])->setCol(0x4C);
        }

    }

    /* SET MAP */
    // Main Menu
    if (mapNum == 0 && mapSel == false)
    {
        g_Console.writeToBuffer(3, 3, "                                     _____            _ _   _             ", 0xB6);
        g_Console.writeToBuffer(3, 4, "    /\\                              |_   _|          (_) | (_)            ", 0xB6);
        g_Console.writeToBuffer(3, 5, "   /  \\   _ __ ___ __ _ _ __   ___    | |  __ _ _ __  _| |_ _  ___  _ __  ", 0xBC);
        g_Console.writeToBuffer(3, 6, "  / /\\ \\ | '__/ __/ _` | '_ \\ / _ \\   | | / _` | '_ \\| | __| |/ _ \\| '_ \\ ", 0xBC);
        g_Console.writeToBuffer(3, 7, " / ____ \\| | | (_| (_| | | | |  __/  _| || (_| | | | | | |_| | (_) | | | |", 0xBC);
        g_Console.writeToBuffer(3, 8, "/_/    \\_\\_|  \\___\\__,_|_| |_|\\___| |_____\\__, |_| |_|_|\\__|_|\\___/|_| |_|", 0xB4);
        g_Console.writeToBuffer(3, 9, "                                           __/ |                          ", 0xB4);
        g_Console.writeToBuffer(3, 10, "                                          |___/                           ", 0xB4);
        g_Console.writeToBuffer(31, 14, "Press 1 to play", 0xB4);
        g_Console.writeToBuffer(26, 15, "Press 2 to open settings", 0xB4);
        g_Console.writeToBuffer(26, 16, "Press 3 to play tutorial", 0xB4);
        g_Console.writeToBuffer(25, 17, "Press Esc to quit the game", 0xB4);
    }

    // Load Selected Map
    else if (mapNum == 1 && mapSel == true)
    {
        g_Console.clearBuffer();
        std::ifstream mapFile;

        if (whichMap == 1)
        {
            fileName = "Map 1.txt";
        }
        else if (whichMap == 2)
        {
            fileName = "Map 2.txt";
        }
        else if (whichMap == 3)
        {
            fileName = "Map 3.txt";
        }

        mapFile.open(fileName, std::ifstream::in);

        for (int y = 0; y < 26; y++)
        {
            for (int x = 0; x < 81; x++)
            {
                char c = mapFile.get();
                mapArray[x][y] = c;

                // Renders the wall and ground near players
                if (((pow(x - g_sChar.m_cLocation.X, 2) + pow(y - g_sChar.m_cLocation.Y, 2) * 2) <= 36) || (pow(x - g_sChar2.m_cLocation.X, 2) + pow(y - g_sChar2.m_cLocation.Y, 2) * 2) <= 36)
                {
                    if (c == '1')
                    {
                        g_Console.writeToBuffer(x, y, " °±²Û", wall);
                    }
                    else if (c == '0' || c == '2')
                    {
                        g_Console.writeToBuffer(x, y, " °±²Û", ground);
                    }
                }

                // Turns other areas of the map dark
                else
                {
                    g_Console.writeToBuffer(x, y, " °±²Û", 0x00);
                }

                for (int n = 0; n < 10; n++)
                {
                    // Renders the wall and ground near burning NPCs
                    if ((static_cast<npc*>(npcPtr[n])->getSecsOnFire() > 0 && (pow(x - (static_cast<npc*>(npcPtr[n])->getCoords()).X, 2) + pow(y - (static_cast<npc*>(npcPtr[n])->getCoords()).Y, 2) * 2 <= 16)))
                    {
                        if (c == '1')
                        {
                            g_Console.writeToBuffer(x, y, " °±²Û", wall);
                        }
                        else if (c == '0' || c == '2')
                        {
                            g_Console.writeToBuffer(x, y, " °±²Û", ground);
                        }
                    }
                }
            }
        }
        
        g_Console.writeToBuffer(20, 0, "Fireboy lives:" + std::to_string(FBLives), 0x07); // print number of fireboy lives UI
        g_Console.writeToBuffer(38, 0, "Number of NPCs left:" + std::to_string(5 - dead), 0x07);//print number of NPCs left UI
            
        
        // Win conditions
        if (fbwin == true)// if fireboy's win condition are met 
        {
            g_Console.writeToBuffer(3, 10, "FiRE BOY WINS!", 0x1A);//stop the game and print "Fireboy wins!"
        }
        if (FBLives == 0)//if Fireboy's number of lives == 0,
        {
            g_Console.writeToBuffer(62, 10, "WATER BOY WINS!", 0x1A);//stop the game and print "Waterboy wins!"
        }
    }
        
    // Settings
    else if (mapNum == 2 && mapSel == true)
    {
        int maxSettings = 2;
        int maxCOption = 5;
        std::string colourTxt = " Map colour (Wall: " + wallTxt + ", Ground: " + groundTxt + ") ";
        // Settings option
        g_Console.writeToBuffer(20, 1, "  __   __ ___ ___ ___       ___   __  ", 0XB0);
        g_Console.writeToBuffer(20, 2, " |__  |__  |   |   |  |\\ | |  _  |__  ", 0XB0);
        g_Console.writeToBuffer(20, 3, "  __| |__  |   |  _|_ | \\| |___|  __| ", 0XB0);
        g_Console.writeToBuffer(21, 5, "Up/Down arrow to choose which option", 0xB0);
        g_Console.writeToBuffer(20, 6, "Left/Right arrows to change the option", 0xB0);
        g_Console.writeToBuffer(25, 7, "Press esc for Main Menu", 0xB0);
        // Choosing option
        if (g_skKeyEvent[K_UP].keyReleased && settingsOption > 1)
        {
            settingsOption--;
        }
        // Choosing option
        if (g_skKeyEvent[K_DOWN].keyReleased && settingsOption < maxSettings)
        {
            settingsOption++;
        }
        // Change Map Option
        if (settingsOption == 1)
        {
            g_Console.writeToBuffer(31, 12, " Map Selection: " + std::to_string(whichMap) + " ", 0xF0);
            g_Console.writeToBuffer(20, 14, colourTxt, 0xB0);
            // Choosing Map
            if (g_skKeyEvent[K_LEFT].keyReleased)
            {
                if (whichMap > 1)
                {
                    whichMap--;
                }
            }
            if (g_skKeyEvent[K_RIGHT].keyReleased)
            {
                if (whichMap < totalMaps)
                {
                    whichMap++;
                }
            }
        }
        // Changing Map Colour Option
        if ( settingsOption == 2)
        {
            g_Console.writeToBuffer(31, 12, " Map Selection: " + std::to_string(whichMap) + " ", 0xB0);
            g_Console.writeToBuffer(20, 14, colourTxt, 0xF0);
            // Choosing Colour
            if (g_skKeyEvent[K_LEFT].keyReleased)
            {
                if (colourOption > 1)
                {
                    colourOption--;
                }
                renderMapColour();
            }
            if (g_skKeyEvent[K_RIGHT].keyReleased)
            {
                if (colourOption < maxCOption)
                {
                    colourOption++;
                }
                renderMapColour();
            }
        }
    }

    // Tutorial
    else if (mapNum == 3 && mapSel == true)
    {
        g_Console.clearBuffer();
        std::ifstream mapFile;
        mapFile.open("TutorialMap.txt", std::ifstream::in);

        for (int y = 0; y < 26; y++)
        {
            for (int x = 0; x < 81; x++)
            {
                char c = mapFile.get();
                mapArray[x][y] = c;

                // Renders the wall and ground near players
                if (((pow(x - g_sChar.m_cLocation.X, 2) + pow(y - g_sChar.m_cLocation.Y, 2) * 2) <= 36) || (pow(x - g_sChar2.m_cLocation.X, 2) + pow(y - g_sChar2.m_cLocation.Y, 2) * 2) <= 36)
                {
                    if (c == '1')
                    {
                        g_Console.writeToBuffer(x, y, " °±²Û", wall);
                    }
                    else if (c == '0' || c == '2')
                    {
                        g_Console.writeToBuffer(x, y, " °±²Û", ground);
                    }
                }

                // Turns everything else dark
                else
                {
                    g_Console.writeToBuffer(x, y, " °±²Û", 0x00);
                }

                for (int n = 0; n < 10; n++)
                {
                    // // Renders the wall and ground near burning NPCs
                    if ((static_cast<npc*>(npcPtr[n])->getSecsOnFire() > 0 && (pow(x - (static_cast<npc*>(npcPtr[n])->getCoords()).X, 2) + pow(y - (static_cast<npc*>(npcPtr[n])->getCoords()).Y, 2) * 2 <= 16)))
                    {
                        if (c == '1')
                        {
                            g_Console.writeToBuffer(x, y, " °±²Û", wall);
                        }
                        else if (c == '0' || c == '2')
                        {
                            g_Console.writeToBuffer(x, y, " °±²Û", ground);
                        }
                    }
                }
            }
        }
        mapFile.close();

        // Fireboy tutorial check
        if (g_sChar.m_cLocation.X == 35 && g_sChar.m_cLocation.Y == 9)
        {
            showFcontrols = false;
            showFnpc = true;
        }
        if (g_sChar.m_cLocation.X == 51 && g_sChar.m_cLocation.Y == 11)
        {
            showFnpc = false;
            showFnpc2 = true;
        }
        if (g_sChar.m_cLocation.X == 67 && g_sChar.m_cLocation.Y == 11)
        {
            showFnpc2 = false;
            showFability = true;
        }
        // Waterboy tutorial check
        if (npcPtr[0]->getAlive() == false && npcPtr[1]->getAlive() == false && npcPtr[2]->getAlive() == false && npcPtr[3]->getAlive() == false)
        {
            showFability = false;
            showFobjective = true;
            showWcontrols = true;
        }
        if (g_sChar2.m_cLocation.X == 35 && g_sChar2.m_cLocation.Y == 15)
        {
            showWcontrols = false;
            shownWcontrols = true;
            showWnpc = true;
        }
        if (g_sChar2.m_cLocation.X == 51 && g_sChar2.m_cLocation.Y == 13)
        {
            showWnpc = false;
            showWnpc2 = true;
        }
        if (g_sChar2.m_cLocation.X == 67 && g_sChar2.m_cLocation.Y == 13)
        {
            showWnpc2 = false;
            showWability = true;
            shownWability = true;
        }
        if (static_cast<npc*>(npcPtr[5])->getCol() == 0xA0 && static_cast<npc*>(npcPtr[6])->getCol() == 0xA0 && static_cast<npc*>(npcPtr[7])->getCol() == 0xA0 && static_cast<npc*>(npcPtr[8])->getCol() == 0xA0 && shownWability == true)
        {
            showWability = false;
            showWobjective = true;
        }

        // Fireboy tutorial
        if (showFcontrols == true)
        {
            g_Console.writeToBuffer(2, 7, "Fireboy Movement:", 0x0F);
            g_Console.writeToBuffer(9, 8, "WASD", 0x0F);
        }
        if (showFnpc == true)
        {
            g_Console.writeToBuffer(35, 3, "This is an NPC", 0x0F);
            g_Console.writeToBuffer(25, 4, "The NPC will run away from Fireboy", 0x0F);
            g_Console.writeToBuffer(30, 5, "Press R to shoot the NPC", 0x0F);
        }
        if (showFnpc2 == true)
        {
            g_Console.writeToBuffer(15, 4, "As Fireboy, You can place traps by pressing F", 0x0F);
            g_Console.writeToBuffer(5, 5, "If Waterboy runs into it, they'll be frozen in place for a few seconds", 0x0F);
            g_Console.writeToBuffer(23, 6, "(Max of 3 traps can be placed)", 0x0F);
        }
        if (showFability == true)
        {
            g_Console.writeToBuffer(27, 3, "Press T to activate your ability", 0x0F);
            g_Console.writeToBuffer(22, 4, "then press R to burn multiple NPCs at once!", 0x0F);
            g_Console.writeToBuffer(16, 5, "It burns NPCs in a small circle around the first NPC shot", 0x0F);
        }
        if (showFobjective == true)
        {
            g_Console.writeToBuffer(5, 4, "Your mission is to kill every NPC without getting caught by Waterboy", 0x0F);
        }
        // Waterboy tutorial
        if (showWcontrols == true && shownWcontrols == false)
        {
            g_Console.writeToBuffer(2, 16, "Waterboy Movement:", 0x0F);
            g_Console.writeToBuffer(6, 17, "Arrow Keys", 0x0F);
        }
        if (showWnpc == true)
        {
            g_Console.writeToBuffer(35, 20, "This is an NPC", 0x0F);
            g_Console.writeToBuffer(23, 21, "NPCs do not run away from Waterboy", 0x0F);
            g_Console.writeToBuffer(30, 22, "Press , to drench the NPC", 0x0F);
            g_Console.writeToBuffer(21, 23, "Drenching an NPC puts out a burning NPC and", 0x0F);
            g_Console.writeToBuffer(19, 24, "prevents it from catching fire for a few seconds", 0x0F);
        }
        if (showWnpc2 == true)
        {
            g_Console.writeToBuffer(15, 21, "You can also walk into the NPC to drench it or put it out", 0x0F);
            g_Console.writeToBuffer(0, 22, "Note: NPCs will move and light up their surroundings when on fire in actual game", 0x0F);
            
        }   
            
        if (showWability == true)
        {
            g_Console.writeToBuffer(24, 20, "Press . to activate your ability", 0x0F);
            g_Console.writeToBuffer(18, 21, "then press , to drench multiple NPCs at once!", 0x0F);
            g_Console.writeToBuffer(5, 22, "It drenches other NPCs that are in a small range around the first NPC", 0x0F);
            g_Console.writeToBuffer(12, 14, "Waterboy can also place traps by pressing /", 0x0F);
            g_Console.writeToBuffer(12, 15, "(Max of 3 traps be placed)", 0x0F);
            g_Console.writeToBuffer(12, 16, "These traps decreases Fireboy's", 0x0F);
            g_Console.writeToBuffer(12, 17, "total lives by 1 if they step on it and", 0x0F);
            g_Console.writeToBuffer(12, 18, "drench any nearby NPCs", 0x0F);
        }
        if (showWobjective == true && shownWability == true)
        {
            g_Console.writeToBuffer(3, 20, "Your mission is to put out Fireboy and prevent him from killing every NPC", 0x0F);
            g_Console.writeToBuffer(20, 21, "Press esc to go back to the main menu", 0x0F);
        }
    }
}

void renderMapColour()
{
    // Map Colour Scheme 1
    if (colourOption == 1)
    {
        wallTxt = "White";
        wall = 0xF6;
        groundTxt = "Blue";
        ground = 0x1B;
    }
    // Map Colour Scheme 2
    else if (colourOption == 2)
    {
        wallTxt = "Green";
        wall = 0x20;
        groundTxt = "Gray";
        ground = 0x8B;
    }
    // Map Colour Scheme 3
    else if (colourOption == 3)
    {
        wallTxt = "Black";
        wall = 0x00;
        groundTxt = "White";
        ground = 0xFB;
    }
    // Map Colour Scheme 4
    else if (colourOption == 4)
    {
        wallTxt = "White";
        wall = 0xFF;
        groundTxt = "White";
        ground = 0xFB;
    }
    // Map Colour Scheme 5
    else if (colourOption == 5)
    {
        wallTxt = "Black";
        wall = 0x00;
        groundTxt = "Black";
        ground = 0x0B;
    }
}

// Changing the colours of Fire boy and Water boy
void renderCharacter()
{
    // Draw the location of the character
    g_Console.writeToBuffer(g_sPjtl.m_cLocation, ' ', 0x4F);
    g_Console.writeToBuffer(g_sPjtl2.m_cLocation, ' ', 0x90);
    g_Console.writeToBuffer(g_sChar.m_cLocation, 'F', 0x4F);
    g_Console.writeToBuffer(g_sChar2.m_cLocation, 'W', 0x90);
    //Draw the location of Waterboy traps
    for (int t = 0; t < 3; t++)
    {
        if (WBTraps[t]->getAlive() == true && (pow(WBTraps[t]->getCoords().X - g_sChar.m_cLocation.X, 2) + pow(WBTraps[t]->getCoords().Y - g_sChar.m_cLocation.Y, 2) * 2 <= 36 || pow(WBTraps[t]->getCoords().X - g_sChar2.m_cLocation.X, 2) + pow(WBTraps[t]->getCoords().Y - g_sChar2.m_cLocation.Y, 2) * 2 <= 36))
        {
            g_Console.writeToBuffer(WBTraps[t]->getCoords(), 'T', ground);
        }
    }

    // Draw the location of FTrap
    for (int t = 0; t < 3; t++)
    {
        if (FtrapPtr[t]->getAlive() == true && (pow(FtrapPtr[t]->getCoords().X - g_sChar.m_cLocation.X, 2) + pow(FtrapPtr[t]->getCoords().Y - g_sChar.m_cLocation.Y, 2) * 2 <= 36 || pow(FtrapPtr[t]->getCoords().X - g_sChar2.m_cLocation.X, 2) + pow(FtrapPtr[t]->getCoords().Y - g_sChar2.m_cLocation.Y, 2) * 2 <= 36))
        {
            g_Console.writeToBuffer(FtrapPtr[t]->getCoords(), 'T', ground);
        }
    }

    if (fA == false)
    {
        g_Console.writeToBuffer(g_sPjtl.m_cLocation, ' ', 0x4F);
        g_Console.writeToBuffer(g_sChar.m_cLocation, 'F', 0x4F);
    }
    else if (fA == true)
    {
        g_Console.writeToBuffer(g_sPjtl.m_cLocation, ' ', 0xCF);
        g_Console.writeToBuffer(g_sChar.m_cLocation, 'F', 0xCF);
    }
    if (wA == false)
    {
        g_Console.writeToBuffer(g_sPjtl2.m_cLocation, ' ', 0x90);
        g_Console.writeToBuffer(g_sChar2.m_cLocation, 'W', 0x90);
    }
    else if (wA == true)
    {
        g_Console.writeToBuffer(g_sPjtl2.m_cLocation, ' ', 0xB0);
        g_Console.writeToBuffer(g_sChar2.m_cLocation, 'W', 0xB0);
    }
}

void renderNPC()
{
    for (int n = 0; n < 10; n++)
    {
        updateNPC(n);

        if (npcPtr[n]->getAlive() == true)
        {
            if (pow(npcPtr[n]->getCoords().X - g_sChar.m_cLocation.X, 2) + pow(npcPtr[n]->getCoords().Y - g_sChar.m_cLocation.Y, 2) * 2 <= 36 || pow(npcPtr[n]->getCoords().X - g_sChar2.m_cLocation.X, 2) + pow(npcPtr[n]->getCoords().Y - g_sChar2.m_cLocation.Y, 2) * 2 <= 36)
            {
                g_Console.writeToBuffer(npcPtr[n]->getCoords(), 'N', static_cast<npc*>(npcPtr[n])->getCol());
            }
            if (static_cast<npc*>(npcPtr[n])->getSecsOnFire() > 0)
            {
                g_Console.writeToBuffer(npcPtr[n]->getCoords(), 'N', static_cast<npc*>(npcPtr[n])->getCol());
            }
        }
        else
        {
            g_Console.writeToBuffer(npcPtr[n]->getCoords(), ' ', 0x00);
        }
    }
}

bool Collision(COORD position, char direction)//function for collision
{
    if (direction == 'U') // direction given is U 
    {
        if (mapArray[position.X][position.Y - 1] == '1')//if Position of entity Y - 1 on map = '1' 
        {
            return true;//return true to say that its a wall and cannot go through
        }
        return false;//else return false
    }
    if (direction == 'D')// direction given is D
    {
        if (mapArray[position.X][position.Y + 1] == '1')
        {
            return true;//return true to say that its a wall and cannot go through
        }
        return false;//else return false
    }
    if (direction == 'L')// direction given is L
    {
        if (mapArray[position.X - 1][position.Y] == '1')//if Position of entity Y - 1 on map = '1' 
        {
            return true;//return true to say that its a wall and cannot go through
        }
        return false;//else return false
    }
    if (direction == 'R')// direction given is R
    {
        if (mapArray[position.X + 1][position.Y] == '1')//if Position of entity Y - 1 on map = '1' 
        {
            return true;//return true to say that its a wall and cannot go through
        }
        return false;//else return false
    }
    return false;//else return false
}
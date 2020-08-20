﻿// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include "entity.h"
#include "npc.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <fstream>


double  g_dElapsedTime;
double  g_dDeltaTime;
int lastMove;
int lastMove2;
int doneShoot = 0;
int rOrC;
SKeyEvent g_skKeyEvent[K_COUNT];
SMouseEvent g_mouseEvent;
WORD npcCol = 0xB0;
// NPC related stopwatch
CStopWatch fireWatch;
double secsPassed = 0;

// Game specific variables here
SGameChar   g_sChar;
SGameChar   g_sPjtl;
SGameChar   g_sChar2;
SGameChar   g_sPjtl2;
EGAMESTATES g_eGameState = S_SPLASHSCREEN; // initial state
npc         npc1(10, 10);

// Console object
Console g_Console(80, 25, "SP1 Framework");

//--------------------------------------------------------------
// Purpose  : Initialisation function
//            Initialize variables, allocate memory, load data from file, etc. 
//            This is called once before entering into your main loop
// Input    : void
// Output   : void
//--------------------------------------------------------------
void init( void )
{
    // Set precision for floating point output
    g_dElapsedTime = 0.0;    

    // sets the initial state for the game
    g_eGameState = S_SPLASHSCREEN;

    // Zavier's
    /*
    g_sChar.m_cLocation.X = g_Console.getConsoleSize().X / 44;
    g_sChar.m_cLocation.Y = g_Console.getConsoleSize().Y / 1.05;
    g_sChar2.m_cLocation.X = g_Console.getConsoleSize().X / 1.025;
    g_sChar2.m_cLocation.Y = g_Console.getConsoleSize().Y / 15;
    */
    // Tutorial Level
    g_sChar.m_cLocation.X = g_Console.getConsoleSize().X / 7;
    g_sChar.m_cLocation.Y = g_Console.getConsoleSize().Y / 1.25;
    g_sChar.m_bActive = true;
    g_sChar2.m_cLocation.X = g_Console.getConsoleSize().X / 1.2;
    g_sChar2.m_cLocation.Y = g_Console.getConsoleSize().Y / 1.2;
    g_sChar2.m_bActive = true;

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
void shutdown( void )
{
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
void getInput( void )
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
    case VK_UP: key = K_UP; break;
    case VK_DOWN: key = K_DOWN; break;
    case VK_LEFT: key = K_LEFT; break; 
    case VK_RIGHT: key = K_RIGHT; break;
    case VK_OEM_COMMA: key = K_COMMA; break;
    case VK_OEM_PERIOD: key = K_PERIOD; break;
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
        case S_SPLASHSCREEN : splashScreenWait(); // game logic for the splash screen
            break;
        case S_GAME: updateGame(); // gameplay logic when we are in the game
            break;
    }
}


void splashScreenWait()    // waits for time to pass in splash screen
{
    if (g_dElapsedTime > 3.0) // wait for 3 seconds to switch to game mode, else do nothing
        g_eGameState = S_GAME;
}

void updateGame()       // gameplay logic
{
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    moveCharacter();    // moves the character, collision detection, physics, etc
                        // sound can be played here too.
    moveNPC();
}

void moveCharacter()
{
    // Updating the location of the character based on the key release
    // providing a beep sound whenver we shift the character
    if (doneShoot == 0)
    {
        if (g_skKeyEvent[K_W].keyReleased && g_sChar.m_cLocation.Y > 0)
        {
            //Beep(1440, 30);
            g_sChar.m_cLocation.Y--;
            g_sPjtl.m_cLocation.X = g_sChar.m_cLocation.X;
            g_sPjtl.m_cLocation.Y = g_sChar.m_cLocation.Y;
            lastMove = 1;
        }
        if (g_skKeyEvent[K_A].keyReleased && g_sChar.m_cLocation.X > 0)
        {
            //Beep(1440, 30);
            g_sChar.m_cLocation.X--;
            g_sPjtl.m_cLocation.X = g_sChar.m_cLocation.X;
            g_sPjtl.m_cLocation.Y = g_sChar.m_cLocation.Y;
            lastMove = 2;
        }
        if (g_skKeyEvent[K_S].keyReleased && g_sChar.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
        {
            //Beep(1440, 30);
            g_sChar.m_cLocation.Y++;
            g_sPjtl.m_cLocation.X = g_sChar.m_cLocation.X;
            g_sPjtl.m_cLocation.Y = g_sChar.m_cLocation.Y;
            lastMove = 3;
        }
        if (g_skKeyEvent[K_D].keyReleased && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 1)
        {
            //Beep(1440, 30);
            g_sChar.m_cLocation.X++;
            g_sPjtl.m_cLocation.X = g_sChar.m_cLocation.X;
            g_sPjtl.m_cLocation.Y = g_sChar.m_cLocation.Y;
            lastMove = 4;
        }
        if (g_skKeyEvent[K_R].keyReleased)
        {
            rOrC = 1;
            doneShoot++;
        }
        if (g_skKeyEvent[K_UP].keyReleased && g_sChar2.m_cLocation.Y > 0)
        {
            //Beep(1440, 30);
            g_sChar2.m_cLocation.Y--;
            g_sPjtl2.m_cLocation.X = g_sChar2.m_cLocation.X;
            g_sPjtl2.m_cLocation.Y = g_sChar2.m_cLocation.Y;
            lastMove2 = 1;
        }
        if (g_skKeyEvent[K_LEFT].keyReleased && g_sChar2.m_cLocation.X > 0)
        {
            //Beep(1440, 30);
            g_sChar2.m_cLocation.X--;
            g_sPjtl2.m_cLocation.X = g_sChar2.m_cLocation.X;
            g_sPjtl2.m_cLocation.Y = g_sChar2.m_cLocation.Y;
            lastMove2 = 2;
        }
        if (g_skKeyEvent[K_DOWN].keyReleased && g_sChar2.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
        {
            //Beep(1440, 30);
            g_sChar2.m_cLocation.Y++;
            g_sPjtl2.m_cLocation.X = g_sChar2.m_cLocation.X;
            g_sPjtl2.m_cLocation.Y = g_sChar2.m_cLocation.Y;
            lastMove2 = 3;
        }
        if (g_skKeyEvent[K_RIGHT].keyReleased && g_sChar2.m_cLocation.X < g_Console.getConsoleSize().X - 1)
        {
            //Beep(1440, 30);
            g_sChar2.m_cLocation.X++;
            g_sPjtl2.m_cLocation.X = g_sChar2.m_cLocation.X;
            g_sPjtl2.m_cLocation.Y = g_sChar2.m_cLocation.Y;
            lastMove2 = 4;
        }
        if (g_skKeyEvent[K_COMMA].keyReleased)
        {
            rOrC = 0;
            doneShoot++;
        }
        if (g_skKeyEvent[K_SPACE].keyReleased)
        {
            // g_sChar.m_bActive = !g_sChar.m_bActive;
            // g_sChar2.m_bActive = !g_sChar2.m_bActive;
        }
    }
    else if (doneShoot > 0 && doneShoot < 11)
    {
        // Fire boy Shooting
        if (rOrC == 1)
        {
            if (lastMove == 1)
                g_sPjtl.m_cLocation.Y -= 1;
            else if (lastMove == 2)
                g_sPjtl.m_cLocation.X -= 1;
            else if (lastMove == 3)
                g_sPjtl.m_cLocation.Y += 1;
            else if (lastMove == 4)
                g_sPjtl.m_cLocation.X += 1;
        }
        // Water boy Shooting
        else if (rOrC == 0)
        {
            if (lastMove2 == 1)
                g_sPjtl2.m_cLocation.Y -= 1;
            else if (lastMove2 == 2)
                g_sPjtl2.m_cLocation.X -= 1;
            else if (lastMove2 == 3)
                g_sPjtl2.m_cLocation.Y += 1;
            else if (lastMove2 == 4)
                g_sPjtl2.m_cLocation.X += 1;
        }
        doneShoot++;
    }
    else if (doneShoot == 11)
    {
        g_sPjtl.m_cLocation.X = g_sChar.m_cLocation.X;
        g_sPjtl.m_cLocation.Y = g_sChar.m_cLocation.Y;
        g_sPjtl2.m_cLocation.X = g_sChar2.m_cLocation.X;
        g_sPjtl2.m_cLocation.Y = g_sChar2.m_cLocation.Y;
        doneShoot = 0;
    }
}

void moveNPC()
{
    if (npc1.getSecsOnFire() > 0)
    {
        secsPassed += fireWatch.getElapsedTime();

        if (secsPassed > 0.33)
        {
            // Randomly runs
            int randomInt = rand() % 4 + 1;
            if (randomInt == 1) // Up
            {
                if (npc1.getCoords().Y - 1 >= 0)
                {
                    npc1.setCoords(npc1.getCoords().X, npc1.getCoords().Y - 1);
                }
            }
            else if (randomInt == 2) // Down
            {
                if (npc1.getCoords().Y + 1 <= g_Console.getMaxConsoleSize().Y - 1)
                {
                    npc1.setCoords(npc1.getCoords().X, npc1.getCoords().Y + 1);
                }
            }
            else if (randomInt == 3) // Left
            {
                if (npc1.getCoords().X - 1 >= 0)
                {
                    npc1.setCoords(npc1.getCoords().X - 1, npc1.getCoords().Y);
                }
            }
            else // Right
            {
                if (npc1.getCoords().X + 1 >= 0)
                {
                    npc1.setCoords(npc1.getCoords().X + 1, npc1.getCoords().Y);
                }
            }
            npc1.setSecsOnFire(npc1.getSecsOnFire() - 0.33);
            if (npc1.getSecsOnFire() <= 0)
            {
                npc1.setAlive(false);
            }
            secsPassed = 0;
        }
    }

    else if (npc1.getAlive() == true)
    {
        // check if player is in range of NPC
        if ((pow(g_sChar.m_cLocation.X - npc1.getCoords().X, 2) + pow(g_sChar.m_cLocation.Y - npc1.getCoords().Y, 2) * 2) <= 25)
        {
            int npc1L, npc1R, npc1U, npc1D;
            npc1L = npc1.getCoords().X - 1;
            npc1R = npc1.getCoords().X + 1;
            npc1U = npc1.getCoords().Y - 1;
            npc1D = npc1.getCoords().Y + 1;

            npc1L = (pow(g_sChar.m_cLocation.X - npc1L, 2) + pow(g_sChar.m_cLocation.Y - npc1.getCoords().Y, 2));
            npc1R = (pow(g_sChar.m_cLocation.X - npc1R, 2) + pow(g_sChar.m_cLocation.Y - npc1.getCoords().Y, 2));
            npc1U = (pow(g_sChar.m_cLocation.X - npc1.getCoords().X, 2) + pow(g_sChar.m_cLocation.Y - npc1U, 2));
            npc1D = (pow(g_sChar.m_cLocation.X - npc1.getCoords().X, 2) + pow(g_sChar.m_cLocation.Y - npc1D, 2));

            if (npc1L < npc1R && npc1L < npc1D && npc1L < npc1U && npc1.getCoords().X + 1 <= g_Console.getConsoleSize().X - 1)
            {
                npc1.setCoords(npc1.getCoords().X + 1, npc1.getCoords().Y);
            }
            else if (npc1R < npc1L && npc1R < npc1D && npc1R < npc1U && npc1.getCoords().X - 1 >= 0)
            {
                npc1.setCoords(npc1.getCoords().X - 1, npc1.getCoords().Y);
            }
            else if (npc1U < npc1R && npc1U < npc1D && npc1U < npc1L && npc1.getCoords().Y + 1 <= g_Console.getConsoleSize().Y - 1)
            {
                npc1.setCoords(npc1.getCoords().X, npc1.getCoords().Y + 1);
            }
            else if (npc1D < npc1R && npc1D < npc1L && npc1D < npc1U && npc1.getCoords().Y - 1 >= 0)
            {
                npc1.setCoords(npc1.getCoords().X, npc1.getCoords().Y - 1);
            }
        }
    }
}

void processUserInput()
{
    // quits the game if player hits the escape key
    if (g_skKeyEvent[K_ESCAPE].keyReleased)
        g_bQuitGame = true;
}

void updateNPC()
{
    if (g_sPjtl.m_cLocation.X == npc1.getCoords().X && g_sPjtl.m_cLocation.Y == npc1.getCoords().Y && npc1.getAlive() == true && npc1.getSecsOnFire() <= 0)
    {
        npc1.setSecsOnFire(5);
        npcCol = 0x4C;
        
        fireWatch.startTimer();
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
    case S_SPLASHSCREEN: renderSplashScreen();
        break;
    case S_GAME: renderGame();
        break;
    }
    renderFramerate();      // renders debug information, frame rate, elapsed time, etc
    renderInputEvents();    // renders status of input events
    renderToScreen();       // dump the contents of the buffer to the screen, one frame worth of game
}

void clearScreen()
{
    // Clears the buffer with this colour attribute
    g_Console.clearBuffer(0x1F);
}

void renderToScreen()
{
    // Writes the buffer to the console, hence you will see what you have written
    g_Console.flushBufferToConsole();
}

void renderSplashScreen()  // renders the splash screen
{
    // Main Menu
    COORD c = g_Console.getConsoleSize();
    c.Y /= 3;
    c.X = c.X / 2 - 3;
    g_Console.writeToBuffer(c, "Start", 0x03);
    c.Y += 7;
    c.X = g_Console.getConsoleSize().X / 2 - 6;
    g_Console.writeToBuffer(c, "Instructions", 0x09);
}

void renderGame()
{
    renderMap();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
    renderNPC();
}

void renderMap()
{
    std::ifstream mapFile;
    mapFile.open("TutorialMap.txt", std::ifstream::in);

    for (int y = 0; y < 80; y++)
    {
        for (int x = 0; x < 81; x++)
        {
            char c = mapFile.get();
            
            if (((pow(x - g_sChar.m_cLocation.X, 2) + pow(y - g_sChar.m_cLocation.Y, 2) * 2) <= 36) || (pow(x - g_sChar2.m_cLocation.X, 2) + pow(y - g_sChar2.m_cLocation.Y, 2) * 2) <= 36)
            {
                if (c == '1')
                {
                    g_Console.writeToBuffer(x, y, " °±²Û", 0xF6);
                }
                else if (c == '0')
                {
                    g_Console.writeToBuffer(x, y, " °±²Û", 0x1B);
                }
            }

            else
            {
                g_Console.writeToBuffer(x, y, " °±²Û", 0x00);
            }
        }
    }
    mapFile.close();

   /*
    // Set up sample colours, and output shadings
    const WORD colors[] = {
        0x1B, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
        0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
    };

    COORD c;
    for (int i = 0; i < 12; ++i)
    {
        c.X = 20 * i;
        c.Y = i + 0;
        colour(colors[i]);
        g_Console.writeToBuffer(c, " °±²Û", colors[i]);
    }
    */
}

void renderCharacter()
{
    // Draw the location of the character
    WORD charColor = 0x4F;
    WORD charColor2 = 0x90;
    //if (g_sChar.m_bActive)
        //charColor = 0x0A;
    g_Console.writeToBuffer(g_sChar.m_cLocation, (char)1, charColor);
    g_Console.writeToBuffer(g_sPjtl.m_cLocation, (char)1, charColor);
    g_Console.writeToBuffer(g_sChar2.m_cLocation, (char)1, charColor2);
    g_Console.writeToBuffer(g_sPjtl2.m_cLocation, (char)1, charColor2);
}

void renderNPC()
{
    updateNPC();
    if (npc1.getAlive() == true)
    {
        if (pow(npc1.getCoords().X - g_sChar.m_cLocation.X, 2) + pow(npc1.getCoords().X - g_sChar.m_cLocation.Y, 2) * 2 <= 36)
        {
            g_Console.writeToBuffer(npc1.getCoords(), 'N', npcCol);
        }
        if (npc1.getSecsOnFire() > 0)
        {
            g_Console.writeToBuffer(npc1.getCoords(), 'N', npcCol);
        }
    }
    else
    {
        g_Console.writeToBuffer(npc1.getCoords(), ' ', 0x00);
    }
}

void renderFramerate()
{
    COORD c;
    // displays the framerate
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(3);
    ss << 1.0 / g_dDeltaTime << "fps";
    c.X = g_Console.getConsoleSize().X - 9;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str());

    // displays the elapsed time
    ss.str("");
    ss << g_dElapsedTime << "secs";
    c.X = 0;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str(), 0x59);
}

// this is an example of how you would use the input events
void renderInputEvents()
{
    // keyboard events
    COORD startPos = {50, 2};
    std::ostringstream ss;
    std::string key;
    /*for (int i = 0; i < K_COUNT; ++i)
    {
        ss.str("");
        switch (i)
        {
        case K_W: key = "W";
            break;
        case K_A: key = "A";
            break;
        case K_S: key = "S";
            break;
        case K_D: key = "D";
            break;
        case K_R: key = "R";
            break;
        case K_T: key = "T";
            break;
        case K_UP: key = "UP";
            break;
        case K_DOWN: key = "DOWN";
            break;
        case K_LEFT: key = "LEFT";
            break;
        case K_RIGHT: key = "RIGHT";
            break;
        case K_COMMA: key = ",";
            break;
        case K_PERIOD: key = ".";
            break;
        case K_SPACE: key = "SPACE";
            break;
        default: continue;
        }
        if (g_skKeyEvent[i].keyDown)
            ss << key << " pressed";
        else if (g_skKeyEvent[i].keyReleased)
            ss << key << " released";
        else
            ss << key << " not pressed";

        COORD c = { startPos.X, startPos.Y + i };
        g_Console.writeToBuffer(c, ss.str(), 0x17);
    }*/

    // mouse events    
    ss.str("");
    /*ss << "Mouse position (" << g_mouseEvent.mousePosition.X << ", " << g_mouseEvent.mousePosition.Y << ")";*/
    g_Console.writeToBuffer(g_mouseEvent.mousePosition, ss.str(), 0x59);
    ss.str("");
   /* switch (g_mouseEvent.eventFlags)
    {
    case 0:
        if (g_mouseEvent.buttonState == FROM_LEFT_1ST_BUTTON_PRESSED)
        {
            ss.str("Left Button Pressed");
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 1, ss.str(), 0x59);
        }
        else if (g_mouseEvent.buttonState == RIGHTMOST_BUTTON_PRESSED)
        {
            ss.str("Right Button Pressed");
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 2, ss.str(), 0x59);
        }
        else
        {
            ss.str("Some Button Pressed");
            g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 3, ss.str(), 0x59);
        }
        break;
    case DOUBLE_CLICK:
        ss.str("Double Clicked");
        g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 4, ss.str(), 0x59);
        break;        
    case MOUSE_WHEELED:
        if (g_mouseEvent.buttonState & 0xFF000000)
            ss.str("Mouse wheeled down");
        else
            ss.str("Mouse wheeled up");
        g_Console.writeToBuffer(g_mouseEvent.mousePosition.X, g_mouseEvent.mousePosition.Y + 5, ss.str(), 0x59);
        break;*/
   /* default:        
        break;
    }*/
    
}




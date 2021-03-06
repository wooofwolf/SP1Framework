#ifndef _GAME_H
#define _GAME_H

#include "Framework\timer.h"

extern CStopWatch g_swTimer;
extern bool g_bQuitGame;

// struct to store keyboard events
// a small subset of KEY_EVENT_RECORD
struct SKeyEvent
{
    bool keyDown;
    bool keyReleased;
};

// struct to store mouse events
// a small subset of MOUSE_EVENT_RECORD
struct SMouseEvent
{
    COORD mousePosition;
    DWORD buttonState;
    DWORD eventFlags;
};

// Enumeration to store the control keys that your game will have
enum EKEYS
{
    K_W,
    K_A,
    K_S,
    K_D,
    K_R,
    K_T,
    K_F,
    K_UP,
    K_DOWN,
    K_LEFT,
    K_RIGHT,
    K_COMMA,
    K_PERIOD,
    K_DIVIDE,
    K_ESCAPE,
    K_1,
    K_2,
    K_3,
    K_SPACE,
    K_COUNT
};

// Enumeration for the different screen states
enum EGAMESTATES
{
    S_SPLASHSCREEN,
    S_GAME,
    S_COUNT
};

// struct for the game character
struct SGameChar
{
    COORD m_cLocation;
    bool  m_bActive;
};

void init        ( void );      // initialize your variables, allocate memory, etc
void getInput    ( void );      // get input from player
void update      ( double dt ); // update the game and the state of the game
void render      ( void );      // renders the current state of the game to the console
void shutdown    ( void );      // do clean up, free memory


void splashScreenWait();    // waits for time to pass in splash screen
void updateGame();          // gameplay logic
void moveCharacter();       // moves the character, collision detection, physics, etc
void tpProj1();      // tp projectile to character
void tpProj2();      // tp projectile2 to character2
void charAbility();
void drenchNpc(int sd);
void moveNPC();             // checks how far the player is from npc, npc moves if player is close.
void processUserInput();    // checks if you should change states or do something else with the game, e.g. pause, exit
void updateNPC(int n);           // checks if the NPC gets caught on fire
void clearScreen();         // clears the current screen and draw from scratch 
void renderGame();          // renders the game stuff
void renderMap();           // renders the map to the buffer first
void renderMapColour();
void renderCharacter();     // renders the character into the buffer
void renderNPC();           // renders npc into the buffer
void renderToScreen();      // dump the contents of the buffer to the screen, one frame worth of game
bool Collision(COORD position, char direction);
// keyboard and mouse input event managers
void keyboardHandler(const KEY_EVENT_RECORD& keyboardEvent);  // define this function for the console to call when there are keyboard events
void mouseHandler(const MOUSE_EVENT_RECORD& mouseEvent);      // define this function for the console to call when there are mouse events

void gameplayKBHandler(const KEY_EVENT_RECORD& keyboardEvent);   // handles keyboard events for gameplay 
void gameplayMouseHandler(const MOUSE_EVENT_RECORD& mouseEvent); // handles mouse events for gameplay 


#endif // _GAME_H
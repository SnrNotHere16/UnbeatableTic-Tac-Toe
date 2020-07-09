/* 
GPIO ports
	1. 18 outputs  (LED) 
		a. set1(PB0-PB7, PA4)
		b. set2(PD2-PD3, PD6-PD7,  PC5-PC7, PE0,PE3)
	2. 3 inputs (Buttons) (PF0,PF4)
	3. 2 inputs (ADC) (PE1-PE2)
	4. 2 outputs (PWM) (PC4,PE4)
*/ 
/* 
Declarations for the Tic Tac Toe game functions. 
*/ 
#include "tm4c123gh6pm.h"
#include <stdint.h>
#include <stdbool.h>
 #ifndef TICTACTOE_H
 #define TICTACTOE_H
//The player is met with this function. The beginning of the game. 
void GameStart(void);
//The game plays with a dumb AI.
void GamePlayDumbAI(void);
//The game plays witha smart AI
void GamePlaySmartAI(void);
//The game ends. 
void GameOver(void); 
//Reset the Tic Tac Toe board 
void ResetBoard(void); 
//Reset flags and status
void ResetFlags(void);
//Check if the game has ended
// check - true -> check player
// check - false -> chek bot
bool CheckGameOver(bool check);
//Clears the LEDS
void ClearAllLeds(void);
//Light up all LEDS
void LightAllLeds(void);
//Light up the Analog LED position 
void LightAnalogLed(void);
//Light up the player's LEDs
void LightPlayerLeds(void);
//Light up the Bot's LEDs
void LightBotLeds(void);
//Adds Player's position to board, returns 
//true if the tile was sucessfully added.
bool AddPosition(uint8_t position);
//Adds the bot's position automatically for dumb AI
void AddPositionBotAuto(void);
//Adds bot's positon smart
bool AddPositionBotSmart(uint8_t position);
//Search free positions, returns the position of the 
//tile to change is returned
int FreePosition(uint8_t position);
//Searches the player's or bot's array for 
// an element. 
// true - element exists
//false - element does not exist
bool SearchElement(int element, bool check);
//add score to win position values. 
void AddWinScore(bool check, uint8_t position);
//Makes the move of the smart AI
int MakeAIMove();
//AI checks for an Available Win
bool AvailableWin();
//AI checks for an Avaliable Block (win)
bool AvailableBlock();
//AI checks for an Available Fork
bool AvailableFork();
//AI checks for a fork to block
bool BlockFork();
//AI checks for an Available Center
bool AvailableCenter();
//AI checks for an opposite corner
bool AvailableOppositeCorner();
//AI checks for an Empty Corner
bool AvailableEmptyCorner();
//AI checks for an Empty Side
bool AvailableEmptySide();
// 10 ms Delay
void DelayWait10ms(uint32_t n);

#endif
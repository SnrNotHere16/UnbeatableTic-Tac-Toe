/* 
GPIO ports
	1. 18 outputs  (LED) 
		a. set1(PB0-PB7, PA4)
		b. set2(PD2-PD3, PD6-PD7,  PC5-PC7, PE0,PE3)
	2. 3 inputs (Buttons) (PF0,PF4,PE5)
	3. 2 inputs (ADC) (PE1-PE2)
	4. 2 outputs (PWM) (PC4,PE4)
*/ 
/* 
Definitions for the Tic Tac Toe game functions. 
*/ 
/***
Winning Score for Player:9
Winning Score for Bot: 12
Player's Score is incremented by: 3
Bot's Score is incremented by: 4
***/
#include "tm4c123gh6pm.h"
#include "TicTacToe.h"
#include "ST7735.h"
#include <stdint.h>
unsigned long In;
struct Positions {
	int freePositions[9];
	int takePositionsA[5];
	int takePositionsB[4];
	//Center wins 
	//[0,4,8],[1,4,7],[2,4,6],[3,4,5]
	uint8_t winCenter[4]; 
	//[0,3,6], [2,5,8]
	uint8_t winMod[2];
	//[0,1,2], [6,7,8]
	uint8_t winBelow[2];
};
struct LEDS {
	uint8_t PlayerLEDS [3][3];
	uint8_t BotLEDS [3][3];
};

struct Status {
	bool GameStart;
	bool GamePlayDumbAI;
	bool GamePlaySmartAI;
	bool gameOver;
	bool botWon;
	bool playerWon;
	uint8_t playerMoves;
	uint8_t freeTiles;
	uint8_t centerTile;
};

struct AnalogAttributes {
	uint8_t analogValueR;
	uint8_t analogValueC;
};

struct Positions gamePositions; 
struct Status gameStatus;
struct LEDS gameLeds = {
	{{0x01, 0x02, 0x04}, 
	{0x08, 0x10, 0x20},
  {0x40, 0x80, 0x10},},
	
	{{0x04, 0x08, 0x40}, 
	{0x80, 0x20, 0x40},
  {0x80, 0x01, 0x08},}
};
struct AnalogAttributes g = {
	 0,
	 0
};


//The player is met with this function. The beginning of the game. 
void GameStart(){
		ST7735_FillScreen(0x0);            // set screen to black
		ST7735_SetCursor(0,0);
		ResetFlags();
	//Reset the boards and positions 
	ResetBoard();
	ST7735_OutString("\n  TIC TAC TOE...\n  SW1: Dumb AI\n  SW2: Smart AI\n  Press to Play\n");
	while(!gameStatus.gameOver){
		In = GPIO_PORTE_DATA_R&0x10; // read PE4 into In
    if(In == 0x00){              // PE4 is low
				ClearAllLeds();
		} else{                      // PE4 is high
      LightAllLeds();
			}
	if (gameStatus.GamePlayDumbAI){
		  GamePlayDumbAI();
		ST7735_OutString("\n GameOver \n  ");
	}
	else if (gameStatus.GamePlaySmartAI){
		GamePlaySmartAI();
	}
}
	GameOver();
	
}
//The game plays with a dumb AI.
void GamePlayDumbAI(){
			ST7735_FillScreen(0x0);            // set screen to black
			ST7735_SetCursor(0,0);
			ST7735_OutString("\n  TIC TAC TOE...\n  Dumb AI\n  Play!\n");
			ClearAllLeds();
			while (!gameStatus.gameOver){
				LightAnalogLed();
				LightPlayerLeds();
				LightBotLeds();
			}
}
void GamePlaySmartAI(){
			ST7735_FillScreen(0x0);            // set screen to black
			ST7735_SetCursor(0,0);
			ST7735_OutString("\n  TIC TAC TOE...\n  Smart AI\n  Play SMART!\n");
			ClearAllLeds();
			while(!gameStatus.gameOver){
				LightAnalogLed();
				LightPlayerLeds();
				LightBotLeds();
			}
			
};
//The game ends. 
void GameOver(){
	ST7735_SetCursor(0,0);
  ST7735_FillScreen(0x0);
	PrintResults();
	ClearAllLeds();
	while(gameStatus.gameOver){
		if(gameStatus.botWon){
			LightBotLeds();
		}
		else if (gameStatus.playerWon){
			LightPlayerLeds();
		}
		else {
			LightPlayerLeds();
			LightBotLeds();
		}
	}
	}

void PrintResults(){
	if(gameStatus.GamePlayDumbAI)
		ST7735_OutString("\n  DumbAI MODE");
	else 
		ST7735_OutString("\n  SmartAI MODE");
	if(gameStatus.botWon)
		ST7735_OutString("\n  BOT WON");
	else if(gameStatus.playerWon)
		ST7735_OutString("\n  Player Won");
	else 
		ST7735_OutString("\n  Draw");
	ST7735_OutString("\n  PlayerMoves: ");
	ST7735_OutUDec(gameStatus.playerMoves);
	
	


}
//Reset the Tic Tac Toe board 
void ResetBoard(){
	int i; 
	//Reset all the positions to free on the board.
	for ( i = 0; i<=8; i++){
		gamePositions.freePositions[i] = i;
	}
	//Reset the taken A positions to none.
	for ( i = 0; i<=4; i++){
		gamePositions.takePositionsA[i] = -1;
	}
	//Reset the taken B positions to none. 
	for ( i = 0; i<=3; i++){
		gamePositions.takePositionsB[i] = -1;
	}

}

//Reset Status and Flags
void ResetFlags(){
	int i;
	gameStatus.gameOver = false;
	gameStatus.GamePlaySmartAI = false;
	gameStatus.GamePlayDumbAI = false;
	gameStatus.playerWon = false;
	gameStatus.botWon = false;
	gameStatus.playerMoves = 0;
	gameStatus.freeTiles = 9;
	gameStatus.centerTile = 0;
		for ( i = 0; i<=3; i++){
		gamePositions.winCenter[i] = 0;
	}
			for ( i = 0; i<=1; i++){
		gamePositions.winMod[i] = 0;
		gamePositions.winBelow[i] = 0;
	}
		
		
	
	}

//Checks if the game has ended
//if check(true) check the player
bool CheckGameOver(bool check){
	int i; 
	//Checks the player's tile
	if (gameStatus.playerMoves > 1 ){
		//Check center victory
			if (gameStatus.centerTile == 1){ //only check if center tile is in Player's hand
				for (i = 0; i <=3; i++){
					if (gamePositions.winCenter[i] == 9){
						ST7735_OutString("\nWIN");
						gameStatus.playerWon = true;
						return true; 
					}
				}	
			}
			if (gameStatus.centerTile == 2) {
				for (i = 0; i <=3; i++){
					if (gamePositions.winCenter[i] == 12){
						ST7735_OutString("\nWIN");
						gameStatus.botWon = true;
						return true; 
					}
				}
			}	
				
			
			for (i = 0; i <= 1; i++){
				if (gamePositions.winBelow[i] == 9){
						gameStatus.playerWon = true;
						return true; 
				}
				if (gamePositions.winBelow[i] == 12){
						ST7735_OutString("\nWIN");
						gameStatus.botWon = true;
						return true; 
				}
				if (gamePositions.winMod[i] == 9){
					gameStatus.playerWon = true;
					return true;
				}
				if (gamePositions.winMod[i] == 12){
					gameStatus.botWon = true;
					return true;
				}
			}
		}
	
			if (gameStatus.freeTiles == 0){
				ST7735_OutString("\nGameEnd");
				return true;
		}
			return false;
}

		
		


//Clears the LEDS on the tic tac toe board
void ClearAllLeds(){
			GPIO_PORTA_DATA_R &= ~0x10;
			GPIO_PORTB_DATA_R &= ~0xFF;
			GPIO_PORTD_DATA_R &= ~0xCC;
			GPIO_PORTC_DATA_R &= ~0xE0;
			GPIO_PORTE_DATA_R &= ~0x09;
}
void LightAllLeds(){
			GPIO_PORTA_DATA_R |= 0x10;
			GPIO_PORTB_DATA_R |= 0xFF;
			GPIO_PORTD_DATA_R |= 0xCC;
			GPIO_PORTC_DATA_R |= 0xE0;
			GPIO_PORTE_DATA_R |= 0x09;
}

void LightPlayerLeds(){
	int i;
	In = GPIO_PORTE_DATA_R&0x10; // read PE4 into In
			if(gameStatus.playerMoves > 0 ){
			 	if (In == 0x00){				// PE4 is low
					for (i = 0; i < gameStatus.playerMoves; i++){
						if(gamePositions.takePositionsA[i]!=8)
							GPIO_PORTB_DATA_R &= 
								~gameLeds.PlayerLEDS[gamePositions.takePositionsA[i]/3][gamePositions.takePositionsA[i]%3];
						else 
							GPIO_PORTA_DATA_R &= 
								~gameLeds.PlayerLEDS[gamePositions.takePositionsA[i]/3][gamePositions.takePositionsA[i]%3];
					}
				}
					else
						for (i = 0; i < gameStatus.playerMoves; i++){
							if(gamePositions.takePositionsA[i]!=8)
								GPIO_PORTB_DATA_R |= 
									gameLeds.PlayerLEDS[gamePositions.takePositionsA[i]/3][gamePositions.takePositionsA[i]%3];
							else 
								GPIO_PORTA_DATA_R |= 
									gameLeds.PlayerLEDS[gamePositions.takePositionsA[i]/3][gamePositions.takePositionsA[i]%3];
						}
			}
}

void LightBotLeds(){
	int i;
	In = GPIO_PORTE_DATA_R&0x10; // read PE4 into In
			if(gameStatus.playerMoves > 0  ){
			 	if (In == 0x00)	{			// PE4 is low
					for (i = 0; i < gameStatus.playerMoves; i++){
						if (i <= 3){
						 if(gamePositions.takePositionsB[i] < 4)
							 GPIO_PORTD_DATA_R &= 
								~gameLeds.BotLEDS[gamePositions.takePositionsB[i]/3][gamePositions.takePositionsB[i]%3];
						 else if(gamePositions.takePositionsB[i] < 7)
								GPIO_PORTC_DATA_R &= 
								~gameLeds.BotLEDS[gamePositions.takePositionsB[i]/3][gamePositions.takePositionsB[i]%3];
							else 
								GPIO_PORTE_DATA_R &= 
								~gameLeds.BotLEDS[gamePositions.takePositionsB[i]/3][gamePositions.takePositionsB[i]%3];
					}
				}
				}
				else {
					for (i = 0; i < gameStatus.playerMoves; i++){
						if(i <= 3){
							if(gamePositions.takePositionsB[i] < 4)
								GPIO_PORTD_DATA_R |= 
								gameLeds.BotLEDS[gamePositions.takePositionsB[i]/3][gamePositions.takePositionsB[i]%3];
							else if(gamePositions.takePositionsB[i] < 7)
								GPIO_PORTC_DATA_R |= 
								gameLeds.BotLEDS[gamePositions.takePositionsB[i]/3][gamePositions.takePositionsB[i]%3];
							else 
								GPIO_PORTE_DATA_R |= 
								gameLeds.BotLEDS[gamePositions.takePositionsB[i]/3][gamePositions.takePositionsB[i]%3];
					}	
				}
			}
		}
}
void LightAnalogLed(){
	In = GPIO_PORTC_DATA_R&0x10; // read PC4 into In
    if(In == 0x00){              // PC4 is low
				if (g.analogValueC == 2 && g.analogValueR == 2)
					GPIO_PORTA_DATA_R &= ~0x10;
			else 
					GPIO_PORTB_DATA_R &= ~gameLeds.PlayerLEDS[g.analogValueR][g.analogValueC];
					
		} else{                      // PC4 is high
				if (g.analogValueC == 2 && g.analogValueR == 2)
					GPIO_PORTA_DATA_R |= 0x10;
			else 
					GPIO_PORTB_DATA_R |= gameLeds.PlayerLEDS[g.analogValueR][g.analogValueC];
					
			}
	
}

bool AddPosition(uint8_t position){
	int arrayPosition = FreePosition(position);
	if (arrayPosition != -1){
				gamePositions.takePositionsA[gameStatus.playerMoves] = arrayPosition;
				gameStatus.playerMoves++;
				if (arrayPosition == 4) 
				gameStatus.centerTile = 1; 
				gameStatus.freeTiles--;
				AddWinScore(true, position);
				return true;
			}
	else return false;
	
}
bool AddPositionBotSmart(uint8_t position){
	int arrayPosition = FreePosition(position);
	if (arrayPosition != -1){
				//gameStatus.PlayermMoves has to be decremented because it increases 
				// the player's moves. 
				gamePositions.takePositionsB[gameStatus.playerMoves-1] = arrayPosition;
				if (arrayPosition == 4) 
				gameStatus.centerTile = 2; 
				gameStatus.freeTiles--;
				AddWinScore(false, position);
				return true;
			}
	else return false;
}

 void AddPositionBotAuto(){
	 uint8_t arrayCounter = 0;
	 while (gamePositions.freePositions[arrayCounter] == -1){
		 ++arrayCounter;
	 }
	 if (arrayCounter == 4) 
		gameStatus.centerTile = 2; 
	 gamePositions.takePositionsB[gameStatus.playerMoves-1] = 
	 gamePositions.freePositions[arrayCounter];
	 gamePositions.freePositions[arrayCounter] = -1;
	 AddWinScore(false, arrayCounter);
	 gameStatus.freeTiles--;
 }

	void AddWinScore(bool check, uint8_t position){
	//Center wins, Mod wins, Below wins
	//[0,4,8],[1,4,7],[2,4,6],[3,4,5]
	//[0,3,6], [2,5,8]
	//[0,1,2], [6,7,8]
		int i; 
		if (check){
			if(position == 4){
				for (i = 0; i <= 3; i++){
					gamePositions.winCenter[i] += 3;	//[0,4,8],[1,4,7],[2,4,6],[3,4,5]
				}
			}
			else if (position < 4){
				gamePositions.winCenter[position%4] += 3; //[0,4,8],[1,4,7],[2,4,6],[3,4,5]
				if(position < 3)
					gamePositions.winBelow[0] += 3; //[0,1,2]
				if (position%3 == 0)
					gamePositions.winMod[0] += 3; //[0,3,6]
				if (position%3 == 2)
					gamePositions.winMod[1] += 3; //[2,5,8]
			}
		
			
			else {
					gamePositions.winCenter[8-position]+=3; //[0,4,8],[1,4,7],[2,4,6],[3,4,5]
				if(position > 5)
					gamePositions.winBelow[1] += 3; //[6,7,8]
				if (position%3 == 0)
					gamePositions.winMod[0] += 3; //[0,3,6]
				if (position%3 == 2)
					gamePositions.winMod[1] += 3; //[2,5,8]
			}
		}
			
			
//			
//		
		else {
			if(position == 4){
				for ( i = 0; i <= 3; i++){
					gamePositions.winCenter[i] += 4;	//[0,4,8],[1,4,7],[2,4,6],[3,4,5]
				}
			}
			else if (position < 4){
				gamePositions.winCenter[position%4] += 4; //[0,4,8],[1,4,7],[2,4,6],[3,4,5]
				if(position < 3)
					gamePositions.winBelow[0] += 4; //[0,1,2]
				if (position%3 == 0)
					gamePositions.winMod[0] += 4; //[0,3,6]
				if (position%3 == 2)
					gamePositions.winMod[1] += 4; //[2,5,8]
			}
			
			else {
					gamePositions.winCenter[8-position]+=4; //[0,4,8],[1,4,7],[2,4,6],[3,4,5]
				if(position > 5)
					gamePositions.winBelow[1] += 4; //[6,7,8]
				if (position%3 == 0)
					gamePositions.winMod[0] += 4; //[0,3,6]
				if (position%3 == 2)
					gamePositions.winMod[1] += 4; //[2,5,8]
			}
	}
}
int FreePosition(uint8_t position){
	int i; 
	for (i = 0; i<=8; i++){
		if (position == gamePositions.freePositions[i]){
			gamePositions.freePositions[i] = -1;
			return i;
		}
	}
return -1;
}

bool SearchElement (int element, bool check){
	int i;
	if (check){
		for (i = 0; i < gameStatus.playerMoves; i++){
			if (gamePositions.takePositionsA[i] == element)
				return true;
		}
	}
	else {
		for (i = 0; i < gameStatus.playerMoves; i++){
			if (gamePositions.takePositionsB[i] == element)
				return true;
		}
	}
	return false;
}

int MakeAIMove(){
	if (gameStatus.playerMoves > 1){
		if(AvailableWin())
			return 0;
		if(AvailableBlock())
			return 0; 
	}
	if (gameStatus.playerMoves == 2 || gameStatus.playerMoves == 3){
		if(AvailableFork())
			return 0;
		if(BlockFork())
			return 0;
	}
	if (AvailableCenter())
		return 0; 
	if(AvailableOppositeCorner())
		return 0;
	if(AvailableEmptyCorner())
		return 0;
	if(AvailableEmptySide())
		return 0;
	
	
	return 0;
}

//AI checks for an Available Win and adds the position
bool AvailableWin(){
	int i; 
		if (gameStatus.centerTile == 2) {
				for (i = 0; i <=3; i++){
					if (gamePositions.winCenter[i] == 8 && (AddPositionBotSmart(8-i) || AddPositionBotSmart(i%4))){
						ST7735_OutString("\nWIN");
						return true; 
					}
		}
	}

	for(i = 0; i<=1; i++){
		if (gamePositions.winBelow[i] == 8){
						if (i == 0 && (AddPositionBotSmart(0) || AddPositionBotSmart(1) || AddPositionBotSmart(2)))
							return true;
						if (i == 1 && (AddPositionBotSmart(6) || AddPositionBotSmart(7) || AddPositionBotSmart(8)))
							return true;
				}

				if (gamePositions.winMod[i] == 8){
					if (i == 0 && (AddPositionBotSmart(0) || AddPositionBotSmart(3) || AddPositionBotSmart(6)))
						return true;
					if (i == 1 && (AddPositionBotSmart(2) || AddPositionBotSmart(5) || AddPositionBotSmart(8)))
						return true;
				}
	}
	return false;
}
//AI checks for an Avaliable Block (win)
bool AvailableBlock(){
		int i; 
		if (gameStatus.centerTile == 1) {
				for (i = 0; i <=3; i++){
					if (gamePositions.winCenter[i] == 6 && (AddPositionBotSmart(8-i) || AddPositionBotSmart(i%4))){
						return true; 
					}
		}
	}

	for(i = 0; i<=1; i++){
		if (gamePositions.winBelow[i] == 6){
						if (i == 0 && (AddPositionBotSmart(0) || AddPositionBotSmart(1) || AddPositionBotSmart(2)))
							return true;
						if (i == 1 && (AddPositionBotSmart(6) || AddPositionBotSmart(7) || AddPositionBotSmart(8)))
							return true;
				}

				if (gamePositions.winMod[i] == 6){
					if (i == 0 && (AddPositionBotSmart(0) || AddPositionBotSmart(3) || AddPositionBotSmart(6)))
						return true;
					if (i == 1 && (AddPositionBotSmart(2) || AddPositionBotSmart(5) || AddPositionBotSmart(8)))
						return true;
				}
	}
	return false;
	
}
//AI checks for an Available Fork
bool AvailableFork(){
	if(SearchElement(2,false)&& SearchElement(6,false) && AddPositionBotSmart(5))
		return true;
	if(SearchElement(0,false)&& SearchElement(8,false) && AddPositionBotSmart(3))
		return true;
	if(gameStatus.centerTile == 2){
		if(SearchElement(0,false)&& SearchElement(4,false) && (AddPositionBotSmart(2) || AddPositionBotSmart(6)))
			return true;
		if(SearchElement(2,false)&& SearchElement(4,false) && (AddPositionBotSmart(0) || AddPositionBotSmart(8)))
			return true;
		if(SearchElement(6,false)&& SearchElement(4,false) && (AddPositionBotSmart(0) || AddPositionBotSmart(8)))
			return true;
		if(SearchElement(8,false)&& SearchElement(4,false) && (AddPositionBotSmart(2) || AddPositionBotSmart(6)))
			return true;
	}
	return false;
}
//AI checks for a fork to block
bool BlockFork(){
	if(SearchElement(2,true)&& SearchElement(6,true) && AddPositionBotSmart(5))
		return true;
	if(SearchElement(0,true)&& SearchElement(8,true) && AddPositionBotSmart(3))
		return true;
	return false;
}
//AI checks for an Available Center
bool AvailableCenter(){
	if (gameStatus.centerTile==0){
		AddPositionBotSmart(4);
		return true;
	}
	return false;


}
////AI checks for an opposite corner
bool AvailableOppositeCorner(){
//corners available are 0,2,6,8
//sets [8,0], [0,8], [2,6],[6,2]
	if (SearchElement(0, true) && AddPositionBotSmart(8))
		return true;
	if (SearchElement(8, true) && AddPositionBotSmart(0))
		return true;
	if (SearchElement(2, true) && AddPositionBotSmart(6))
		return true;
	if (SearchElement(6, true) && AddPositionBotSmart(2))
		return true;
	return false;
}
////AI checks for an Empty Corner
bool AvailableEmptyCorner(){
	if (AddPositionBotSmart(0))
		return true;
	if (AddPositionBotSmart(2))
		return true;
	if (AddPositionBotSmart(6))
		return true;
	if (AddPositionBotSmart(8))
		return true;
	return false;
}
//AI checks for an Empty Side
bool AvailableEmptySide(){
		if (AddPositionBotSmart(1))
		return true;
	if (AddPositionBotSmart(3))
		return true;
	if (AddPositionBotSmart(5))
		return true;
	if (AddPositionBotSmart(7))
		return true;
	return false;
	
}
void DelayWait10ms(uint32_t n){uint32_t volatile time;
  while(n){
    time = 727240*2/91;  // 10msec
    while(time){
      time--;
    }
    n--;
  }
}

void GPIOPortF_Handler(){
		if ((GPIO_PORTF_RIS_R&0x01) == 0x01 ) {    // detects if switch 2 is pressed
			GPIO_PORTF_ICR_R = 0x01;  // acknowledge flag0
			GPIO_PORTF_DATA_R = 0x04;
			gameStatus.GamePlaySmartAI = true;
			if(gameStatus.gameOver)
				gameStatus.gameOver = false;
		}
		else if ((GPIO_PORTF_RIS_R&0x10) == 0x10 ) {  // detects if switch 1 is pressed
			GPIO_PORTF_ICR_R = 0x10;  // acknowledge flag0
			GPIO_PORTF_DATA_R = 0x08;
			gameStatus.GamePlayDumbAI = true;
				if(gameStatus.gameOver)
				gameStatus.gameOver = false;
		}
								 
}

void ADC1Seq2_Handler(void){
uint32_t Adirection[2];  
  ADC1_PSSI_R = 0x0004;            // 1) initiate SS2 (pg845)
  while((ADC1_RIS_R&0x04)==0){};   // 2) wait for conversion done ADC raw interrupt status, 
																	 //    checks for SS2, pg823

		Adirection[0] = ADC1_SSFIFO2_R&0xFFF;  // read second result, y direction read.
		Adirection[1] = ADC1_SSFIFO2_R&0xFFF;  // read first result, x direction read. 
			if((gameStatus.GamePlayDumbAI == true || gameStatus.GamePlaySmartAI == true) && !gameStatus.gameOver){
			 if (Adirection[1] >= 3700){
				GPIO_PORTF_DATA_R = 0x08;
				GPIO_PORTB_DATA_R &= ~gameLeds.PlayerLEDS[g.analogValueR][g.analogValueC];
				g.analogValueC += 1;
				g.analogValueC %= 3;
				DelayWait10ms(100);
			}
			else if (Adirection[1] <= 400){
				GPIO_PORTF_DATA_R = 0x08;
				GPIO_PORTB_DATA_R &= ~gameLeds.PlayerLEDS[g.analogValueR][g.analogValueC];
				g.analogValueC += 2;
				g.analogValueC%=3;
				DelayWait10ms(100);
			}
			else {
				GPIO_PORTB_DATA_R &= ~0xff;
				GPIO_PORTA_DATA_R &= ~0x10;
			}
			
			if (Adirection[0] >= 3000){
				GPIO_PORTF_DATA_R = 0x08;
				GPIO_PORTB_DATA_R &= ~gameLeds.PlayerLEDS[g.analogValueR][g.analogValueC];
				g.analogValueR += 2;
				g.analogValueR %= 3;
				DelayWait10ms(100);
			}
			else if (Adirection[0] <= 1200){
				GPIO_PORTF_DATA_R = 0x08;
				GPIO_PORTB_DATA_R &= ~gameLeds.PlayerLEDS[g.analogValueR][g.analogValueC];
				g.analogValueR+=1;
				g.analogValueR%=3;
				DelayWait10ms(100);
			}
			else {
				GPIO_PORTB_DATA_R &= ~0xff;
				GPIO_PORTA_DATA_R &= ~0x10;
			}
		}

	ADC1_ISC_R = 0x0004;             // 4) clear interrupt, ADC interrupt status and clear, pg 803,823  
}

void GPIOPortE_Handler(){
		if ((GPIO_PORTE_RIS_R&0x20) == 0x20 ) {    // detects if PE5 is pressed
			GPIO_PORTE_ICR_R = 0x20;  // acknowledge flag0
			GPIO_PORTF_DATA_R = 0x04;
			//allows the player to finalize a position on the board
			if ((gameStatus.GamePlayDumbAI  || gameStatus.GamePlaySmartAI) && !gameStatus.gameOver){
				if(AddPosition(((3*g.analogValueR)+g.analogValueC))){
				gameStatus.gameOver = CheckGameOver(true);
					DelayWait10ms(700);		
					if(!gameStatus.gameOver ) { 
						if (gameStatus.GamePlayDumbAI)
							AddPositionBotAuto();
						else 
							MakeAIMove();
						gameStatus.gameOver = CheckGameOver(false);
					}
				}
				
				else {ST7735_OutString("\n Position Not Added!\n");
							ST7735_OutUDec((3*g.analogValueR)+g.analogValueC);
				}
				
			}

			

		}
					DelayWait10ms(300);			 
}

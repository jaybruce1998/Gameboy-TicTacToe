#include <gb/gb.h>
#include <stdio.h>
void init(void);
void updateSwitches(void);
void checkInput(void);
const unsigned char sprites[] =
{
  0x7E,0x7E,0x81,0x81,0xA5,0xA5,0x81,0x81,
  0x81,0x81,0xBD,0xBD,0x81,0x81,0x7E,0x7E,
  0x7E,0x7E,0x81,0x81,0xA5,0xA5,0x81,0x81,
  0x99,0x99,0xA5,0xA5,0x81,0x81,0x7E,0x7E
};
//waitpad(J_START);//wait for start to be pressed

// The player array will hold the player's position as X ([0]) and Y ([1])
UINT8 player[2];
char board[3][3]={{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};
char symbol, state;
UINT8 i, r, c, turns;
void main(void) {
	init();
	while(1) {
		checkInput();					// Check for user input (and act on it)
		updateSwitches();			// Make sure the SHOW_SPRITES and SHOW_BKG switches are on each loop
		wait_vbl_done();			// Wait until VBLANK to avoid corrupting memory
	}
}
void showBoard(void)
{
	for(r=0; r<3; r++)
	{
		printf(" -----\n|");
		for(c=0; c<3; c++)
			printf("%c|", board[r][c]);
		printf("\n");
	}
	printf(" -----\n");
	if(state==0)
		printf("%c's turn", symbol);
	else if(state==1)
		printf("It's a tie!");
	else
		printf("%c won!", state);
	printf("\n\n\n\n\n\n\n\n\n\n");
}
void init(void) {
	symbol='X';
	player[0] = 16;
	player[1] = 24;
	state=0;
	turns=0;
	DISPLAY_ON;
	showBoard();
	// Load the 'sprites' tiles into sprite memory
	set_sprite_data(0, 1, sprites);
	// Set the first movable sprite (0) to be the first tile in the sprite memory (0)
	set_sprite_tile(0, 0);
	//printf("%s\n", _io_status);
}
void updateSwitches(void) {
	HIDE_WIN;
	SHOW_SPRITES;
	SHOW_BKG;
}
void checkInput(void) {
	if(joypad()&J_START&&state)
	{
		for(r=0; r<3; r++)
			for(c=0; c<3; c++)
				board[r][c]=' ';
		symbol='X';
		state=0;
		turns=0;
		showBoard();
	}
	if(joypad()&J_A&&state==0)
	{
		r=player[1]/16-1;
		c=player[0]/16-1;
		if(board[r][c]==' ')
		{
			board[r][c]=symbol;
			if((board[r][0]==board[r][1]&&board[r][0]==board[r][2])||
				(board[0][c]==board[1][c]&&board[0][c]==board[2][c])||
				(board[1][1]==symbol&&
					((board[0][0]==symbol&&board[2][2]==symbol)||
					(board[0][2]==symbol&&board[2][0]==symbol))))
				state=symbol;
			else if(++turns==9)
				state=1;
			if(symbol=='X')
				symbol='O';
			else
				symbol='X';
			showBoard();
		}
		waitpadup();
	}
	// UP
	if (joypad() & J_UP) {
		if(player[1]==24)
			player[1]=56;
		else
			player[1]-=16;
		waitpadup();
	}
	// DOWN
	if (joypad() & J_DOWN) {
		if(player[1]==56)
			player[1]=24;
		else
			player[1]+=16;
		waitpadup();
	}
	// LEFT
	if (joypad() & J_LEFT) {
		if(player[0]==16)
			player[0]=48;
		else
			player[0]-=16;
		waitpadup();
	}
	// RIGHT
	if (joypad() & J_RIGHT) {
		if(player[0]==48)
			player[0]=16;
		else
			player[0]+=16;
		waitpadup();
	}
	// Move the sprite in the first movable sprite list (0)
	// to the position of X (player[0]) and y (player[1])
	move_sprite(0, player[0], player[1]);
}
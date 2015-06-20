/****************************************************************
 *   Filename: TicTacToe.cpp                    Date: 2/12/2010 *
 *   Author: Kevin Tindall                                      * 
 *--------------------------------------------------------------*
 *           Implementation file for TicTacToe.h                *
 *--------------------------------------------------------------*
 * Summary of features:                                         *
 *   Constuctor for initializing a TicTacToe object with a game *
 * board to be full of spaces.                                  *
 *   SetValue function takes in a row, column, and player index *
 * and tries to put that players token in the row and column    *
 * specified as long as it is a valid move.                     *
 *   GetStatus function returns an integer value between zero   *
 * and three.  Zero represents a tie, one represents a win by   *
 * Player1, two represents a win by Player2, and three          *
 * represents a tie.                                            *
 *   Operator Overload for << is able to display a TicTacToe    *
 * object to the console.                                       *
 ****************************************************************/

#include <iostream>

#include "TicTacToe.h"

//initializes ' ' for every element on the 3*3 character game board
TicTacToe::TicTacToe()
{
	for(int i = 0; i < 3; i++)
   {
		for(int j = 0; j < 3; j++)
      {
			board[i][j] = ' ';
      }
   }
}

//function to set a marker X or O on the game board
//SetValue returns true only for legal moves
//TODO(Kevin): check number of x's and number
bool TicTacToe::SetValue(int rowIndex, int colIndex, int playerIndex)
{
   //check for invalid range on game board
	if(rowIndex < 0 || rowIndex > 2 || colIndex < 0 || colIndex > 2)
   {
      return false;
   }
   
   //check for already used space on game board
	if(board[rowIndex][colIndex] == 'X' || board[rowIndex][colIndex] == 'O')
   {
      return false;
   }
   
   //at this point it is a legal move, so depending on the player index a marker becomes placed
	if(playerIndex == 1)
   {
      board[rowIndex][colIndex] = 'X';
   }
	else //if (playerIndex == 2)
   {
      board[rowIndex][colIndex] = 'O';
   }
	return true; //yay for legal move!
}

//returns 0 representing a tie, 1 representing a win by Player1, 2 representing a win by Player2, and 3 otherwise.
int TicTacToe::GetStatus()
{
   //row and column check
	for(int i = 0; i < 3; i++)
   {
      // 3 in a row check
		if(board[i][0] == board[i][1] && board[i][1] == board[i][2])
      {
         switch(board[i][0])
         {
            case 'X' : return 1;
            case 'O' : return 2;
         }
		}
      // 3 in a column check
		if(board[0][i] == board[1][i] && board[1][i] == board[2][i])
      {
         switch(board[0][i])
         {
            case 'X' : return 1; //x won the game
            case 'O' : return 2; //o won the game
         }
		}
	}
   
   // 3 in a diagonal check
	if((board[0][0] == board[1][1] && board[1][1] == board[2][2]) || (board[0][2] == board[1][1] && board[1][1] == board[2][0]))
   {
      switch(board[1][1])
      {
         case 'X' : return 1; //x won the game
         case 'O' : return 2; //o won the game
      }
	}
   
	bool tieTest = true;
   // tie game check
	for(int i = 0; tieTest && i < 3; i++)
   {
		for(int j = 0; j < 3; j++)
      {
			if(board[i][j] == ' ')
         {
            tieTest = false;
            break;
         }
      }
   }
	if(tieTest)
   {
      return 0; //the game is tied
   }
	return 3; //the game must go on!
}

std::ostream& operator<< (std::ostream& out, const TicTacToe& x)
{
	out << "\n "
       << x.board[0][0] << " | " << x.board[0][1] << " | " << x.board[0][2] << " \n"
		 << "-----------\n "
		 << x.board[1][0] << " | " << x.board[1][1] << " | " << x.board[1][2] << " \n"
		 << "-----------\n "
		 << x.board[2][0] << " | " << x.board[2][1] << " | " << x.board[2][2] << "\n\n";
	return out;
}


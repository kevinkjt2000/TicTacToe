/**************************************************************************
 *   Filename: TicTacToe.h                                Date: 2/12/2010 *
 *   Author: Kevin Tindall                                                *
 *------------------------------------------------------------------------*
 *            Header file for TicTacToe class definition                  *
 *------------------------------------------------------------------------*
 * Descriptions for functions can be found in the implementation cpp file *
 **************************************************************************/

#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <iostream>

class TicTacToe {
private:
  char board[3][3];

public:
  TicTacToe();
  bool SetValue(int row, int col, int playerIndex);
  int GetStatus();
  friend std::ostream& operator<<(std::ostream&, const TicTacToe&);
};

#endif

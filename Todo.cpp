//
//  COMP 2012H Programming Assignment 1: Go
//  Filename: todo.cpp
//

#include "Helper.h"
#include "Todo.h"
#include <algorithm>
#include <vector>

using std::fill;
using std::vector;

/***********************************************************************
 * TODO_1: Check the number of liberties of a given stone. Mark the
 * connected stones and liberties, and return the number of liberties.
 * 
 * - Check whether the input row and column number is out-of-bounds. 
 * 	 If so, return -1 and do nothing.
 * - Check whether the intersection is empty. If so, return -2 and do 
 *   nothing.
 * - Identify the connected part that the selected stone belongs to, 
 *   find and count the number of liberties. Set the corresponding 
 *   elements in connected_part or liberties to be true for any found 
 *   connected stones or liberties.
 ***********************************************************************/

enum class Moves { NONE, UP, DOWN, LEFT, RIGHT };

struct StonePlusPlus {
	bool isChecked;
	Stone stone;
};

vector<Moves> check_move_valid(int row, int col) {
	vector<Moves> moves_to_check;
	// Can go up
	if (row - 1 >= 0) {	
		moves_to_check.push_back(Moves::UP);
	}
	// Can go right
	if (col + 1 <= 18) {
		moves_to_check.push_back(Moves::RIGHT);
	}
	// Can go down
	if (row + 1 <= 18) {
		moves_to_check.push_back(Moves::DOWN);
	}
	// Can go left
	if (col - 1 >= 0) {
		moves_to_check.push_back(Moves::LEFT);
	}
	return moves_to_check;
}

void recursive_connected_liberty(StonePlusPlus boardpp[][19], int row, int col, bool connected_part[][19], bool liberties[][19], Stone stone) {
	// If itself is not of the same stone, then it is done
	if (boardpp[row][col].stone != stone || boardpp[row][col].isChecked) {
		return;
	}
	else {
		// it is the stone to find and it is not checked
		boardpp[row][col].isChecked = true;
		connected_part[row][col] = true;
		vector<Moves> move_to_do = check_move_valid(row, col);
		for (Moves move : move_to_do) {
			switch (move) {
			case Moves::UP:
				recursive_connected_liberty(boardpp, row - 1, col, connected_part, liberties, stone);
				break;
			case Moves::DOWN:
				recursive_connected_liberty(boardpp, row + 1, col, connected_part, liberties, stone);
				break;
			case Moves::LEFT:
				recursive_connected_liberty(boardpp, row, col - 1, connected_part, liberties, stone);
				break;
			case Moves::RIGHT:
				recursive_connected_liberty(boardpp, row, col + 1, connected_part, liberties, stone);
				break;
			case Moves::NONE:
				break;
			default:
				std::cout << "Why does it arrive here???" << std::endl;
			}
		}
	}
}

int helper_function(StonePlusPlus boardpp[][19], int row, int col, bool connected_part[][19], bool liberties[][19]) {
	int total_liberty = 0;
	recursive_connected_liberty(boardpp, row, col, connected_part, liberties, boardpp[row][col].stone);
	return total_liberty;
}

int check_liberties(const Stone board[][19], int row, int col, bool connected_part[][19], bool liberties[][19]) {
	StonePlusPlus boardpp[19][19];
	if (row < 0 || row > 18 || col < 0 || col > 18) {
		return -1;
	}
	else if (board[row][col] == 0) {
		return -2;
	}
	else {
		for (int i = 0; i < 19; ++i) {
			for (int j = 0; j < 19; ++j) {
				boardpp[i][j].isChecked = false;
				boardpp[i][j].stone = board[i][j];
			}
		}
	}
	int total_liberty = 0;
	total_liberty = helper_function(boardpp, row, col, connected_part, liberties);
	return total_liberty;
}

/***********************************************************************
 * TODO_2: Check whether a player has captured stones, and mark them.
 * 
 * - Check the whole game board for any captured stones of the selected 
 *   player. Set the corresponding elements in captured to be true. 
 *   Return true if and only if there are any captured stones.
 ***********************************************************************/

bool find_captured(const Stone board[][19], Stone player, bool captured[][19]) {
	return false;
}

/***********************************************************************
 * TODO_3: Check whether the move is valid and edit the board.
 * 
 * - Check whether the input row and column number is out-of-bounds. If 
 *   so, return -1 and do nothing.
 * - Check whether the intersection is occupied. If so, return -2 and do 
 *   nothing.
 * - Check whether this is suicide. If so, return -3 and do nothing. 
 * - Modify board, including both adding the stone and removing captured 
 *   stones if applicable. Update record and return 0.
 * - Append the current move to the record. You need to set counter and 
 *   max_steps correctly.
 ***********************************************************************/

int edit(Stone board[][19], Stone player, int row, int col, int record[][2], int& counter, int& max_steps) {
	return -1;
}

/***********************************************************************
 * TODO_4: Jump to the given step of the game.
 * 
 * - Set the board to the state at target step, and correctly set the 
 *   value of counter and max_steps.
 ***********************************************************************/

void jump_to(Stone board[][19], int target, int record[][2], int& counter, int& max_steps) {
	
}
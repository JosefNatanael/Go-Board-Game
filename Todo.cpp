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
	bool isLibertyCounted;
	Stone stone;
};

int count_mark_nearby_liberty(StonePlusPlus boardpp[][19], int row, int col, bool liberties[][19]) {
	int total = 0;
	if (row - 1 >= 0) {
		if (boardpp[row - 1][col].stone == Empty && !boardpp[row - 1][col].isLibertyCounted) {
			boardpp[row - 1][col].isLibertyCounted = true;
			liberties[row - 1][col] = 1;
			++total;
		}
	}
	if (col + 1 <= 18) {
		if (boardpp[row][col + 1].stone == Empty && !boardpp[row][col + 1].isLibertyCounted) {
			boardpp[row][col + 1].isLibertyCounted = true;
			liberties[row][col + 1] = 1;
			++total;
		}
	}
	if (row + 1 <= 18) {
		if (boardpp[row + 1][col].stone == Empty && !boardpp[row + 1][col].isLibertyCounted) {
			boardpp[row + 1][col].isLibertyCounted = true;
			liberties[row + 1][col] = 1;
			++total;
		}
	}
	if (col - 1 >= 0) {
		if (boardpp[row][col - 1].stone == Empty && !boardpp[row][col - 1].isLibertyCounted) {
			boardpp[row][col - 1].isLibertyCounted = true;
			liberties[row][col - 1] = 1;
			++total;
		}
	}
	return total;
}

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

void recursive_connected_liberty(StonePlusPlus boardpp[][19], int row, int col, bool connected_part[][19], bool liberties[][19], Stone stone, int& liberty) {
	// If itself is not of the same stone, then it is done
	if (boardpp[row][col].stone != stone || boardpp[row][col].isChecked) {
		return;
	}
	else {
		// it is the stone to find and it is not checked
		// count number of liberties
		boardpp[row][col].isChecked = true;
		connected_part[row][col] = true;
		liberty += count_mark_nearby_liberty(boardpp, row, col, liberties);
		vector<Moves> move_to_do = check_move_valid(row, col);
		for (Moves move : move_to_do) {
			switch (move) {
			case Moves::UP:
				recursive_connected_liberty(boardpp, row - 1, col, connected_part, liberties, stone, liberty);
				break;
			case Moves::DOWN:
				recursive_connected_liberty(boardpp, row + 1, col, connected_part, liberties, stone, liberty);
				break;
			case Moves::LEFT:
				recursive_connected_liberty(boardpp, row, col - 1, connected_part, liberties, stone, liberty);
				break;
			case Moves::RIGHT:
				recursive_connected_liberty(boardpp, row, col + 1, connected_part, liberties, stone, liberty);
				break;
			case Moves::NONE:
				break;
			default:
				std::cout << "Why does it arrive here???" << std::endl;
			}
		}
	}
}

int helper_function(StonePlusPlus boardpp[][19], int row, int col, bool connected_part[][19], bool liberties[][19], const Stone board[][19]) {
	int total_liberty = 0;
	recursive_connected_liberty(boardpp, row, col, connected_part, liberties, boardpp[row][col].stone, total_liberty);
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
				boardpp[i][j].isLibertyCounted = false;	
			}
		}
	}
	int total_liberty = 0;
	total_liberty = helper_function(boardpp, row, col, connected_part, liberties, board);
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
	bool flag_return = false;
	for (int i = 0; i < 19; ++i) {
		for (int j = 0; j < 19; ++j) {
			if (board[i][j] != player) {
				continue;
			}
			bool connected_part[19][19], liberties[19][19];
			fill(&connected_part[0][0], &connected_part[18][18] + 1, false);
			fill(&liberties[0][0], &liberties[18][18] + 1, false);
			if (check_liberties(board, i, j, connected_part, liberties) == 0) {
				flag_return = true;
				captured[i][j] = true;
			}
		}
	}
	return flag_return;
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

bool isSuicide(const Stone board[][19], const Stone player, const int row, const int col) {
	struct CapturedPair {
		int row;
		int col;
	};
	bool captured[19][19];
	fill(&captured[0][0], &captured[18][18] + 1, false);
	Stone copy_board[19][19];
	for (int i = 0; i < 19; ++i) {
		for (int j = 0; j < 19; ++j) {
			copy_board[i][j] = board[i][j];
		}
	}
	player == Black ? find_captured(copy_board, White, captured) : find_captured(copy_board, Black, captured);
	int initial_number_of_captured = 0;
	for (int i = 0; i < 19; ++i) {
		for (int j = 0; j < 19; ++j) {
			if (captured[i][j] == 1) {
				++initial_number_of_captured;
			}
		}
	}
	copy_board[row][col] = player;
	player == Black ? find_captured(copy_board, White, captured) : find_captured(copy_board, Black, captured);
	int final_number_of_captured = 0;
	for (int i = 0; i < 19; ++i) {
		for (int j = 0; j < 19; ++j) {
			if (captured[i][j] == 1) {
				++final_number_of_captured;
			}
		}
	}
	if (final_number_of_captured != initial_number_of_captured) {
		return false;
	}
	return true;
}

int edit(Stone board[][19], Stone player, int row, int col, int record[][2], int& counter, int& max_steps) {
	if (row < 0 || row > 18 || col < 0 || col > 18) {
		return -1;
	}
	if (board[row][col] != Empty) {
		return -2;
	}
	Stone copy_board[19][19];
	for (int i = 0; i < 19; ++i) {
		for (int j = 0; j < 19; ++j) {
			copy_board[i][j] = board[i][j];
		}
	}
	copy_board[row][col] = player;
	bool connected_part[19][19], liberties[19][19];
	fill(&connected_part[0][0], &connected_part[18][18] + 1, false);
	fill(&liberties[0][0], &liberties[18][18] + 1, false);
	if (check_liberties(copy_board, row, col, connected_part, liberties) == 0) {
		if (isSuicide(board, player, row, col)) {
			return -3;
		}	
	}
	board[row][col] = player;
	bool captured[19][19];
	fill(&captured[0][0], &captured[18][18] + 1, false);
	player == Black ? find_captured(board, White, captured) : find_captured(board, Black, captured);
	struct CapturedPair {
		int row;
		int col;
	};
	vector<CapturedPair> to_remove_from_board;
	for (int i = 0; i < 19; ++i) {
		for (int j = 0; j < 19; ++j) {
			if (captured[i][j] == 1) {
				CapturedPair a;
				a.row = i;
				a.col = j;
				to_remove_from_board.push_back(a);
			}
		}
	}
	for (CapturedPair i : to_remove_from_board) {
		board[i.row][i.col] = Empty;
	}
	record[counter][0] = row;
	record[counter][1] = col;
	++counter;
	max_steps = counter;
	return 0;
}

/***********************************************************************
 * TODO_4: Jump to the given step of the game.
 * 
 * - Set the board to the state at target step, and correctly set the 
 *   value of counter and max_steps.
 ***********************************************************************/

void edit_from_jump(Stone board[][19], Stone player, int row, int col) {
	Stone copy_board[19][19];
	for (int i = 0; i < 19; ++i) {
		for (int j = 0; j < 19; ++j) {
			copy_board[i][j] = board[i][j];
		}
	}
	copy_board[row][col] = player;
	bool connected_part[19][19], liberties[19][19];
	fill(&connected_part[0][0], &connected_part[18][18] + 1, false);
	fill(&liberties[0][0], &liberties[18][18] + 1, false);
	if (check_liberties(copy_board, row, col, connected_part, liberties) == 0) {
		return;
	}
	board[row][col] = player;
	bool captured[19][19];
	fill(&captured[0][0], &captured[18][18] + 1, false);
	player == Black ? find_captured(board, White, captured) : find_captured(board, Black, captured);
	struct CapturedPair {
		int row;
		int col;
	};
	vector<CapturedPair> to_remove_from_board;
	for (int i = 0; i < 19; ++i) {
		for (int j = 0; j < 19; ++j) {
			if (captured[i][j] == 1) {
				CapturedPair a;
				a.row = i;
				a.col = j;
				to_remove_from_board.push_back(a);
			}
		}
	}
	for (CapturedPair i : to_remove_from_board) {
		board[i.row][i.col] = Empty;
	}
}

void jump_to(Stone board[][19], int target, int record[][2], int& counter, int& max_steps) {
	Stone copy_board[19][19];
	initialize_board(copy_board);
	Stone current_player = Black;
	counter = 0;
	for (int i = 0; i < target; ++i) {
		current_player = i % 2 == 0 ? Black : White;
		if (record[i][0] == -1 && record[i][1] == -1) {
			continue;
		}
		edit_from_jump(copy_board, current_player, record[i][0], record[i][1]);
	}
	for (int i = 0; i < 19; ++i) {
		for (int j = 0; j < 19; ++j) {
			board[i][j] = copy_board[i][j];
		}
	}
	counter = target;
}
#include <iostream>
#include <string>
#include <ctime>
#include <vector>
// #include "sqlite/sqlite3.h"

#include "global_functions.h"
#include "Board.h"



using namespace std;

Board::Board()
{
	resetBoard();

	srand(time(0));  // needed once per program run
}

void Board::resetBoard()
{
	board = createMatrix(this->size);

	//places initial pieces
	place_piece(2, 3, 3);
	place_piece(1, 4, 3);
	place_piece(1, 3, 4);
	place_piece(2, 4, 4);
}


//places piece down at specified position
bool Board::place_piece(int piece, int col, int row)
{

	try{
		board[col][row] = piece;
	} catch(exception &ex)
	{
		return false;
	}

	//// flips other player's pieces////

	int other_piece = 0;


	if(piece == white_piece)
		other_piece = black_piece;
	else
		other_piece = white_piece;



	//flips pieces down the column
	for(int x = row+1; x < size; x++)
	{
		//if encountered my piece, traverse back and flip previous pieces
		if(board[col][x]==piece)
		{
			//reverses back over previous squares and flips the pieces
			for(int temp_x = x-1; temp_x >= row+1; temp_x--)
				board[col][temp_x] = piece;

			break;
		}
		//if encountered blank space, stop
		else if(board[col][x]==0)
			break;
	}

	//flips pieces up the column
	for(int x = row-1; x >= 0; x--)
	{
		//if encountered my piece, traverse back and flip previous pieces
		if(board[col][x]==piece)
		{
			//reverses back over previous squares and flips the pieces
			for(int temp_x = x+1; temp_x <= row-1; temp_x++)
				board[col][temp_x] = piece;

			break;
		}
		//if encountered blank space, stop
		else if(board[col][x]==0)
			break;
	}

	//flips pieces right the row
	for(int x = col+1; x < size; x++)
	{
		//if encountered my piece, traverse back and flip previous pieces
		if(board[x][row]==piece)
		{
			//reverses back over previous squares and flips the pieces
			for(int temp_x = x-1; temp_x >= col+1; temp_x--)
				board[temp_x][row] = piece;

			break;
		}
		//if encountered blank space, stop
		else if(board[x][row]==0)
			break;
	}

	//flips pieces left the row
	for(int x = col-1; x >= 0; x--)
	{
		//if encountered my piece, traverse back and flip previous pieces
		if(board[x][row]==piece)
		{
			//reverses back over previous squares and flips the pieces
			for(int temp_x = x+1; temp_x <= col-1; temp_x++)
				board[temp_x][row] = piece;

			break;
		}
		//if encountered blank space, stop
		else if(board[x][row]==0)
			break;
	}


	//flips pieces up the positive slope diagonal
	int temp_col = col+1;
	int temp_row = row-1;
	while(temp_col < size && temp_row >= 0)
	{
		//if encountered my piece, traverse back and flip previous pieces
		if(board[temp_col][temp_row]==piece)
		{
			//reverses back over previous squares and flips the pieces
			temp_col = temp_col-1;
			temp_row = temp_row+1;
			while(temp_col >= col+1 && temp_row <= row-1)
			{
				board[temp_col][temp_row] = piece;

				temp_col--;
				temp_row++;
			}

			break;
		}
		//if encountered blank space, stop
		else if(board[temp_col][temp_row]==0)
			break;

		temp_col++;
		temp_row--;
	}


	//flips pieces down the positive slope diagonal
	temp_col = col-1;
	temp_row = row+1;
	while(temp_col >= 0 && temp_row < size)
	{
		//if encountered my piece, traverse back and flip previous pieces
		if(board[temp_col][temp_row]==piece)
		{
			//reverses back over previous squares and flips the pieces
			temp_col = temp_col+1;
			temp_row = temp_row-1;
			while(temp_col <= col-1 && temp_row >= row+1)
			{
				board[temp_col][temp_row] = piece;

				temp_col++;
				temp_row--;
			}

			break;
		}
		//if encountered blank space, stop
		else if(board[temp_col][temp_row]==0)
			break;

		temp_col--;
		temp_row++;
	}

	//flips pieces up the negative slope diagonal
	temp_col = col+1;
	temp_row = row+1;
	while(temp_col < size && temp_row < size)
	{
		//if encountered my piece, traverse back and flip previous pieces
		if(board[temp_col][temp_row]==piece)
		{
			//reverses back over previous squares and flips the pieces
			temp_col = temp_col-1;
			temp_row = temp_row-1;
			while(temp_col >= col+1 && temp_row >= row+1)
			{
				board[temp_col][temp_row] = piece;

				temp_col--;
				temp_row--;
			}

			break;
		}
		//if encountered blank space, stop
		else if(board[temp_col][temp_row]==0)
			break;

		temp_col++;
		temp_row++;
	}

	//flips pieces down the negative slope diagonal
	temp_col = col-1;
	temp_row = row-1;
	while(temp_col >= 0 && temp_row >= 0)
	{
		//if encountered my piece, traverse back and flip previous pieces
		if(board[temp_col][temp_row]==piece)
		{
			//reverses back over previous squares and flips the pieces
			temp_col = temp_col+1;
			temp_row = temp_row+1;
			while(temp_col <= col-1 && temp_row <= row-1)
			{
				board[temp_col][temp_row] = piece;

				temp_col++;
				temp_row++;
			}

			break;
		}
		//if encountered blank space, stop
		else if(board[temp_col][temp_row]==0)
			break;

		temp_col--;
		temp_row--;
	}


	return true;
}





//returns list of coordinates of legal moves
int Board::getPossibleMovesCount(int piece)
{

	int other_piece = 0;


	if(piece == white_piece)
		other_piece = black_piece;
	else
		other_piece = white_piece;


	int num_moves = 0;
	int num_flips = 0;
	Board board_copy;
				

	//iterates through entire board
	for(int x = 0; x < size; x++)
	{
		for(int y = 0; y < size; y++)
		{
			//if a piece already resides here, skip
			if(board[x][y]!=0)
				continue;

			//gets neighboring spots around current spot
			int* neighbors = new int[9];
			get_neighbors(neighbors, x, y);

			//gets if other piece is around this spot
			bool has_neighbors = false;
			for(int z = 0; z < 9; z++)
			{
				if(neighbors[z]==other_piece)
				{
					has_neighbors = true;
					break;
				}
			}


			//checks if this spot is a possible move by seeing if other pieces are flipped
			if(has_neighbors)
			{
				//gets copy of board to simulate what would happen if piece is placed
				// int** board_copy = copyBoard(board, size);
				board_copy.copyBoard(this);

				int piece_count = board_copy.countPieces(piece);
				int other_piece_count = board_copy.countPieces(other_piece);

				board_copy.place_piece(piece, x, y);

				int new_piece_count = board_copy.countPieces(piece);
				int new_other_piece_count = board_copy.countPieces(other_piece);

				//if flipped any opponent's pieces
				if(other_piece_count > new_other_piece_count)
					// num_moves++;
					num_flips += new_piece_count-piece_count;
			}

		}
	}


	return num_moves;
}


//returns list of coordinates of legal moves
vector<vector<int>> Board::getPossibleMoveCoordinates(int piece)
{

	vector<vector<int>> possible_moves;

	int other_piece = 0;


	if(piece == white_piece)
		other_piece = black_piece;
	else
		other_piece = white_piece;


	Board board_copy;
				

	//iterates through entire board
	for(int x = 0; x < size; x++)
	{
		for(int y = 0; y < size; y++)
		{

			//if a piece already resides here, skip
			if(board[x][y]!=0)
				continue;

			//gets neighboring spots around current spot
			int* neighbors = new int[9];
			get_neighbors(neighbors, x, y);

			//gets if other piece is around this spot
			bool has_neighbors = false;
			for(int z = 0; z < 9; z++)
			{
				if(neighbors[z]==other_piece)
				{
					has_neighbors = true;
					break;
				}
			}


			//checks if this spot is a possible move by seeing if other pieces are flipped
			if(has_neighbors)
			{
				//gets copy of board to simulate what would happen if piece is placed
				// int** board_copy = copyBoard(board, size);
				board_copy.copyBoard(this);

				int piece_count = board_copy.countPieces(piece);
				int other_piece_count = board_copy.countPieces(other_piece);

				board_copy.place_piece(piece, x, y);

				int new_piece_count = board_copy.countPieces(piece);
				int new_other_piece_count = board_copy.countPieces(other_piece);

				//if flipped any opponent's pieces
				if(other_piece_count > new_other_piece_count)
				{
					vector<int> pair;
					pair.push_back(x);
					pair.push_back(y);
					possible_moves.push_back(pair);
				}
			}

		}
	}


	return possible_moves;
}

//returns list of legal board states that are possible for piece
vector<Board> Board::getPossibleMoveBoards(int piece)
{
	vector<Board> possible_boards;

	//gets opponent's piece
	int other_piece = 0;
	if(piece == white_piece)
		other_piece = black_piece;
	else
		other_piece = white_piece;
				

	//iterates through entire board
	for(int x = 0; x < size; x++)
	{
		for(int y = 0; y < size; y++)
		{

			//if a piece already resides here, skip
			if(board[x][y]!=0)
				continue;

			//gets neighboring spots around current spot
			int* neighbors = new int[9];
			get_neighbors(neighbors, x, y);

			//gets if other piece is around this spot
			bool has_neighbors = false;
			for(int z = 0; z < 9; z++)
			{
				if(neighbors[z]==other_piece)
				{
					has_neighbors = true;
					break;
				}
			}


			//checks if this spot is a possible move by seeing if other pieces are flipped
			if(has_neighbors)
			{
				//gets copy of board to simulate what would happen if piece is placed
				Board board_copy;
				board_copy.copyBoard(this);

				int piece_count = board_copy.countPieces(piece);
				int other_piece_count = board_copy.countPieces(other_piece);

				board_copy.place_piece(piece, x, y);

				int new_piece_count = board_copy.countPieces(piece);
				int new_other_piece_count = board_copy.countPieces(other_piece);

				//if flipped any opponent's pieces
				if(other_piece_count > new_other_piece_count)
					possible_boards.push_back(board_copy);
			}

		}
	}


	return possible_boards;
}

void Board::get_neighbors(int* neighbors, int col, int row)
{

	//iterates through all neighbors
	int index = 0;
	for(int x = col-1; x <= col+1; x++)
	{
		for(int y = row-1; y <= row+1; y++)
		{
			//if out of bounds, skip
			if(x <0 || y <0 || x>=size || y >= size)
				neighbors[index] = -1;
			else
				neighbors[index] = board[x][y];
			
			index++;
		}
	}
}



//returns the count of piece on the board
int Board::countPieces(int piece)
{
	int count = 0;
	for(int x = 0; x < size; x++)
	{
		for(int y = 0; y < size; y++)
		{
			if(board[x][y]==piece)
				count++;
		}
	}

	return count;
}

//compares boards
bool Board::isEqual(Board other_board)
{
	bool matches = true;
	for(int x = 0; x < size; x++)
	{
		for(int y = 0; y < size; y++)
		{
			if(board[x][y]!=other_board.board[x][y])
				matches = false;
		}
	}

	return matches;
}


void Board::printBoard(int num_indents)
{
	//adds 2*num_indents spaces in front of each printed line
	string padding = "";
	for(int x = 0; x < num_indents; x++)
		padding += "   ";


	//prints column headers
	char header[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	cout<<padding<<"  "; 
	for(int x = 0; x < size; x++)
	{
		cout<<" "<<header[x]<<" ";
	}
	cout<<endl;


	//prints board contents
	for(int x = 0; x < size; x++)
	{
		//prints row header
		cout<<padding<<(x+1)<<" ";

		for(int y = 0; y < size; y++)
		{
			//1 = X
			// if(board[x][y]==1)
			// 	cout<<"[B]";
			// else if(board[x][y]==2)
			// 	cout<<"[W]";
			// else
			// 	cout<<"[ ]";
			if(board[y][x]==1)
				cout<<"[B]";
			else if(board[y][x]==2)
				cout<<"[W]";
			else
				cout<<"[ ]";
		}
		cout<<endl;
	}

}

int Board::getPieceAtPosition(int col, int row)
{
	if(col<0 || col>size || row<0 || row>size)
		return -1;

	return board[col][row];
}

void Board::copyBoard(Board *old_board)
{
	for(int x = 0; x < size; x++)
	{
		for(int y = 0; y < size; y++)
		{
			board[x][y] = old_board->getPieceAtPosition(x, y);
		}
	}
}
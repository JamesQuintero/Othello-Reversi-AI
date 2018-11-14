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
	// resetBoard();

	// srand(time(0));  // needed once per program run



}

void Board::resetBoard(char**& board)
{
	// board = new char*[size];
	createMatrix(board, this->size);

	//places initial pieces
	place_piece(board, '2', 3, 3);
	place_piece(board, '1', 4, 3);
	place_piece(board, '1', 3, 4);
	place_piece(board, '2', 4, 4);
}


//places piece down at specified position
bool Board::place_piece(char**& board, char piece, int col, int row)
{

	try{
		board[col][row] = piece;
	} catch(exception &ex)
	{
		return false;
	}

	//// flips other player's pieces////

	char other_piece = '0';


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
		else if(board[col][x] == '0')
			break;
	}

	//flips pieces up the column
	for(int x = row-1; x >= 0; x--)
	{
		//if encountered my piece, traverse back and flip previous pieces
		if(board[col][x] == piece)
		{
			//reverses back over previous squares and flips the pieces
			for(int temp_x = x+1; temp_x <= row-1; temp_x++)
				board[col][temp_x] = piece;

			break;
		}
		//if encountered blank space, stop
		else if(board[col][x] == '0')
			break;
	}

	//flips pieces right the row
	for(int x = col+1; x < size; x++)
	{
		//if encountered my piece, traverse back and flip previous pieces
		if(board[x][row] == piece)
		{
			//reverses back over previous squares and flips the pieces
			for(int temp_x = x-1; temp_x >= col+1; temp_x--)
				board[temp_x][row] = piece;

			break;
		}
		//if encountered blank space, stop
		else if(board[x][row] == '0')
			break;
	}

	//flips pieces left the row
	for(int x = col-1; x >= 0; x--)
	{
		//if encountered my piece, traverse back and flip previous pieces
		if(board[x][row] == piece)
		{
			//reverses back over previous squares and flips the pieces
			for(int temp_x = x+1; temp_x <= col-1; temp_x++)
				board[temp_x][row] = piece;

			break;
		}
		//if encountered blank space, stop
		else if(board[x][row] == '0')
			break;
	}


	//flips pieces up the positive slope diagonal
	int temp_col = col+1;
	int temp_row = row-1;
	while(temp_col < size && temp_row >= 0)
	{
		//if encountered my piece, traverse back and flip previous pieces
		if(board[temp_col][temp_row] == piece)
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
		else if(board[temp_col][temp_row] == '0')
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
		if(board[temp_col][temp_row] == piece)
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
		else if(board[temp_col][temp_row] == '0')
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
		if(board[temp_col][temp_row] == piece)
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
		else if(board[temp_col][temp_row] == '0')
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
		if(board[temp_col][temp_row] == piece)
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
		else if(board[temp_col][temp_row] == '0')
			break;

		temp_col--;
		temp_row--;
	}


	return true;
}

//returns total of the weights that piece occupies
double Board::countPositionWeights(char**& board, int level, char piece)
{
	double total = 0;
	for(int x = 0; x < size; x++)
	{
		for(int y = 0; y < size; y++)
		{
			if(board[x][y] == piece)
				total += getWeight(level, x, y);
		}
	}

	return total;
}


//returns list of coordinates of legal moves
double Board::getPossibleMovesCount(char**& board, char piece)
{

	char other_piece = '0';


	if(piece == white_piece)
		other_piece = black_piece;
	else
		other_piece = white_piece;


	int num_moves = 0;
	double num_moves_weighted = 0;
	int num_flips = 0;
	// Board board_copy;
	int num_neighbors = 0;

	// char** board_copy = createMatrix(this->size);
	char** board_copy = new char*[size];
	createMatrix(board_copy, this->size);

				

	//iterates through entire board
	for(int x = 0; x < size; x++)
	{
		for(int y = 0; y < size; y++)
		{
			//if a piece already resides here, skip
			if(board[x][y]!='0')
				continue;

			//gets neighboring spots around current spot
			char* neighbors = new char[9];
			get_neighbors(board, neighbors, x, y);

			//gets if other piece is around this spot
			short neighbor_count;
			for(int z = 0; z < 9; z++)
			{
				if(neighbors[z]==other_piece)
					neighbor_count++;
			}

			//garbage collection
			delete[] neighbors;


			//checks if this spot is a possible move by seeing if other pieces are flipped
			if(neighbor_count > 0)
			{
				//gets copy of board to simulate what would happen if piece is placed
				// int** board_copy = copyBoard(board, size);
				// board_copy.copyBoard(this);
				copyBoard(board_copy, board);

				int piece_count = countPieces(board_copy, piece);
				int other_piece_count = countPieces(board_copy, other_piece);

				place_piece(board_copy, piece, x, y);

				int new_piece_count = countPieces(board_copy, piece);
				int new_other_piece_count = countPieces(board_copy, other_piece);

				//if flipped any opponent's pieces
				if(other_piece_count > new_other_piece_count)
				{
					num_moves++;
					num_moves_weighted += weights[0][x][y];
					num_flips += (new_piece_count-piece_count - 1);
					// num_flips += (new_piece_count - piece_count);

					//add to num_neighbors separately in case want to get average
					num_neighbors += neighbor_count;
				}
			}

		}
	}

	//garbage collection
	if(board_copy!=NULL)
	{
		for(int x = 0; x < size; x++)
			delete[] board_copy[x];
		delete[] board_copy;
	}


	// return (double)num_neighbors;
	// return (double)num_moves;
	// return (double)num_moves_weighted;
	// return num_moves_weighted/(double)num_moves;
	return (double)num_flips;
	// if(num_moves > 0)
	// 	return (double)num_flips/(double)num_moves;
	// else
	// 	return 0;
}


//returns list of coordinates of legal moves
vector<vector<int>> Board::getPossibleMoveCoordinates(char**& board, char piece)
{

	vector<vector<int>> possible_moves;

	char other_piece = '0';


	if(piece == white_piece)
		other_piece = black_piece;
	else
		other_piece = white_piece;


	// Board board_copy;
	// char** board_copy = createMatrix(this->size);
	char** board_copy = new char*[size];
	createMatrix(board_copy, size);
				

	//iterates through entire board
	for(int x = 0; x < size; x++)
	{
		for(int y = 0; y < size; y++)
		{

			//if a piece already resides here, skip
			if(board[x][y]!='0')
				continue;

			//gets neighboring spots around current spot
			char* neighbors = new char[9];
			get_neighbors(board, neighbors, x, y);

			//gets if other piece is around this spot
			short neighbor_count = 0;
			for(int z = 0; z < 9; z++)
			{
				if(neighbors[z]==other_piece)
					neighbor_count++;
			}

			//garbage collection
			delete[] neighbors;


			//checks if this spot is a possible move by seeing if other pieces are flipped
			if(neighbor_count > 0)
			{
				//gets copy of board to simulate what would happen if piece is placed
				// int** board_copy = copyBoard(board, size);
				copyBoard(board_copy, board);

				int piece_count = countPieces(board_copy, piece);
				int other_piece_count = countPieces(board_copy, other_piece);

				place_piece(board_copy, piece, x, y);

				int new_piece_count = countPieces(board_copy, piece);
				int new_other_piece_count = countPieces(board_copy, other_piece);

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

	//garbage collection
	if(board_copy!=NULL)
	{
		for(int x = 0; x < size; x++)
			delete[] board_copy[x];
		delete[] board_copy;
	}


	return possible_moves;
}

// //returns list of legal board states that are possible for piece
// void Board::getPossibleMoveBoards(vector<char**>& possible_boards, char**& board, char piece)
// {

// 	//gets opponent's piece
// 	char other_piece = '0';
// 	if(piece == white_piece)
// 		other_piece = black_piece;
// 	else
// 		other_piece = white_piece;
				

// 	//iterates through entire board
// 	for(int x = 0; x < size; x++)
// 	{
// 		for(int y = 0; y < size; y++)
// 		{
// 			// cout<<"x,y: "<<x<<","<<y<<": "<<board[x][y]<<endl;
// 			//if a piece already resides here, skip
// 			if(board[x][y]!='0')
// 				continue;

// 			//gets neighboring spots around current spot
// 			char* neighbors = new char[9];
// 			get_neighbors(board, neighbors, x, y);

// 			//gets if other piece is around this spot
// 			bool has_neighbors = false;
// 			for(int z = 0; z < 9; z++)
// 			{
// 				if(neighbors[z]==other_piece)
// 				{
// 					has_neighbors = true;
// 					break;
// 				}
// 			}

// 			//garbage collection
// 			delete[] neighbors;


// 			//checks if this spot is a possible move by seeing if other pieces are flipped
// 			if(has_neighbors)
// 			{
// 				char** board_copy = new char*[this->size];
// 				createMatrix(board_copy, size);

// 				copyBoard(board_copy, board);

// 				int piece_count = countPieces(board_copy, piece);
// 				int other_piece_count = countPieces(board_copy, other_piece);

// 				place_piece(board_copy, piece, x, y);

// 				int new_piece_count = countPieces(board_copy, piece);
// 				int new_other_piece_count = countPieces(board_copy, other_piece);

// 				//if flipped any opponent's pieces
// 				if(other_piece_count > new_other_piece_count)
// 				{
// 					possible_boards.push_back(board_copy);
// 				}
// 				else
// 				{
// 					//garbage collection
// 					for(int z = 0; z < this->size; z++)
// 						delete[] board_copy[z];
// 					delete[] board_copy;
// 				}
// 			}

// 		}
// 	}
// }

void Board::get_neighbors(char**& board, char*& neighbors, int col, int row)
{

	//iterates through all neighbors
	int index = 0;
	for(int x = col-1; x <= col+1; x++)
	{
		for(int y = row-1; y <= row+1; y++)
		{
			//if out of bounds, skip
			if(x <0 || y <0 || x>=size || y >= size)
				neighbors[index] = ' ';
			else
				neighbors[index] = board[x][y];
			
			index++;
		}
	}
}

//returns the weight at specified coordinates
double Board::getWeight(int level, int col, int row)
{
	if(level<=40)
		return weights[0][col][row];
	else if(level<=50)
		return weights[1][col][row];
	else
		return weights[2][col][row];
}



//returns the count of piece on the board
int Board::countPieces(char**& board, char piece)
{
	int count = 0;
	for(int x = 0; x < size; x++)
	{
		for(int y = 0; y < size; y++)
		{
			if(board[x][y] == piece)
				count++;
		}
	}

	return count;
}

//compares boards
bool Board::isEqual(char**& board, char**& other_board)
{
	bool matches = true;
	for(int x = 0; x < size; x++)
	{
		for(int y = 0; y < size; y++)
		{
			if(board[x][y] != other_board[x][y])
			{
				matches = false;
				break;
			}
		}
	}

	return matches;
}

//returns list of coordinates where the two boards differ
vector<vector<int>> Board::getDifferenceCoordinates(char**& board, char**& board2)
{
	vector<vector<int>> coordinates;

	try
	{
		for(int x = 0; x < size; x++)
		{
			for(int y = 0; y < size; y++)
			{
				// board[x][y] = old_board->getPieceAtPosition(x, y);
				// if(board[x][y] != board2[x][y])
				if((board[x][y] != '0' && board2[x][y] == '0') || (board[x][y] == '0' && board2[x][y] != '0'))
				{
					vector<int> coordinate;
					coordinate.push_back(x);
					coordinate.push_back(y);
					coordinates.push_back(coordinate);
				}
			}
		}
	} catch(exception &ex)
	{
		cout<<"Crashed in getDifferenceCoordinates()"<<endl;
	}

	return coordinates;
}


void Board::printBoard(char**& board, int num_indents)
{

	// cout<<"printBoard()"<<endl;
	// cout<<"Addr: "<<&board<<endl;

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
			if(board[y][x]=='1')
				cout<<"[B]";
			else if(board[y][x]=='2')
				cout<<"[W]";
			else
				cout<<"[ ]";
		}
		cout<<endl;
	}

}

char Board::getPieceAtPosition(char**& board, int col, int row)
{
	if(col<0 || col>size || row<0 || row>size)
		return ' ';

	return board[col][row];
}

void Board::copyBoard(char**& board, char**& old_board)
{
	for(int x = 0; x < size; x++)
	{
		for(int y = 0; y < size; y++)
		{
			// board[x][y] = old_board->getPieceAtPosition(x, y);
			board[x][y] = old_board[x][y];
		}
	}
}
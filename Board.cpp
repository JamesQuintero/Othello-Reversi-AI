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

void Board::resetBoard(char (&board)[size+1][size+1])
{
	// board = new char*[size];
	// createMatrix(board, this->size);
	for(int x = 0; x < size; x++)
		for(int y = 0; y < size; y++)
			board[x][y] = '0';

	//places initial pieces
	place_piece(board, '2', 3, 3);
	place_piece(board, '1', 4, 3);
	place_piece(board, '1', 3, 4);
	place_piece(board, '2', 4, 4);
}


//places piece down at specified position
bool Board::place_piece(char (&board)[size+1][size+1], char piece, int col, int row)
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

//Count the number of other pieces flipped if piece placed at (col,row)
int Board::countFlips(char (&board)[size+1][size+1], char piece, int col, int row)
{

	char other_piece = '0';


	if(piece == white_piece)
		other_piece = black_piece;
	else
		other_piece = white_piece;

	int num_flips = 0;


	//flips pieces down the column
	for(int x = row+1; x < size; x++)
	{
		//if encountered my piece, traverse back and flip previous pieces
		if(board[col][x] == piece)
		{
			//reverses back over previous squares and flips the pieces
			for(int temp_x = x-1; temp_x >= row+1; temp_x--)
				num_flips++;

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
				num_flips++;

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
				num_flips++;

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
				num_flips++;

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
				num_flips++;

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
				num_flips++;

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
				num_flips++;

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
				num_flips++;

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


	return num_flips;
}

//returns total of the weights that piece occupies
double Board::countPositionWeights(char (&board)[size+1][size+1], int level, char piece)
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


//returns values relating to mobility of piece
vector<double> Board::getMobility(char (&board)[size+1][size+1], char piece)
{

	char other_piece = '0';


	if(piece == white_piece)
		other_piece = black_piece;
	else
		other_piece = white_piece;


	int num_moves = 0;
	double num_moves_weighted = 0;
	double num_flips = 0;

	int total_potential_mobility = 0;

				

	//iterates through entire board
	for(int x = 0; x < size; x++)
	{
		for(int y = 0; y < size; y++)
		{
			//if a piece already resides here, maybe count it's potential mobility, and skip counting regular mobility
			if(board[x][y]!='0')
			{
				//if not dealing with opponent's piece, then skip
				if(board[x][y] == piece)
					continue;

				//// determines potential mobility ////

				//gets neighboring spots around current spot
				char* neighbors = new char[9];
				get_neighbors(board, neighbors, x, y);

				//gets if other piece is around this spot
				short empty_space_count = 0;
				short piece_count = 0;
				for(int z = 0; z < 9; z++)
				{
					if(neighbors[z]=='0')
						empty_space_count++;
					// else if(neighbors[z]==other_piece)
					// 	piece_count++;
				}

				//garbage collection
				delete[] neighbors;


				total_potential_mobility += (empty_space_count);





				continue;
			}


			//// determines mobility ////
			int flips = countFlips(board, piece, x, y);

			//if flipped any opponent's pieces
			if(flips > 0)
			{
				num_moves++;
				num_moves_weighted += weights[0][x][y];
				num_flips += flips;
			}

		}
	}


	vector<double> to_return;
	to_return.push_back(num_flips); //returns mobility
	to_return.push_back(total_potential_mobility); //returns potential mobility

	return to_return;

	// return (double)num_neighbors;
	// return (double)num_moves;
	// return (double)num_moves_weighted;
	// return num_moves_weighted/(double)num_moves;
	// return num_flips;
	// if(num_moves > 0)
	// 	return (double)num_flips/(double)num_moves;
	// else
	// 	return 0;
}


//returns a stability value based on current pieces
double Board::getPieceStabilityScore(char (&board)[size+1][size+1], char piece)
{
	char other_piece = '0';


	if(piece == white_piece)
		other_piece = black_piece;
	else
		other_piece = white_piece;


	//counts number of stable, semi-stable, and unstable pieces
	int* stabilities = new int[3];

	int stability_score = 0;

	int stability_weights[size][size] = {
        { 4,-3, 2, 2, 2, 2,-3, 4},
        {-3,-4,-1,-1,-1,-1,-4,-3},
        { 2,-1, 1, 0, 0, 1,-1, 2},
        { 2,-1, 0, 1, 1, 0,-1, 2},
        { 2,-1, 0, 1, 1, 0,-1, 2},
        { 2,-1, 1, 0, 0, 1,-1, 2},
        {-3,-4,-1,-1,-1,-1,-4,-3},
        {4 ,-3, 2, 2, 2, 2,-3, 4}
	};

				

	//iterates through entire board
	for(int x = 0; x < size; x++)
	{
		for(int y = 0; y < size; y++)
		{
			//if piece does not reside here, skip
			if(board[x][y]==piece)
				stability_score += stability_weights[x][y];
		}
	}

	return stability_score;
}


//returns list of coordinates of legal moves
vector<vector<int>> Board::getPossibleMoveCoordinates(char (&board)[size+1][size+1], char piece)
{

	vector<vector<int>> possible_moves;

	char other_piece = '0';


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
			if(board[x][y]!='0')
				continue;

			if(countFlips(board, piece, x, y) > 0)
			{
				vector<int> pair;
				pair.push_back(x);
				pair.push_back(y);
				possible_moves.push_back(pair);
			}

			// //gets neighboring spots around current spot
			// char* neighbors = new char[9];
			// get_neighbors(board, neighbors, x, y);

			// //gets if other piece is around this spot
			// short neighbor_count = 0;
			// for(int z = 0; z < 9; z++)
			// {
			// 	if(neighbors[z]==other_piece)
			// 		neighbor_count++;
			// }

			// //garbage collection
			// delete[] neighbors;


			// //checks if this spot is a possible move by seeing if other pieces are flipped
			// if(neighbor_count > 0)
			// {
			// 	// //gets copy of board to simulate what would happen if piece is placed
			// 	// copyBoard(board_copy, board);

			// 	// int piece_count = countPieces(board_copy, piece);
			// 	// int other_piece_count = countPieces(board_copy, other_piece);

			// 	// place_piece(board_copy, piece, x, y);

			// 	// int new_piece_count = countPieces(board_copy, piece);
			// 	// int new_other_piece_count = countPieces(board_copy, other_piece);




			// 	//if flipped any opponent's pieces
			// 	// if(other_piece_count > new_other_piece_count)
			// 	// {
			// 	// 	vector<int> pair;
			// 	// 	pair.push_back(x);
			// 	// 	pair.push_back(y);
			// 	// 	possible_moves.push_back(pair);
			// 	// }
			// }

		}
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

void Board::get_neighbors(char (&board)[size+1][size+1], char*& neighbors, int col, int row)
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
int Board::countPieces(char (&board)[size+1][size+1], char piece)
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
bool Board::isEqual(char (&board)[size+1][size+1], char (&other_board)[size+1][size+1])
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
vector<vector<int>> Board::getDifferenceCoordinates(char (&board)[size+1][size+1], char (&board2)[size+1][size+1])
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


void Board::printBoard(char (&board)[size+1][size+1], int num_indents)
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

char Board::getPieceAtPosition(char (&board)[size+1][size+1], int col, int row)
{
	if(col<0 || col>size || row<0 || row>size)
		return ' ';

	return board[col][row];
}

void Board::copyBoard(char (&board)[size+1][size+1], char (&old_board)[size+1][size+1])
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
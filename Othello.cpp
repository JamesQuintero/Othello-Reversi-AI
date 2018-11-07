
#include <iostream>
#include <string>
#include <vector>
#include "global_functions.h"
#include "Othello.h"



Othello::Othello()
{
	Tree tree;

	//player is white
	player_piece = 2;
	//AI is black
	AI_piece = 1;

	resetGame();
}

//runs the game
void Othello::run()
{
	while(true)
	{

		printBoard(board, size);

		cout<<endl;
		cout<<"Menu: "<<endl;
		cout<<"1) Play against AI"<<endl;
		// cout<<"2) AI vs AI"<<endl;
		cout<<"3) Print Game Tree"<<endl;

		cout<<"Choice: ";
		int choice;
		cin>>choice;

		cout<<endl<<endl;


		//Human vs AI
		if(choice==1)
		{
				tree.piece = AI_piece;

				cout<<"New game Human vs AI"<<endl;
				resetGame();

				bool player_won = false;
				bool AI_won = false;
				bool success = true;

				//goes forever until someone has an unsuccessful move (board is full or no legal moves)
				while(success==true)
				{
					//player moves
					if(isPlayersTurn())
						success = playersMove();
					//AI moves
					else
						success = AIMove(1);

					changeTurn();
				}

				//determines who won
				if(playerWon())
				{
					player_won = true;
					cout<<endl;
					cout<<"Player WON"<<endl;
					cout<<endl<<endl;
				}
				else if(AIWon())
				{
					AI_won = true;
					cout<<"AI WON"<<endl;
					cout<<endl<<endl;
				}

		}
		//Prints the neural nets
		else
		{
			tree.printNet(tree.root);

			cout<<endl<<endl<<endl;
		}
	
	}

}


//Player's turn to move. returns true if successful
bool Othello::playersMove()
{
	cout<<"Player's move. "<<endl;
	printBoard(board, size);

	string choice = "";
	int col = -1;
	int row = -1;

	bool valid_move = false;
	while (valid_move == false)
	{
		//returns array of possible move coordinates, with each index being an array of size 2: (col, row)
		vector<vector<int>> possible_moves = possibleMoves(player_piece);


		cout<<"Legal moves: "<<endl;
		for(int x = 0; x < possible_moves.size(); x++)
		{
			string notation = convert_to_notation(possible_moves[x][0], possible_moves[x][1]);
			cout<<notation<<endl;	
		}
		cout<<endl;
		

		//no possible moves
		if(possible_moves.size()==0)
		{
			cout<<"No legal moves"<<endl;
			return false;
		}

		cout<<"Where to move? (ex: c3): ";
		cin>>choice;

		int* coordinates = new int[2];
		convert_to_coordinates(choice, coordinates);

		col = coordinates[0];
		row = coordinates[1];


		//determines if given move is a possible move
		for(int x =0; x < size*size; x++)
		{
			if(possible_moves[x][0]==col && possible_moves[x][1]==row)
			{
				valid_move=true;
				break;
			}
		}

	}

	//player places piece
	place_piece(board, player_piece, col, row);

	//add player's move to neural net
	// tree.playerMove(board);

	cout<<endl<<endl<<endl;

	return true;
}

//AI's turn to move. returns true if successful. 
bool Othello::AIMove(int AI_version)
{

	//will get children from the tree

	cout<<"AI's move. "<<endl;
	printBoard(board, size);

	string choice = "";
	int col = -1;
	int row = -1;

	bool valid_move = false;
	while (valid_move == false)
	{
		//returns array of possible move coordinates, with each index being an array of size 2: (col, row)
		vector<vector<int>> possible_moves = possibleMoves(AI_piece);


		cout<<"Legal moves: "<<endl;
		for(int x = 0; x < possible_moves.size(); x++)
		{
			string notation = convert_to_notation(possible_moves[x][0], possible_moves[x][1]);
			cout<<notation<<endl;	
		}
		cout<<endl;
		

		//no possible moves
		if(possible_moves.size()==0)
		{
			cout<<"No legal moves"<<endl;
			return false;
		}

		
		srand(time(NULL));

		int random_index = randNum(0, possible_moves.size());



		col = possible_moves[random_index][0];
		row = possible_moves[random_index][1];

		valid_move = true;
	}

	//player places piece
	place_piece(board, AI_piece, col, row);

	//add player's move to neural net
	// tree.playerMove(board);

	cout<<endl<<endl<<endl;

	return true;
}

//places piece down at specified position
bool Othello::place_piece(int** board, int piece, int col, int row)
{

	try{
		board[col][row] = piece;
	} catch(exception &ex)
	{
		return false;
	}

	//// flips other player's pieces////

	int other_piece = 0;


	if(piece==player_piece)
		other_piece = AI_piece;
	else
		other_piece = player_piece;



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

//returns coordinates of legal moves
vector<vector<int>> Othello::possibleMoves(int piece)
{

	vector<vector<int>> possible_moves;

	int other_piece = 0;


	if(piece==player_piece)
		other_piece = AI_piece;
	else
		other_piece = player_piece;


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
				int** board_copy = copyBoard(board, size);

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


	return possible_moves;
}

void Othello::get_neighbors(int* neighbors, int col, int row)
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
int Othello::countPieces(int** board, int piece)
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


//returns true if player won the game
bool Othello::playerWon()
{
	return false;
}

//returns true if AI won the game
bool Othello::AIWon()
{
	return false;
}

//returns true if it's player's turn to move.
bool Othello::isPlayersTurn()
{
	return turn;
}

//if it was player's turn, it's now AI's turn
void Othello::changeTurn()
{
	//flips the boolean
	turn = !turn;
}

//resets the game
void Othello::resetGame()
{
	board = createMatrix(size);

	//places initial pieces
	place_piece(board, 2, 3,3);
	place_piece(board, 1, 4,3);
	place_piece(board, 1, 3,4);
	place_piece(board, 2, 4,4);

	//AI goes first if turn = false
	turn = false;

	//resets current game state to root node
	tree.ptr = tree.root;
}


//converts a string position, like "d4" into index coordinates, like [4,3]
void Othello::convert_to_coordinates(string position, int * coordinates)
{
	int letter = int(position[0]);
	int number = int(position[1]); //converts "4" to ascii 52 

	int row = 0;
	int col = number - 48 - 1; //-48 to get actual number from ascii, then -1 to get proper index

	//if letter is lowercase
	if (letter >= 97)
		row = letter-97;
	//if letter is uppercase
	else
		row = letter-65;


	// cout<<"Letter: "<<letter<<endl;
	// cout<<"Number: "<<number<<endl;
	// cout<<"coor: ("<<col<<","<<row<<")"<<endl<<endl;


	// coordinates[0] = col;
	// coordinates[1] = row;
	coordinates[0] = row;
	coordinates[1] = col;
}

//converts a string position, like "d4" into index coordinates, like [4,3]
string Othello::convert_to_notation(int col, int row)
{
	char letter = 'a'+col;
	char c = '1'+row;

	string to_return = string() + letter+c;
	return to_return;
}

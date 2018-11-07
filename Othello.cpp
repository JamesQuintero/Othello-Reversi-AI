
#include <iostream>
#include <string>
#include "global_functions.h"
#include "Othello.h"


using namespace std;

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

		cout<<"Menu: "<<endl;
		cout<<"1) Play against AI"<<endl;
		// cout<<"2) AI vs AI"<<endl;
		cout<<"3) Print Game Tree"<<endl;

		cout<<"Choice: ";
		int choice;
		cin>>choice;


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
	cout<<"Current board: "<<endl;
	printBoard(board, size);

	string choice = "";
	int col = -1;
	int row = -1;

	bool valid_move = false;
	while (valid_move == false)
	{
		//returns array of possible move coordinates, with each index being an array of size 2: (col, row)
		int** possible_moves = new int*[size*size];
		possibleMoves(possible_moves);

		//no possible moves
		if(possible_moves[0][0]==-1)
			return false;

		cout<<"Position (ex: c3): ";
		cin>>choice;

		cout<<"You chose: "<<choice<<endl;

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

	cout<<"Is valid move"<<endl;

	//player places piece
	place_piece(player_piece, choice);

	//add player's move to neural net
	// tree.playerMove(board);

	cout<<endl<<endl<<endl;

	return true;
}

//AI's turn to move. returns true if successful. 
bool Othello::AIMove(int AI_version)
{

	return true;
}

//places piece down at specified position
bool Othello::place_piece(int piece, string position)
{
	int* coordinates;
	convert_to_coordinates(position, coordinates);

	int col = coordinates[0];
	int row = coordinates[1];

	try{
		board[col][row] = piece;
	} catch(exception &ex)
	{
		return false;
	}

	return true;
}

//converts a string position, like "d4" into index coordinates, like [4,3]
void Othello::convert_to_coordinates(string position, int coordinates[])
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


	coordinates[0] = col;
	coordinates[1] = row;
}

//returns coordinates of possible board configurations for next move consideration.
//utreeeeded parts of the list have coordinates (-1,-1).
void Othello::possibleMoves(int ** moves)
{
	int index = 0;

	for(int x = 0; x < size; x++)
	{
		for(int y = 0; y < size; y++)
		{
			//open space to move
			if(board[x][y]==0)
			{
				int* pair = new int[2];
				pair[0]=x;
				pair[1]=y;
				moves[index] = pair;
				// cout<<"Addr "<<index<<" :"<<moves[index]<<endl;
				index++;
			}
		}
	}

	//voids rest of list
	for(int x = index; x < size*size; x++)
	{
		int* pair = new int[2];
		pair[0]=-1;
		pair[1]=-1;

		moves[index] = pair;
		// cout<<"Addr "<<index<<" :"<<moves[index]<<endl;
		index++;
	}
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
	place_piece(2, "d4");
	place_piece(1, "e4");
	place_piece(1, "d5");
	place_piece(2, "e5");

	//AI goes first if turn = false
	turn = false;

	//resets current game state to root node
	tree.ptr = tree.root;
}

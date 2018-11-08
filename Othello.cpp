
#include <iostream>
#include <string>
#include <vector>
#include "global_functions.h"
#include "Othello.h"



Othello::Othello()
{
	// Tree tree;

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

		// printBoard(board, size);
		board.printBoard();

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
				// tree.piece = AI_piece;

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
			// tree.printNet(tree.root);

			cout<<endl<<endl<<endl;
		}
	
	}

}


//Player's turn to move. returns true if successful
bool Othello::playersMove()
{
	cout<<"Player's move. "<<endl;
	// printBoard(board, size);
	board.printBoard();

	string choice = "";
	int col = -1;
	int row = -1;

	bool valid_move = false;
	while (valid_move == false)
	{
		//returns array of possible move coordinates, with each index being an array of size 2: (col, row)
		vector<vector<int>> possible_moves = board.possibleMoves(player_piece);


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
		for(int x =0; x < possible_moves.size(); x++)
		{
			if(possible_moves[x][0]==col && possible_moves[x][1]==row)
			{
				valid_move=true;
				break;
			}
		}

	}

	//player places piece
	// place_piece(board, player_piece, col, row);
	board.place_piece(player_piece, col, row);

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
	board.printBoard();

	string choice = "";
	int col = -1;
	int row = -1;

	bool valid_move = false;
	while (valid_move == false)
	{
		//returns array of possible move coordinates, with each index being an array of size 2: (col, row)
		vector<vector<int>> possible_moves = board.possibleMoves(AI_piece);


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
	// place_piece(board, AI_piece, col, row);
	board.place_piece(AI_piece, col, row);

	//add player's move to neural net
	// tree.playerMove(board);

	cout<<endl<<endl<<endl;

	return true;
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
	board.resetBoard();

	//AI goes first if turn = false
	turn = false;

	//resets current game state to root node
	// tree.ptr = tree.root;
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

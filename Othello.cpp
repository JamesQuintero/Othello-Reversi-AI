
#include <iostream>
#include <string>
#include <vector>
#include "global_functions.h"
#include "Othello.h"



Othello::Othello()
{

	//player is white
	player_piece = '2';
	//AI is black
	AI_piece = '1';

	tree.player_piece = player_piece;
	tree.AI_piece = AI_piece;


	resetGame(true);

	//gets first possible moves for black
	// tree.determinePossibleMoves(&*tree.ptr, AI_piece);
}

//runs the game
void Othello::run()
{
	while(true)
	{
		cout<<endl;
		cout<<"Menu: "<<endl;
		cout<<"1) Play against AI"<<endl;
		cout<<"2) AI against AI"<<endl;
		// cout<<"2) AI vs AI"<<endl;
		cout<<"3) Print Game Tree"<<endl;
		cout<<"4) Purge Game Tree"<<endl;

		cout<<"Choice: ";
		int choice;
		cin>>choice;

		// int choice = 1;

		cout<<endl<<endl;


		//Human vs AI
		if(choice==1)
		{
				tree.piece = AI_piece;
				//the player will be at a disadvantage
				tree.worse_heuristic_piece = player_piece;

				cout<<"New game Human vs AI"<<endl;
				resetGame();

				
				//goes forever until someone has an unsuccessful move (board is full or no legal moves)
				bool player_move_success = true;
				bool AI_move_success = true;
				while(player_move_success == true || AI_move_success == true)
				{
					//player moves
					if(isPlayersTurn())
						player_move_success = playersMove(0);
					//AI moves
					else
						AI_move_success = AIMove(1);

					changeTurn();
				}

				//determines who won
				int player_won = determineWinner();
				if(player_won==1)
				{
					cout<<endl;
					cout<<"Player WON"<<endl;
					cout<<endl<<endl;

					//gives bad reinforcement to tree
					tree.reinforceGood(tree.ptr);
				}
				else if(player_won==-1)
				{
					cout<<endl;
					cout<<"AI WON"<<endl;
					cout<<endl<<endl;

					//gives good reinforcement to tree
					tree.reinforceBad(tree.ptr);
				}
				else
				{
					cout<<endl;
					cout<<"TIE"<<endl;
					cout<<endl<<endl;
				}

				int num_player_pieces = board_obj.countPieces(tree.ptr->board, player_piece);
				int num_AI_pieces = board_obj.countPieces(tree.ptr->board, AI_piece);
				cout<<num_player_pieces<<" to "<<num_AI_pieces<<endl;

		}
		//AI vs AI
		else if(choice==2)
		{
				tree.piece = AI_piece;
				//the player will be at a disadvantage
				tree.worse_heuristic_piece = player_piece;

				int num_games = 0;
				cout<<"Num games: ";
				cin>>num_games;

				char verbose_choice;
				cout<<"Want to print game states as they play? (y/n): ";
				cin>>verbose_choice;

				bool verbose = false;
				if(verbose_choice=='y')
					verbose = true;

				char reset_tree_choice;
				cout<<"Reset tree every time? (y/n): ";
				cin>>reset_tree_choice;

				bool reset_tree = false;
				if(reset_tree_choice=='y')
					reset_tree = true;



				cout<<"New game AI vs AI"<<endl;
				int total_player_pieces = 0;
				int total_AI_pieces = 0;
				int total_player_wins = 0;
				int total_AI_wins = 0;
				for(int x = 0; x < num_games; x++)
				{
					// cout<<"Resetting game"<<endl;
					resetGame(reset_tree);

					
					//goes forever until someone has an unsuccessful move (board is full or no legal moves)
					bool player_move_success = true;
					bool AI_move_success = true;
					while(player_move_success == true || AI_move_success == true)
					{
						//player moves
						if(isPlayersTurn())
							player_move_success = playersMove(1);
						//AI moves
						else
							AI_move_success = AIMove(1, verbose);

						changeTurn();
					}

					cout<<"Game "<<x<<" ";

					//determines who won
					int player_won = determineWinner();
					if(player_won==1)
					{
						total_player_wins++;

						cout<<"AI #1 Won ";

						//gives bad reinforcement to tree
						tree.reinforceGood(tree.ptr);
					}
					else if(player_won==-1)
					{
						total_AI_wins++;

						cout<<"AI #2 Won ";

						//gives good reinforcement to tree
						tree.reinforceBad(tree.ptr);
					}
					else
					{
						cout<<"TIE ";

						//gives good reinforcement to tree
						tree.reinforceBad(tree.ptr);
					}

					int num_player_pieces = board_obj.countPieces(tree.ptr->board, player_piece);
					int num_AI_pieces = board_obj.countPieces(tree.ptr->board, AI_piece);
					total_player_pieces += num_player_pieces;
					total_AI_pieces += num_AI_pieces;

					cout<<num_player_pieces<<" to "<<num_AI_pieces<<endl;

					// cout<<endl;
					// cout<<"End board: "<<endl;
					// tree.printNode(tree.ptr);
				}

				// tree.printNode(tree.ptr);

				cout<<endl<<endl;
				cout<<"AI #1 wins: "<<total_player_wins<<endl;
				cout<<"AI #2 wins: "<<total_AI_wins<<endl;
				cout<<"Average AI #1 pieces: "<<((double)total_player_pieces/(double)num_games)<<endl;
				cout<<"Average AI #2 pieces: "<<((double)total_AI_pieces/(double)num_games)<<endl;
				cout<<endl;
		}
		//Prints the neural nets
		else if(choice == 3)
		{
			tree.printNet(tree.root);
			// tree.determinePossibleMoves(&*tree.ptr, AI_piece);

			cout<<endl<<endl<<endl;
		}
		else if(choice == 4)
		{
			resetGame(true);
		}
	
	}

}


//Player's turn to move. returns true if successful
bool Othello::playersMove(int player_type)
{
	// cout<<"Player moving"<<endl;
	// tree.printNode(tree.ptr);

	//also traverse the tree in player so that if AI has to skip its move 
	tree.iterateTreeDepth(tree.ptr, player_piece, 1, tree.max_depth);
	// tree.getMaxHeuristic(tree.ptr, tree.ptr, MIN, MAX, tree.max_h_depth+1);
	tree.negamax(tree.ptr, tree.ptr, tree.max_h_depth+2, MIN, MAX, 1);


	
	//print if player is playing
	if(player_type == 0)
	{
		// cout<<"Player's move. "<<endl;
		// tree.printNet(tree.ptr);
		// tree.printNode(tree.ptr);
	}

	// //for testing, prints out tree
	// cout<<"For testing"<<endl;
	// tree.printNet(tree.ptr);
	// string to_continue;
	// cin>>to_continue;

	string choice = "";
	int col = -1;
	int row = -1;

	bool valid_move = false;
	while (valid_move == false)
	{
		//returns array of possible move coordinates, with each index being an array of size 2: (col, row)
		vector<vector<int>> possible_moves = board_obj.getPossibleMoveCoordinates(tree.ptr->board, player_piece);

		//no possible moves
		if(possible_moves.size()==0)
		{
			//print if player is playing, otherwise it's an AI
			if(player_type==0)
				cout<<"No legal moves"<<endl;
			return false;
		}

		//print if player is playing, otherwise it's an AI
		if(player_type == 0)
		{
			cout<<"Legal moves: "<<endl;
			for(int x = 0; x < possible_moves.size(); x++)
			{
				string notation = convert_to_notation(possible_moves[x][0], possible_moves[x][1]);
				cout<<notation<<endl;	
			}
			cout<<endl;
		}
	


		//// Comment this if you want the player to move randomly ////
		if(player_type==0)
		{
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

		//if AI is playing for player
		if(player_type==1)
		{
			// cout<<"Printing net"<<endl;
			// tree.printNet(tree.ptr);

			//Get MAX heuristic and move there. But pass in that it's a player requesting it so that it gets the worse heuristic. 
			//returns board corresponding with the minimum heuristic
			char** new_board = tree.getBoardMinHeuristic(tree.ptr);
			// char** new_board = tree.getBoardMaxHeuristic(tree.ptr);

			// cout<<"New board: "<<endl;
			// board_obj.printBoard(new_board);

			//add player's move to neural net
			tree.move(new_board);





			//// unncomment this if you want the player to move randomly ////
			// int random_index = randNum(0, possible_moves.size());
			// col = possible_moves[random_index][0];
			// row = possible_moves[random_index][1];
			// valid_move = true;

			//stops the method so that the code after the loop doesn't run, thereby double moving
			return true;
		}

	}

	//player places piece
	// place_piece(board, player_piece, col, row);
	// board_obj.place_piece(tree.ptr->board, player_piece, col, row);

	//add player's move to neural net
	tree.playerMove(col, row);
	// tree.iterateTreeDepth(tree.ptr, AI_piece, 1, tree.max_depth);


	//print if player is playing
	if(player_type==0)
		cout<<endl<<endl<<endl;

	return true;
}

//AI's turn to move. returns true if successful. 
bool Othello::AIMove(int AI_version, bool verbose)
{
	tree.iterateTreeDepth(tree.ptr, AI_piece, 1, tree.max_depth);
	// tree.getMinHeuristic(tree.ptr, tree.ptr, MIN, MAX, tree.max_h_depth+1);
	tree.negamax(tree.ptr, tree.ptr, tree.max_h_depth+2, MIN, MAX, -1);

	//will get children from the tree

	// //for testing, prints out tree
	// tree.printNet(tree.ptr);
	// cout<<"AI's move. "<<endl;
	// tree.printNode(tree.ptr);
	// string to_continue;
	// cin>>to_continue;
	

	string choice = "";
	int col = -1;
	int row = -1;

	// Board new_board;
	char** new_board; 
	bool valid_move = false;
	while (valid_move == false)
	{
		//if no boards
		if(tree.hasLegalMoves(tree.ptr) == false)
			return false;

		//returns board corresponding with the minimum heuristic
		new_board = tree.getBoardMinHeuristic(tree.ptr);
		// new_board = tree.getBoardMaxHeuristic(tree.ptr);
		
		

		valid_move = true;
	}

	//add AI's move to neural net
	tree.move(new_board);

	//get where AI moved: 
	vector<vector<int>> coordinates = board_obj.getDifferenceCoordinates(tree.ptr->board, tree.ptr->prev->board);

	if(verbose == true)
	{
		cout<<"AI's move: "<<convert_to_notation(coordinates[0][0], coordinates[0][1])<<endl;
		cout<<endl<<endl;
	}

	return true;
}


//returns true if player won the game
int Othello::determineWinner()
{
	int num_player_pieces = board_obj.countPieces(tree.ptr->board, player_piece);
	// cout<<"Num player pieces: "<<num_player_pieces<<endl;

	int num_AI_pieces = board_obj.countPieces(tree.ptr->board, AI_piece);
	// cout<<"Num AI pieces: "<<num_AI_pieces<<endl;

	if(num_player_pieces > num_AI_pieces)
		return 1;
	else if(num_player_pieces < num_AI_pieces)
		return -1;
	else
		return 0;
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
void Othello::resetGame(bool reset_tree)
{
	// board_obj.resetBoard();

	//AI goes first if turn = false
	turn = false;

	//resets current game state to root node
	tree.ptr = tree.root;

	// board_obj.printBoard(tree.ptr->board);

	if(reset_tree)
	{
		tree.resetTree();
		//gets first possible moves for black
		tree.determinePossibleMoves(&*tree.ptr, AI_piece);
	}


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

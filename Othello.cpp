
#include <iostream>
#include <string>
#include <fstream>
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

}

//runs the game
void Othello::run()
{
	while(true)
	{
		// cout<<endl;
		// cout<<"Menu: "<<endl;
		// cout<<"1) Play against AI"<<endl;
		// cout<<"2) AI against AI"<<endl;
		// cout<<"3) Print Game Tree"<<endl;
		// cout<<"4) Purge Game Tree"<<endl;

		// cout<<"Choice: ";
		// int choice;
		// cin>>choice;

		int choice = 1;

		cout<<endl<<endl;


		//Human vs AI
		if(choice==1)
		{
				tree.piece = AI_piece;
				//the player will be at a disadvantage
				tree.worse_heuristic_piece = player_piece;

				// int depth = 0;
				// cout<<"Depth (4 is recommended): ";
				// cin>>depth;
				int depth = 4;

				tree.max_depth = depth;
				tree.max_h_depth = depth;

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
					{
						vector<int> move = AIMove(1);
						if(move.size()==2)
							AI_move_success = true;
						else
							AI_move_success = false;
						// AI_move_success = AIMove(1);
					}

					//clears tree except for tree.ptr, which becomes new root
					tree.eraseParentNodes(tree.ptr);

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

				// cout<<"Moves: "<<endl;
				// for(int x = 0; x < moves.size(); x++)
				// {
				// 	cout<<moves[x]<<", ";
				// 	if(x%2 != 0)
				// 		cout<<endl;
				// }

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

				int depth = 0;
				cout<<"Depth (4 is recommended): ";
				cin>>depth;

				tree.max_depth = depth;
				tree.max_h_depth = depth;

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
				// bool reset_tree = true;


				// //opens move file
				// ofstream myfile;
				// myfile.open("moves.txt");



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
						{
							vector<int> move = AIMove(1, verbose);
							if(move.size()==2)
								AI_move_success = true;
							else
								AI_move_success = false;
							// AI_move_success = true
						}

						//clears tree except for tree.ptr, which becomes new root
						tree.eraseParentNodes(tree.ptr);

						changeTurn();
					}

					//prints out moves vector
					cout<<"Moves: "<<endl;
					// myfile << "Moves: \n";
					for(int x = 0; x < moves.size(); x++)
					{
						cout<<moves[x]<<", ";
						// myfile<<moves[x]<<", ";
						if(x%2 != 0)
						{
							cout<<endl;
							// myfile<<"\n";
						}
					}
					// myfile<<"\n";
					cout<<endl;



					cout<<"Game "<<x<<" ";
					// myfile<<"Game "<<x<<"\n";

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
					// myfile<<num_player_pieces<<" to "<<num_AI_pieces<<"\n\n\n";

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

				// myfile.close();

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
	//also traverse the tree in player so that if AI has to skip its move 
	//Minimax
	tree.iterateTreeDepth(tree.ptr, player_piece, 1, tree.max_depth);

	//calculates heuristics
	tree.getMaxHeuristic(tree.ptr, tree.ptr, MIN, MAX, tree.max_h_depth+2);

	//Negamax
	// tree.negamax(tree.ptr, tree.ptr, tree.max_h_depth+2, MIN, MAX, 1);


	
	//print if player is playing, and not AI vs AI
	if(player_type == 0)
	{
		cout<<"Player's move. "<<endl;
		tree.printNode(tree.ptr);
	}


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
			moves.push_back("-");

			return false;
		}

		//print if player is playing, otherwise it's an AI
		if(player_type == 0)
		{
			cout<<"Legal moves: "<<endl;
			for(int x = 0; x < possible_moves.size(); x++)
			{
				//prints string notation, ex: c3
				string notation = convert_to_notation(possible_moves[x][0], possible_moves[x][1]);
				cout<<notation<<endl;	

				// //prints coordinates
				// cout<<"("<<possible_moves[x][0]<<", "<<possible_moves[x][1]<<")"<<endl;
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

			// int i = 0;
			// int j = 0;
			// cout<<"Where to move? (i,j) "<<endl;
			// cout<<"i: ";
			// cin>>i;
			// cout<<"j: ";
			// cin>>j;

			// int coordinates[2] = {i, j};


			//adds to global moves list
			choice = convert_to_notation(coordinates[0], coordinates[1]);
			moves.push_back(choice);

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

			//Get MAX heuristic and move there. But pass in that it's a player requesting it so that it gets the worse heuristic. 
			//returns child index corresponding with the minimum heuristic
			// int child_index = tree.getIndexMinHeuristic(tree.ptr);
			int child_index = tree.getIndexMaxHeuristic(tree.ptr);

			//add player's move to neural net
			tree.move(child_index);


			//determines player's move
			vector<vector<int>> coordinates = board_obj.getDifferenceCoordinates(tree.ptr->board, tree.ptr->prev->board);
			string move = convert_to_notation(coordinates[0][0], coordinates[0][1]);
			moves.push_back(move);



			//stops the method so that the code after the loop doesn't run, thereby double moving
			return true;
		}

	}

	//add player's move to neural net
	tree.playerMove(col, row);


	//print if player is playing
	if(player_type==0)
		cout<<endl<<endl<<endl;

	return true;
}




//AI's turn to move. returns a vector of the coordinates where AI moved
vector<int> Othello::AIMove(int AI_version, bool verbose)
{
	tree.iterateTreeDepth(tree.ptr, AI_piece, 1, tree.max_depth);
	tree.getMinHeuristic(tree.ptr, tree.ptr, MIN, MAX, tree.max_h_depth+2);
	// tree.negamax(tree.ptr, tree.ptr, tree.max_h_depth+2, MIN, MAX, -1);
	

	//coordinates to return
	vector<int> to_return;

	//if no boards
	if(tree.hasLegalMoves(tree.ptr) == false)
	{
		moves.push_back("-");
		// return false;
		return to_return;
	}

	//returns board corresponding with the minimum heuristic
	int child_index = tree.getIndexMinHeuristic(tree.ptr);
		

	//add AI's move to neural net
	tree.move(child_index);

	//get where AI moved: 
	vector<vector<int>> coordinates = board_obj.getDifferenceCoordinates(tree.ptr->board, tree.ptr->prev->board);


	string move = convert_to_notation(coordinates[0][0], coordinates[0][1]);
	moves.push_back(move);

	if(verbose == true)
	{
		cout<<"AI's move: "<<endl;
		cout<<"("<<coordinates[0][0]<<", "<<coordinates[0][1]<<")"<<endl;
		// cout<<move<<endl;
		cout<<endl<<endl;

	}

	// return true;
	to_return.push_back(coordinates[0][0]);
	to_return.push_back(coordinates[0][1]);
	return to_return;
}


//returns true if player won the game
int Othello::determineWinner()
{
	int num_player_pieces = board_obj.countPieces(tree.ptr->board, player_piece);

	int num_AI_pieces = board_obj.countPieces(tree.ptr->board, AI_piece);

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
	//AI goes first if turn = false
	turn = false;

	//resets current game state to root node
	tree.ptr = tree.root;

	moves.clear();

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

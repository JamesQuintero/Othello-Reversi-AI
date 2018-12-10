#include <iostream>
#include <string>
#include <ctime>
// #include "sqlite/sqlite3.h"

#include "global_functions.h"
#include "Tree.h"



using namespace std;

Tree::Tree()
{
	resetTree();

	srand(time(0));  // needed once per program run
}

void Tree::eraseParentNodes(node * ptr)
{
	eraseBranch(root, ptr);

	this->root = ptr;
	this->ptr = ptr;
}

//erases ptr and its children, as long as it's not ptr_to_keep
void Tree::eraseBranch(node * ptr, node * ptr_to_keep)
{
	if(ptr != NULL && ptr!=ptr_to_keep)
	{
		for(int x = 0; x < numChildren(ptr); x++)
			eraseBranch(ptr->next[x], ptr_to_keep);

		delete ptr;
		ptr = NULL;
	}
}

void Tree::resetTree()
{
	// cout<<"Erasing tree"<<endl;
	eraseTree(root);

	// cout<<"New root: "<<endl;
	root = new node();

	board_obj.resetBoard(root->board);

	// printNode(root);

	this->ptr = root;
}

void Tree::eraseTree(node * ptr)
{
	if(ptr != NULL)
	{
		for(int x = 0; x < numChildren(ptr); x++)
			eraseTree(ptr->next[x]);

		delete ptr;
		ptr = NULL;
	}
}

//determines the possible moves by piece, then adds them as children to the current pointer
void Tree::determinePossibleMoves(node* ptr, char piece)
{
	//stop if has children, meaning it's already been expanded
	if(numChildren(ptr)>0)
		return;

	vector<vector<int>> coordinates = board_obj.getPossibleMoveCoordinates(ptr->board, piece);

	for(int x = 0; x < coordinates.size(); x++)
	{
		//creates new node with current board
		newNode(ptr, ptr->board, piece);
		//places piece at new location of new board
		board_obj.place_piece(ptr->next[numChildren(ptr)-1]->board, piece, coordinates[x][0], coordinates[x][1]);
	}
	
}

//returns tree if there are legal moves, i.e. if there are children to specified node
bool Tree::hasLegalMoves(node* ptr)
{
	if(numChildren(ptr)>0)
		return true;
	else
		return false;
}

//returns the node with the smallest heuristic
int Tree::getIndexMinHeuristic(node* ptr)
{
	vector<int> minimum_indices;

	double min_heuristic = numeric_limits<double>::max();
	for(int x = 0; x < numChildren(ptr); x++)
	{
		double h = ptr->next[x]->h;
		if(h < min_heuristic)
		{
			min_heuristic = h;
			minimum_indices.clear();
			minimum_indices.push_back(x);
		}
		else if(h==min_heuristic)
			minimum_indices.push_back(x);

	}

	//chooses randomly out of all the smallest heuristics
	int random_index = randNum(0, minimum_indices.size());
	int index = minimum_indices[random_index];

	return index;
}


//returns the node with the largest heuristic
int Tree::getIndexMaxHeuristic(node* ptr)
{
	vector<int> maximum_indices;

	double max_heuristic = numeric_limits<double>::lowest();
	for(int x = 0; x < numChildren(ptr); x++)
	{
		double h = ptr->next[x]->h;

		if(h > max_heuristic)
		{
			max_heuristic = h;
			maximum_indices.clear();
			maximum_indices.push_back(x);
		}
		else if(h == max_heuristic)
			maximum_indices.push_back(x);

	}

	//chooses randomly out of all the smallest heuristics
	int random_index = randNum(0, maximum_indices.size());
	int index = maximum_indices[random_index];

	return index;
}


//runs monte carlo 
void Tree::monteCarlo(node* ptr, int num_runs)
{
	//initializes temporary board
	char board[size+1][size+1] = {"00000000",
								"00000000",
								"00000000",
								"00000000",
								"00000000",
								"00000000",
								"00000000",
								"00000000"};

	
	//performs num_runs number of runs for Monte Carlo tree search
	for(int i = 0; i < num_runs; i++)
	{
		//sets board to the current board
		board_obj.copyBoard(board, ptr->board);


		char piece_to_go = getOtherPiece(ptr->piece);
		short failed_attempts = 0;
		vector<int> x_moves;
		vector<int> y_moves;

		//goes until haven't moved successfully for 2 times, which means neither player can move, therefore stop the random moves
		while(failed_attempts < 2)
		{

			//gets possible moves
			for(int x = 0; x < size; x++)
			{
				for(int y = 0; y < size; y++)
				{
					if(board[x][y]=='0')
					{
						if(board_obj.countFlips(board, piece_to_go, x, y) > 0)
						{
							x_moves.push_back(x);
							y_moves.push_back(y);
						}
					}
				}
			}

			//if there is a legal move
			if(x_moves.size() > 0)
			{
				//chooses randomly out of all the smallest heuristics
				int random_index = randNum(0, x_moves.size());

				//places piece on temporary board
				board_obj.place_piece(board, piece_to_go, x_moves[random_index], y_moves[random_index]);

				x_moves.clear();
				y_moves.clear();
			}
			//increments number of failed attempts
			else
				failed_attempts++;

			//changes turn
			piece_to_go = getOtherPiece(piece_to_go);
		}


		//counts number of pieces for each player to determine the winner
		int num_piece = board_obj.countPieces(board, ptr->piece);
		int num_other_piece = board_obj.countPieces(board, getOtherPiece(ptr->piece));


		//// Back propogation ////
		node* temp = ptr;
		while(temp != root)
		{
			//if this node's piece won
			if( (temp->piece == ptr->piece && num_piece > num_other_piece) || (temp->piece != ptr->piece && num_piece < num_other_piece))
				temp->num_wins++;

			temp->num_plays++;

			temp = temp->prev;
		}
	}
}



//different version of minimax
//is_maximizing is {-1,1} if 
double Tree::negamax(node* start, node* ptr, int depth_left, double alpha /* starts as -INFINITY */, double beta /* starts as INFINITY */, int is_maximizing)
{
	double cur_heuristic = -1;
	//if has no children or reached max depth
	if(numChildren(ptr) == 0 || depth_left <= 0)
	{
		// cout<<"Num children: "<<numChildren(ptr)<<", depth left: "<<depth_left<<endl;
		// printNode(ptr);
		// cout<<endl;
		cur_heuristic = calculateHeuristic(start, ptr) * is_maximizing;
		ptr->h = cur_heuristic;

		return cur_heuristic;
	}


	//Negamax. 
	// function negamax(node, depth, α, β, color) is
	//     if depth = 0 or node is a terminal node then
	//         return color × the heuristic value of node

	//     childNodes := generateMoves(node)
	//     childNodes := orderMoves(childNodes)

	//     value := −∞
	//     foreach child in childNodes do
	//         value := max(value, −negamax(child, depth − 1, −β, −α, −color))
	//         α := max(α, value)
	//         if α ≥ β then
	//             break (* cut-off *)
	//     return value


	//iterates through all children
	double max_h = numeric_limits<double>::lowest(); // min value
	double h = max_h;
	for(int x = 0; x < numChildren(ptr); x++)
	{
		//have depth_left-1 instead of depth_left--, because then that actually decrements depth_left
		h = -negamax(start, ptr->next[x], depth_left-1, -beta, -alpha, -is_maximizing);
		if(h > max_h)
			max_h = h;

		if(max_h > alpha)
			alpha = max_h;

		//prunes
		if(beta <= alpha)
			break;

	}

	ptr->h = max_h;

	return max_h;
}



//min version of Minimax
//calculates heuristics for AI down to a certain depth
//considers current node as the AI's move, and therefore will be minimizing heuristic
double Tree::getMinHeuristic(node* start, node * ptr, double alpha /* starts as -INFINITY */, double beta /* starts as INFINITY */, int depth_left)
{
	double cur_heuristic = -1;

	//if has no children
	if(numChildren(ptr) == 0 || depth_left<=0)
	{
		cur_heuristic = calculateHeuristic(start, ptr);
		ptr->h = cur_heuristic;

		return cur_heuristic;
	}

	// bestVal = +INFINITY 
 //    for each child node :
 //        value = minimax(node, depth+1, true, alpha, beta)
 //        bestVal = min( bestVal, value) 
 //        beta = min( beta, bestVal)
 //        if beta <= alpha:
 //            break
 //    return bestVal


	//iterates through all children
	double min_heuristic = numeric_limits<double>::max(); // max value
	for(int x = 0; x < numChildren(ptr); x++)
	{
		double h = getMaxHeuristic(start, ptr->next[x], alpha, beta, depth_left-1);
		if(h < min_heuristic)
			min_heuristic = h;

		if(min_heuristic < beta)
			beta = min_heuristic;

		//prunes
		if(beta <= alpha)
			break;

	}

	ptr->h = min_heuristic;

	return min_heuristic;


}

//max portion of Minimax
double Tree::getMaxHeuristic(node* start, node * ptr, double alpha /* starts as -INFINITY */, double beta /* starts as INFINITY */, int depth_left)
{
	double cur_heuristic = -1;
	
	//if has no children
	if(numChildren(ptr) == 0 || depth_left<=0)
	{
		cur_heuristic = calculateHeuristic(start, ptr);
		ptr->h = cur_heuristic;

		return cur_heuristic;
	}

	// bestVal = -INFINITY 
 //    for each child node :
 //        value = minimax(node, depth+1, false, alpha, beta)
 //        bestVal = max( bestVal, value) 
 //        alpha = max( alpha, bestVal)
 //        if beta <= alpha:
 //            break
 //    return bestVal


	//iterates through all children
	double max_heuristic = numeric_limits<double>::lowest(); // minimum value
	for(int x = 0; x < numChildren(ptr); x++)
	{
		double h = getMinHeuristic(start, ptr->next[x], alpha, beta, depth_left-1);
		if(h > max_heuristic)
			max_heuristic = h;

		if(max_heuristic > alpha)
			alpha = max_heuristic;

		//prune the rest
		if(beta <= alpha)
			break;

	}
	ptr->h = max_heuristic;

	return max_heuristic;
}

//returns heuristic for specified pointer
double Tree::calculateHeuristic(node* start, node* ptr)
{
	int level = ptr->level;
	
	int player_count = board_obj.countPieces(ptr->board, player_piece);
	int AI_count = board_obj.countPieces(ptr->board, AI_piece);

	//returns total of score where score = player_piece_position * weight_at_position. 
	double player_score = board_obj.countPositionWeights(ptr->board, level, player_piece);
	double AI_score = board_obj.countPositionWeights(ptr->board, level, AI_piece);

	double player_stability_score = board_obj.getPieceStabilityScore(ptr->board, player_piece);
	double AI_stability_score = board_obj.getPieceStabilityScore(ptr->board, AI_piece);

	// //runs Monte-carlo tree search
	// monteCarlo(ptr, 100);
	// double MCTS_ratio = 100 - ((double)ptr->num_wins/(double)ptr->num_plays)*100;




	double AI_mobility = 0;
	double AI_potential_mobility = 0;
	double player_mobility = 0;
	double player_potential_mobility = 0;

	double total_player_mobility = 0;
	double total_player_potential_mobility = 0;
	double total_AI_mobility = 0;
	double total_AI_potential_mobility = 0;
	double count = 0;
	node* temp = ptr;


	//traverses from current node to start node
	//when traversing up counting possible moves for player and AI, try and average per level so that it's not a total. 
	//A total would give a lower total for unvisited children, and therefore not be a good heuristic. 
	while(true)
	{
		//if haven't calculated this node's mobility yet
		vector<double> AI_mobility_vector = board_obj.getMobility(temp->board, AI_piece);
		AI_mobility = AI_mobility_vector[0];
		AI_potential_mobility = AI_mobility_vector[1];

		//player's mobility
		vector<double> player_mobility_vector = board_obj.getMobility(temp->board, player_piece);
		player_mobility = player_mobility_vector[0];
		player_potential_mobility = player_mobility_vector[1];


		total_AI_mobility += AI_mobility;
		total_AI_potential_mobility += AI_potential_mobility;
		total_player_mobility += player_mobility;
		total_player_potential_mobility += player_potential_mobility;

		count++;

		//stop after processing start node
		if(temp == start)
			break;

		temp = temp->prev;
	}
	player_mobility = total_player_mobility/count;
	player_potential_mobility = total_player_potential_mobility/count;
	AI_mobility = total_AI_mobility/count;
	AI_potential_mobility = total_AI_potential_mobility/count;


	//// Reinforcement learning portion ////
	double good = ptr->good;
	double bad = ptr->bad;




	/// Dynamic weighting ///

	double heuristic = 0;
	//bad heuristic
	if(getOtherPiece(start->piece) == worse_heuristic_piece)
	{
		//"unweighted", since we're running 1 variable at a time, each can have a weight of 1. 
		double weights[1][7] = {
			//levels 0-40
			{	1 /*piece count*/, 
				2 /*scores*/, 
				10 /*mobility*/, 
				1 /*potential mobility*/, 
				3 /*stability*/,
				1 /*reinforcement*/, 
				1 /*MCTS*/
			},
		};

		//static weights
		int weight_index = 0;


		// heuristic =  weights[weight_index][0]*(player_count - AI_count);
		 heuristic =  weights[weight_index][0]*(player_count - AI_count) + 
					 weights[weight_index][1]*(player_score - AI_score) + 
					 weights[weight_index][2]*(player_mobility - AI_mobility) + 
					 weights[weight_index][3]*(player_potential_mobility - AI_potential_mobility) + 
					 weights[weight_index][4]*(player_stability_score - AI_stability_score) + 
					 weights[weight_index][5]*(good - bad);



		// heuristic =  weights[weight_index][0]*(player_count - AI_count) + 
		// 			 weights[weight_index][1]*(player_score - AI_score) + 
		// 			 weights[weight_index][2]*(player_mobility - AI_mobility) + 
		// 			 weights[weight_index][3]*(player_potential_mobility - AI_potential_mobility) + 
		// 			 weights[weight_index][4]*(player_stability_score - AI_stability_score) + 
		// 			 weights[weight_index][5]*(good - bad) + 
		// 			 weights[weight_index][6]*(MCTS_ratio);


		// heuristic = 0;

		
	}
	//good heuristic
	else
	{

		//"unweighted", since we're running 1 variable at a time, each can have a weight of 1. 
		// double weights[1][7] = {
		// 	//levels 0-40
		// 	{	1 /*piece count*/, 
		// 		1 /*scores*/, 
		// 		1 /*mobility*/, 
		// 		1 /*potential mobility*/, 
		// 		1 /*stability*/,
		// 		1 /*reinforcement*/, 
		// 		1 /*MCTS*/
		// 	}
		// };

		double weights[4][7] = {
			//levels 0-40
			{	0 /*piece count*/, 
				3 /*scores*/, 
				10 /*mobility*/, 
				0 /*potential mobility*/, 
				5 /*stability*/,
				1 /*reinforcement*/, 
				1 /*MCTS*/
			},
			//levels 40-50
			{	0 /*piece count*/, 
				2 /*scores*/, 
				10 /*mobility*/, 
				0 /*potential mobility*/, 
				1.5 /*stability*/,
				1 /*reinforcement*/, 
				1 /*MCTS*/
			},
			//levels 50-55
			{	1 /*piece count*/, 
				1 /*scores*/, 
				10 /*mobility*/, 
				0 /*potential mobility*/, 
				1 /*stability*/,
				1 /*reinforcement*/, 
				1 /*MCTS*/
			},
			//levels 55-60
			{	3 /*piece count*/, 
				1 /*scores*/, 
				10 /*mobility*/, 
				0 /*potential mobility*/, 
				0 /*stability*/,
				0 /*reinforcement*/, 
				3 /*MCTS*/
			}
		};



		//dynamic weights
		int weight_index = 0;
		if(level <= 40)
			weight_index = 0;
		else if(level <= 50)
			weight_index = 1;
		else if(level <= 55)
			weight_index = 2;
		else
			weight_index = 3;


		// //static weights
		// int weight_index = 0;


		//0
		// heuristic = 0;

		//1
		// heuristic =  (player_count - AI_count); 

		//2
		// heuristic =  (player_score - AI_score);
		
		//3
		// heuristic =  (player_mobility - AI_mobility);

		//4
		// heuristic =  (player_potential_mobility - AI_potential_mobility);

		//5
		// heuristic =  (player_stability_score - AI_stability_score);

		//6
		// heuristic =  (good - bad);

		//7
		// heuristic = (-1*MCTS_ratio);



		//1 + 2
		// heuristic =  (player_count - AI_count) +  
		// 			 (player_score - AI_score);

		//1 + 2 + 3
		// heuristic =  (player_count - AI_count) +  
		// 			 (player_score - AI_score) + 
		// 			 (player_mobility - AI_mobility);

		//1 + 2 + 3 + 4
		// heuristic =  (player_count - AI_count) +  
		// 			 (player_score - AI_score) + 
		// 			 (player_mobility - AI_mobility) + 
		// 			 (player_potential_mobility - AI_potential_mobility);

		//1 + 2 + 3 + 4 + 5
		// heuristic =  (player_count - AI_count) +  
		// 			 (player_score - AI_score) + 
		// 			 (player_mobility - AI_mobility) + 
		// 			 (player_potential_mobility - AI_potential_mobility) + 
		// 			 (player_stability_score - AI_stability_score);

		// //1 + 2 + 3 + 4 + 5 + 6
		// heuristic =  (player_count - AI_count) +  
		// 			 (player_score - AI_score) + 
		// 			 (player_mobility - AI_mobility) + 
		// 			 (player_potential_mobility - AI_potential_mobility) + 
		// 			 (player_stability_score - AI_stability_score) + 
		// 			 (good - bad);

		// //1 + 2 + 3 + 4 + 5 + 6
		// heuristic =  (player_count - AI_count) +  
		// 			 (player_score - AI_score) + 
		// 			 (player_mobility - AI_mobility) + 
		// 			 (player_potential_mobility - AI_potential_mobility) + 
		// 			 (player_stability_score - AI_stability_score) + 
		// 			 (good - bad) + 
		// 			 (-1*MCTS_ratio);


		heuristic =  weights[weight_index][0]*(player_count - AI_count) + 
					 weights[weight_index][1]*(player_score - AI_score) + 
					 weights[weight_index][2]*(player_mobility - AI_mobility) + 
					 weights[weight_index][3]*(player_potential_mobility - AI_potential_mobility) + 
					 weights[weight_index][4]*(player_stability_score - AI_stability_score) + 
					 weights[weight_index][5]*(good - bad); 
					 // weights[weight_index][6]*(-1*MCTS_ratio);


		// heuristic = 0;
	}

	return heuristic;

	//reatio heuristic doesn't work very well, and crashes...
	// return ((double)player_count/((double)player_count+(double)AI_count));
}

//reinforcement methods
void Tree::reinforceGood(node * ptr)
{
	double increment = this->good_weight;
	node * temp = ptr;

	//iterate up the tree using parents until hit the root node
	while(temp != root)
	{

		temp->good += increment;

		//decreases increment by 1/20th of itself each time
		increment *= 17.0/20.0;

		//goes to parent
		temp = temp->prev;
	}
}


void Tree::reinforceBad(node * ptr)
{
	double increment = this->good_weight;
	node * temp = ptr;

	//iterate up the tree using parents until hit the root node
	while(temp != root)
	{
		temp->bad += increment;

		//decreases increment by 1/20th of itself each time
		increment *= 17.0/20.0;
		temp = temp->prev;
	}
}


//Player moves to (col,row), so determine which board that is and move there
void Tree::playerMove(int col, int row)
{
	//iterates through current ptrs children
	for(int x = 0; x < numChildren(ptr); x++)
	{
		//if board matches by seeing if a piece has been placed in specified position
		if(ptr->next[x]->board[col][row] == player_piece)
		{
			ptr = ptr->next[x];
			break;
		}
	}
}


//returns the Board object that the AI moves to
void Tree::AIMove(int col, int row)
{

	//iterates through current ptrs children
	for(int x = 0; x < numChildren(ptr); x++)
	{

		//if board matches by seeing if a piece has been placed in specified position
		if(ptr->next[x]->board[col][row] == AI_piece)
		{
			ptr = ptr->next[x];
			break;
		}
	}

}


//moves to child with board
void Tree::move(int child_index)
{
	if (child_index != -1)
		ptr = ptr->next[child_index];
	else
		cout<<"Error, couldn't find move!"<<endl;
}


//iterates down the tree and populates with possible moves down to specified depth
void Tree::iterateTreeDepth(node* ptr, char piece, int cur_depth, int max_depth)
{

	//checks if ptr has no children
	//if has no children, determinePosslbeMoves()
	//iterate through new children and run recursively
	
	int new_cur_depth = cur_depth;

	//if has no children, then create the children
	if(numChildren(ptr) == 0)
	{
		determinePossibleMoves(ptr, piece);
		new_cur_depth++;
	}
	//comment out this else if you want the tree to add max_depth amount of new layers each time
	//or leave if you want the tree to count already existing layers towards the depth. 
	else
		new_cur_depth++;

	//if reached max depth, stop
	if((new_cur_depth-1)==max_depth)
		return;


	//iterates through all children
	for(int x = 0; x <  numChildren(ptr); x++)
		iterateTreeDepth(ptr->next[x], getOtherPiece(piece), new_cur_depth, max_depth);
	
}


//links a new node to ptr, with initialized board
void Tree::newNode(node * ptr, char (&new_board)[size+1][size+1], char piece)
{
	ptr->next.push_back(new node());

	//ptr to new next node
	node* next = ptr->next[numChildren(ptr)-1];

	//copies new board into new node
	board_obj.copyBoard(next->board, new_board);

	//piece is the piece that is placed to get to this new board
	next->piece = piece;

	//increases level
	next->level = ptr->level + 1;

	//pointer to parent
	next->prev = ptr;

	num_nodes++;
}


//prints node's contents in a readable format
void Tree::printNode(node * ptr, int indents /*default is 0 */)
{
	//prints tic-tac-toe board
	board_obj.printBoard(ptr->board, indents);
	cout<<"Move: "<<ptr->level<<endl;
	// cout<<"Num next nodes: "<<numChildren(ptr)<<endl;
	cout<<"Heuristic: "<<ptr->h<<endl;
	// cout<<"Reinforcement: (Good: "<<ptr->good<<", Bad: "<<ptr->bad<<")"<<endl;
	// cout<<"Monte carlo results: ("<<ptr->num_wins<<", "<<ptr->num_plays<<") ";
	if(ptr->num_plays > 0)
		cout<<(((double)ptr->num_wins/(double)ptr->num_plays)*100)<<endl;
	else
		cout<<endl;
}

char Tree::getOtherPiece(char piece)
{
	if(piece == AI_piece)
		return player_piece;
	else
		return AI_piece;
}

int Tree::numChildren(node* ptr)
{
	return ptr->next.size();
}

//prints the neural network in one long output.
void Tree::printNet(node * ptr, int indents /*default is 0 */)
{
	printNode(ptr, indents);
	cout<<endl;

	// //prints 2nd level of tree
	// for(int x = 0; x < numChildren(ptr); x++)
	// 	printNode(ptr->next[x], indents+1);

	//prints whole tree
	for(int x = 0; x < numChildren(ptr); x++)
		printNet(ptr->next[x], indents+1);


}


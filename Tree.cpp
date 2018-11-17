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
		{
			eraseBranch(ptr->next[x], ptr_to_keep);
		}

			// for(int x = 0; x < ptr->next_index; x++)
			// 	delete[] ptr->next[x];
			// delete[] ptr->next;

		for(int x = 0; x < numChildren(ptr); x++)
			delete ptr->next[x];

		delete ptr;

		ptr = NULL;
	}
}

void Tree::resetTree()
{
	cout<<"Erasing tree"<<endl;
	eraseTree(root);

	cout<<"New root: "<<endl;
	root = new node();

	board_obj.resetBoard(root->board);

	printNode(root);

	this->ptr = root;
}

void Tree::eraseTree(node * ptr)
{
	if(ptr != NULL)
	{
		for(int x = 0; x < numChildren(ptr); x++)
		{
			eraseTree(ptr->next[x]);
		}

			// for(int x = 0; x < ptr->next_index; x++)
			// 	delete[] ptr->next[x];
			// delete[] ptr->next;


		// cout<<"Num children: "<<numChildren(ptr)<<endl;

		// delete ptr->next;
		for(int x = 0; x < numChildren(ptr); x++)
			ptr->next[x] = NULL;

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

// //returns the node with the smallest heuristic
// char Tree::getBoardMinHeuristic(node* ptr)
// {
// 	vector<int> minimum_indices;

// 	// int index = 0;
// 	double min_heuristic = numeric_limits<double>::max();
// 	for(int x = 0; x < ptr->next_index; x++)
// 	{
// 		double h = ptr->next[x]->h;
// 		if(h < min_heuristic)
// 		{
// 			min_heuristic = h;
// 			// index = x;
// 			minimum_indices.clear();
// 			minimum_indices.push_back(x);
// 		}
// 		else if(h==min_heuristic)
// 		{
// 			minimum_indices.push_back(x);
// 		}

// 	}

// 	//chooses randomly out of all the smallest heuristics
// 	int random_index = randNum(0, minimum_indices.size());
// 	int index = minimum_indices[random_index];

// 	return ptr->next[index]->board;
// }


// //returns the node with the largest heuristic
// char Tree::getBoardMaxHeuristic(node* ptr)
// {
// 	vector<int> maximum_indices;

// 	// int index = 0;
// 	double max_heuristic = numeric_limits<double>::lowest();
// 	// cout<<"Max h: "<<max_heuristic<<endl;
// 	for(int x = 0; x < ptr->next_index; x++)
// 	{
// 		double h = ptr->next[x]->h;
// 		// cout<<"h: "<<h<<endl;
// 		if(h > max_heuristic)
// 		{
// 			// cout<<"New max: "<<endl;
// 			max_heuristic = h;
// 			maximum_indices.clear();
// 			maximum_indices.push_back(x);
// 		}
// 		else if(h == max_heuristic)
// 		{
// 			// cout<<"Same max h"<<endl;
// 			maximum_indices.push_back(x);
// 		}

// 	}

// 	// cout<<"Size: "<<maximum_indices.size()<<endl;

// 	//chooses randomly out of all the smallest heuristics
// 	int random_index = randNum(0, maximum_indices.size());
// 	int index = maximum_indices[random_index];

// 	return ptr->next[index]->board;
// }

//returns the node with the smallest heuristic
int Tree::getIndexMinHeuristic(node* ptr)
{
	vector<int> minimum_indices;

	// int index = 0;
	double min_heuristic = numeric_limits<double>::max();
	for(int x = 0; x < numChildren(ptr); x++)
	{
		double h = ptr->next[x]->h;
		if(h < min_heuristic)
		{
			min_heuristic = h;
			// index = x;
			minimum_indices.clear();
			minimum_indices.push_back(x);
		}
		else if(h==min_heuristic)
		{
			minimum_indices.push_back(x);
		}

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

	// int index = 0;
	double max_heuristic = numeric_limits<double>::lowest();
	// cout<<"Max h: "<<max_heuristic<<endl;
	for(int x = 0; x < numChildren(ptr); x++)
	{
		double h = ptr->next[x]->h;
		// cout<<"h: "<<h<<endl;
		if(h > max_heuristic)
		{
			// cout<<"New max: "<<endl;
			max_heuristic = h;
			maximum_indices.clear();
			maximum_indices.push_back(x);
		}
		else if(h == max_heuristic)
		{
			// cout<<"Same max h"<<endl;
			maximum_indices.push_back(x);
		}

	}

	// cout<<"Size: "<<maximum_indices.size()<<endl;

	//chooses randomly out of all the smallest heuristics
	int random_index = randNum(0, maximum_indices.size());
	int index = maximum_indices[random_index];

	return index;
}





//different version of minimax
//is_maximizing is {-1,1} if 
double Tree::negamax(node* start, node* ptr, int depth_left, double alpha /* starts as -INFINITY */, double beta /* starts as INFINITY */, int is_maximizing)
{
	double cur_heuristic = -1;
	//if has no children or reached max depth
	if(numChildren(ptr) == 0 || depth_left <= 0)
	{
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
		h = -negamax(start, ptr->next[x], depth_left--, -beta, -alpha, -is_maximizing);
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



// //min version of Minimax
// //calculates heuristics for AI down to a certain depth
// //considers current node as the AI's move, and therefore will be minimizing heuristic
// double Tree::getMinHeuristic(node* start, node * ptr, double alpha /* starts as -INFINITY */, double beta /* starts as INFINITY */, int depth_left)
// {
// 	double cur_heuristic = -1;

// 	//if has no children
// 	if(ptr->next.size() == 0 || depth_left<=0)
// 	{
// 		cur_heuristic = calculateHeuristic(start, ptr);
// 		ptr->h = cur_heuristic;

// 		return cur_heuristic;
// 	}

// 	// bestVal = +INFINITY 
//  //    for each child node :
//  //        value = minimax(node, depth+1, true, alpha, beta)
//  //        bestVal = min( bestVal, value) 
//  //        beta = min( beta, bestVal)
//  //        if beta <= alpha:
//  //            break
//  //    return bestVal


// 	//iterates through all children
// 	double min_heuristic = numeric_limits<double>::max(); // max value
// 	for(int x = 0; x < ptr->next_index; x++)
// 	{
// 		double h = getMaxHeuristic(start, ptr->next[x], alpha, beta, depth_left--);
// 		if(h < min_heuristic)
// 			min_heuristic = h;

// 		if(min_heuristic < beta)
// 			beta = min_heuristic;

// 		//prunes
// 		if(beta <= alpha)
// 			break;

// 	}

// 	ptr->h = min_heuristic;

// 	return min_heuristic;


// }

// //max portion of Minimax
// double Tree::getMaxHeuristic(node* start, node * ptr, double alpha /* starts as -INFINITY */, double beta /* starts as INFINITY */, int depth_left)
// {
// 	double cur_heuristic = -1;
	
// 	//if has no children
// 	if(ptr->next_index == 0 || depth_left<=0)
// 	{
// 		cur_heuristic = calculateHeuristic(start, ptr);
// 		ptr->h = cur_heuristic;

// 		return cur_heuristic;
// 	}

// 	// bestVal = -INFINITY 
//  //    for each child node :
//  //        value = minimax(node, depth+1, false, alpha, beta)
//  //        bestVal = max( bestVal, value) 
//  //        alpha = max( alpha, bestVal)
//  //        if beta <= alpha:
//  //            break
//  //    return bestVal


// 	//iterates through all children
// 	double max_heuristic = numeric_limits<double>::lowest(); // minimum value
// 	for(int x = 0; x < ptr->next_index; x++)
// 	{
// 		double h = getMinHeuristic(start, ptr->next[x], alpha, beta, depth_left--);
// 		if(h > max_heuristic)
// 			max_heuristic = h;

// 		if(max_heuristic > alpha)
// 			alpha = max_heuristic;

// 		//prune the rest
// 		if(beta <= alpha)
// 			break;

// 	}
// 	ptr->h = max_heuristic;

// 	return max_heuristic;
// }

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



	double AI_mobility = 0;
	double AI_potential_mobility = 0;
	double player_mobility = 0;
	double player_potential_mobility = 0;

	double total_player_mobility = 0;
	double total_player_potential_mobility = 0;
	double total_AI_mobility = 0;
	double total_AI_potential_mobility = 0;
	double count = 0;
	double smallest_num_player_moves = 100000;
	double smallest_num_AI_moves = 100000 ;
	double largest_num_player_moves = 0;
	double largest_num_AI_moves = 0 ;
	node* temp = ptr;


	//traverses from current node to start node
	//when traversing up counting possible moves for player and AI, try and average per level so that it's not a total. 
	//A total would give a lower total for unvisited children, and therefore not be a good heuristic. 
	while(true)
	{
		// cout<<"Temp: "<<endl;
		// board_obj.printBoard(temp->board);

		// if(temp->piece == player_piece)
			// double AI_moves = board_obj.getPossibleMovesCount(temp->board, AI_piece);
		// else if(temp->piece == AI_piece)
			// double player_moves = board_obj.getPossibleMovesCount(temp->board, player_piece);

		// int count_AI = 0;
		// int count_player = 0;
		// for(int x = 0; x < temp->next_index; x++)
		// {
		// 	if(temp->piece == AI_piece)
		// 		count_AI++;
		// 	else if(temp->piece == player_piece)
		// 		count_player++;
		// }

		// // total_AI_moves += AI_moves;
		// // total_player_moves += player_moves;

		// if(count_AI>0 || count_player>0)
		// {
		// 	// cout<<"Original AI moves: "<<AI_moves<<endl;
		// 	// cout<<"Original player moves: "<<player_moves<<endl;
		// 	// cout<<"New AI moves: "<<count_AI<<endl;
		// 	// cout<<"New player moves: "<<count_player<<endl;
		// 	//cout<<endl;
		// }


		// //if haven't calculated this node's mobility yet
		// if(temp->AI_mobility < 0)
		// {
		// 	vector<double> AI_mobility = board_obj.getMobility(temp->board, AI_piece);
		// 	temp->AI_mobility = AI_mobility[0];
		// 	temp->AI_potential_mobility = AI_mobility[1];

		// 	vector<double> player_mobility = board_obj.getMobility(temp->board, player_piece);
		// 	temp->player_mobility = player_mobility[0];
		// 	temp->player_potential_mobility = player_mobility[1];
		// }

		// if(temp->AI_mobility < smallest_num_AI_moves)
		// 	smallest_num_AI_moves = temp->AI_mobility;
		// if(temp->player_mobility < smallest_num_player_moves)
		// 	smallest_num_player_moves = temp->player_mobility;
		// total_AI_mobility += temp->AI_mobility;
		// total_AI_potential_mobility += temp->AI_potential_mobility;
		// total_player_mobility += temp->player_mobility;
		// total_player_potential_mobility += temp->player_potential_mobility;


		//if haven't calculated this node's mobility yet
		vector<double> AI_mobility_vector = board_obj.getMobility(temp->board, AI_piece);
		AI_mobility = AI_mobility_vector[0];
		AI_potential_mobility = AI_mobility_vector[1];

		vector<double> player_mobility_vector = board_obj.getMobility(temp->board, player_piece);
		player_mobility = player_mobility_vector[0];
		player_potential_mobility = player_mobility_vector[1];


		smallest_num_AI_moves = min(AI_mobility, smallest_num_AI_moves);
		smallest_num_player_moves = min(player_mobility, smallest_num_player_moves);
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


	// //gets difference of this pointer's board, and its parent's board to determine where player moved
	// vector<vector<int>> coordinates = board_obj.getDifferenceCoordinates(ptr->board, ptr->prev->board);

	// double pos_weight = 0;
	// if(coordinates.size() > 0)
	// {
	// 	int col = coordinates[0][0];
	// 	int row = coordinates[0][1];
	// 	pos_weight = board_obj.getWeight(ptr->level, col, row);
	// }


	//// Reinforcement learning portion ////
	double good = ptr->good;
	double bad = ptr->bad;




	/// Dynamic weighting ///

	double heuristic = 0;
	//bad heuristic
	if(getOtherPiece(start->piece) == worse_heuristic_piece)
	{
		double weights[3][6] = {
			//levels 0-40
			{	1 /*piece count*/, 
				1 /*scores*/, 
				10 /*mobility*/, 
				1 /*potential mobility*/, 
				3 /*stability*/,
				1 /*reinforcement*/
			},
			//levels 40-50
			{	1 /*piece count*/, 
				1 /*scores*/, 
				10 /*mobility*/, 
				1 /*potential mobility*/, 
				3 /*stability*/,
				1 /*reinforcement*/
			},
			//levels 50-60
			{	1 /*piece count*/, 
				1 /*scores*/, 
				10 /*mobility*/, 
				1 /*potential mobility*/, 
				3 /*stability*/,
				1 /*reinforcement*/
			}
		};

		int weight_index = 0;
		if(level <= 40)
			weight_index = 0;
		else if(level<=50)
			weight_index = 1;
		else
			weight_index = 2;


		 heuristic =  weights[weight_index][0]*(player_count - AI_count) + 
					 weights[weight_index][1]*(player_score - AI_score) + 
					 weights[weight_index][2]*(player_mobility - AI_mobility) + 
					 weights[weight_index][3]*(player_potential_mobility - AI_potential_mobility) + 
					 weights[weight_index][4]*(player_stability_score - AI_stability_score) + 
					 weights[weight_index][5]*(good - bad);

		// heuristic =  weights[0]*(player_count - AI_count) + 
		// 			 weights[1]*(player_score - AI_score) + 
		// 			 weights[2]*(smallest_num_player_moves - smallest_num_AI_moves);
		// 			 weights[3]*(good - bad);

		 // heuristic =  weights[0]*(player_count - AI_count) + 
			// 		 weights[1]*(player_score - AI_score) + 
			// 		 weights[2]*(player_mobility - AI_mobility) + 
			// 		 weights[3]*(player_potential_mobility - AI_potential_mobility) + 
			// 		 weights[4]*(player_stability_score - AI_stability_score) + 
			// 		 weights[5]*(good - bad);


		// heuristic =  weights[0]*(player_count-AI_count);
		// heuristic =  weights[0][1]*(player_score-AI_score);

		// heuristic =  weights[0]*(player_count-AI_count) + 
		// 			 weights[1]*(player_score-AI_score) + 
		// 			 // weights[2]*(smallest_num_player_moves - smallest_num_AI_moves);
		// 			 weights[2]*(player_mobility - AI_mobility) + 
		// 			 weights[3]*(player_potential_mobility - AI_potential_mobility);
		// // 			 weights[4]*(good - bad);

		// heuristic = weights[2]*(player_mobility - AI_mobility)+
					// weights[3]*(player_potential_mobility - AI_potential_mobility);

		// heuristic = weights[3]*(player_potential_mobility - AI_potential_mobility);

		// heuristic = (player_stability_score - AI_stability_score);

		// moves randomly
		// heuristic = 0;
	}
	//good heuristic
	else
	{
		// double weights[3][6] = {
		// 	//levels 0-40
		// 	{	0 /*piece count*/, 
		// 		3 /*scores*/, 
		// 		10 /*mobility*/, 
		// 		0 /*potential mobility*/, 
		// 		3 /*stability*/,
		// 		1 /*reinforcement*/
		// 	},
		// 	//levels 40-50
		// 	{	0 /*piece count*/, 
		// 		2 /*scores*/, 
		// 		10 mobility, 
		// 		0 /*potential mobility*/, 
		// 		1.5 /*stability*/,
		// 		1 /*reinforcement*/
		// 	},
		// 	//levels 50-60
		// 	{	0 /*piece count*/, 
		// 		1 /*scores*/, 
		// 		20 /*mobility*/, 
		// 		0 /*potential mobility*/, 
		// 		0 /*stability*/,
		// 		1 /*reinforcement*/
		// 	}
		// };

		double weights[3][6] = {
			//levels 0-40
			{	1 /*piece count*/, 
				1 /*scores*/, 
				10 /*mobility*/, 
				1 /*potential mobility*/, 
				3 /*stability*/,
				1 /*reinforcement*/
			},
			//levels 40-50
			{	1 /*piece count*/, 
				1 /*scores*/, 
				10 /*mobility*/, 
				1 /*potential mobility*/, 
				3 /*stability*/,
				1 /*reinforcement*/
			},
			//levels 50-60
			{	1 /*piece count*/, 
				1 /*scores*/, 
				10 /*mobility*/, 
				1 /*potential mobility*/, 
				3 /*stability*/,
				1 /*reinforcement*/
			}
		};

		// heuristic =  weights[0]*(player_count - AI_count) + 
		// 			 weights[1]*(player_score - AI_score) + 
		// 			 weights[2]*(smallest_num_player_moves - smallest_num_AI_moves);
		// 			 weights[3]*(good - bad);

		int weight_index = 0;
		if(level <= 40)
			weight_index = 0;
		else if(level<=50)
			weight_index = 1;
		else
			weight_index = 2;


		 // heuristic =  weights[weight_index][0]*(player_count - AI_count) + 
			// 		 weights[weight_index][1]*(player_score - AI_score) + 
			// 		 weights[weight_index][2]*(player_mobility - AI_mobility) + 
			// 		 weights[weight_index][3]*(player_potential_mobility - AI_potential_mobility) + 
			// 		 weights[weight_index][4]*(player_stability_score - AI_stability_score) + 
			// 		 weights[weight_index][5]*(good - bad);

		 heuristic =  weights[weight_index][0]*(player_count - AI_count) + 
					 weights[weight_index][1]*(player_score - AI_score) + 
					 weights[weight_index][2]*(player_mobility - AI_mobility) + 
					 weights[weight_index][3]*(player_potential_mobility - AI_potential_mobility) + 
					 weights[weight_index][4]*(player_stability_score - AI_stability_score); 
					 // weights[weight_index][5]*(good - bad);


		// heuristic = 0;

		// heuristic = (player_stability_score - AI_stability_score);



		
		// heuristic =  weights[0]*(player_count-AI_count) + 
		// 			 weights[1]*(player_score-AI_score) + 
		// 			 weights[2]*(player_moves-AI_moves);
		// heuristic =  weights[0]*(player_count-AI_count) + 
		// 			 weights[1]*(player_score-AI_score);

		// heuristic =  weights[0]*(player_count-AI_count);
		// heuristic =  weights[1]*(player_score-AI_score);
		// heuristic = weights[2]*(player_moves-AI_moves);
		// heuristic = weights[2]*(smallest_num_player_moves - smallest_num_AI_moves);
		// heuristic = weights[2]*(largest_num_player_moves - largest_num_AI_moves);
		// heuristic = weights[3]*(good - bad);

		//moves randomly
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


		// cout<<"Reinforcing Good"<<endl;
		// printNode(temp);

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
		// Board temp_board = ptr->next[x]->board_obj;


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
	// cout<<"Children: "<<ptr->next_index<<endl;
	// cout<<"Child index: "<<child_index<<endl;

	if (child_index != -1)
	{
		ptr = ptr->next[child_index];

	}
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
	// ptr->next[ptr->next_index] = new node();
	ptr->next.push_back(new node());
	// ptr->next.push_back(new node());

	//ptr to new next node
	// node * next = ptr->next[ptr->next_index];
	node* next = ptr->next[numChildren(ptr)-1];

	// createMatrix(next->board, size);

	//copies new board into new node
	board_obj.copyBoard(next->board, new_board);

	//piece is the piece that is placed to get to this new board
	next->piece = piece;

	//increases level
	next->level = ptr->level + 1;

	//increments number of next nodes
	// ptr->next_index++;

	//pointer to parent
	next->prev = ptr;

	num_nodes++;
}


//prints node's contents in a readable format
void Tree::printNode(node * ptr, int indents /*default is 0 */)
{
	//prints tic-tac-toe board
	board_obj.printBoard(ptr->board, indents);
	cout<<"Level: "<<ptr->level<<endl;
	// cout<<"Num next nodes: "<<ptr->next_index<<endl;
	cout<<"Num next nodes: "<<numChildren(ptr)<<endl;
	cout<<"Heuristic: "<<ptr->h<<endl;
	cout<<"Reinforcement: (Good: "<<ptr->good<<", Bad: "<<ptr->bad<<")"<<endl;
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

	//prints 2nd level of tree
	// for(int x = 0; x < ptr->next_index; x++)
	for(int x = 0; x < numChildren(ptr); x++)
		printNode(ptr->next[x], indents+1);

	// //prints whole tree
	// for(int x = 0; x < ptr->next.size(); x++)
	// 	printNet(ptr->next[x], indents+1);


}


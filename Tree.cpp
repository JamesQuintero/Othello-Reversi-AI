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

void Tree::resetTree()
{
	eraseTree(root);

	root = new node();
	// root->board = createMatrix(size);
	board_obj.resetBoard(root->board);

	// string something = "";
	// cin>>something;

	this->ptr = root;
}

void Tree::eraseTree(node * ptr)
{
	if(ptr != NULL)
	{
		for(int x = 0; x < ptr->next_index; x++)
		{
			eraseTree(ptr->next[x]);
		}

		// cout<<"Deleting"<<endl;
		for(int x = 0; x < size; x++)
			delete[] ptr->board[x];
		delete[] ptr->board;

		delete[] ptr->next;

		delete ptr;

		ptr = NULL;
	}
}

//determines the possible moves by piece, then adds them as children to the current pointer
void Tree::determinePossibleMoves(node* ptr, char piece)
{
	//stop if has children, meaning it's already been expanded
	if(ptr->next_index>0)
		return;

	vector<vector<int>> coordinates = board_obj.getPossibleMoveCoordinates(ptr->board, piece);

	for(int x = 0; x < coordinates.size(); x++)
	{
		char** new_board = new char*[size];
		createMatrix(new_board, size);
		board_obj.copyBoard(new_board, ptr->board);
		board_obj.place_piece(new_board, piece, coordinates[x][0], coordinates[x][1]);


		newNode(ptr, new_board, piece);

		//garbage collection
		for(int y = 0; y < size; y++)
			delete[] new_board[y];
		delete[] new_board;
	}
	
}

//returns tree if there are legal moves, i.e. if there are children to specified node
bool Tree::hasLegalMoves(node* ptr)
{
	if(ptr->next_index>0)
		return true;
	else
		return false;
}

//returns the node with the smallest heuristic
char** Tree::getBoardMinHeuristic(node* ptr)
{
	vector<int> minimum_indices;

	// int index = 0;
	double min_heuristic = numeric_limits<double>::max();
	for(int x = 0; x < ptr->next_index; x++)
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

	return ptr->next[index]->board;
}


//returns the node with the largest heuristic
char** Tree::getBoardMaxHeuristic(node* ptr)
{
	vector<int> maximum_indices;

	// int index = 0;
	double max_heuristic = numeric_limits<double>::lowest();
	// cout<<"Max h: "<<max_heuristic<<endl;
	for(int x = 0; x < ptr->next_index; x++)
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

	return ptr->next[index]->board;
}



//calculates heuristics for AI down to a certain depth
//considers current node as the AI's move, and therefore will be minimizing heuristic
double Tree::getMinHeuristic(node* start, node * ptr, int alpha /* starts as -INFINITY */, int beta /* starts as INFINITY */, int depth_left)
{
	double cur_heuristic = -1;

	//if has no children
	if(ptr->next_index == 0 || depth_left<=0)
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
	double min_heuristic = (double)numeric_limits<int>::max(); // max value
	for(int x = 0; x < ptr->next_index; x++)
	{
		double h = getMaxHeuristic(start, ptr->next[x], alpha, beta, depth_left--);
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

double Tree::getMaxHeuristic(node* start, node * ptr, int alpha /* starts as -INFINITY */, int beta /* starts as INFINITY */, int depth_left)
{
	double cur_heuristic = -1;
	
	//if has no children
	if(ptr->next_index == 0 || depth_left<=0)
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
	double max_heuristic = (double)numeric_limits<int>::min(); // minimum value
	for(int x = 0; x < ptr->next_index; x++)
	{
		double h = getMinHeuristic(start, ptr->next[x], alpha, beta, depth_left--);
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


	//count the number of lines a player has or has gotten


	double total_player_moves = 0;
	double total_AI_moves = 0;
	double count = 0;
	double smallest_num_player_moves = 100000;
	double smallest_num_AI_moves = 100000 ;
	double largest_num_player_moves = 0;
	double largest_num_AI_moves = 0 ;
	node* temp = ptr;
	// cout<<"Start: "<<endl;
	// board_obj.printBoard(start->board);
	//traverses from current node to start node
	//when traversing up counting possible moves for player and AI, try and average per level so that it's not a total. 
	//A total would give a lower total for unvisited children, and therefore not be a good heuristic. 
	while(true)
	{
		// cout<<"Temp: "<<endl;
		// board_obj.printBoard(temp->board);

		// if(temp->piece == player_piece)
			double AI_moves = board_obj.getPossibleMovesCount(temp->board, AI_piece);
		// else if(temp->piece == AI_piece)
			double player_moves = board_obj.getPossibleMovesCount(temp->board, player_piece);

		int count_AI = 0;
		int count_player = 0;
		for(int x = 0; x < temp->next_index; x++)
		{
			if(temp->piece == AI_piece)
				count_AI++;
			else if(temp->piece == player_piece)
				count_player++;
		}

		// total_AI_moves += AI_moves;
		// total_player_moves += player_moves;

		if(count_AI>0 || count_player>0)
		{
			// cout<<"Original AI moves: "<<AI_moves<<endl;
			// cout<<"Original player moves: "<<player_moves<<endl;
			// cout<<"New AI moves: "<<count_AI<<endl;
			// cout<<"New player moves: "<<count_player<<endl;
			//cout<<endl;
		}



		// double AI_moves = board_obj.getPossibleMovesCount(temp->board, AI_piece);
		// if(AI_moves < smallest_num_AI_moves)
		// 	smallest_num_AI_moves = AI_moves;
		// double player_moves = board_obj.getPossibleMovesCount(temp->board, player_piece);
		// if(player_moves < smallest_num_player_moves)
		// 	smallest_num_player_moves = player_moves;

		// if(AI_moves > largest_num_AI_moves)
		// 	largest_num_AI_moves = AI_moves;
		// if(player_moves > largest_num_player_moves)
		// 	largest_num_player_moves = player_moves;

		count++;

		// cout<<"total moves: ("<<total_player_moves<<","<<total_AI_moves<<")"<<endl;

		//stop after processing start node
		if(temp == start)
			break;

		temp = temp->prev;
	}
	double player_moves = total_player_moves/count;
	double AI_moves = total_AI_moves/count;


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




	//Looks at opponent's moves vs player's moves
	// return player_count-AI_count;

	// return (player_count - AI_count) + (num_player_flips - num_AI_flips);

	int weights[4] = {1, 1, 10, 1};

	double heuristic = 0;
	//bad heuristic
	if(getOtherPiece(start->piece) == worse_heuristic_piece)
	{
		// heuristic =  weights[0]*(player_count-AI_count);
		// heuristic =  weights[1]*(player_score-AI_score);

		//moves randomly
		heuristic = 0;
	}
	//good heuristic
	else
	{
		heuristic =  weights[0]*(player_count-AI_count) + 
					 weights[1]*(player_score-AI_score) + 
					 weights[2]*(player_moves-AI_moves) + 
					 weights[3]*(good - bad);
		
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
	


	// cout<<"Player score: "<<player_score<<endl;
	// cout<<"AI score: "<<AI_score<<endl;
	// cout<<"Difference: "<<(player_score - AI_score)<<endl;
	// board_obj.printBoard(ptr->board);
	// cout<<"Num player possible moves: "<<player_moves<<endl;
	// cout<<"Num AI possible moves: "<<AI_moves<<endl;
	// cout<<"Difference: "<<(player_moves - AI_moves)<<endl;
	// cout<<endl;



	// double heuristic = (player_count - AI_count);
	// double heuristic = num_player_flips - num_AI_flips;
	// double heuristic = pos_weight;
	// cout<<"Heuristic: "<<heuristic<<endl;
	// cout<<endl;

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
	for(int x = 0; x < ptr->next_index; x++)
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
	for(int x = 0; x < ptr->next_index; x++)
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
void Tree::move(char**& new_board)
{
	//iterates through current ptrs children
	for(int x = 0; x < ptr->next_index; x++)
	{

		//if board matches by seeing if a piece has been placed in specified position
		if(board_obj.isEqual(new_board, ptr->next[x]->board))
		{
			ptr = ptr->next[x];
			return;
		}
	}

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
	if(ptr->next_index == 0)
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
	for(int x = 0; x < ptr->next_index; x++)
		iterateTreeDepth(ptr->next[x], getOtherPiece(piece), new_cur_depth, max_depth);
	
}


//links a new node to ptr, with initialized board
void Tree::newNode(node * ptr, char**& new_board, char piece)
{
	ptr->next[ptr->next_index] = new node();
	// ptr->next.push_back(new node());

	//ptr to new next node
	node * next = ptr->next[ptr->next_index];
	// next->board = createMatrix(size);

	// next->board = new char*[size];
	createMatrix(next->board, size);

	//copies new board into new node
	board_obj.copyBoard(next->board, new_board);

	//piece is the piece that is placed to get to this new board
	next->piece = piece;

	//increases level
	next->level = ptr->level + 1;

	//increments number of next nodes
	ptr->next_index++;

	//pointer to parent
	next->prev = ptr;
}


//prints node's contents in a readable format
void Tree::printNode(node * ptr, int indents /*default is 0 */)
{
	//prints tic-tac-toe board
	board_obj.printBoard(ptr->board, indents);
	cout<<"Level: "<<ptr->level<<endl;
	cout<<"Num next nodes: "<<ptr->next_index<<endl;
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

//prints the neural network in one long output.
void Tree::printNet(node * ptr, int indents /*default is 0 */)
{
	printNode(ptr, indents);
	cout<<endl;

	for(int x = 0; x < ptr->next_index; x++)
	{
		// printNet(ptr->next[x], indents+1);
		printNode(ptr->next[x], indents+1);
	}

}


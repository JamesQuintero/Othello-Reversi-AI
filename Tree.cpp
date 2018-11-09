#include <iostream>
#include <string>
#include <ctime>
// #include "sqlite/sqlite3.h"

#include "global_functions.h"
#include "Tree.h"



using namespace std;

Tree::Tree()
{
	root = new node();
	// root->board = createMatrix(size);
	this->ptr = root;

	srand(time(0));  // needed once per program run
}

//determines the possible moves by piece, then adds them as children to the current pointer
void Tree::determinePossibleMoves(node* ptr, int piece)
{
	//stop if has children, meaning it's already been expanded
	if(ptr->next_index>0)
		return;


	vector<Board> possible_move_boards = ptr->board.getPossibleMoveBoards(piece);

	for(int x = 0; x < possible_move_boards.size(); x++)
	{
		// possible_move_boards[x].printBoard();
		newNode(ptr, possible_move_boards[x], piece);
		// cout<<"Added new board"<<endl;
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
Board Tree::getBoardMinHeuristic(node* ptr)
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


		if(h==min_heuristic)
		{
			minimum_indices.push_back(x);
		}

	}

	//chooses randomly out of all the smallest heuristics
	int random_index = randNum(0, minimum_indices.size());
	int index = minimum_indices[random_index];

	return ptr->next[index]->board;
}



//calculates heuristics for AI down to a certain depth
//considers current node as the AI's move, and therefore will be minimizing heuristic
double Tree::getMinHeuristic(node * ptr, int alpha /* starts as -INFINITY */, int beta /* starts as INFINITY */, int depth_left)
{
	double cur_heuristic = -1;

	//if has no children
	if(ptr->next_index == 0 || depth_left<=0)
	{
		cur_heuristic = calculateHeuristic(ptr);
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
		double h = getMaxHeuristic(ptr->next[x], alpha, beta, depth_left--);
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

double Tree::getMaxHeuristic(node * ptr, int alpha /* starts as -INFINITY */, int beta /* starts as INFINITY */, int depth_left)
{
	double cur_heuristic = -1;
	
	//if has no children
	if(ptr->next_index == 0 || depth_left<=0)
	{
		cur_heuristic = calculateHeuristic(ptr);
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
		double h = getMinHeuristic(ptr->next[x], alpha, beta, depth_left--);
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
double Tree::calculateHeuristic(node* ptr)
{
	
	int player_count = ptr->board.countPieces(player_piece);
	int AI_count = ptr->board.countPieces(AI_piece);

	double num_player_flips = ptr->board.getPossibleMovesCount(player_piece);
	double num_AI_flips = ptr->board.getPossibleMovesCount(AI_piece);



	//Looks at opponent's moves vs player's moves
	// return player_count-AI_count;

	// return (player_count - AI_count) + (num_player_flips - num_AI_flips);

	int* weights = new int[3];
	weights[0] = 1;
	weights[1] = 1;

	// cout<<"Num player flips: "<<num_player_flips<<endl;
	// cout<<"Num AI flips: "<<num_AI_flips<<endl;
	double heuristic = weights[0]*(player_count-AI_count) + weights[1]*(num_player_flips - num_AI_flips);
	
	return heuristic;

	//reatio heuristic doesn't work very well, and crashes...
	// return (player_count/(player_count+AI_count)) + (num_player_moves/(num_player_moves+num_AI_moves));
}


//Player moves to (col,row), so determine which board that is and move there
void Tree::playerMove(int col, int row)
{
	//iterates through current ptrs children
	for(int x = 0; x < ptr->next_index; x++)
	{
		Board temp_board = ptr->next[x]->board;

		//if board matches by seeing if a piece has been placed in specified position
		if(temp_board.board[col][row]!=0)
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
		Board temp_board = ptr->next[x]->board;

		//if board matches by seeing if a piece has been placed in specified position
		if(temp_board.board[col][row]!=0)
		{
			ptr = ptr->next[x];
			break;
		}
	}

}


//moves to child with board
void Tree::move(Board new_board)
{
	//iterates through current ptrs children
	for(int x = 0; x < ptr->next_index; x++)
	{

		//if board matches by seeing if a piece has been placed in specified position
		if(new_board.isEqual(ptr->next[x]->board)==true)
		{
			ptr = ptr->next[x];
			return;
		}
	}

	cout<<"Error, couldn't find move!"<<endl;
}


//iterates down the tree and populates with possible moves down to specified depth
void Tree::iterateTreeDepth(node* ptr, int piece, int cur_depth, int max_depth)
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
void Tree::newNode(node * ptr, Board new_board, int piece)
{
	ptr->next[ptr->next_index] = new node();
	// ptr->next.push_back(new node());

	//ptr to new next node
	node * next = ptr->next[ptr->next_index];

	//copies new board into new node
	next->board.copyBoard(&new_board);

	//increments number of next nodes
	ptr->next_index++;

	//pointer to parent
	next->prev = ptr;
}


//prints node's contents in a readable format
void Tree::printNode(node * ptr, int indents /*default is 0 */)
{
	//prints tic-tac-toe board
	ptr->board.printBoard(indents);
	cout<<"Num next nodes: "<<ptr->next_index<<endl;
	cout<<"Heuristic: "<<ptr->h<<endl;
}

int Tree::getOtherPiece(int piece)
{
	if(piece==1)
		return 2;
	else
		return 1;
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


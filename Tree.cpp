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
	vector<Board> possible_move_boards = ptr->board.getPossibleMoveBoards(piece);

	for(int x = 0; x < possible_move_boards.size(); x++)
	{
		// possible_move_boards[x].printBoard();
		newNode(ptr, &possible_move_boards[x], piece);
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
	int index = 0;
	int min_heuristic = numeric_limits<int>::max();
	for(int x = 0; x < ptr->next_index; x++)
	{
		int h = ptr->next[x]->h;
		if(h < min_heuristic)
		{
			min_heuristic = h;
			index = x;
		}
	}

	return ptr->next[index]->board;
}



//calculates heuristics for AI down to a certain depth
//considers current node as the AI's move, and therefore will be minimizing heuristic
int Tree::getMinHeuristic(node * ptr, int alpha /* starts as -INFINITY */, int beta /* starts as INFINITY */, int depth_left)
{
	int cur_heuristic = -1;

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
	int min_heuristic = numeric_limits<int>::max(); // max value
	for(int x = 0; x < ptr->next_index; x++)
	{
		int h = getMaxHeuristic(ptr->next[x], alpha, beta, depth_left--);
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

int Tree::getMaxHeuristic(node * ptr, int alpha /* starts as -INFINITY */, int beta /* starts as INFINITY */, int depth_left)
{
	int cur_heuristic = -1;
	
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
	int max_heuristic = numeric_limits<int>::min(); // minimum value
	for(int x = 0; x < ptr->next_index; x++)
	{
		int h = getMinHeuristic(ptr->next[x], alpha, beta, depth_left--);
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
int Tree::calculateHeuristic(node* ptr)
{
	Board board = ptr->board;

	
	int player_count = board.countPieces(player_piece);
	int AI_count = board.countPieces(AI_piece);

	//Looks at opponent's moves vs player's moves
	return player_count-AI_count;
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
void Tree::move(Board board)
{
	//iterates through current ptrs children
	for(int x = 0; x < ptr->next_index; x++)
	{
		Board temp_board = ptr->next[x]->board;

		//if board matches by seeing if a piece has been placed in specified position
		if(board.isEqual(temp_board)==true)
		{
			ptr = ptr->next[x];
			break;
		}
	}
}


//iterates down the tree and populates with possible moves down to specified depth
void Tree::iterateTreeDepth(node* ptr, int piece, int cur_depth, int max_depth)
{

	//checks if ptr has no children
	//if has no children, determinePosslbeMoves()
	//iterate through new children and run recursively
	

	//if has no children
	if(ptr->next_index == 0)
		determinePossibleMoves(&*ptr, piece);

	//if reached max depth, stop
	if(cur_depth==max_depth)
		return;


	//iterates through all children
	for(int x = 0; x < ptr->next_index; x++)
		iterateTreeDepth(ptr->next[x], getOtherPiece(piece), cur_depth+1, max_depth);
	
}


//links a new node to ptr, with initialized board
void Tree::newNode(node * ptr, Board * new_board, int piece)
{
	ptr->next[ptr->next_index] = new node();

	//ptr to new next node
	node * next = ptr->next[ptr->next_index];

	//copies new board into new node
	next->board.copyBoard(new_board);

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
		printNet(ptr->next[x], indents+1);
	}

}


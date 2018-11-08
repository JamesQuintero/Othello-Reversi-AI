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


//Player moves to (col,row), so determine which board that is and move there
void Tree::playerMove(int col, int row)
{
	return;
}


//returns the Board object that the AI moves to
void Tree::AIMove(int col, int row)
{
	return;
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
	
}


//prints node's contents in a readable format
void Tree::printNode(node * ptr)
{
	//prints tic-tac-toe board
	ptr->board.printBoard();
	cout<<"Num next nodes: "<<ptr->next_index<<endl;
	cout<<"Heuristic: "<<ptr->h<<endl;
}

//prints the neural network in one long output.
void Tree::printNet(node * ptr)
{
	printNode(ptr);
	cout<<endl;

	for(int x =0; x < ptr->next_index; x++)
	{
		printNet(ptr->next[x]);
	}

}


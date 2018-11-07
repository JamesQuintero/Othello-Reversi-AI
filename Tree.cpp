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
	root->board = createMatrix(size);
	this->ptr = root;

	srand(time(0));  // needed once per program run
}

//links a new node to ptr, with initialized board
void Tree::newNode(node * ptr, int** board)
{
	ptr->next[ptr->next_index] = new node();

	//ptr to new next node
	node * next = ptr->next[ptr->next_index];
	next->board = board;

	//increments number of next nodes
	ptr->next_index++;

	//pointer to parent
	next->prev = ptr;	
}


//Player moves, so add to neural net
void Tree::playerMove(int** board)
{
	return;
}


//neural network determines next best move and returns its coordinates
int** Tree::AIMove(int** board, int** possible_moves)
{
	return NULL;
}


//saves the neural network to a db file for later retrieval
void Tree::saveNet()
{
	// //opens sqlite file
	// sqlite3 *db;
	// sqlite3_open("ttt_Tree.db", &db);






 //  string createQuery = "CREATE TABLE IF NOT EXISTS items
 //   (userid INTEGER PRIMARY KEY, ipaddr TEXT,username TEXT,useradd TEXT,userphone INTEGER,age INTEGER, time TEXT NOT NULL DEFAULT (NOW()));";

	// sqlite3_stmt *createStmt;
	// cout << "Creating Table Statement" << endl;
	// sqlite3_prepare(db, createQuery.c_str(), createQuery.size(), &createStmt, NULL);

	// cout << "Stepping Table Statement" << endl;
	// if (sqlite3_step(createStmt) != SQLITE_DONE) cout << "Didn't Create Table!" << endl;

	// string insertQuery = "INSERT INTO items 
	// (time, ipaddr,username,useradd,userphone,age) VALUES 
	// ('7:30', '192.187.27.55','vivekanand','kolkatta','04456823948',74);"; // WORKS!

	// sqlite3_stmt *insertStmt;
	// cout << "Creating Insert Statement" << endl;
	// sqlite3_prepare(db, insertQuery.c_str(), insertQuery.size(), &insertStmt, NULL);
	// cout << "Stepping Insert Statement" << endl;
	// if (sqlite3_step(insertStmt) != SQLITE_DONE) cout << "Didn't Insert Item!" << endl;  
	// 	return 0;	
     
}

//saves node to db file
void Tree::saveNode()
{

}


//prints node's contents in a readable format
void Tree::printNode(node * ptr)
{
	//prints tic-tac-toe board
	printBoard(ptr->board, size);
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


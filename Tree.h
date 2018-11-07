#include <iostream>
#include <string>



#ifndef Tree_H
#define Tree_H

using namespace std;



class Tree
{
	private:
		//side length of the Othello board
		static const int size = 8;
		//percentage of the "neuron" that gets increment if its path results in a good outcome
		// static constexpr double perc = 0.25;

		
		struct node
		{
			//matrix of board state
			//acts like the ID of the node
			int** board;
			//heuristic
			double h = 0;
			
			//indices for children
			int next_index = 0;
			int prev_index = 0;

			//children
			node** next = new node*[size*size];
			//parent
			node* prev;
		};
		

	public:
		int piece = 0;

		//root node of the tree
		node * root;

		//current position in the tree
		node * ptr;


		Tree();
		//links a new node to ptr, with initialized board
		void newNode(node * ptr, int** board);

		//returns a new board containing Tree's next move
		int** AIMove(int** board, int** possible_moves);
		void playerMove(int** board);

		void saveNet();
		void saveNode();

		void printNode(node * ptr);
		void printNet(node * ptr);



};



#endif



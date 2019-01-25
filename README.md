# Othello-AI
Very good AI that can play Othello/Reversi board game.

![Preview](./documents/Othello%20board.png)

### AI techniques used: ###
- Minimax/Negamax
- Heuristic formula
- Monte-Carlo Tree Search
- Reinforcement learning

### Heuristic formula where each variable is the difference between each player: ###
- Difference in piece count
- Difference in weight of position of pieces
- Difference in mobility
- Difference in potential mobility
- Difference in stability

For the heuristic formula, each variable is also weighted based on its importance, and that importance was determined by how well the AI performed against a benchmark. We found that giving the most weight to mobility, gave our AI the best chance of success. We also found that dynamically changing the weights of each variable in the heuristic during the game's progression, also increased the AI's success rate.


We played this AI against popular online Othello AIs, and beat them on hardest difficulties. Our AI was able to beat WZebra, a well respected and one of the best Othello enginers, was searching in depths 1 and 2, but our AI fell to WZebra when it was searching at depth 3. Our AI is able to search up to depth 6 before computing a move starts taking longer than a couple seconds. 

![Preview](./documents/othello%20output.PNG)

To play, run "Othello.out" on any computer that has gcc/g++ installed


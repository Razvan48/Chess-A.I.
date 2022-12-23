# Chess-A.I. | _C++, MinMax Algorithm, Alpha-Beta Pruning, position hashing, move ordering_

&emsp; The program is able to play Chess at an ELO rating of 1300 points. <br/>
&emsp; It is based on a brute-force search with a few optimisations applied (Alpha-Beta Pruning, position hashing, move ordering and others). <br/>
&emsp; It uses bitboards for faster moves generation. <br/>
&emsp; A lot of logical operations and instructions were used. <br/>

**How to use the program:**

&emsp; First, the program asks what color the player and the A.I. will be. It will also ask for the maximum depth of the move search. It is recommended to choose a search depth of 4 or 5 (5 still takes 15 seconds for each move on my computer). <br/>

<br/>

&emsp; When the application is running, the font size in the executable must be set to 8, otherwise the board will be too large for the executable screen. <br/>

<br/>

&emsp; To perform a move, type the id of piece to be moved, its start position and its end position. <br/>
For example, for a white rook from e2 to e4, we will type "Re2e4" and then press enter. For a black queen from 7f to 8f, we will type "q7f8f" and press enter. <br/>

<br/>

&emsp; Pieces IDS: <br/>

<br/>
 
P / p = white / black pawn; <br/>
R / r = white / black rook; <br/>
N / n = white / black knight; <br/>
B / b = white / black bishop; <br/>
Q / q = white / black queen; <br/>
K / k = white / black king; <br/>

<br/>

&emsp; This is how most moves work. There are only a few exceptions: <br/>

<br/>

&emsp; If we make an en passant, then a "+" must be typed after the usual 5 characters. <br/>
&emsp; If we promote a pawn, then we also type r / n / b / q for black and R / N / B / Q for white, to specify what the respective pawn turns into. <br/>

<br/>

&emsp; The program stops itself if there is a draw or checkmate for someone. <br/>

<br/>

&emsp; The player can set from which perspective to see the pieces: from that of white (white down) or black (black down). <br/>

<br/>

&emsp; The application interface communicates with the player, since it tells him at any moment whose turn it is, if someone is in chess, if there is a draw or if there is a checkmate for someone. <br/>

<br/>

&emsp; The code can be easily modified to allow playing Chess between 2 people, not between the person and the A.I.. <br/>

<br/>








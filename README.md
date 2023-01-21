# Adventure - Game Project
A little Game Project made for a class assigment.

Made for my Data Structure class for college, using C language.
The assigment was to make a game using data structures learned in class (Stacks, Queues, lists, ...), and some other stuff, having a Hangman game as example for the rules we had to follow.

I decided to give myself some trouble and try to make a RPG ASCII game... Which meant I didn't actually finish the game, BUT
even tough my code is kinda of a mess, and not all of the screens and menus where finished, and there wasn't an actual story and maps, it has a temp map with some events for testing and an automatic battle. It has also difficulty selection, and Player VS Player and Player VS Computer, that doesn't make much of a difference sice therre is no story or game progression, just functions testing.

Well, I still got a 8,5/10 for it, so it was all good.

# Assigment Rules
- It has to read the player name; :white_check_mark:
- It has to allow player to choose 1 or 2 players (Player vs Player, Player vs Computer); :white_check_mark:
  - The players where to battle after some xx in-game days to choose the winner, but as it wasn't implemented, I just made it check money and items the players have and give them points based on that to choose a winner.
- It has to allow player to choose the game difficulty (easy, normal, hard); :white_check_mark:
  - Players start with more STR and money the easier the difficulty chosen, which allow them to buy more items from the beginning.... even though the shop wasn't made in time.
- It has to allow to two kinds of operations: Replay of moves made, and Rewind moves made. And all moves have to be recorded in a linear structure, such as lists, queues, stacks, etc. :x:
  - Replay: I made a linked list to save 10(lastest) of the player battles, to allow to replay them: although no one can actually replay them, I didn't finish it, haha.
  - Rewind/Redo: for that, my intention was to let the player undo their last action, in case they wasted their energy/actions (each player would have an aloted number of actions/energy per day to use, which he had to spend wisely to become stronger than his adversary) with something while their turn, but I wasn't able to do it. Ex: started a 5hours sword training instead of 2? Undo.

# Afterthoughts
I may finish, or simply fix/clean the code in the near future, but maybe in a new repository, just so I can keep this one as I delivered and was graded for.

(There may be some english mistakes in this README.md, but bear with me. You can tell me where to fix if you wish, that would help lots.)

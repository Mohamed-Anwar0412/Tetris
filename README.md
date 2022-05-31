# Tetris
a simple Tetris game with a soundtrack and a simple scoring &amp; leveling system.
How it works:
    There is the map (1d array) which represent the game.
    There is the field (1d array) which represent the moving piece.
    There is the next (1d array) which represent the following piece.
    We used 1d arrays instead of 2d arrays to use pointers to assign the moving piece to the field and next.
• the method we used is representing a 2d array as a 1d array by simply use [i*width +j] instead of [i][j].
    There is a very simple collision detection function which check if the assigned move from the player is allowed or not.
• If it’s not allowed the moving piece stop and get copied to the map.
• Then, there two functions that immediately check two things:
  - If there is a winning row then, shifts the winning rows
  - If the player loses or not.
• the field gets the following piece that is showed in next.
   The player can also rotate the moving piece using a rotation function that rotate the moving piece 90o each time it is called.
   The player has the option to restart the game at any time by pressing P.
   There is timing in the game that makes the piece move down and it move faster as score & the level increase.
   If its game over there is option to continue playing from beginning or close the game by simply break the rendering loop.
   The graphic library is used to draw the map and the moving piece along with the controls, score and level.
   There is a sound track to the game and also there are an impact sound, winning sound, losing sound.
All images of the pieces are self-designed.
Soundtrack used in the game:
  • Sonic the Hedgehog 3 soundtrack.
    - File Select - Sonic the Hedgehog 3 [OST] - YouTube

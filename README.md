# Memory-Test-Arduino-Game
A simple memory game for the Arduino Uno and LCD keypad shield



## Summary
In the memory game the Arduino shows a sequence of characters on the screen, blanks the screen and
the user then repeats that sequence using the buttons. If the player is correct, then the game repeats
becoming increasingly complicated. The system maintains a high score table of progress.



## How it works
In the basic game the display shows a sequence S of N characters chosen from a set of M possible
characters, where M is a subset of {LEFT, RIGHT, UP, DOWN}.

Each character is displayed in the same position on the display and is shown for an amount of time D
(the delay).

The user remembers S and then enters it into the Arduino using the direction keys. In its simplest form
the game displays 4 characters from a set of 2, say, LEFT and RIGHT.

As the game becomes more complicated the number of characters in N and M increases.

If the user enters the sequence correctly, then flash the backlight and move to the next stage.

If the sequence is entered incorrectly, then a suitable message is printed and the game is over.




## Instructions
MENU: To navigate the menu, use the left or right buttons to cycle through pages. To select a page, press the select button.

PRACTICE & STORY: There are 4 symbols, (+, -, *, /) that correspond to the “left”, “up”, “right” and “down” buttons respectively. Match the symbols to complete a round.

GAME SETTINGS: To navigate the game settings, hold the right button to cycle through options. Hold the up button to increase the selected value, hold the down button to decrease the selected value. To reset the selected value to its default, hold the left button.

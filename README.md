# Strategy Game (name pending)

This game will be a turn-based medieval fantasy style strategy game, where each player will have to successfully conquer the opponent. Each player will get an amount of mana each turn to spend on units or buildings.

## Units

Each unit has multiple attributes, including:
* Health
* Attack
* Speed (How far it can move per turn)
* Vision (How much of the map it reveals)
* Range (How far it can attack)

## Known Bugs

* Units and damage circles appear lower than they should, below the center of the tile.
* After one player moves, the creature is deselcted for that player and selected for at least one other player. That player can then make the creature move or attack.
* When the board is not square, errors with rendering terrain textures, centering the board, and calculating the tile at which the mouse is located occur.

## Upcoming Additions

* Adding vision to map.
* Adding menu, opening interface.
* Adding a winning mechanism: Add a single building that when it is killed, that player loses.
* Have each player start with a number of creatures.

## Authors

This game was created by [Jake Sanders](https://github.com/ja-San) and [Michael Caosun](https://github.com/theonlycaosun).

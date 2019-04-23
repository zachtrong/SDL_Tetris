# Scoring system

| Action | Points |
|:-------|:-------|
| Single | 100 |
| Double | 200 |
| Triple | 300 |
| T-Spin | 400 |
| Tetris/ T-Spin Single | 800 |
| BtB T-Spin Single/ BtB Tetris/ T-Spin Double | 1200 |
| T-Spin Triple | 1600 |
| BtB T-Spin Double | 1800 |
| BtB T-Spin Triple | 2400 |
| _Combo_ | 50 x combo count |
| _Soft drop_ | 1 point per cell |
| _Hard drop_ | 2 points per cell |

> Reference: https://tetris.fandom.com/wiki/Scoring
> 
> **Note**: _Mini T-spin_ is not implemented in our Tetris.

## What is a T-spin?

The game rewards for twisting a tetromino into a tight space. When this is done with a T tetromino, it is called a **T-spin**.

### Basic algorithm used to detect and reward in-place spins
**3-corner T**: A T-spin bonus is awarded if all of the following are true:

1. Tetromino being locked is T.

2. Last successful movement of the tetromino was a rotate, as opposed to sideways movement, downward movement, or falling due to gravity. The tetromino doesn't even have to end up in a different orientation than it was dropped in.

3. Three of the 4 squares diagonally adjacent to the T's center are occupied. In our game, the walls and floor surrounding the playfield are considered "occupied".

> Reference: https://tetris.fandom.com/wiki/T-Spin

## Combo
**Combos** are bonuses that exist in certain games which reward multiple line clears in quick succession.

## Back-to-back (BtB)
**Back-to-Back (BtB)** or **Back-2-Back (B2B)** clears are any combination of two or more "difficult" line clears without an "easy" line clear between them.
### Recognition
|Clear Type|	Recognition|
|:---------|:--------------|
|Single|	No|
|Double|	No|
|Triple|	No|
|Tetris|	Yes|
|T-Spin Single|	Yes|
|T-Spin Double|	Yes|
|T-Spin Triple|	Yes|

> Reference: https://harddrop.com/wiki/Back-to-Back

/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements the Game of Fifteen (generalized to d x d).
 *
 * Usage: ./fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [MIN,MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// board's minimal dimension
#define MIN 3

// board's maximal dimension
#define MAX 9

// board, whereby board[i][j] represents row i and column j
int board[MAX][MAX];

// board's dimension
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
void save(void);

int main(int argc, string argv[])
{
    // greet player
    greet();

    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < MIN || d > MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            MIN, MIN, MAX, MAX);
        return 2;
    }

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // saves the current state of the board (for testing)
        save();

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep for animation's sake
        usleep(500000);
    }

    // that's all folks
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1,
 * (i.e., fills board with values but does not actually print them),
 * whereby board[i][j] represents row i and column j.
 */
void init(void)
{
    // TODO
   // for loop to fill array of d dimensions 

   for (int row = 0, tileNum = (d *d); row < d; row++)
   {
    for (int col = 0; col < d; col++)   
    {

            board[row][col] = --tileNum;
    }
   }
   //  set bottom right corner value to total grid spaces for won check
   board[d-1][d-1] = (d*d);
   
   //  switch 1 and 2 tiles when odd amount of tiles
   if (d %2 == 0)
   {
        board[d-1][d-2] = 2;
        board[d-1][d-3] = 1;
   }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // TODO
    printf("\n");
    for (int pRow = 0; pRow < d; pRow++)
    {
        for (int pCol = 0; pCol < d; pCol++)
        {
            if (board[pRow][pCol] != (d*d) )
            {
                printf(" %2d ", board[pRow][pCol]);
            }
            else
            {
                printf(" __ ");             
            }
            
        }
        printf("\n\n");
    }

}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // TODO
    if (!(tile > 0 && tile <= (d*d)-1))
    {
        return false;
    }
    else
    {
    
    //  find empty tile and store in variable
    int emptyRow;
    int emptyCol;
    
    for (int mRow = 0; mRow < d; mRow++)
    {
        for (int mCol = 0; mCol < d; mCol++)
        {
            if (board[mRow][mCol] == (d*d) )
            {
                emptyRow = mRow;
                emptyCol = mCol;
            }
        }
    }
    
    // find tile position
    // variable for tile row and col 
    int tilePosRow;
    int tilePosCol;
    // linear search through array
    for (int tileRow = 0; tileRow < d; tileRow++)
    {
        for (int tileCol = 0; tileCol < d; tileCol++)
        {
            if (board[tileRow][tileCol] == tile)
            {
                tilePosRow = tileRow;
                tilePosCol = tileCol;
            }
        }
    }
    
    // check for valid move adjacent to empty tile using absolute values of difference
    int rowDiff = emptyRow - tilePosRow;
    int colDiff = emptyCol - tilePosCol;
    rowDiff = abs(rowDiff);
    colDiff = abs(colDiff);
    
    //  condition checks that tile and empty are adjacent and not diagnol
    if ( ( (rowDiff <= 1)  && (colDiff <= 1) ) && !( rowDiff == 1 && colDiff ==1)   )
    {
        int swapTemp = board[emptyRow][emptyCol];
        board[emptyRow][emptyCol] = board[tilePosRow][tilePosCol];
        board[tilePosRow][tilePosCol] = swapTemp;
        return true;
    }
    
 } // end else
 return false;
}  // end move

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{ 
    int winning = 1;
    if (board[d-1][d-1] == (d*d) )
    {

        for (int row = 0; row < d; row++)
        {
             for (int col = 0; col < d; col++)   
            {
                if ( board[row][col] == winning )
                    winning++;
                else
                    return false;
            }
         }
       return true;
    }
    else
    return false;   
}

/**
 * Saves the current state of the board to disk (for testing).
 */
void save(void)
{
    // log
    const string log = "log.txt";

    // delete existing log, if any, before first save
    static bool saved = false;
    if (!saved)
    {
        unlink(log);
        saved = true;
    }

    // open log
    FILE* p = fopen(log, "a");
    if (p == NULL)
    {
        return;
    }

    // log board
    fprintf(p, "{");
    for (int i = 0; i < d; i++)
    {
        fprintf(p, "{");
        for (int j = 0; j < d; j++)
        {
            fprintf(p, "%i", board[i][j]);
            if (j < d - 1)
            {
                fprintf(p, ",");
            }
        }
        fprintf(p, "}");
        if (i < d - 1)
        {
            fprintf(p, ",");
        }
    }
    fprintf(p, "}\n");

    // close log
    fclose(p);
}

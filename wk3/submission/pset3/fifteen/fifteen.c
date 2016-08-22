/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements Game of Fifteen (generalized to dim x dim).
 *
 * Usage: fifteen dim
 *
 * whereby the board's dimensions are to be dim x dim,
 * where dim must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int dim;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
int find(int tile);
bool neighbours(int pos_one, int pos_two);
void swap(int pos_one, int pos_two);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen dim\n");
        return 1;
    }

    // ensure valid dimensions
    dim = atoi(argv[1]);
    if (dim < DIM_MIN || dim > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < dim; i++)
        {
            for (int j = 0; j < dim; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < dim - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();
        
        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(200000);
        }

        // sleep thread for animation's sake
        usleep(200000);
    }
    
    // close log
    fclose(file);

    // success
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
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(500000);
}

/**
 * Initializes the game's board with tiles numbered 1 through dim * dim - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    int tile = dim * dim - 1;
    bool odd = tile % 2;
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            board[i][j] = tile--;
        }
    }
    if (odd)
    {
        board[dim - 1][dim - 3] = 1;
        board[dim - 1][dim - 2] = 2;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    char first, second, between;
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            first = (board[i][j] < 10) ? ' ' : (char) (board[i][j] / 10 + 48);
            second = (board[i][j]) ? (char) (board[i][j] % 10 + 48) : '_';
            between = (j < dim - 1) ? '|' : '\0';
            printf(" %c%c %c", first, second, between);
        }
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // find the tile
    int tile_pos = find(tile);
    if (tile_pos == -1 || tile == 0)
    {
        printf("someone is playing silly buggers\n");
        return false;
    }
    
    // find the blank tile
    int blank_pos = find(0);
    
    // check if they neighbour
    if (neighbours(tile_pos, blank_pos))
    {
        // yes? swap them
        swap(tile_pos, blank_pos);
        return true;
    }
    
    // no? false
    return false;
}

/**
 * Find the position (1D unwound array index) of a tile and return it, or if
 * the tile is not present, return -1.
 */
int find(int tile)
{
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            if (board[i][j] == tile)
            {
                return i * dim + j;
            }
        }
    }
    return -1;
}

/**
 * Returns true if the unwound 1D positions neighbour, else false.
 */
bool neighbours(int pos_one, int pos_two)
{
    // checks that any of the following are true, in order:
    // pos_two is above pos_one
    // pos_two is right of pos_one (on the board)
    // pos_two is left of pos_one (on the board)
    // pos_two is below pos_one
    return (pos_two == pos_one - dim) || 
           (pos_two == pos_one + 1 && pos_two % dim != 0) || 
           (pos_two == pos_one - 1 && pos_one % dim != 0) || 
           (pos_two == pos_one + dim);
}

/**
 * Swaps the tiles at the two unwound 1D positions.
 */
void swap(int pos_one, int pos_two)
{
    int tmp = board[pos_one / dim][pos_one % dim];
    board[pos_one / dim][pos_one % dim] = board[pos_two / dim][pos_two % dim];
    board[pos_two / dim][pos_two % dim] = tmp;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            // if any tile at board[i][j] isn't the unwound index + 1 or zero
            if (board[i][j] != i * dim + j + 1 && board[i][j] != 0)
            {
                // we haven't won
                return false;
            }
        }
    }
    // if all tiles are in the correct position, we fall through and win
    return true;
}
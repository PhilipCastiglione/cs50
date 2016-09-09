#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#define _XOPEN_SOURCE 500
#define DIM_MIN 3
#define DIM_MAX 5
#define LOTS 65536

int validate_args(int argc, char *argv[]);
void initialize_board(int **board, int dim);
void set_starting_board(int **board, int dim);
void randomize_board(int **board, int dim);
void run_game(int **board, int dim);
void draw_board(int **board, int dim);
int is_board_correct(int **board, int dim);
void accept_user_move(int **board, int dim);
int idx_for_tile(int **board, int dim, int tile);
void move_tile_at_idx(int **board, int dim, int tile_idx);
void god(int **board, int dim);
int is_valid_move(int **board, int dim, int tile_idx);
void malloc_arrays(int *tiles, int *costs, int *parents, int *explored, int ***checked_positions, int dim);
void populate_move_idxs(int **board, int dim, int *move_idxs);
void add_new_moves(int **board, int dim, int *tiles, int *costs, int *parent_idxs, int *explored, int ***checked_positions, int *local_move_idxs, int parent_idx, int *size, int *won);
int is_new_valid_move(int **board, int dim, int move_idx, int ***checked_positions, size);
int is_new_move(int **board, int dim, int move_idx, int ***checked_positions, int size);
void add_move_to_arrays(int **board, int dim, int *tiles, int *costs, int *parent_idxs, int *explored, int ***checked_positions, int move_idx, int parent_idx, int *won, int *size);
int board_cost(board, dim)
void add_board_to_checked_positions(int **board, int dim, int ***checked_positions, int size)
void move_to_best_position(int **board, int dim, int *costs, int *explored, int ***checked_positions, int size, int *parent_idx);

int main(int argc, string argv[])
{
    if (int code = validate_args(argc, argv))
    {
      return code;
    }

    int dim = atoi(argv[1]);
    int **current_board = malloc(dim * dim * sizeof(int));

    initialize_board(current_board, dim);

    run_game(current_game, dim);

    return 0;
}

int validate_args(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }
    else if (atoi(argv[1]) < DIM_MIN || atoi(argv[1]) > DIM_MAX)
    {
        printf("Usage: fifteen d\n");
        printf("d must be between %i and %i inclusive.\n", DIM_MIN, DIM_MIN);
        return 2;
    }
    return 0;
}

void initialize_board(int **board, int dim)
{
    set_starting_board(board, dim);
    randomize_board(board, dim);
}

void set_starting_board(int **board, int dim)
{
    int tile = dim * dim - 1;
    int odd = tile % 2;
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

void randomize_board(int **board, int dim)
{
    srand((int) time(NULL) % 32768);
    int move_count = rand() % (dim * 10) + (dim * 10);
    for (int i = 0; i < move_count; i++)
    {
        int *local_move_idxs = malloc(4 * sizeof(int));
        populate_move_idxs(board, dim, local_move_idxs);

        int random_index;
        do
        {
          random_index = rand() % 4;
        }
        while (local_move_idxs[random_index] != -1)
        move_tile_at_idx(board, dim, local_move_idxs[random_index]);
    }
}

void run_game(int **board, int dim)
{
    while (1)
    {
        draw_board(board, dim);

        if (is_board_correct(board, dim))
        {
            printf("You win. Congratulations. I hope it was worth it.\n");
            break;
        }

        accept_user_move(board, dim);
    }
}

void draw_board(int **board, int dim)
{
    usleep(200000);
    char c1, c2, sep;
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            c1 = (board[i][j] < 10) ? ' ' : (char) (board[i][j] / 10 + 48);
            c2 = (board[i][j]) ? (char) (board[i][j] % 10 + 48) : '_';
            sep = (j < dim - 1) ? '|' : '\0';
            printf(" %c%c %c", c1, c2, sep);
        }
        printf("\n");
    }
}

int is_board_correct(int **board, int dim)
{
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            if (board[i][j] != i * dim + j + 1 && board[i][j] != 0)
            {
                return 0;
            }
        }
    }
    return 1;
}

void accept_user_move(int **board, int dim)
{
    char *move = "";
    do
    {
        printf("Tile to move: ");
        char input[4];
        read(STDIN_FILE_END, input, 3);
        input[3] = '\0';
        int tile_idx = idx_for_tile(board, dim, atoi(input))
        if (strcmp(input, "GOD") == 0)
        {
           god(board, dim);
           // TODO: first, just print the moves, later refactor to make them and break
           // break;
        }
        else if (is_valid_move(board, dim, tile_idx))
        {
            move_tile_at_idx(board, dim, tile_idx);
            strcpy(move, input);
        }
        else
        {
           printf("Illegal move.\n");
        }
    }
    while(strcmp(move, "") == 0);
}

int idx_for_tile(int **board, int dim, int tile)
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

void god(int **board, int dim)
{
    // TODO: should tiles be tile idxs?
    int* tiles, costs, parent_idxs, explored;
    int ***checked_positions;
    malloc_arrays(tiles, costs, parent_idxs, explored, checked_positions, dim);

    int size = 0;
    int parent_idx = -1;
    int won = 0;

    while (!won)
    {
        int *local_move_idxs = malloc(4 * sizeof(int));

        populate_move_idxs(board, dim, local_move_idxs);

        add_new_moves(board, dim, tiles, costs, parent_idxs, explored, checked_positions, local_move_idxs, parent_idx, &size, &won);

        move_to_best_position(board, dim, costs, explored, checked_positions, size, &parent_idx);
    }

    // TODO: change printing the won stuff to returning/setting/doing whatever
    printf("winning moves, in reverse order!\n");
    while (size > 0)
    {
        printf("%d\n", tiles[size]);
        size = parents[size];
    }
}

void move_tile_at_idx(int **board, int dim, int tile_idx)
{
    int zero_idx = idx_for_tile(board, dim, 0);
    int tmp = board[zero_idx / dim][zero_idx % dim];
    board[zero_idx / d][zero_idx % dim] = board[tile_idx / dim][tile_idx % dim];
    board[tile_idx / dim][tile_idx % dim] = tmp;
}

int is_valid_move(int **board, int dim, int tile_idx)
{
    int zero_idx = idx_for_tile(board, dim, 0);
    if (tile_idx == -1 || tile_idx == zero_idx)
    {
      return 0;
    }
    return (tile_idx == zero_idx - dim) || // North neighbour
           (tile_idx == zero_idx + 1 && tile_idx % dim != 0) || // East neighbour
           (tile_idx == zero_idx + dim) || // South neighbour
           (tile_idx == zero_idx - 1 && zero_idx % dim != 0); // West neighbour
}

void malloc_arrays(int *tiles, int *costs, int *parents, int *explored, int ***checked_positions, int dim)
{
    tiles = malloc(LOTS * sizeof(int));
    costs = malloc(LOTS * sizeof(int));
    parent_idxs = malloc(LOTS * sizeof(int));
    explored = malloc(LOTS * sizeof(int));
    checked_positions = malloc(LOTS * dim * dim * sizeof(int*));
    for (int i = 0; i < LOTS; i++)
    {
        checked_positions[i] = malloc(dim * dim * sizeof(int*));
        for (int j = 0; j < dim; j++)
        {
            checked_positions[i][j] = malloc(dim * sizeof(int));
        }
    }
}

void populate_move_idxs(int **board, int dim, int *move_idxs)
{
    int zero_idx = idx_for_tile(board, dim, 0);
    int i = zero_idx / dim;
    int j = zero_idx % dim;
    move_idxs[0] = (i == 0) ? -1 : zero_idx - dim; // North idx
    move_idxs[1] = (j == dim - 1) ? -1 : zero_idx + 1; // East idx
    move_idxs[2] = (i == dim - 1) ? -1 : zero_idx + dim; // South idx
    move_idxs[3] = (j == 0) ? -1 : zero_idx - 1; // West idx
}

void add_new_moves(int **board, int dim, int *tiles, int *costs, int *parent_idxs, int *explored, int ***checked_positions, int *local_move_idxs, int parent_idx, int *size, int *won)
{
    for (int i = 0; i < 4; i++)
    {
        if (!won && is_new_valid_move(board, dim, local_move_idxs[i], checked_positions, size))
        {
            add_move_to_arrays(board, dim, tiles, costs, parent_idxs, explored, checked_positions, local_move_idxs[i], parent_idx, &won, &size);
        }
    }
}

int is_new_valid_move(int **board, int dim, int move_idx, int ***checked_positions, size)
{
    return is_valid_move(board, dim, local_move_idxs[i]) &&
      is_new_move(board, dim, local_move_idxs[i], checked_positions, size);
}

int is_new_move(int **board, int dim, int move_idx, int ***checked_positions, int size)
{
    int zero_idx = idx_for_tile(board, dim, 0);
    move_tile_at_idx(board, dim, move_idx);
    for (int i = 0; i < size; i++)
    {
        int matching = 1;
        for (int j = 0; j < dim; j++)
        {
            for (int k = 0; k < dim; k++)
            {
                if (checked_positions[i][j][k] != board[j][k])
                {
                    matching = 0;
                    break;
                }
            }
            if (!matching)
            {
              break;
            }
        }
        if (matching)
        {
            move_tile_at_idx(board, dim, zero_idx);
            return 0;
        }
    }
    move_tile_at_idx(board, dim, zero_idx);
    return 1;
}


void add_move_to_arrays(int **board, int dim, int *tiles, int *costs, int *parent_idxs, int *explored, int ***checked_positions, int move_idx, int parent_idx, int *won, int *size)
{
    tiles[size] = tile_for_idx(move_idx);

    int zero_idx = idx_for_tile(board, dim, 0);
    move_tile_at_idx(board, dim, move_idx);

    explored[size] = 0;
    int cost = board_cost(board, dim);
    costs[size] = (parent_idx >= 0) ? costs[parent_idxs[parent_idx]] + cost : cost;
    parents_idxs[size] = parent_idx;

    add_board_to_checked_positions(board, dim, checked_positions, size);

    move_tile_at_idx(board, dim, zero_idx);

    (cost == 0)? *won++ : size++ ;
}

int board_cost(board, dim)
{
    int acc = 0;
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            if (board[i][j] != 0)
            {
                acc += abs((board[i][j] - 1) / dim - i) + abs((board[i][j] - 1) % dim - j);
            }
        }
    }
    return acc;
}

void add_board_to_checked_positions(int **board, int dim, int ***checked_positions, int size)
{
    for (int j = 0; j < dim; j++)
    {
        for (int k = 0; k < dim; k++)
        {
            checked_positions[size][j][k] = board[j][k];
        }
    }
}

void move_to_best_position(int **board, int dim, int *costs, int *explored, int ***checked_positions, int size, int *parent_idx)
{
    int lowest_cost_parent = -1;
    int lowest_cost = INT_MAX;
    
    for (int i = 0; i < size; i++)
    {
        if (!explored[i])
        {
            if (costs[i] < lowest_cost)
            {
                lowest_cost_parent = i;
                lowest_cost = costs[i];
            }
        }
    }
    
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            board[i][j] = checked_positions[lowest_cost_parent][i][j];
        }
    }
    
    explored[lowest_cost_parent] = true;
    *parent_idx = lowest_cost_parent;
}

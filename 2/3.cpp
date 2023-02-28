#include <cstdio>
#include <iostream>
#include <vector>
#include <cstdio>

using namespace std;

#define SIZE 7

const char dirs[5] = "LUDR";

char to_char(int);
int to_dir(vector<int> board, int from, char dir);

struct Move {
    int src, dest;

    string to_string() {
        string output, dir;
        switch (dest - src) {
            case 2:
                dir = "RIGHT";
                break;
            case -2:
                dir = "LEFT";
                break;
            case 2*SIZE:
                dir = "DOWN";
                break;
            case -2*SIZE:
                dir = "UP";
                break;
        }
        output += ((int)(src/SIZE))+'A';
        output += (src%SIZE)+'1';
        output += " " + dir;
        return output;
    }
};

typedef vector<int> Board;

int number_of_pieces(Board board) {
    int counter = 0;
    for (auto cell: board) 
        if (cell == 1)
            counter++;

    return counter;
}

vector<Move> possible_moves(Board board, int from) {
    vector<Move> poss_mvs;
    int to;
    for (int i=0; i<4; i++) {
        to = to_dir(board, from, dirs[i]);
        if (to != -1)
            poss_mvs.push_back({from, to});
    }

    return poss_mvs;
}

void inspect(Board board) {
    for (int i=0; i<SIZE; i++) {
        for (int j=0; j<SIZE; j++)
            cout << to_char(board.at(i*SIZE + j));

        cout << endl;
    }
}

/*
    -1 => not in the board
     0 => empty
     1 => filled
*/
Board initialize_board() {
    Board board;
    string line;
    for (int l=0; l<SIZE; l++) {
        cin >> line;
        for (int r=0; r<SIZE; r++) {
            int n;
            switch (line[r]) {
                case 'X':
                    n = -1;
                    break;
                case 'O':
                    n = 0;
                    break;
                case 'N':
                    n = 1;
                    break;
            }
            board.push_back(n);
        }
    }
    return board;
}

Board make_move(Board board, Move move) {
    Board new_board(board);
    new_board.at(move.src) = 0;
    new_board.at((move.src + move.dest)/2) = 0;
    new_board.at(move.dest) = 1;
    return new_board;
}

Board undo_move(Board board, Move move) {
    Board new_board(board);
    new_board.at(move.src) = 1;
    new_board.at((move.src + move.dest)/2) = 1;
    new_board.at(move.dest) = 0;
    return new_board;
}

int to_dir(vector<int> board, int from, char dir) {
    bool condition_for_invalid;
    int to;
    switch (dir) {
        case 'L':
            condition_for_invalid = from%SIZE < 2 || board.at(from-1) != 1 || board.at(from-2) != 0 || board.at(from-1) != 1;
            to = from-2;
            break;
        case 'R':
            condition_for_invalid = from%SIZE > 4 || board.at(from+1) != 1 || board.at(from+2) != 0 || board.at(from+1) != 1;
            to = from+2;
            break;
        case 'U':
            condition_for_invalid = (int)from/SIZE < 2 || board.at(from-SIZE) != 1 || board.at(from-2*SIZE) != 0;
            to = from-2*SIZE;
            break;
        case 'D':
            condition_for_invalid = (int)from/SIZE > 4 || board.at(from+SIZE) != 1 || board.at(from+2*SIZE) != 0;
            to = from+2*SIZE;
            break;
    }

    if (condition_for_invalid) 
        return -1;
    else
        return to;
}

char to_char(int n) {
    switch (n) {
        case -1:
            return 'X';
            break;
        case 0:
            return 'O';
            break;
        case 1:
            return 'N';
            break;
        default:
            return ' ';
            break;
    }
}

bool search(Board board, vector<Move>& moves) {
    for (int i=0; i<SIZE*SIZE; i++) {
        if (board.at(i) != 1)
            continue;

        int num_of_pieces = number_of_pieces(board);
        if (num_of_pieces == 1 && board.at((int)((SIZE*SIZE)/2)) == 1)
            return true;

        if (num_of_pieces == 1)
            return false;
        
        for (auto mv: possible_moves(board, i)) {
            moves.push_back(mv);
            if (search(make_move(board, mv), moves)) {
                return true;
            } else {
                moves.pop_back();
            }
        }
    }

    return false;
}

int main() {
    Board board = initialize_board();
    vector<Move> moves = vector<Move>();
    search(board, moves);

    for (auto mv: moves)
        cout << mv.to_string() << endl;

    if (!moves.size())
        cout << "Loser" << endl;

    return 0;
}

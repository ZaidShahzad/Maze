/*
MIT License

Copyright (c) 2023 Zaid Shahzad

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#ifndef MAZE_H
#define MAZE_H

#include <iostream>
#include <vector>
#include <bitset>

struct MazeCell {

public:
        std::vector<std::string> walls = {"1", "1", "1", "1"}; // North, South, East, West
        bool visited;
        std::pair<int, int> coordinate;

        void setVisited(bool isVisited) {
            this->visited = isVisited;
        }

        void removeWall(std::string direction) {
            if(direction == "North") {
                this->walls[0] = "0";
            }
            else if(direction == "South") {
                this->walls[1] = "0";
            }
            else if(direction == "East") {
                this->walls[2] = "0";
            }
            else if(direction == "West") {
                this->walls[3] = "0";
            }
        }

        int getWallNumber() {
            std::string bitstring;
            for(long unsigned int i = 0; i < this->walls.size(); i++) {
                bitstring += this->walls[i];
            }
            return std::stoi(bitstring, 0, 2);
        }

        MazeCell() {
            this->visited = false;
        }
};

struct NeighborLocation {
    MazeCell* mazeCell;
    std::string direction;

    NeighborLocation(MazeCell* mazeCell, std::string direction) {
        this->mazeCell = mazeCell;
        this->direction = direction;
    }
};

struct MazeRow {
    public:
        int rowNumber;
        std::vector<MazeCell*> mazeRowVector;

    MazeRow(int rowNumber) {
        this->rowNumber = rowNumber;
    }
};


class maze {

    private:
        int rows;
        int columns;
        MazeCell* currentCell = nullptr;
        std::vector<MazeRow*> mazeVector;

        MazeCell* getNeighborCellFromDirection(std::string direction);
        MazeCell* getCellFromCoords(int x, int y);

        std::vector<NeighborLocation*> getNeighborCellsNotVisited();

        bool isCoordinateOutOfBounds(int x, int y);
        bool isCellCoordinate(MazeCell* mazeCell, int x, int y);

        void setCurrentCell(MazeCell* newCurrentCell);
        void removeWall(MazeCell* neighborCell, NeighborLocation* location);

        void createMaze();

    public:
        void printMaze();
        void output(std::string fileName);
        void createRandomPath();

        maze(int rows, int columns);
};


#endif

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

#include <iostream>
#include "maze.h"
#include <fstream>

maze::maze(int rows, int columns) {
    this->rows = rows;
    this->columns = columns;
    createMaze();
}

bool maze::isCellCoordinate(MazeCell* mazeCell, int x, int y) {
    return mazeCell->coordinate.first == x && mazeCell->coordinate.second == y;
}


MazeCell* maze::getCellFromCoords(int x, int y) {
    return this->mazeVector[x]->mazeRowVector[y];
}
void maze::createMaze() {
    for(int rowNumber = 1; rowNumber <= this->rows; rowNumber++) {
        MazeRow* mazeRow = new MazeRow(rowNumber);
        for(int column = 1; column <= this->columns; column++) {
            MazeCell* mazeCell = new MazeCell();

            mazeCell->coordinate.first = (rowNumber - 1);
            mazeCell->coordinate.second = (column - 1);

            if(isCellCoordinate(mazeCell, 0, 0)) {
                this->currentCell = mazeCell;
            }

            mazeRow->mazeRowVector.push_back(mazeCell);
        }
        this->mazeVector.push_back(mazeRow);
    }

    MazeCell* entranceCell = getCellFromCoords(0, 0);
    MazeCell* exitCell = getCellFromCoords(this->rows - 1, this->columns - 1);

    entranceCell->setVisited(true);
    entranceCell->removeWall("North");
    exitCell->removeWall("South");
}

bool maze::isCoordinateOutOfBounds(int x, int y) {
    if(x < 0 || x >= this->rows) return true;
    if(y < 0 || y >= this->columns) return true;
    return false;
}

MazeCell* maze::getNeighborCellFromDirection(std::string direction) {
    int x = 0;
    int y = 0;
    if(direction == "North") {
        x = this->currentCell->coordinate.first - 1;
        y = this->currentCell->coordinate.second;
    }
    else if(direction == "South") {
        x = this->currentCell->coordinate.first + 1;
        y = this->currentCell->coordinate.second;
    }
    else if(direction == "East") {
        x = this->currentCell->coordinate.first;
        y = this->currentCell->coordinate.second + 1;
    }
    else if(direction == "West") {
        x = this->currentCell->coordinate.first;
        y = this->currentCell->coordinate.second - 1;
    }
    if(isCoordinateOutOfBounds(x, y)) {
        return nullptr;
    }
    return this->mazeVector[x]->mazeRowVector[y];
}

std::vector<NeighborLocation*> maze::getNeighborCellsNotVisited() {
    std::vector<NeighborLocation*> mazeCellsNotVisited;
    std::vector<std::string> directions = {"North", "South", "East", "West"};

    for(long unsigned int i = 0; i < directions.size(); i++) {
        MazeCell* neighborCell = getNeighborCellFromDirection(directions[i]);
        if(neighborCell == nullptr || neighborCell->visited == true) {
            continue;
        }
        NeighborLocation* location = new NeighborLocation(neighborCell, directions[i]);
        mazeCellsNotVisited.push_back(location);
    }

    return mazeCellsNotVisited;
}

void maze::setCurrentCell(MazeCell *newCurrentCell) {
    this->currentCell = newCurrentCell;
}

void maze::removeWall(MazeCell* neighborCell, NeighborLocation* location) {
    if(location->direction == "North") {
        this->currentCell->removeWall("North");
        neighborCell->removeWall("South");
    }
    else if (location->direction == "South"){
        this->currentCell->removeWall("South");
        neighborCell->removeWall("North");
    }
    else if (location->direction == "East"){
        this->currentCell->removeWall("East");
        neighborCell->removeWall("West");
    }
    else if (location->direction == "West") {
        this->currentCell->removeWall("West");
        neighborCell->removeWall("East");
    }
}

void maze::createRandomPath() {
    std::vector<MazeCell*> a;

    MazeCell* entranceCell = getCellFromCoords(0, 0);
    // No need to set visited to entrance cell because already done in createMaze() function so it would be kinda redundant
    a.push_back(entranceCell);

    while(!a.empty()) {
        setCurrentCell(a[a.size() - 1]);
        a.pop_back();

        std::vector<NeighborLocation*> neighbors = getNeighborCellsNotVisited();
        if(!neighbors.empty()) {
            a.push_back(this->currentCell);
            int index = std::rand() / ((RAND_MAX + 1u) / neighbors.size());

            MazeCell* randomChosenNeighbor = neighbors[index]->mazeCell;
            NeighborLocation* location = neighbors[index];

            removeWall(randomChosenNeighbor, location);

            randomChosenNeighbor->setVisited(true);
            a.push_back(randomChosenNeighbor);
        }
    }
}

void maze::output(std::string fileName) {
    std::ofstream file;
    file.open(fileName);
    for(long unsigned int rowNumber = 0; rowNumber < this->mazeVector.size(); rowNumber++) {
        MazeRow* mazeRow = this->mazeVector[rowNumber];
        for(long unsigned int columnNumber = 0; columnNumber < mazeRow->mazeRowVector.size(); columnNumber++) {
            MazeCell* mazeCell = mazeRow->mazeRowVector[columnNumber];
            file << mazeCell->getWallNumber() << " ";
        }
        file << std::endl;
    }
}
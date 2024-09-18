#include "led-matrix.h"

#include "stdlib/matrices.h"
#include "stdlib/panels.h"

#include "object.h"
#include "miscsymbols.h"
#include "blockgroup.h"
#include "util.h"
#include "grid.h"
#include "solver.h"
#include "randgrid.h"

using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

// Global Variables

Solver puzzleSolver = Solver();

RandGrid gridgen;

int dx[4] = {01, 00, -1, 00};
int dy[4] = {00, 01, 00, -1};

// Functions and shit

void render(Solver solver, Color bg, Color line, Color path);

// Animation

std::vector<std::vector<std::vector<bool>>> animation{grid0, grid1, grid2, grid3, grid4, grid5, grid6, grid7, grid8};
int blinkdelay = 0;
int blinkthreshold = 0;
static void anim_loop(int color) {
  std::vector<std::vector<int>> col(ROWS, std::vector<int>(COLS));
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) col[i][j] = color;
  }
  delay(100);
  // Serial.println(rand() % 1024);
  disp(col, grid0);

  blinkdelay++;
  if (blinkdelay >= blinkthreshold) {
    blinkdelay = 0;
    blinkthreshold = (int)(runif(16, 64));
    playAnimation(col, animation, 20, false);
    playAnimation(col, animation, 20, true);
  }
}

// Random grids

void randTests() {
  gridgen.pathfind();
  int index = gridgen.randint(7);
  Grid grid;
  if (index == 0) grid = (gridgen.randBlobs(9, 3, 2));
  else if (index == 1) grid = (gridgen.randMaze());
  else if (index == 2) grid = (gridgen.randStars());
  else if (index == 3) grid = (gridgen.randBlobs(12, 2, 0));
  else if (index == 4) grid = (gridgen.randTriangles(8, 0));
  else if (index == 5) grid = (gridgen.randBlocks(4, gridgen.randint(4)));
  else if (index == 6) grid = (gridgen.randDots(4 + gridgen.randint(4), gridgen.randint(8)));
  puzzleSolver.set(grid);
  render(puzzleSolver, BLACK, WHITE, GREEN);
  delay(1000);
  puzzleSolver.solve();
  if (puzzleSolver.solution.size() == 0) {
    std::cout << "NO SOLUTION :(\n";
    canvas->Fill(0, 0, 0);
  }
  else {
    render(puzzleSolver, BLACK, WHITE, GREEN);
  }
}

void randChallenge() {
  gridgen.pathfind();
  int index = gridgen.randint(7);
  Grid grid;
  if (index == 0) grid = (gridgen.randMaze()); // Panels 1 + 2
  else if (index == 1) grid = (gridgen.randBlobs(8, 2, 4 + gridgen.randint(4))); // Panel 3
  else if (index == 2) grid = (gridgen.randDots(2, 12 + gridgen.randint(4))); // Panel 4
  else if (index == 3) grid = (gridgen.randChallengeBlocks(4 + gridgen.randint(6))); // Blocks and stars panel
  else if (index == 4) grid = (gridgen.randChallengeStars(4 + gridgen.randint(6))); // Stars and dots
  else if (index == 5) grid = (gridgen.randBlobs(9, 3, 0));
  else if (index == 6) grid = (gridgen.randTriangles(6 + gridgen.randint(4), 0));
  puzzleSolver.set(grid);

  render(puzzleSolver, BLACK, WHITE, GREEN);
  delay(1000);
  puzzleSolver.solve();
  if (puzzleSolver.solution.size() == 0) {
    std::cout << "NO SOLUTION :(\n";
    canvas->Fill(0, 0, 0);
  }
  else {
    render(puzzleSolver, BLACK, WHITE, GREEN);
  }
}

void randFull() {
  gridgen.pathfind();
  int index = gridgen.randint(10);
  Grid grid;
  if (index == 0) grid = (gridgen.randMaze()); // Panels 1 + 2
  else if (index == 1) grid = (gridgen.randBlobs(8, 2, 4 + gridgen.randint(4))); // Panel 3
  else if (index == 2) grid = (gridgen.randDots(2, 12 + gridgen.randint(4))); // Panel 4
  else if (index == 3) grid = (gridgen.randChallengeBlocks(4 + gridgen.randint(6))); // Blocks and stars panel
  else if (index == 4) grid = (gridgen.randChallengeStars(4 + gridgen.randint(6))); // Stars and dots
  else if (index == 5) grid = (gridgen.randBlobs(9, 3, 0));
  else if (index == 6) grid = (gridgen.randTriangles(6 + gridgen.randint(4), 0));

  else if (index == 7) grid = (gridgen.randStars());
  else if (index == 8) grid = (gridgen.randBlocks(4, gridgen.randint(4)));
  else if (index == 9) grid = (gridgen.randDots(4 + gridgen.randint(4), gridgen.randint(8)));

  puzzleSolver.set(grid);

  render(puzzleSolver, BLACK, WHITE, GREEN);
  delay(1000);
  puzzleSolver.solve();
  if (puzzleSolver.solution.size() == 0) {
    std::cout << "NO SOLUTION :(\n";
    canvas->Fill(0, 0, 0);
  }
  else {
    render(puzzleSolver, BLACK, WHITE, GREEN);
  }
}

void miniChallenge() {
  std::vector<Grid> grids;
  grids.push_back(gridgen.randMaze());
  grids.push_back(gridgen.randBlobs(8, 2, 4 + gridgen.randint(4)));
  grids.push_back(gridgen.randDots(2, 12 + gridgen.randint(4)));
  grids.push_back(gridgen.randChallengeBlocks(4 + gridgen.randint(6)));
  grids.push_back(gridgen.randChallengeStars(4 + gridgen.randint(6)));
  grids.push_back(gridgen.randBlobs(9, 3, 0));
  grids.push_back(gridgen.randTriangles(6 + gridgen.randint(4), 0));

  for (auto grid : grids) {
    puzzleSolver.set(grid);
    render(puzzleSolver, BLACK, WHITE, GREEN);
    delay(1000);
    puzzleSolver.solve();
    if (puzzleSolver.solution.size() == 0) {
      std::cout << "NO SOLUTION :(\n";
      canvas->Fill(0, 0, 0);
    }
    else {
      render(puzzleSolver, BLACK, WHITE, GREEN);
    }
    delay(2000);
  }
}

// RENDER GRID

const double fade_scale = 1.0 / 3.0;

void render(Solver solver, Color bg, Color line, Color path) {
  solver.activate();
    
  if (solver.grid.n > 9 || solver.grid.m > 9) {
    canvas->Fill((bg>>16) % (1<<16), (bg>>8) % (1<<8), bg % (1<<8));
    return;
  }
  std::cout << "RENDERING!!!!" << "\n";

  const int rad = 3;

  int height = (solver.grid.n)>>1;
  int width = (solver.grid.m)>>1;

  std::cout << width << " " << height << "\n";

  int top = 15 - rad * height;
  int left = 20 - rad * width;

  canvas->Fill((bg>>16) % (1<<16), (bg>>8) % (1<<8), bg % (1<<8));

  for (int yayayaya = 0; yayayaya < 2; yayayaya++) {
  // std::cout << top << " " << left << "\n";
  // matrixpanel->fillRect(left, top, (rad<<1) * width + 2, (rad<<1) * height + 2, line);
  for (int i = 0; i <= height; i++) {
    drawLine(left, top + (rad<<1) * i, left + (rad<<1) * width, top + (rad<<1) * i, (line));
  }

  for (int i = 0; i <= width; i++) {
    drawLine(left + (rad<<1) * i, top, left + (rad<<1) * i, top + (rad<<1) * height, (line));
  }

  for (int i = 0; i < solver.grid.n; i++) { // y coordinate aka. vertical
    for (int j = 0; j < solver.grid.m; j++) { // x coordinate aka. horizontal
      int xcoord = left + rad * j;
      int ycoord = top + rad * i;
      std::shared_ptr<Object> o = solver.grid.board[i][j];
      bool mov = o->isPath;
      bool sol = o->isPathOccupied;
      if (!mov) {
        if (j % 2 == 1) drawLine(xcoord - 2, ycoord, xcoord + 2, ycoord, (bg));
        else if (i % 2 == 1) drawLine(xcoord, ycoord - 2, xcoord, ycoord + 2, (bg));
      }

      if (sol) {
        drawPixel(xcoord, ycoord, (path));
        if (j % 2 == 1) drawLine(xcoord - 2, ycoord, xcoord + 2, ycoord, (path));
        else if (i % 2 == 1) drawLine(xcoord, ycoord - 2, xcoord, ycoord + 2, (path));
      }
    }
  }

  // 2nd round

  for (int i = 0; i < solver.grid.n; i++) {
    for (int j = 0; j < solver.grid.m; j++) {
      int xcoord = left + rad * j;
      int ycoord = top + rad * i;
      std::shared_ptr<Object> o = solver.grid.board[i][j];
      bool mov = o->isPath;
      bool sol = o->isPathOccupied;
      if (isStartingPoint(o)) drawRect(xcoord - 1, ycoord - 1, 3, 3, sol ? path : line);
      if (isEndingPoint(o)) {
        uint32_t color = sol ? path : line;
        if (i == 0 && j == 0) drawLine(xcoord - 2, ycoord - 2, xcoord, ycoord, color);
        else if (i == solver.grid.n - 1 && j == 0) drawLine(xcoord - 2, ycoord + 2, xcoord, ycoord, color);
        else if (i == 0 && j == solver.grid.n - 1) drawLine(xcoord + 2, ycoord - 2, xcoord, ycoord, color);
        else if (i == solver.grid.n - 1 && j == solver.grid.m - 1) drawLine(xcoord + 2, ycoord + 2, xcoord, ycoord, color);
        
        else if (i == 0) drawLine(xcoord, ycoord - 2, xcoord, ycoord, color);
        else if (i == solver.grid.n - 1) drawLine(xcoord, ycoord + 2, xcoord, ycoord, color);
        else if (j == 0) drawLine(xcoord - 2, ycoord, xcoord, ycoord, color);
        else if (j == solver.grid.m - 1) drawLine(xcoord + 2, ycoord, xcoord, ycoord, color);
        
        else drawRect(xcoord - 2, ycoord - 2, 5, 5, sol ? path : line);
      }
    }
  }

  // 3rd round

  for (int i = 0; i < solver.grid.n; i++) {
    for (int j = 0; j < solver.grid.m; j++) {
      int xcoord = left + rad * j;
      int ycoord = top + rad * i;
      std::shared_ptr<Object> o = solver.grid.board[i][j];
      bool mov = o->isPath;
      bool sol = o->isPathOccupied;

      if (instanceof<Dot>(o)) drawPixel(xcoord, ycoord, RED);
      if (instanceof<Blob>(o)) fillRect(xcoord - 1, ycoord - 1, 3, 3, o->color);
      if (instanceof<Star>(o)) {
        drawLine(xcoord, ycoord - 1, xcoord, ycoord + 1, o->color);
        drawLine(xcoord + 1, ycoord, xcoord - 1, ycoord, o->color);
      }
      if (instanceof<Cancel>(o)) {
        drawPixel(xcoord, ycoord, WHITE);
        drawPixel(xcoord, ycoord - 1, WHITE);
        drawPixel(xcoord - 1, ycoord + 1, WHITE);
        drawPixel(xcoord + 1, ycoord + 1, WHITE);
      }

      if (instanceof<Triangle>(o)) {
        // Draw 4 dots to differentiate between what's not.
        for (int d = 0; d < 4; d++) drawPixel(xcoord + 2 * dx[d], ycoord + 2 * dy[d], scale(o->color, fade_scale));

        int num = (std::dynamic_pointer_cast<Triangle>(o))->x;
        if (num == 1) drawPixel(xcoord, ycoord, o->color);
        else if (num == 2) {
          drawPixel(xcoord - 1, ycoord, o->color);
          drawPixel(xcoord + 1, ycoord, o->color);
        }
        else if (num == 3) {
          drawPixel(xcoord - 1, ycoord + 1, o->color);
          drawPixel(xcoord + 1, ycoord + 1, o->color);
          drawPixel(xcoord, ycoord - 1, o->color);
        }
      }

      if (instanceof<BlockGroup>(o)) {  
        if (std::dynamic_pointer_cast<BlockGroup>(o)->n <= 0) continue;
        BlockGroup group = (std::dynamic_pointer_cast<BlockGroup>(o))->clone();
        group.normalize();
        std::set<pair<int, int>> blocks = group.pairs;
        bool orientation = group.oriented;
        std::pair<int, int> sides = group.boundingbox;

        drawRect(xcoord - 2, ycoord - 2, 5, 5, scale(o->color, fade_scale)); // draw a rect to differentiate

        if (!orientation) {
          for (int d = 0; d < 4; d++) drawPixel(xcoord + 2 * dx[d], ycoord + 2 * dy[d], bg);
        }
        for (auto i : blocks) drawPixel(xcoord - ((sides.first)>>1) + i.first, ycoord + ((sides.second)>>1) - i.second, o->color);
      }
    }
  }

    left += (127 - (left<<1) - (rad<<1) * width);
  }
  solver.deactivate();
}

// MAIN

const int MODE = 2;

int main(int argc, char *argv[]) {
  matrixsetup(argc, argv);

  if (canvas == NULL)
    return 1;

  while (!interrupt_received) {
    // EVERYTHING MEANINGFUL GOES HERE
    // anim_loop(0xFFFFFF);

    /*

    Grid g = gridgen.randStars();
    puzzleSolver.set(g);
    render(puzzleSolver, BLACK, WHITE, GREEN);
    delay(1000);

    */
    
    if (MODE == 0) randFull();
    else if (MODE == 1) randTests();
    else if (MODE == 2) randChallenge();
    else randTests(); // Default
  
    delay(2000);
  
  }

  // Animation finished. Shut down the RGB matrix.
  canvas->Clear();
  delete canvas;

  return 0;
}
#include <GLContext.h>
#include <random>

using namespace GLContext;

struct Cell {
    vec4 color = vec4(0, 0, 0, 0);
    bool empty = true;
    uvec2 pos = uvec2(0, 0);
    uvec2 neighbors[8];
};

const int gridWidth = 100;
const int gridHeigth = 100;
vector<vector<Cell>> grid(gridWidth, vector<Cell>(gridHeigth));


vec4 getRandomColor()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    float red = dis(gen);
    float green = dis(gen);
    float blue = dis(gen);
    float alpha = 1.0f;

    return vec4(red, green, blue, alpha);
}

void populateNeighbors(Cell& cell) {
    int x = cell.pos.x;
    int y = cell.pos.y;
    for (uvec2& c : cell.neighbors) {
        c = uvec2(-1,-1);
    }
    if(x - 1 > 0         && y + 1 < gridHeigth)cell.neighbors[0] = uvec2(x - 1, y + 1);
    if(                     y + 1 < gridHeigth)cell.neighbors[1] = uvec2(x    , y + 1);
    if(x + 1 < gridWidth && y + 1 < gridHeigth)cell.neighbors[2] = uvec2(x + 1, y + 1);
    if(x + 1 < gridWidth                      )cell.neighbors[3] = uvec2(x + 1, y    );
    if(x - 1 > 0                              )cell.neighbors[7] = uvec2(x - 1, y    );
    if(x + 1 < gridWidth && y - 1 > 0         )cell.neighbors[4] = uvec2(x + 1, y - 1);
    if(                     y - 1 > 0         )cell.neighbors[5] = uvec2(x    , y - 1);
    if(x - 1 > 0         && y - 1 > 0         )cell.neighbors[6] = uvec2(x - 1, y - 1);
}


void populateGrid() {
    for (int i = 0; i < gridWidth; i++) {
        for (int u = 0; u < gridHeigth; u++) {
            grid[i][u].color = getRandomColor();
            grid[i][u].pos = uvec2(i, u);
            populateNeighbors(grid[i][u]);
        }
    }
}

vector<Cell> getNeighbors(Cell& cell) {
    vector<Cell> neighbors;
    for (uvec2 n : cell.neighbors) {
        if (n.x != -1) {
            neighbors.push_back(grid[n.x][n.y]);
        }
    }
    return neighbors;
}


void drawCell(Cell& cell) {
    float x = ((float)cell.pos.x / (float)gridWidth) * 2.0f - 1.0f + (1.0f / (float)gridWidth);
    float y = ((float)cell.pos.y / (float)gridHeigth) * 2.0f - 1.0f + (1.0f / (float)gridHeigth);
    drawPoint(vec2(x, y), 10.0f, cell.color);
}

void drawGrid() {
    for (int i = 0; i < gridWidth; i++) {
        for (int u = 0; u < gridHeigth; u++) {
            drawCell(grid[i][u]);
        }
    }
}

void drawNeighbors(Cell& cell) {
    for (Cell& c : getNeighbors(cell)) {
        drawCell(c);
    }
}



void init() {
    populateGrid();
}

void draw() {
    drawNeighbors(grid[9][0]);
}




int WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, char* szCmdLine, int iCmdShow) {
    GLContext::window_name = "Huegene";
    initialize = init;
    onDraw = draw;
    GLContext::init(1200, 1200);
    return 0;
}



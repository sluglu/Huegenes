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
    float alpha = 1.0f; // You can modify this if you want random alpha values as well

    return vec4(red, green, blue, alpha);
}

void getNeighbors(Cell& cell) {
    int x = cell.pos.x;
    int y = cell.pos.y;
    for (uvec2& c : cell.neighbors) {
        c = uvec2(-1,-1);
    }

    if (y < gridHeigth) {
        cell.neighbors[0] = uvec2(x - 1, y + 1);
        cell.neighbors[1] = uvec2(x, y + 1);
        cell.neighbors[2] = uvec2(x + 1, y + 1);
    }

    if (x < gridWidth) {
        cell.neighbors[3] = uvec2(x + 1, y);
    }
    
    if (x > 0) {
        cell.neighbors[7] = uvec2(x - 1, y);
    }

    if (y > 0) {
        cell.neighbors[4] = uvec2(x + 1, y - 1);
        cell.neighbors[5] = uvec2(x, y - 1);
        cell.neighbors[6] = uvec2(x - 1, y - 1);
    } 
}


void populateGrid(vector<vector<Cell>>& grid) {
    for (int i = 0; i < gridWidth; i++) {
        for (int u = 0; u < gridHeigth; u++) {
            grid[i][u].color = getRandomColor();
            grid[i][u].pos = uvec2(i, u);
            getNeighbors(grid[i][u]);
        }
    }
}

void drawGrid(vector<vector<Cell>>& grid) {
    for (int i = 0; i < gridWidth; i++) {
        for (int u = 0; u < gridHeigth; u++) {
            float x = ((float)grid[i][u].pos.x / (float)gridWidth) * 2.0f - 1.0f + (1.0f/ (float)gridWidth);
            float y = ((float)grid[i][u].pos.y / (float)gridHeigth) * 2.0f - 1.0f + (1.0f / (float)gridHeigth);
            drawPoint(vec2(x,y), 10.0f, grid[i][u].color);
        }
    }
}


void init() {
    populateGrid(grid);
}

void draw() {
    drawGrid(grid);
}




int __stdcall WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, char* szCmdLine, int iCmdShow) {
    GLContext::window_name = "Huegene";
    initialize = init;
    onDraw = draw;
    GLContext::init(1200, 1200);
    return 0;
}



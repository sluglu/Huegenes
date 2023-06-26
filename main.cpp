#include <GLContext.h>
#include <random>

using namespace GLContext;


bool pause = false;

float maxMut = 10.0f;
float minMut = -10.0f;
std::vector<ivec2> startingPoints = { vec2(150, 150) };
int gridSize = 300;

string message = " ";
float pointSize = 3.5f;

struct Cell {
    vec4 color = vec4(0, 0, 0, 0);
    bool empty = true;
    uvec2 pos = uvec2(0, 0);
    uvec2 neighbors[8];
};


vector<vector<Cell>> grid(gridSize, vector<Cell>(gridSize));


// Function to mutate the hue of an RGBA color
vec4 mutateHue(vec4 color) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(minMut, maxMut);
    float hueMutation = dis(gen);
    // Convert RGB to HSV
    float r = color.x;
    float g = color.y;
    float b = color.z;
    float maxVal = std::max(std::max(r, g), b);
    float minVal = std::min(std::min(r, g), b);
    float delta = maxVal - minVal;

    float h, s, v;
    v = maxVal;

    if (maxVal != 0.0f) {
        s = delta / maxVal;
        if (r == maxVal) {
            h = (g - b) / delta;
        }
        else if (g == maxVal) {
            h = 2.0f + (b - r) / delta;
        }
        else {
            h = 4.0f + (r - g) / delta;
        }

        h *= 60.0f;
        if (h < 0.0f) {
            h += 360.0f;
        }
    }
    else {
        h = 0.0f;
        s = 0.0f;
    }

    // Mutate the hue
    h += hueMutation;
    if (h > 360.0f) {
        h -= 360.0f;
    }

    // Convert HSV back to RGB
    float c = v * s;
    float x = c * (1 - std::abs(fmod(h / 60.0f, 2) - 1));
    float m = v - c;

    if (h >= 0.0f && h < 60.0f) {
        r = c + m;
        g = x + m;
        b = m;
    }
    else if (h >= 60.0f && h < 120.0f) {
        r = x + m;
        g = c + m;
        b = m;
    }
    else if (h >= 120.0f && h < 180.0f) {
        r = m;
        g = c + m;
        b = x + m;
    }
    else if (h >= 180.0f && h < 240.0f) {
        r = m;
        g = x + m;
        b = c + m;
    }
    else if (h >= 240.0f && h < 300.0f) {
        r = x + m;
        g = m;
        b = c + m;
    }
    else if (h >= 300.0f && h <= 360.0f) {
        r = c + m;
        g = m;
        b = x + m;
    }
    return vec4(r, g, b, color.w);
}

uvec2 getRandomElement(vector<uvec2>& vec) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, vec.size() - 1);
    return vec[dis(gen)];
}

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
    if(x - 1 > 0         && y + 1 < gridSize)cell.neighbors[0] = uvec2(x - 1, y + 1);
    if(                     y + 1 < gridSize)cell.neighbors[1] = uvec2(x    , y + 1);
    if(x + 1 < gridSize && y + 1 < gridSize)cell.neighbors[2] = uvec2(x + 1, y + 1);
    if(x + 1 < gridSize                      )cell.neighbors[3] = uvec2(x + 1, y    );
    if(x - 1 > 0                              )cell.neighbors[7] = uvec2(x - 1, y    );
    if(x + 1 < gridSize && y - 1 > 0         )cell.neighbors[4] = uvec2(x + 1, y - 1);
    if(                     y - 1 > 0         )cell.neighbors[5] = uvec2(x    , y - 1);
    if(x - 1 > 0         && y - 1 > 0         )cell.neighbors[6] = uvec2(x - 1, y - 1);
}


void populateGrid() {
    grid = vector<vector<Cell>>(gridSize, vector<Cell>(gridSize));
    pointSize = ceil((float)GLContext::SCR_WIDTH / (float)gridSize);
    for (int i = 0; i < gridSize; i++) {
        for (int u = 0; u < gridSize; u++) {
            grid[i][u].pos = uvec2(i, u);
            grid[i][u].empty = true;
            grid[i][u].color = vec4(0, 0, 0, 0);
            populateNeighbors(grid[i][u]);
        }
    }
    for (ivec2 p : startingPoints) {
        grid[p.x][p.y].empty = false;
        grid[p.x][p.y].color = getRandomColor();
    }
}

vector<uvec2> getEmptyNeighbors(Cell& cell) {
    vector<uvec2> emptyNeighbors;
    for (uvec2& c : cell.neighbors) {
        if (c.x != -1) {
            if (grid[c.x][c.y].empty) {
                emptyNeighbors.push_back(c);
            }
        }
    }
    return emptyNeighbors;
}

void drawCell(Cell& cell) {
    float x = ((float)cell.pos.x / gridSize) * 2.0f - 1.0f + (1.0f / (float)gridSize);
    float y = ((float)cell.pos.y / gridSize) * 2.0f - 1.0f + (1.0f / (float)gridSize);
    drawPoint(vec2(x, y), pointSize, cell.color);
}

void propagation() {
    for (int i = 0; i < gridSize; i++) {
        for (int u = 0; u < gridSize; u++) {
            if (!grid[i][u].empty) {
                vector<uvec2>emptyNeighbors = getEmptyNeighbors(grid[i][u]);
                if (emptyNeighbors.size() > 0) {
                    uvec2 pos = getRandomElement(emptyNeighbors);
                    grid[pos.x][pos.y].empty = false;
                    grid[pos.x][pos.y].color = mutateHue(grid[i][u].color);
                }
            }
        }
    }
}

void drawGrid() {
    for (int i = 0; i < gridSize; i++) {
        for (int u = 0; u < gridSize; u++) {
            drawCell(grid[i][u]);
        }
    }
}

void init() {
    populateGrid();
    
}
void draw() {
    if (!pause) {
        propagation();
    }
    drawGrid();
}

void drawStartingPointsList()
{
    ImGui::Text("starting points");
    for (int i = 0; i < startingPoints.size(); i++)
    {
        ImGui::PushID(i);
        ImGui::InputInt2(("point " + to_string(i)).c_str(), &startingPoints[i].x);
        ImGui::PopID();
    }
    if (ImGui::Button("Remove")){startingPoints.pop_back();}
    if (ImGui::Button("Add point")){startingPoints.push_back(uvec2());}
}

//starting colors
//starting points (mouse)



void ui() {
    ImGui::Begin("parameters");
    drawStartingPointsList();
    if (ImGui::InputInt("grid size", &gridSize)) {
        populateGrid();
    }
    ImGui::InputFloat("max mutation", &maxMut);
    ImGui::InputFloat("min mutation", &minMut);
    ImGui::Checkbox("pause", &pause);
    if (ImGui::Button("Reset")) { populateGrid(); }
    if (ImGui::Button("take screenshot")) { message = TakeScreenshot(); }
    ImGui::Text(message.c_str());

    ImGui::End();
    
}



int WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, char* szCmdLine, int iCmdShow) {
    GLContext::window_name = "Huegene";
    initialize = init;
    onDraw = draw;
    onDrawUI = ui;
    GLContext::init(1200, 1000);
    return 0;
}

//TODO : clamp parameters
//TODO : optimise
//TODO : many starting point with mouse
//TODO : bug growing rate (bottom right)



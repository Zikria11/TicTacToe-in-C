#include <raylib.h>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

using namespace std;


Color primaryX = { 255, 75, 85, 255 };      
Color secondaryX = { 255, 120, 130, 255 };  
Color primaryO = { 75, 150, 255, 255 };     
Color secondaryO = { 120, 180, 255, 255 };  
Color backgroundColor = { 20, 25, 40, 255 };
Color gridColor = { 80, 90, 120, 255 };    
Color accentColor = { 255, 215, 0, 255 };  

struct CellAnimation {
    float fadeIn = 0.0f;
    float scale = 0.0f;
    float rotation = 0.0f;
    float bounce = 0.0f;
    bool isAnimating = false;
};

struct ParticleEffect {
    Vector2 position;
    Vector2 velocity;
    float life;
    float maxLife;
    Color color;
    float size;
};

struct WinLineAnimation {
    Vector2 start;
    Vector2 end;
    float progress = 0.0f;
    bool active = false;
    float glowIntensity = 0.0f;
};

int main() {
    const int screenWidth = 800;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Tic-Tac-Toe");
    SetTargetFPS(60);

    vector<vector<char>> board(3, vector<char>(3, ' '));
    vector<vector<CellAnimation>> cellAnims(3, vector<CellAnimation>(3));
    vector<ParticleEffect> particles;
    WinLineAnimation winLine;

    bool playerX = true;
    bool gameOver = false;
    string result = "";
    float resultScale = 1.0f;
    float resultTimer = 0.0f;
    float backgroundPulse = 0.0f;
    float gridGlow = 0.0f;
    int winRow = -1, winCol = -1, winType = -1; 
    const float gridSize = 480.0f;
    const float cellSize = gridSize / 3.0f;
    const Vector2 gridOffset = { (screenWidth - gridSize) / 2.0f, (screenHeight - gridSize) / 2.0f };

    int hoverRow = -1, hoverCol = -1;
    float hoverAlpha = 0.0f;

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        backgroundPulse += deltaTime;

        if (!gameOver) {
            Vector2 mouse = GetMousePosition();
            int newHoverRow = (mouse.y - gridOffset.y) / cellSize;
            int newHoverCol = (mouse.x - gridOffset.x) / cellSize;

            if (newHoverRow >= 0 && newHoverRow < 3 && newHoverCol >= 0 && newHoverCol < 3 &&
                board[newHoverRow][newHoverCol] == ' ') {
                hoverRow = newHoverRow;
                hoverCol = newHoverCol;
                hoverAlpha = min(hoverAlpha + deltaTime * 3.0f, 0.3f);
            }
            else {
                hoverAlpha = max(hoverAlpha - deltaTime * 5.0f, 0.0f);
                if (hoverAlpha <= 0.0f) {
                    hoverRow = hoverCol = -1;
                }
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                int row = (mouse.y - gridOffset.y) / cellSize;
                int col = (mouse.x - gridOffset.x) / cellSize;
                if (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == ' ') {
                    board[row][col] = playerX ? 'X' : 'O';
                    cellAnims[row][col].isAnimating = true;
                    cellAnims[row][col].fadeIn = 0.0f;
                    cellAnims[row][col].scale = 0.0f;
                    cellAnims[row][col].rotation = 0.0f;
                    cellAnims[row][col].bounce = 0.0f;

                    for (int p = 0; p < 15; p++) {
                        ParticleEffect particle;
                        particle.position = { gridOffset.x + col * cellSize + cellSize / 2.0f,
                                            gridOffset.y + row * cellSize + cellSize / 2.0f };
                        float angle = (float)p / 15.0f * 2.0f * PI;
                        float speed = 50.0f + (rand() % 100);
                        particle.velocity = { cosf(angle) * speed, sinf(angle) * speed };
                        particle.maxLife = particle.life = 1.0f + (rand() % 100) / 100.0f;
                        particle.color = playerX ? primaryX : primaryO;
                        particle.size = 3.0f + (rand() % 5);
                        particles.push_back(particle);
                    }

                    playerX = !playerX;
                }
            }

            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (cellAnims[i][j].isAnimating) {
                        cellAnims[i][j].fadeIn += deltaTime * 3.0f;
                        cellAnims[i][j].scale += deltaTime * 4.0f;
                        cellAnims[i][j].rotation += deltaTime * 720.0f; 
                        cellAnims[i][j].bounce = sinf(cellAnims[i][j].fadeIn * 8.0f) * 0.1f;

                        if (cellAnims[i][j].fadeIn >= 1.0f) {
                            cellAnims[i][j].fadeIn = 1.0f;
                            cellAnims[i][j].scale = 1.0f;
                            cellAnims[i][j].rotation = 0.0f;
                            cellAnims[i][j].bounce = 0.0f;
                            cellAnims[i][j].isAnimating = false;
                        }
                    }
                }
            }

            for (int i = 0; i < 3; i++) {
                if (board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
                    gameOver = true;
                    result = string(1, board[i][0]) + " Wins!";
                    winRow = i; winType = 0;
                    winLine.start = { gridOffset.x, gridOffset.y + i * cellSize + cellSize / 2.0f };
                    winLine.end = { gridOffset.x + gridSize, gridOffset.y + i * cellSize + cellSize / 2.0f };
                    winLine.active = true;
                }
            }
            for (int j = 0; j < 3; j++) {
                if (board[0][j] != ' ' && board[0][j] == board[1][j] && board[1][j] == board[2][j]) {
                    gameOver = true;
                    result = string(1, board[0][j]) + " Wins! ";
                    winCol = j; winType = 1;
                    winLine.start = { gridOffset.x + j * cellSize + cellSize / 2.0f, gridOffset.y };
                    winLine.end = { gridOffset.x + j * cellSize + cellSize / 2.0f, gridOffset.y + gridSize };
                    winLine.active = true;
                }
            }
            if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
                gameOver = true;
                result = string(1, board[0][0]) + " Wins! ";
                winType = 2;
                winLine.start = { gridOffset.x, gridOffset.y };
                winLine.end = { gridOffset.x + gridSize, gridOffset.y + gridSize };
                winLine.active = true;
            }
            if (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
                gameOver = true;
                result = string(1, board[0][2]) + " Wins! ";
                winType = 3;
                winLine.start = { gridOffset.x + gridSize, gridOffset.y };
                winLine.end = { gridOffset.x, gridOffset.y + gridSize };
                winLine.active = true;
            }

            bool full = true;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (board[i][j] == ' ') full = false;
                }
            }
            if (full && !gameOver) {
                gameOver = true;
                result = "It's a Draw! ";
            }
        }
        else {
            resultTimer += deltaTime;
            resultScale = 1.0f + 0.15f * sinf(resultTimer * 3.0f);

            if (winLine.active) {
                winLine.progress = min(winLine.progress + deltaTime * 2.0f, 1.0f);
                winLine.glowIntensity = 0.5f + 0.5f * sinf(resultTimer * 6.0f);
            }

            if (result.find("Wins") != string::npos && (int)(resultTimer * 10) % 3 == 0) {
                for (int p = 0; p < 3; p++) {
                    ParticleEffect particle;
                    particle.position = { (float)(rand() % screenWidth), (float)(rand() % screenHeight) };
                    particle.velocity = { (rand() % 200) - 100.0f, (rand() % 200) - 100.0f };
                    particle.maxLife = particle.life = 2.0f;
                    particle.color = accentColor;
                    particle.size = 4.0f + (rand() % 6);
                    particles.push_back(particle);
                }
            }

            if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                board = vector<vector<char>>(3, vector<char>(3, ' '));
                cellAnims = vector<vector<CellAnimation>>(3, vector<CellAnimation>(3));
                particles.clear();
                winLine = WinLineAnimation();
                playerX = true;
                gameOver = false;
                result = "";
                resultTimer = 0.0f;
                winRow = winCol = winType = -1;
            }
        }

        for (auto it = particles.begin(); it != particles.end();) {
            it->life -= deltaTime;
            it->position.x += it->velocity.x * deltaTime;
            it->position.y += it->velocity.y * deltaTime;
            it->velocity.y += 200.0f * deltaTime;
            it->size *= 0.98f; 

            if (it->life <= 0.0f || it->size < 0.5f) {
                it = particles.erase(it);
            }
            else {
                ++it;
            }
        }

        gridGlow = 0.3f + 0.2f * sinf(backgroundPulse * 2.0f);

        BeginDrawing();

        Color bgTop = { 25, 30, 50, 255 };
        Color bgBottom = { 15, 20, 35, 255 };
        DrawRectangleGradientV(0, 0, screenWidth, screenHeight, bgTop, bgBottom);

        for (int i = 0; i < 5; i++) {
            float x = (float)(screenWidth * i / 5) + sinf(backgroundPulse + i) * 30.0f;
            float y = (float)(screenHeight * i / 5) + cosf(backgroundPulse * 0.7f + i) * 20.0f;
            DrawCircle(x, y, 3.0f, Fade(accentColor, 0.1f));
        }

        Color glowColor = Fade(gridColor, gridGlow);

        DrawRectangleLinesEx({ gridOffset.x + 4, gridOffset.y + 4, gridSize, gridSize }, 6.0f,
            Fade(BLACK, 0.3f));

        DrawRectangleLinesEx({ gridOffset.x, gridOffset.y, gridSize, gridSize }, 6.0f, glowColor);

        for (int i = 1; i < 3; i++) {
            float x = gridOffset.x + i * cellSize;
            float y = gridOffset.y + i * cellSize;

            DrawLineEx({ x + 2, gridOffset.y + 2 }, { x + 2, gridOffset.y + gridSize + 2 },
                6.0f, Fade(BLACK, 0.3f));
            DrawLineEx({ gridOffset.x + 2, y + 2 }, { gridOffset.x + gridSize + 2, y + 2 },
                6.0f, Fade(BLACK, 0.3f)); 

            DrawLineEx({ x, gridOffset.y }, { x, gridOffset.y + gridSize }, 6.0f, glowColor);
            DrawLineEx({ gridOffset.x, y }, { gridOffset.x + gridSize, y }, 6.0f, glowColor);
        }

        if (hoverRow >= 0 && hoverCol >= 0 && hoverAlpha > 0.0f) {
            float x = gridOffset.x + hoverCol * cellSize;
            float y = gridOffset.y + hoverRow * cellSize;
            Color hoverColor = playerX ? Fade(primaryX, hoverAlpha * 0.3f) : Fade(primaryO, hoverAlpha * 0.3f);
            DrawRectangle(x + 5, y + 5, cellSize - 10, cellSize - 10, hoverColor);

            float centerX = x + cellSize / 2.0f;
            float centerY = y + cellSize / 2.0f;
            float previewSize = cellSize / 6.0f;
            Color previewColor = Fade(playerX ? primaryX : primaryO, hoverAlpha * 0.6f);

            if (playerX) {
                DrawLineEx({ centerX - previewSize, centerY - previewSize },
                    { centerX + previewSize, centerY + previewSize }, 4.0f, previewColor);
                DrawLineEx({ centerX + previewSize, centerY - previewSize },
                    { centerX - previewSize, centerY + previewSize }, 4.0f, previewColor);
            }
            else {
                DrawCircleLines(centerX, centerY, previewSize, previewColor);
            }
        }

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] != ' ') {
                    float x = gridOffset.x + j * cellSize + cellSize / 2.0f;
                    float y = gridOffset.y + i * cellSize + cellSize / 2.0f;

                    CellAnimation& anim = cellAnims[i][j];
                    float alpha = anim.fadeIn;
                    float scale = anim.isAnimating ? anim.scale : 1.0f;
                    float rotation = anim.rotation;
                    float bounce = anim.bounce;

                    Color mainColor = (board[i][j] == 'X') ? primaryX : primaryO;
                    Color shadowColor = Fade(BLACK, alpha * 0.5f);

                    if (board[i][j] == 'X') {
                        float size = (cellSize / 3.5f) * scale;
                        Vector2 center = { x, y + bounce * 10.0f };

                        Vector2 shadowOffset = { 3.0f, 3.0f };
                        float cos_r = cosf(rotation * DEG2RAD);
                        float sin_r = sinf(rotation * DEG2RAD);

                        Vector2 p1 = { center.x + shadowOffset.x - size * cos_r + size * sin_r,
                                     center.y + shadowOffset.y - size * sin_r - size * cos_r };
                        Vector2 p2 = { center.x + shadowOffset.x + size * cos_r - size * sin_r,
                                     center.y + shadowOffset.y + size * sin_r + size * cos_r };
                        Vector2 p3 = { center.x + shadowOffset.x + size * cos_r + size * sin_r,
                                     center.y + shadowOffset.y + size * sin_r - size * cos_r };
                        Vector2 p4 = { center.x + shadowOffset.x - size * cos_r - size * sin_r,
                                     center.y + shadowOffset.y - size * sin_r + size * cos_r };

                        DrawLineEx(p1, p2, 8.0f, shadowColor);
                        DrawLineEx(p3, p4, 8.0f, shadowColor);

                        p1 = { center.x - size * cos_r + size * sin_r, center.y - size * sin_r - size * cos_r };
                        p2 = { center.x + size * cos_r - size * sin_r, center.y + size * sin_r + size * cos_r };
                        p3 = { center.x + size * cos_r + size * sin_r, center.y + size * sin_r - size * cos_r };
                        p4 = { center.x - size * cos_r - size * sin_r, center.y - size * sin_r + size * cos_r };

                        DrawLineEx(p1, p2, 8.0f, Fade(mainColor, alpha));
                        DrawLineEx(p3, p4, 8.0f, Fade(mainColor, alpha));

                        DrawLineEx(p1, p2, 4.0f, Fade(secondaryX, alpha * 0.8f));
                        DrawLineEx(p3, p4, 4.0f, Fade(secondaryX, alpha * 0.8f));
                    }
                    else {
                        float radius = (cellSize / 3.5f) * scale;
                        Vector2 center = { x, y + bounce * 10.0f };

                        DrawCircle(center.x + 3, center.y + 3, radius, shadowColor);

                        DrawCircle(center.x, center.y, radius, Fade(mainColor, alpha * 0.3f));
                        DrawCircleLines(center.x, center.y, radius, Fade(mainColor, alpha));
                        DrawCircleLines(center.x, center.y, radius - 4.0f, Fade(secondaryO, alpha * 0.8f));
                        DrawCircleLines(center.x, center.y, radius + 2.0f, Fade(mainColor, alpha * 0.5f));
                    }
                }
            }
        }

        if (winLine.active && winLine.progress > 0.0f) {
            Vector2 currentEnd = {
                winLine.start.x + (winLine.end.x - winLine.start.x) * winLine.progress,
                winLine.start.y + (winLine.end.y - winLine.start.y) * winLine.progress
            };

            Color glowColor = Fade(accentColor, winLine.glowIntensity);
            DrawLineEx(winLine.start, currentEnd, 12.0f, Fade(BLACK, 0.3f));
            DrawLineEx(winLine.start, currentEnd, 8.0f, glowColor);
            DrawLineEx(winLine.start, currentEnd, 4.0f, WHITE);
        }

        for (const auto& particle : particles) {
            float lifeRatio = particle.life / particle.maxLife;
            Color particleColor = Fade(particle.color, lifeRatio);
            DrawCircle(particle.position.x, particle.position.y, particle.size * lifeRatio, particleColor);
        }

        if (gameOver) {
            DrawRectangle(0, screenHeight / 2 - 80, screenWidth, 160, Fade(backgroundColor, 0.9f));
            DrawRectangleLinesEx({ 20, screenHeight / 2 - 80, screenWidth - 40, 160 }, 3.0f,
                Fade(accentColor, 0.8f));

            int textSize = 48 * resultScale;
            int textWidth = MeasureText(result.c_str(), textSize);
            Color resultColor = result.find("Draw") != string::npos ? accentColor :
                (result[0] == 'X' ? primaryX : primaryO);

            DrawText(result.c_str(), screenWidth / 2 - textWidth / 2 + 3,
                screenHeight / 2 - textSize / 2 + 3, textSize, Fade(BLACK, 0.5f));

            DrawText(result.c_str(), screenWidth / 2 - textWidth / 2,
                screenHeight / 2 - textSize / 2, textSize, resultColor);

            float pulseAlpha = 0.7f + 0.3f * sinf(resultTimer * 4.0f);
            string restartText = "Press SPACE or click to play again";
            int restartWidth = MeasureText(restartText.c_str(), 24);
            DrawText(restartText.c_str(), screenWidth / 2 - restartWidth / 2,
                screenHeight / 2 + 40, 24, Fade(WHITE, pulseAlpha));
        }
        else {
            string turnText = "Current Player: ";
            Color turnColor = playerX ? primaryX : primaryO;

            DrawRectangle(20, 20, 300, 60, Fade(backgroundColor, 0.8f));
            DrawRectangleLinesEx({ 20, 20, 300, 60 }, 2.0f, Fade(turnColor, 0.6f));

            DrawText(turnText.c_str(), 30, 30, 24, WHITE);

            float symbolX = 190.0f + sinf(backgroundPulse * 3.0f) * 2.0f;
            float symbolY = 50.0f;
            float symbolSize = 20.0f + sinf(backgroundPulse * 4.0f) * 2.0f;

            if (playerX) {
                DrawLineEx({ symbolX - symbolSize / 2, symbolY - symbolSize / 2 },
                    { symbolX + symbolSize / 2, symbolY + symbolSize / 2 }, 4.0f, turnColor);
                DrawLineEx({ symbolX + symbolSize / 2, symbolY - symbolSize / 2 },
                    { symbolX - symbolSize / 2, symbolY + symbolSize / 2 }, 4.0f, turnColor);
            }
            else {
                DrawCircleLines(symbolX, symbolY, symbolSize / 2, turnColor);
                DrawCircle(symbolX, symbolY, symbolSize / 2 - 3, Fade(turnColor, 0.2f));
            }
        }

        string title = "Tic-Tac-Toe";
        int titleSize = 36;
        int titleWidth = MeasureText(title.c_str(), titleSize);
        float titleGlow = 0.8f + 0.2f * sinf(backgroundPulse * 2.5f);

        DrawText(title.c_str(), screenWidth / 2 - titleWidth / 2 + 2,
            screenHeight - 80 + 2, titleSize, Fade(BLACK, 0.4f));
        DrawText(title.c_str(), screenWidth / 2 - titleWidth / 2,
            screenHeight - 80, titleSize, Fade(accentColor, titleGlow));

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
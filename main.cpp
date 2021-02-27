#include <raylib.h>
#include <iostream>
#include <sstream>

int Width = 1280;
int Height = 720;

struct GameObject {
    Vector2 Position;

    virtual void Update(){};
    virtual void Render(){};
};

struct Paddle : public GameObject {

    // Input
    int KeyUp, KeyDown;
    Rectangle Shape = {Position.x, Position.y, 30, 160};

    float Speed = 7.0f;
    int Score = 0;

    Paddle(int keyUp, int keyDown, Vector2 position) {
        KeyUp = keyUp;
        KeyDown = keyDown;
        Position = position;
    }

    void Update() override {
        Shape.x = Position.x;
        Shape.y = Position.y;

        if (IsKeyDown(KeyUp) && Position.y >= 0)
            Position.y -= Speed;
        if (IsKeyDown(KeyDown) && Position.y + Shape.height <= (float) Height)
            Position.y += Speed;
    }

    void Render() override {
        DrawRectangle((int) Shape.x, (int) Shape.y, (int) Shape.width, (int) Shape.height, WHITE);
    }
};

struct Ball : public GameObject {
    Vector2 Velocity;
    Rectangle Shape = {Position.x, Position.y, 30, 30};

    Ball(Vector2 velocity){
        Position = {(float) Width / 2, (float) Height / 2};
        Velocity = velocity;
    }

    void Update() override {
        Position.x += Velocity.x;
        Position.y += Velocity.y;

        Shape.x = Position.x;
        Shape.y = Position.y;

        if (Position.y + Shape.height >= (float) Height || Position.y <= 0) Velocity.y = -Velocity.y;
    }

    void Render() override {
        DrawRectangle((int) Shape.x, (int) Shape.y, (int) Shape.width, (int) Shape.height, WHITE);
    }
};

void MakeGoal(Ball& ball, Paddle& player) {
    player.Score++;
    ball.Position = {(float) Width/2, (float) Height/2};
    ball.Velocity = {-ball.Velocity.x, -ball.Velocity.y};
}

void ResetRound(Paddle& player1, Paddle& player2) {
    player1.Position.y = (float) Height/2 - (player1.Shape.height/2);
    player2.Position.y = (float) Height/2 - (player2.Shape.height/2);
}

int main() {
    auto player1 = Paddle(KEY_W, KEY_S, {0, 0});
    auto player2 = Paddle(KEY_UP, KEY_DOWN, {1280 - 30, 0});
    auto ball = Ball({7, -7});

    InitWindow(Width, Height, "Pong");
    SetTargetFPS(60);

    ResetRound(player1, player2);

    for (int count = 0; !WindowShouldClose(); count++){
        BeginDrawing();
        ClearBackground(BLACK);

        // Update objects
        player1.Update();
        player2.Update();
        ball.Update();

        // Render objects
        player1.Render();
        player2.Render();
        ball.Render();

        // Check for collision for the ball
        if ((CheckCollisionRecs(ball.Shape, player1.Shape) && ball.Velocity.x < 0) ||
            CheckCollisionRecs(ball.Shape, player2.Shape) && ball.Velocity.x > 0)
            ball.Velocity.x = -ball.Velocity.x;

        // Check for goals
        if (ball.Position.x < -10) {
            MakeGoal(ball,player1);
            ResetRound(player1, player2);
        }
        if (ball.Position.x > (float) Width) {
            MakeGoal(ball, player2);
            ResetRound(player1, player2);
        }

        // Draw score
        auto score = std::stringstream();
        score<<player2.Score<<" - "<<player1.Score;
        DrawText(score.str().c_str(), Width/2 - 86, 0, 64, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

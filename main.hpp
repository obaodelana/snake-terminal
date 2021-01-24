#include <cstdlib> // srand(), rand()
#include <vector>
#include <ncurses.h>

const char gameStr[] = "HappyBdayLase"; // Snake body
const int moveDelay = 70, maxColors = 7;

// Game states
enum class State : int
{
    Win = 0,
    Lose = 1,
    Play = 2
};

typedef struct Vector2
{
    unsigned int x, y;
    
    Vector2()
    {
        x = 0;
        y = 0;
    }

    Vector2(int xx, int yy)
    {
        x = xx;
        y = yy;
    }
} Vector2;

// Setup '==' operator for Vector2
bool operator==(const Vector2 &left, const Vector2 &right)
{
    // Check if the two vectors are the same
    return (left.x == right.x) && (left.y == right.y);
}

typedef struct Snake
{
    std::vector<Vector2> tailPos;
    std::vector<int> tailColorID;
    int keyPressed;

    // Empty constructor
    Snake()
    {
        // Clear list if not empty
        if (!tailPos.empty())
        {
            tailPos.clear();
            tailColorID.clear();
        }

        // Add a tail
        tailPos.push_back((Vector2) {});
        // Add a tail color
        tailColorID.push_back(rand() % maxColors);
        // Start moving to the right
        keyPressed = KEY_RIGHT;
    }
} Snake;

void MoveSnake(int key);
void PlaceFood(void);
void DrawFood(void);
void RestartGame(void);

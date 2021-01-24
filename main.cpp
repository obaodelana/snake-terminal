#include "main.hpp"
#include <string>
#include <ctime> // time()
// For delay
#include <thread>
#include <chrono>

Snake snake = {};
Vector2 foodPos = {};

State gameState = State::Play;

// Character index in <gameStr> string
int foodChrIndex = 0;

int main()
{
    initscr();
    noecho(); // Don't show user input
    cbreak(); // Make input immediately available to program
    keypad(stdscr, true); // Allow to use arrow keys
    nodelay(stdscr, true); // Don't stop when asked for input
    curs_set(0); // disable cursor
    
    // Use terminal colors
    use_default_colors();
    // Enable color mode
    start_color();
    // Create color pairs (-1 denotes default background)
    init_pair(static_cast<int>(State::Win), COLOR_GREEN, -1);
    init_pair(static_cast<int>(State::Lose), COLOR_RED, -1);
    init_pair(2, COLOR_BLUE, -1);
    init_pair(3, COLOR_YELLOW, -1);
    init_pair(4, COLOR_WHITE, -1);
    init_pair(5, COLOR_CYAN, -1);
    init_pair(6, COLOR_MAGENTA, -1);

    // Seed random number generator
    srand(time(0));

    PlaceFood();

    int key;
    std::string gameStateStr = "";
    // Game loop (quit when 'q', 'Q' or ESC is pressed)
    while(key != 'q' && key != 'Q' && key != 27)
    {
        // Get key pressed
        key = getch();

        clear(); // Clear screen
            
            switch(gameState)
            {
                case State::Play:
                    MoveSnake(key);
                    DrawFood();
                break;

                default:
                    gameStateStr = (gameState == State::Win) ? "Happy Birthday Lase!" : "Game Over";
                    gameStateStr.append(" (r) or (q)");
                    // Make text bold and add color
                    attron(A_BOLD | COLOR_PAIR(static_cast<int>(gameState)));
                        // Print game state
                        mvprintw(getmaxy(stdscr) / 2, (getmaxx(stdscr) - gameStateStr.length()) / 2, "%s", gameStateStr.c_str());
                    attroff(A_BOLD | COLOR_PAIR(static_cast<int>(gameState)));

                    // If r is pressed restart game
                    if (key == 'r' || key == 'R')
                        RestartGame();
                break;
            }

        refresh(); // Draw new stuff

        // Sleep for <moveDelay>
        std::this_thread::sleep_for(std::chrono::milliseconds(moveDelay));
    }

    endwin();
    return EXIT_SUCCESS;
}

void DrawSnake(void)
{
    // Draw each tail
    for (int i = 0; i < snake.tailPos.size(); i++)
    {
        // Add color to snake tail
        attron(COLOR_PAIR(snake.tailColorID[i]));
            mvaddch(snake.tailPos[i].y, snake.tailPos[i].x, gameStr[i]);
        attroff(COLOR_PAIR(snake.tailColorID[i]));
    }
}

void CheckTailCollision(void)
{
    // Check if a tail is colliding with another
    bool tailCollided = false;
    // Go through all tails
    for (int i = 0; i < snake.tailPos.size(); i++)
    {
        // Compare current tail with other tails
        for (int j = 0; j < snake.tailPos.size(); j++)
        {
            // If the same tail, skip
            if (i == j) continue;
            // If there's a collision
            else if (snake.tailPos[i] == snake.tailPos[j])
            {
                tailCollided = true;
                break;
            }
        }

        // If there's a collision, it's game over
        if (tailCollided)
        {
            gameState = State::Lose;
            break;
        }
    }
}

void MoveSnake(int key)
{
    // If snake head collides with food
    if (snake.tailPos[0] == foodPos)
    {
        // Add new tail
        snake.tailPos.push_back((Vector2) {});
        // Seed random number generator
        // Set tail color
        snake.tailColorID.push_back(rand() % maxColors);
        // Place food in new position
        PlaceFood();
    }

    // Shift positions back starting from last index
    for (int i = snake.tailPos.size() - 1; i > 0; i--)
        snake.tailPos[i] = snake.tailPos[i - 1];
    
    // If arrow key pressed
    if (key >= KEY_DOWN && key <= KEY_RIGHT)
        // Save key
        snake.keyPressed = key;
    
    // Perform action based on key 
    switch(snake.keyPressed)
    {
        case KEY_DOWN:
            // Moving down is going up on grid
            snake.tailPos[0].y++;
            break;

        case KEY_UP:
            // Moving up is going down on grid
            snake.tailPos[0].y--;
            break;

        case KEY_RIGHT:
            // Move snake 1 to the right
            snake.tailPos[0].x++;
            break;

        case KEY_LEFT:
            // Move snake 1 to the left
            snake.tailPos[0].x--;
            break;
    }

    // If you go out of the screen
    if (snake.tailPos[0].x < 0 || snake.tailPos[0].y < 0 || snake.tailPos[0].x >= getmaxx(stdscr) || snake.tailPos[0].y >= getmaxy(stdscr))
        // Game Over
        gameState = State::Lose;

    // Check if tails are touching each other when there are three tails
    if (snake.tailPos.size() >= 3)
        CheckTailCollision();

    DrawSnake();
}

void DrawFood(void)
{   
    // Color yellow
    attron(COLOR_PAIR(3));
        // Place food at food pos
        mvaddch(foodPos.y, foodPos.x, gameStr[foodChrIndex]);
    attroff(COLOR_PAIR(3));
}

void PlaceFood(void)
{
    foodChrIndex++;
    // Set random food pos
    foodPos = {(rand() % (getmaxx(stdscr) - 2)) + 1, (rand() % (getmaxy(stdscr) - 2) + 1)};

    // If at the end of string, game has been won
    if (gameStr[foodChrIndex] == '\0')
    {
        gameState = State::Win;
        // Don't show null character
        foodChrIndex--;
    }
}

void RestartGame(void)
{
    // Reset snake
    snake = {};
    
    // Replace food
    foodChrIndex = 0;
    PlaceFood();

    // Make game playable
    gameState = State::Play;
}
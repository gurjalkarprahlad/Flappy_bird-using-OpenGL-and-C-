#include <iostream>
#include <cmath>
#include <sstream>
#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int BIRD_SIZE = 30;
const int PIPE_WIDTH = 100;
const int PIPE_HEIGHT = 200;
 // Increased gap between pipes
const int PIPE_GAP = 250;
const float GRAVITY = 0.3f;
const float JUMP_FORCE = 6.0f;
const float HORIZONTAL_SPEED = 2.0f;
//bird properties
struct Bird {
    float x;
    float y;
    float velocity;
    Bird(float xPos, float yPos)
        : x(xPos), y(yPos), velocity(0.0f) {}
};
//Pipes for collision
struct Pipe {
    float x;
    float y;
    Pipe(float xPos, float yPos)
        : x(xPos), y(yPos) {}
};
Bird bird(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
Pipe pipe(WINDOW_WIDTH, 0);
bool gameOver = false;
int score = 0;

// Function to draw the bird on the screen
void drawBird() {
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    glRectf(bird.x, bird.y, bird.x + BIRD_SIZE, bird.y + BIRD_SIZE);
}
// Function to draw a pipe on the screen
void drawPipe(float x, float y) {
    glColor3f(0.0f, 1.0f, 0.0f); // Green color
    glRectf(x, y, x + PIPE_WIDTH, y + PIPE_HEIGHT);
    glRectf(x, y + PIPE_HEIGHT + PIPE_GAP, x + PIPE_WIDTH, WINDOW_HEIGHT);
}
// Function to draw the ground on the screen
void drawGround() {
    glColor3f(0.6f, 0.4f, 0.2f); // Brown color
    glRectf(0, 0, WINDOW_WIDTH, 50);
}
// Function to draw the score on the screen
void drawScore() {
    glColor3f(1.0f, 1.0f, 1.0f); // White color
    glRasterPos2f(10, WINDOW_HEIGHT - 30);
    std::stringstream ss;
    ss << "Score: " << score;
    std::string scoreStr = ss.str();
    for (unsigned int i = 0; i < scoreStr.length(); ++i) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, scoreStr[i]);
    }
}
// Function called to update the display
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (!gameOver) {
        drawBird();
        drawPipe(pipe.x, pipe.y);
        drawGround();
        drawScore();
    }
    glutSwapBuffers();
}
// Function called to update the game state
void update(int value) {
    if (!gameOver) {
        // Update bird position and velocity based on gravity
        bird.y -= bird.velocity;
        bird.velocity -= GRAVITY;
        // Update pipe position
        pipe.x -= HORIZONTAL_SPEED;
        // Collision detection
        if (bird.x + BIRD_SIZE >= pipe.x && bird.x <= pipe.x + PIPE_WIDTH) {
            if (bird.y <= pipe.y + PIPE_HEIGHT || bird.y + BIRD_SIZE >= pipe.y + PIPE_HEIGHT + PIPE_GAP) {
                gameOver = true;
            }
        }
        // Check if bird hits the ground or ceiling
        if (bird.y <= 0 || bird.y >= WINDOW_HEIGHT - BIRD_SIZE - 50) {
            gameOver = true;
        }
        // Check if bird passes the pipe
        if (bird.x > pipe.x + PIPE_WIDTH) {
            pipe.x = WINDOW_WIDTH;
            pipe.y = rand() % (WINDOW_HEIGHT - PIPE_GAP - PIPE_HEIGHT - 50);
            score++;
        }    }
    glutPostRedisplay();
    glutTimerFunc(10, update, 0);
}
// Function called when a key is pressed
void handleKeypress(unsigned char key, int x, int y) {
    if (key == 32 && !gameOver) { // Spacebar key
        bird.velocity = JUMP_FORCE;
    }
}
// Function to initialize OpenGL settings
void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black color
    glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);
}
// Main function
int main(int argc, char** argv) {
    srand(static_cast<unsigned int>(time(0)));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Flappy Bird");
    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0);
    glutKeyboardFunc(handleKeypress);
    init();
    glutMainLoop();
    return 0;
}

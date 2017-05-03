/* 
 * Author: tapin13
 *
 * Created on May 2, 2017, 11:26 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

#include <GL/glew.h>
#include <GL/glut.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define CIRCLE_POINTS 12

struct point {
    float x, y;
};

struct square {
    struct point left_buttom;
    struct point left_top;
    struct point right_top;
    struct point right_buttom;
    float position;
    float step;
};

struct ball {
    float x, y;
    float radius;
    float step_x;
    float step_y;
    int lives;
    struct point points[CIRCLE_POINTS];
};

struct ball gameBall = { 
    0.0, 0.0, // x, y
    0.05, // radius
    0.01, // step_x
    0.01, // step_y
    3 // lives
};

struct square bat = {
    -0.99, 0.0, // left_buttom
    -0.99, 0.2, // left_top
    -0.95, 0.2, // right_top
    -0.95, 0.0, // right_buttom
    0.0, // position
    0.08 // step
};

int score = 0;

int init_resources() {
    return 1;
}

void drawScore() {
    glRasterPos2f(-0.2, 0.0);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'S');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'c');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'o');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'r');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'e');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ' ');
    
    char txt[3];
    sprintf(txt, "%d", score);
    int i;
    for(i = 0; i < strlen(txt); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, txt[i]);
    }
}

void drawLives() {
    glRasterPos2f(-0.2, -0.1);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'L');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'i');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'v');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'e');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 's');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, ' ');
    
    char txt[3];
    sprintf(txt, "%d", gameBall.lives);
    int i;
    for(i = 0; i < strlen(txt); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, txt[i]);
    }
}

void gameOver() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glColor3d(1.0, 0.0, 0.0);
    
    glBegin(GL_LINES);
        glVertex2f(-1.0, 1.0);
        glVertex2f(0.99, -0.99);
    glEnd();
    
    glBegin(GL_LINES);
        glVertex2f(1.0, 1.0);
        glVertex2f(-1.0, -1.0);
    glEnd();
    
    glutSwapBuffers();
    
    sleep(1);
}

void updateGame() {
    if(gameBall.lives == 0) {
        gameOver();
        glutLeaveMainLoop();
    }

    // Ball hit the wall
    if(gameBall.x > (1.0 - gameBall.radius)) { // || gameBall.x < (-1.0 + gameBall.radius)
        gameBall.step_x *= -1;
    }
    
    if(gameBall.y > (0.97 - gameBall.radius) || gameBall.y <= (-0.97 + gameBall.radius)) {
        gameBall.step_y *= -1;
    }
    
    // Ball out of table
    if(gameBall.x < (-1.0 + gameBall.radius)) {
        gameBall.lives--;
        if(gameBall.lives > 0) {
            // set default values
            gameBall.x = 0.0;
            gameBall.y = 0.0;
            gameBall.step_x = 0.01;
            gameBall.step_y = 0.01;
            sleep(3);
        }
    }
    
    // Ball hit the bat
    if(bat.right_top.x >= (gameBall.x - gameBall.radius) 
            && (bat.right_top.y + bat.position) >= (gameBall.y)
            && (bat.right_buttom.y + bat.position) <= (gameBall.y)
            ) {
        gameBall.step_x *= -1;
        
        // speed up the ball
        gameBall.step_x += 0.001;
        gameBall.step_y += 0.001;
        
        score++;
    }

    gameBall.x += gameBall.step_x;
    gameBall.y += gameBall.step_y;
    
    glutPostRedisplay();
}

int frame=0,time,timebase=0;

void displayGame() {
    /*
    frame++;
    time=glutGet(GLUT_ELAPSED_TIME);
    if (time - timebase > 1000) {
        printf("FPS:%4.2f\n",
        frame*1000.0/(time-timebase));
        timebase = time;
        frame = 0;
    }
    */
    
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glColor3d(0.0, 0.0, 0.0);

    glBegin(GL_LINE_STRIP);
        glVertex2f(-0.99, -0.99);
        glVertex2f(-0.99, 0.99);
        glVertex2f(0.99, 0.99);
        glVertex2f(0.99, -0.99);
        glVertex2f(-0.99, -0.99);
    glEnd();
    
    glPushMatrix();

    glTranslatef(0.0, bat.position, 0.0);
    
    glBegin(GL_LINE_STRIP);
        glVertex2f(bat.left_buttom.x   , bat.left_buttom.y);
        glVertex2f(bat.left_top.x      , bat.left_top.y);
        glVertex2f(bat.right_top.x     , bat.right_top.y);
        glVertex2f(bat.right_buttom.x  , bat.right_buttom.y);
        glVertex2f(bat.left_buttom.x   , bat.left_buttom.y);
    glEnd();

    glPopMatrix();
    

    // Draw ball
    
    glColor3d(1.0, 0.0, 0.0);
    
    glBegin(GL_LINE_STRIP);
        glVertex2f(bat.right_top.x  , (bat.right_top.y + bat.position));
        glVertex2f(bat.right_top.x  , (bat.right_buttom.y + bat.position));
    glEnd();

    glColor3d(0.0, 0.0, 0.0);
    
    glPushMatrix();
    glTranslatef(gameBall.x, gameBall.y, 0.0);

    // to make right circle not ellipse on window not 1:1 proportion
    int window_width = glutGet(GLUT_WINDOW_WIDTH);
    int window_height = glutGet(GLUT_WINDOW_HEIGHT);
    glScalef((float)window_height/window_width, 1.0, 0.0);
    
    glBegin(GL_POLYGON);
    int point;
    for(point = 0; point < CIRCLE_POINTS; point++) {
        glVertex2f(gameBall.points[point].x, gameBall.points[point].y);
    }
    glEnd();
    
    glPopMatrix();
    
    drawScore();
    drawLives();
    
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case 'q':
            if((bat.position + bat.step + bat.left_top.y) <= 0.98) {
                bat.position += bat.step;
            } else if ((bat.position + 0.01 + bat.left_top.y) < 0.98) {
                bat.position += 0.01;
            }
            break;
        case 'a':
            if((bat.position - bat.step) >= -0.98) {
                bat.position -= bat.step;
            } else if ((bat.position - 0.01) > -0.98) {
                bat.position -= 0.01;
            }
            break;
    }
}

void free_resources() {
    
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Hit the ball");
    glutKeyboardFunc(keyboard);
            
    GLenum glew_status = glewInit();
    if(glew_status != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
        return (EXIT_FAILURE);
    }

    // Fill ball array
    const float angle = 2.1 * 3.1416 / CIRCLE_POINTS;
    float drawAngle = 0.0;
    int point;
    for(point = 0; point < CIRCLE_POINTS; point++) {
        gameBall.points[point].x = gameBall.radius * cos(drawAngle);
        gameBall.points[point].y = gameBall.radius * sin(drawAngle);
        drawAngle += angle;
    }

    // register callbacks
    glutDisplayFunc(displayGame);
    //glutReshapeFunc(updateGame);
    glutIdleFunc(updateGame);
    //glutSpecialFunc(keyboard);
    
    //glutFullScreen();
    
    if(init_resources() == 1) {
        glutMainLoop();
    }
    
    free_resources();
    return (EXIT_SUCCESS);
}


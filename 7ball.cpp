#include <GL/glut.h>
#include <GL/glu.h>
#include <cstdlib> 
#include <ctime> 
#include <math.h> 
#include <vector>
#include <GL/freeglut.h>
#include <iostream>
#include <utility>
#include<unistd.h>

//Compile with g++ 7ball.cpp -lGL -lGLU -lglut -lm

GLfloat angle, fAspect;
GLfloat getRandomNumber(GLfloat min, GLfloat max);

int nivel = 1;

int width = 1000, height = 600;
int viewport1_width = 600, viewport1_height = 300;
int viewport2_width = 200, viewport2_height = 200;
int viewport3_width = 200, viewport3_height = 200;
int viewport4_width = 200, viewport4_height = 200;

GLfloat speed;

GLfloat ClickxFront; 
GLfloat ClickyFront;
GLfloat ClickxTop;
GLfloat ClickyTop;
GLfloat ClickxRight;
GLfloat ClickyRight;

GLfloat deltaX;
GLfloat deltaY;
GLfloat deltaZ;
GLfloat directionX;
GLfloat directionY;
GLfloat directionZ;
GLfloat magnitude;

struct sphereData {
    GLfloat x;
    GLfloat y;
    GLfloat z;
};

struct Vector3D {
    float x, y, z;
};

std::vector<sphereData> sphereData(7);

bool collisionWithSphere2 = false;
bool collisionWithSphere3 = false;

GLfloat sphereRadius = 0.2f;
GLint sphereSlices = 30;
GLint sphereStacks = 30;

GLfloat cubeWidth = 2.0f;
GLfloat cubeHeight = 2.0f;
GLfloat cubeDepth = 2.0f;

GLfloat minX = -cubeWidth / 2.0f + sphereRadius;
GLfloat maxX = cubeWidth / 2.0f - sphereRadius;
GLfloat minY = -cubeHeight / 2.0f + sphereRadius;
GLfloat maxY = cubeHeight / 2.0f - sphereRadius;
GLfloat minZ = -cubeDepth / 2.0f + sphereRadius;
GLfloat maxZ = cubeDepth / 2.0f - sphereRadius;

void Draw();
void drawCube();
void Initialize();
void drawSphere();
void MouseCallback();
void drawViewport1();
void drawViewport2();
void drawViewport3();
void drawViewport4();
void ViewParameters();
void drawText(const char* text, int x, int y);
void KeyboardCallback(unsigned char key, int x, int y);
bool isSphereOutsideCube(GLfloat sphereX, GLfloat sphereY, GLfloat sphereZ);
bool isSphereCollidingEachOther(int index, GLfloat x1, GLfloat y1, GLfloat z1);


GLfloat cubeVertices[] = {
     // Front face
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    // Back face
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    // Top face
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
    // Bottom face
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    // Right face
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    // Left face
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f
};

GLfloat cubeColors[] = {
    // Front face
    1.0f, 0.0f, 0.0f, 0.75f,
    1.0f, 0.0f, 0.0f, 0.75f,
    1.0f, 0.0f, 0.0f, 0.75f,
    1.0f, 0.0f, 0.0f, 0.75f,
    // Back face
    1.0f, 0.0f, 0.0f, 0.75f,
    1.0f, 0.0f, 0.0f, 0.75f,
    1.0f, 0.0f, 0.0f, 0.75f,
    1.0f, 0.0f, 0.0f, 0.75f,
    // Top face
    1.0f, 0.0f, 0.0f, 0.75f,
    1.0f, 0.0f, 0.0f, 0.75f,
    1.0f, 0.0f, 0.0f, 0.75f,
    1.0f, 0.0f, 0.0f, 0.75f,
    // Bottom face
    1.0f, 0.0f, 0.0f, 0.75f,
    1.0f, 0.0f, 0.0f, 0.75f,
    1.0f, 0.0f, 0.0f, 0.75f,
    1.0f, 0.0f, 0.0f, 0.75f,
    // Right face
    1.0f, 0.0f, 0.0f, 0.75f,
    1.0f, 0.0f, 0.0f, 0.75f,
    1.0f, 0.0f, 0.0f, 0.75f,
    1.0f, 0.0f, 0.0f, 0.75f,
    // Left face
    1.0f, 0.0f, 0.0f, 0.75f,
    1.0f, 0.0f, 0.0f, 0.75f,
    1.0f, 0.0f, 0.0f, 0.75f,
    1.0f, 0.0f, 0.0f, 0.75f,
};

GLubyte cubeIndices[] = {
    // Front face
    0, 1, 2,
    2, 3, 0,
    // Back face
    4, 5, 6,
    6, 7, 4,
    // Top face
    8, 9, 10,
    10, 11, 8,
    // Bottom face
    12, 13, 14,
    14, 15, 12,
    // Right face
    16, 17, 18,
    18, 19, 16,
    // Left face
    20, 21, 22,
    22, 23, 20
};

GLfloat getRandomNumber(GLfloat min, GLfloat max) {
    return min + static_cast <GLfloat> (rand()) / (static_cast <GLfloat> (RAND_MAX / (max - min)));
}

void drawText(const char* text, int x, int y) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glRasterPos2i(x, y);

    const char* c;
    for (c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void ViewParameters(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluPerspective(45, fAspect, 0.1,500);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt(0,80,200, 0,0,0, 0,1,0);
}

void MouseCallback(int button, int state, int x, int y) {

    if (x >= 200 && x < 400 && y >= 300 && y < 500) {
        ClickxFront = ((x-200)/100.0)-1; 
        ClickyFront = -((y-300)/100.0) +1;
        
    }
    else if (x >= 400 && x < 600 && y >= 300 && y < 500) {
        ClickxRight = ((x-400)/100.0)-1;
        ClickyRight = -((y-300)/100.0) +1;
    }
    else if (x >= 600 && x < 800 && y >= 300 && y < 500) {
        ClickxTop = ((x-600)/100.0)-1;
        ClickyTop = -((y-300)/100.0) + 1;
    }
}

bool SphereColision(int index, GLfloat x1, GLfloat y1, GLfloat z1, GLfloat income_speed) {
    
    income_speed /= 2;

    for (int i = 0; i < 7; i++) {

        GLfloat newX = sphereData[i].x;
        GLfloat newY = sphereData[i].y;
        GLfloat newZ = sphereData[i].z;
        GLfloat distance;

        if (!isSphereCollidingEachOther(i, newX, newY, newZ)) {
            distance = sqrt(pow(newX - x1, 2) + pow(newY - y1, 2) + pow(newZ - z1, 2));
        }

        GLfloat minDistance = 2 * sphereRadius;

        if (i == index) 
            continue;
        else if (distance <= minDistance) {
            GLfloat directionX = (newX - x1) / distance;
            GLfloat directionY = (newY - y1) / distance;
            GLfloat directionZ = (newZ - z1) / distance;

            while (!isSphereOutsideCube(newX, newY, newZ) && income_speed > 0){
                newX +=  income_speed * directionX;
                newY +=  income_speed * directionY;
                newZ += income_speed * directionZ;

                if (newX < minX || newX > maxX) {
                    directionX *= -1; 
                    newX = sphereData[i].x + income_speed * directionX;
                    }

                if (newY < minY || newY > maxY) {
                    directionY *= -1; 
                    newY = sphereData[i].y + income_speed * directionY;
                }

                if (newZ < minZ || newZ > maxZ) {
                    directionZ *= -1; 
                    newZ = sphereData[i].z + income_speed * directionZ;
                }

                if (isSphereCollidingEachOther(i, newX, newY, newZ)) {
                    SphereColision(i, newX, newY, newZ, income_speed);
                } else {
                    sphereData[i].x = newX;
                    sphereData[i].y = newY;
                    sphereData[i].z = newZ;
                }       


                income_speed -= 0.001;
                usleep(1000);
                Draw();
            }

            if (!isSphereOutsideCube(newX, newY, newZ)) {
                sphereData[i].x = newX;
                sphereData[i].y = newY;
                sphereData[i].z = newZ;

                if (i == 2)
                    collisionWithSphere2 = true;
                else if (i == 1)
                    collisionWithSphere3 = true;
            }

            return true;
        }
    }

    return false;
}

void Shoot() {

    Vector3D directionXY;
    Vector3D directionXZ;
    Vector3D directionZY;

    directionXY.x = ClickxFront - ClickxRight;
    directionXY.y = ClickyFront - ClickyRight;
    directionXY.z = 0;

    directionZY.x = 0;
    directionZY.y = -ClickyTop  + ClickyRight;
    directionZY.z = -ClickxTop  -ClickxRight;

    directionXZ.x = ClickxFront + ClickxTop;
    directionXZ.y = 0;
    directionXZ.z = -ClickyFront - ClickyTop;

    GLfloat a = directionXY.x + directionXZ.x;
    GLfloat b = directionXY.y + directionZY.y;
    GLfloat c = directionXZ.z + directionZY.z;

    GLfloat deltaX = a - sphereData[0].x;
    GLfloat deltaY = b - sphereData[0].y;
    GLfloat deltaZ = c - sphereData[0].z;
    GLfloat magnitudeXY = sqrt(deltaX * deltaX + deltaY * deltaY);
    GLfloat magnitudeXZ = sqrt(deltaX * deltaX + deltaZ * deltaZ);

    GLfloat directionX = 0;
    GLfloat directionY = 0;
    GLfloat directionZ = 0;


    if (ClickxRight == 0 && ClickyRight == 0 && ClickxTop == 0 && ClickyTop == 0) {
        directionX = deltaX / magnitudeXY;
        directionY = deltaY / magnitudeXY;
        directionZ = 0;
    } else if (ClickxFront == 0 && ClickyFront == 0 && ClickxTop == 0 && ClickyTop == 0){
        directionX = 0;
        directionY = deltaY / magnitudeXY;
        directionZ = deltaZ / magnitudeXZ;
    } else if (ClickxFront == 0 && ClickyFront == 0 && ClickxRight == 0 && ClickyRight == 0){
        directionX = deltaX / magnitudeXY;;
        directionY = 0;
        directionZ = deltaZ / magnitudeXZ;
    } else {
        directionX = deltaX / magnitudeXY;
        directionY = deltaY / magnitudeXY;
        directionZ = deltaZ / magnitudeXZ;

    }

    GLfloat newX = sphereData[0].x;
    GLfloat newY = sphereData[0].y;
    GLfloat newZ = sphereData[0].z;
    GLfloat currentSpeed = speed;

    while (!isSphereOutsideCube(newX, newY, newZ) && currentSpeed > 0) {
        newX += currentSpeed * directionX;
        newY += currentSpeed * directionY;
        newZ += currentSpeed * directionZ;

        if (newX < minX || newX > maxX) {
            directionX *= -1;
            newX = sphereData[0].x + currentSpeed * directionX;
        }

        if (newY < minY || newY > maxY) {
            directionY *= -1; 
            newY = sphereData[0].y + currentSpeed * directionY;
        }

        if (newZ < minZ || newZ > maxZ) {
            directionZ *= -1;
            newZ = sphereData[0].z + currentSpeed * directionZ;
        }

        if (SphereColision(0, newX, newY, newZ, currentSpeed)) {
            directionX *= -1;
            directionY *= -1;
            directionZ *= -1;
            newX = sphereData[0].x + currentSpeed * directionX;
            newY = sphereData[0].y + currentSpeed * directionY;
            newZ = sphereData[0].z + currentSpeed * directionZ;
        }

        sphereData[0].x = newX;
        sphereData[0].y = newY;
        sphereData[0].z = newZ;

        currentSpeed -= 0.002; 
        
        Draw();
        usleep(1000);

    }

    ClickxFront = 0;
    ClickyFront = 0;
    ClickxRight = 0;
    ClickyRight = 0;
    ClickxTop   = 0;
    ClickyTop   = 0;

    if (collisionWithSphere2 && collisionWithSphere3){
        nivel++;

        printf("Pontuação: %d\n", nivel);
        collisionWithSphere3 =false;
        collisionWithSphere2 = false;


        if(nivel == 4){
            nivel = 1;
        }

        Initialize();
    }

    if (collisionWithSphere2) {
        std::cout << "Esfera vermelha atingida" << std::endl;
        collisionWithSphere2 = false;
    }

    if (collisionWithSphere3) {
        std::cout << "Esfera azul atingida" << std::endl;
        collisionWithSphere3 = false;
    }

}

void KeyboardCallback(unsigned char key, int x, int y){
    if(key == 'a')
        angle += 5;
    
    else if(key == 's')
        angle -= 5;

    else if(key == '1')
        speed = 0.05;

    else if(key == '2')
        speed = 0.1;

    else if(key == '3')
        speed = 0.15;
    
    else if(key == 13)
        Shoot();
    
    else if (key == 27)
        exit(0);
    
    else if (key == ' '){
        nivel = 1;
        Initialize();
    } 

    ViewParameters();
    glutPostRedisplay();
}   

void Initialize(){

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    angle = 0.0;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    srand(time(0));
   
    for (int i = 0; i < 7; i++) {
        do {
            sphereData[i].x = getRandomNumber(minX, maxX);
            sphereData[i].y = getRandomNumber(minY, maxY);
            sphereData[i].z = getRandomNumber(minZ, maxZ);
        } while (isSphereOutsideCube(sphereData[i].x, sphereData[i].y, sphereData[i].z) || isSphereCollidingEachOther(i, sphereData[i].x, sphereData[i].y, sphereData[i].z));
    }

    glutMouseFunc(MouseCallback);
    glutKeyboardFunc(KeyboardCallback);
}


bool isSphereOutsideCube(GLfloat sphereX, GLfloat sphereY, GLfloat sphereZ) {
    return sphereX < -cubeWidth / 2.0f || sphereX > cubeWidth / 2.0f ||
           sphereY < -cubeHeight / 2.0f || sphereY > cubeHeight / 2.0f ||
           sphereZ < -cubeDepth / 2.0f || sphereZ > cubeDepth / 2.0f;
}

bool isSphereCollidingEachOther(int index, GLfloat x1, GLfloat y1, GLfloat z1) {
    bool check = false;
    for(int i = 0; i < 7; ++i){
    
        if(index == i){
            continue;
        } else {
            GLfloat dx = sphereData[i].x - x1;
            GLfloat dy = sphereData[i].y - y1;
            GLfloat dz = sphereData[i].z - z1;
            GLfloat distance = sqrt(dx * dx + dy * dy + dz * dz);

            if(distance < 0.4f){
                check = true;
            }
        }
    }
        return check;
}

void drawSphere() {
    
    GLfloat colors[4][4] = {
        {1.0f, 1.0f, 1.0f, 0.75f},   // Branco
        {0.0f, 0.0f, 1.0f, 0.75f},   // Azul
        {1.0f, 0.0f, 0.0f, 0.75f},   // Vermelho
        {0.0f, 0.0f, 0.0f, 0.75f}    // Preto
    };

    minX = -cubeWidth / 2.0f + sphereRadius;
    maxX = cubeWidth / 2.0f - sphereRadius;
    minY = -cubeHeight / 2.0f + sphereRadius;
    maxY = cubeHeight / 2.0f - sphereRadius;
    minZ = -cubeDepth / 2.0f + sphereRadius;
    maxZ = cubeDepth / 2.0f - sphereRadius;

    int numSpheres = 7;
    int num[7];
    num[0] = 0;
    num[1] = 1;
    num[2] = 2;
    num[3] = 3;
    num[4] = 3;
    num[5] = 3;
    num[6] = 3;
    
    for (int i = 0; i < 7; i++){
        glColor4fv(colors[num[i]]);
        glPushMatrix();
        glTranslatef(sphereData[i].x, sphereData[i].y, sphereData[i].z);
        glutSolidSphere(sphereRadius, sphereSlices, sphereStacks);
        glPopMatrix();
    }
}

void drawCube() {

    glRotatef(angle, 1, 1, 0);
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, cubeVertices);
    glColorPointer(3, GL_FLOAT, 0, cubeColors);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, cubeIndices);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

void drawViewport1() {
    glViewport(300, 300, viewport1_width, viewport1_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)viewport1_width / (float)viewport1_height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(3.0f, 3.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    drawCube();
    drawSphere();
}

void drawViewport2() {
    glViewport(200, 100, viewport2_width, viewport2_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)viewport2_width / (float)viewport2_height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    drawCube();
    drawSphere();

    glColor3f(0.0f, 0.0f, 0.0f);
    drawText("Frente", 10, height - viewport2_height  + 150);
}

void drawViewport3() {
    glViewport(400, 100, viewport3_width, viewport3_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)viewport3_width / (float)viewport3_height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    drawCube();
    drawSphere();

    glColor3f(0.0f, 0.0f, 0.0f);
    drawText("Direita", 10, height - viewport3_height + 150);
}

void drawViewport4() {
    glViewport(600, 100, viewport4_width, viewport4_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)viewport4_width / (float)viewport4_height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, .0f, 0.0f, -1.0f);
    drawCube();
    drawSphere();

    drawText("Topo", 10, height - viewport4_height + 150);
}

void Draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawViewport1();
    char nivelText[20];
    snprintf(nivelText, sizeof(nivelText), "Nivel: %d", nivel);
    drawText(nivelText, 10, 500);

    char intensidadeText[20];
    if(speed == 0.05f)
        snprintf(intensidadeText, sizeof(intensidadeText), "Intensidade: 1");
    else if(speed == 0.1f)
        snprintf(intensidadeText, sizeof(intensidadeText), "Intensidade: 2");
    else if(speed == 0.15f)
        snprintf(intensidadeText, sizeof(intensidadeText), "Intensidade: 3");
    else
        snprintf(intensidadeText, sizeof(intensidadeText), "Intensidade: ");
    
    drawText(intensidadeText, 10, 450);
    
    char XText[20];
    char YText[20];
    char ZText[20];
    
    snprintf(XText, sizeof(XText), "X: %lf", sphereData[0].x);
    drawText(XText, 10, 400);
    snprintf(YText, sizeof(YText), "Y: %.2lf", sphereData[0].y);
    drawText(YText, 10, 370);
    snprintf(ZText, sizeof(ZText), "Z: %.2lf", sphereData[0].z);
    drawText(ZText, 10, 340);

    char XYText[50];
    char YZText[50];
    char XZText[50];
    
    snprintf(XYText, sizeof(XYText), "Frente: X: %.2lf e Y: %.3lf", ClickxFront, ClickyFront);
    drawText(XYText, 10, 300);

    snprintf(YZText, sizeof(YZText), "Direita: Y: %.2lf e Z: %.3lf", ClickxRight, ClickyRight);
    drawText(YZText, 10, 270);

    snprintf(XZText, sizeof(XZText), "Topo: X: %.2lf e Z: %.3lf", ClickxTop, ClickyTop);
    drawText(XZText, 10, 240);

    drawViewport2();
    drawViewport3();
    drawViewport4();


    glutSwapBuffers();
}

void reshape(int w, int h) {
    width = w;
    height = h;

    glViewport(0, 0, width, height);
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow("7 Ball Game");

    Initialize();

    glutDisplayFunc(Draw);
    glutReshapeFunc(reshape);

    glutKeyboardFunc (KeyboardCallback);
    glutMainLoop();

    return 0;
}

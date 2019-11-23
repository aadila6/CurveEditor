/*
 * Abudureheman Adila
 * ECS 175 Project #2
 * UC Davis, Fall 2019
 */

#ifdef WIN32
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "tk.h"
#endif

#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#else //linux
#include <GL/gl.h>
#include <GL/glut.h>
#endif

//other includes
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
//other files




/****set in main()****/
//the number of pixels in the grid
char *inputFileName;
float grid_width;
float grid_height;


//the size of pixels sets the inital window height and width
//don't make the pixels too large or the screen size will be larger than
//your display size
float pixel_size;

/*Window information*/
int WIN_HEIGHT = 600;
int WIN_WIDTH = 600;

void init();
void idle();
void display();
void draw_pix(int x, int y);
void draw();
void key(unsigned char ch, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void check();


class Coord{
public:
    float x, y;
public:
    Coord(){};
    Coord(float x, float y) {
        this->x = x;
        this->y = y;
        
    }
};



// class Polygon {
//     //Center of mass is 0 respect to polygon itself
//     //But position vector is the centroid from the viewPort
// public:
//     int count;
//     std::vector<float> position;
//     int edgeCount;
//     Coord transVec;
//     float angle;
//     float scale;
//     std::vector<std::vector<float>> vertices;
//     std::vector<std::vector<int>> edges;
    
// public:
//     void printPolygon();
//     void updateCentroid();
//     Polygon(){};
    
//     Polygon(std::vector<Coord> &vert, std::vector <ECoord> &edges) {
//         float xtotal = 0, ytotal = 0;
//         this -> count = vert.size();
//         this ->edgeCount = edges.size();
//         for(int i = 0; i < vert.size(); i++){
//             vertices.push_back(std::vector<float>{vert[i].x, vert[i].y, vert[i].z});
//         }
//         for(int j=0; j<edges.size();j++){
//             this->edges.push_back(std::vector<int>{edges[j].x, edges[j].y});
//         }
//         this -> updateCentroid();
        
//         transVec.x = 0.0;
//         transVec.y = 0.0;
//         transVec.z = 0.0;
//         angle = 0.0;
//         scale = 1.0;
//     }
// };

// void Polygon::printPolygon() {
//     std::cout<<"numVertices: "<<this->count<<std::endl;
//     for(int i = 0; i < this->count; i++) {
//         std::cout<<"x: "<<(this->vertices[i])[0]<<" y: "<<(this->vertices[i])[1]<<" z: "<<(this->vertices[i])[2]<<std::endl;
//     }
//     //  for(int j = 0; j < this->edgeCount; j++) {
//     //     std::cout<<"Edges "<<(this->edges[j])[0]<<" and "<<(this->edges[j])[1]<<std::endl;
//     // }
//     std::cout<<std::endl;
// }

// void Polygon::updateCentroid() {
//     float xtotal = 0, ytotal = 0, ztotal=0;
//     for(int i = 0; i < this->count; i++){
//         xtotal += this->vertices[i][0];
//         ytotal += this->vertices[i][1];
//         ztotal += this->vertices[i][2];
//     }
//     this->position = {xtotal/(float)(this->count), ytotal/(float)(this->count),  ztotal/(float)(this->count)};
// }

// void readinput(char *filename, std::vector<Polygon> &polygons);
// void writeFile(char *filename, std::vector<Polygon> &polygons);
bool* loadBuffer;
// std::vector<Polygon> polygonList;
// std::vector<Polygon> cPolygonList;
std::vector<Coord> clicked;
char lineMode;


int main(int argc, char **argv)
{
    // inputFileName = "testScene.txt";
    pixel_size = 1;
    
    /*Window information*/
    // win_height = grid_height * pixel_size;
    // win_width = grid_width * pixel_size;
    
    /*Set up glut functions*/
    /** See https://www.opengl.org/resources/libraries/glut/spec3/spec3.html ***/
    
    //float translationX=0, translationY=0 , sFactor=1, cliponeX=0,cliponeY=0, cliptwoX=0, cliptwoY=0;
    grid_width = 1.0f; //500;
    grid_height = 1.0f; //500;
    

    // readinput(inputFileName, polygonList);
    //polygonList[0].printPolygon();
    //polygonList[1].printPolygon();
    //polygonList[2].printPolygon();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    /*initialize variables, allocate memory, create buffers, etc. */
    //create window of size (win_width x win_height
    glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
    //windown title is "glut demo"
    glutCreateWindow("2D Curve Editor");
//    clicked.push_back(Coord(100,100));
    /*defined glut callback functions*/
    glutDisplayFunc(display); //rendering calls here
    
    glutMouseFunc(mouse);     //mouse button events
    glutMotionFunc(motion);   //mouse movement events
    glutKeyboardFunc(key);    //Keyboard events
    glutIdleFunc(idle);       //Function called while program is sitting "idle"
    
    //initialize opengl variables
    init();
    //start glut event loop
    glutMainLoop();
    return 0;
}

/*initialize gl stufff*/
void init()
{
    //set clear color (Default background to white)
    glClearColor(1.0, 1.0, 1.0, 1.0);
    //glLineWidth(1.0f);
    //checks for OpenGL errors
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, grid_width, 0, grid_height, -1, 1);
    check();
}


//called repeatedly when glut isn't doing anything else
void idle(){
    //redraw the scene over and over again
    glutPostRedisplay();
    
}
// void readinput(char *filename, std::vector<Polygon> &polygons){
//     std::ifstream inputFile;
//     inputFile.open(filename);
//     std::string line;
//     int count;
//     int edgeCount;
//     inputFile >> count;
//     getline(inputFile, line); //line 1
//     getline(inputFile, line); //point count
//     for (int i=0; i< count; i++){
//         int num;
//         std::vector <Coord> vertices;
//         std::vector <ECoord> edges;
//         inputFile >> num;
//         getline(inputFile, line);
//         for (int j=0; j<num; j++){
//             float x, y, z;
//             std::string inputX, inputY, inputZ;
//             getline(inputFile, line);
//             std :: istringstream record(line);
//             getline(record, inputX, ' ');
//             getline(record, inputY, ' ');
//             getline(record, inputZ);
//             x = std::stof(inputX);
//             y = std::stof(inputY);
//             z = std::stof(inputZ);
//             Coord point;
//             point.x = x;
//             point.y = y;
//             point.z = z;
//             vertices.push_back(point);
//         }
//         std :: string input;
//         getline(inputFile, line);
//         edgeCount = std::stoi(line);
//         for (int j=0; j<edgeCount; j++){
//             int x, y;
//             std :: string inputX, inputY;
//             getline(inputFile, line);
//             std :: istringstream record(line);
//             getline(record, inputX, ' ');
//             getline(record, inputY);
//             x = std::stoi(inputX);
//             y = std::stoi(inputY);
//             ECoord point;
//             point.x = x;
//             point.y = y;
//             edges.push_back(point);
//         }
//         Polygon polygon(vertices, edges);
//         polygons.push_back(polygon);
//         getline(inputFile, line);
//     }
//     inputFile.close();
// }
// void writeFile(char *filename,std::vector<Polygon> &polygons){
//     std::ofstream outputFile(filename);
//     outputFile << polygons.size() << "\n\n";
//     for (int i = 0; i<polygons.size();i++) {
//         outputFile << polygons[i].count << std::endl;
//         for(int j = 0; j<polygons[i].count;j++) {
//             outputFile << polygons[i].vertices[j][0] << ' ' << polygons[i].vertices[j][1] << ' '<< polygons[i].vertices[j][2]<< std::endl;
//         }
//         outputFile << polygons[i].edgeCount<< std::endl;
//         for(int k = 0; k<polygons[i].edgeCount;k++) {
//             outputFile << polygons[i].edges[k][0] << ' ' << polygons[i].edges[k][1]<<std::endl;
//         }
//         outputFile << std::endl;
//     }
// }
//Bezier
double deCasteljau(int i, int n, double t){
    if(n == 1) {
        if(i == 0) {
            return 1-t;
        }
        if(i == 1) {
            return t;
        }
        
        return 0;
        
    }
    if(i < 0 || i > n) return 0; 
    return (1 - t) * deCasteljau(i, n-1, t) + t * deCasteljau(i-1, n-1, t);
}

//B-Spline
void deBoor(){

}

void drawLine(float x1 ,float y1, float x2, float y2){
    glColor3f(0.41, 0.4, 0.4);
    glBegin(GL_LINES);
    glVertex2f(x1,y1);
    glVertex2f(x2,y2);
    glEnd();
}

void drawSplitLines(){
    glColor3f(0.41, 0.4, 0.4);
    glBegin(GL_LINES);
    glVertex2f(0.5f,0.0f);
    glVertex2f(0.5f, 1.0f);
    glVertex2f(0.0f, 0.5f);
    glVertex2f(1.0f, 0.5f);
    glEnd();
}


//this is where we render the screen
void display()
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    // clicked.push_back(Coord(20,20));
    for(int i = 0;i<clicked.size();i++){
        // Coord vA = clicked[i];
        // Coord vB = clicked[(i + 1) % clicked.size()];
        // drawLine(vA.x/grid_width,vA.y/grid_width,vB.x/grid_width,vB.y/grid_width);
        drawLine(clicked[i].x, clicked[i].y, clicked[i+1].x, clicked[i+1].y);
        draw_pix(clicked[i].x,clicked[i].y);
    }
    // drawSplitLines();
//    std::cout<<"Size of the Clicked Array is : "<<clicked.size()<<std::endl;
//    drawLine(0.9f,0.0f,0.2f, 1.0f);
    glutSwapBuffers();
    check();
}



//Draws a single "pixel" given the current grid size
//don't change anything in this for project 1
void draw_pix(int x, int y)
{
    glBegin(GL_POINTS);
    glColor3f(0.41, 0.4, 0.4);
    glVertex3f(x + .5, y + .5, 0);
    glEnd();
}



//gets called when a key is pressed on the keyboard
void key(unsigned char ch, int x, int y)
{
    switch (ch)
    {
        case 'b':
            lineMode = 'b';
            break;
            
        case 'd':
            lineMode = 'd';
            break;
            
        default:
            //prints out which key the user hit
            printf("User hit the \"%c\" key\n", ch);
            break;
    }
    //redraw the scene after keyboard input
    glutPostRedisplay();
}


//gets called when a mouse button is pressed
void mouse(int button, int state, int x, int y)
{
    //TODO: CHANGE THE MODE, instead just draw the lines!
    //print the pixel location, and the grid location
    //printf("MOUSE AT PIXEL: %d %d, GRID: %d %d\n", x, y, (int)(x / pixel_size), (int)((WIN_HEIGHT - y) / pixel_size));
    float normx = (float)x/WIN_WIDTH;
    float normy = (WIN_HEIGHT - (float)y) / WIN_HEIGHT;
    std::cout << "Raw x and y: " << x << " " << y << std::endl;
    std::cout << "Normalized x and y: " << normx << " " << normy << std::endl;
    Coord newPoint(normx,normy);
    if(clicked.size()==0){
        clicked.push_back(newPoint);
    }
    else if(clicked.back().x != newPoint.x && clicked.back().y != newPoint.y){
        clicked.push_back(newPoint);
    }

    switch (button)
    {
        case GLUT_LEFT_BUTTON: //left button
            printf("LEFT ");
            break;
        case GLUT_RIGHT_BUTTON: //right button
            printf("RIGHT ");
        default:
            printf("UNKNOWN "); //any other mouse button
            break;
    }
    if (state != GLUT_DOWN) //button released
        printf("BUTTON UP\n");
    else
        printf("BUTTON DOWN\n"); //button clicked
    
    //redraw the scene after mouse click
    // drawLine(0,0,x/grid_width,y/grid_width);
    
    glutPostRedisplay();
}

//gets called when the curser moves accross the scene
void motion(int x, int y)
{
    //redraw the scene after mouse movement
    glutPostRedisplay();
}

//checks for any opengl errors in the previous calls and
//outputs if they are present
void check()
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        printf("GLERROR: There was an error %s\n", "error");
        exit(1);
    }
}



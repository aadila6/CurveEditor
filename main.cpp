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

#include "imgui.h"
#include "imgui_impl_glut.h"
#include "imgui_impl_opengl2.h"
#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/freeglut.h>
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



static bool show_demo_window = true;
static bool show_another_window = false;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
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
int WIN_HEIGHT = 500;
int WIN_WIDTH = 500;
//
void init();
void idle();
void display();
void draw_pix(int x, int y);
void draw();
void reshape(int width, int height);
void key(unsigned char ch, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void check();
void initGui(int argc, char **argv);
void my_display_code();
void glut_display_func();

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


int mainWindow;
int guiWindow;
bool bezierMode;
bool bSplineMode;
bool addMode;
bool modifyMode;
bool deleteMode;

class Curve{
    public:
        std::vector<Coord> vertices;
        float u;
        std::vector<float> uValues;
        Coord centroid;
        Curve(){
            u=0;
        }
        Curve(std::vector<Coord> vert){
            for(int i=0;i<vert.size();i++){
                this->vertices.push_back(vert[i]);
            }
        }

    void updateCentroid() {
            float xtotal = 0, ytotal = 0;
            int count = vertices.size();
            for(int i = 0; i < count; i++){
                xtotal += this->vertices[i].x;
                ytotal += this->vertices[i].y;
            }
    this->centroid.x = xtotal/(float)(count);
    this->centroid.y = ytotal/(float)(count);
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
std::vector<Coord> clicked;
std::vector<float> uValues;
char lineMode;
int gloT;
std::vector<Curve> CurveList;
int findNearest(std::vector<Coord> points, Coord cur);
int main(int argc, char **argv)
{
    // inputFileName = "testScene.txt";
    pixel_size = 1;
    grid_width = 1.0f; //500;
    grid_height = 1.0f; //500;
    gloT = 0.0;
    // readinput(inputFileName, polygonList);
//
     glutInit(&argc, argv);
     glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
     /*initialize variables, allocate memory, create buffers, etc. */
     //create window of size (win_width x win_height)
     glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
     //windown title is "glut demo"
     mainWindow = glutCreateWindow("2D Curve Editor");
//    defined glut callback functions
     glutDisplayFunc(display); //rendering calls here
     glutMouseFunc(mouse);     //mouse button events
     glutMotionFunc(motion);   //mouse movement events
     glutReshapeFunc(reshape); //update GL on window size change
     glutKeyboardFunc(key);    //Keyboard events
     glutIdleFunc(idle);       //Function called while program is sitting "idle"
     //initialize opengl variables
     init();

    #ifdef __FREEGLUT_EXT_H__
        glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    #endif
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
        glutInitWindowSize(500, 500);
        guiWindow = glutCreateWindow("Gui Window");
    // Setup GLUT display function
    // We will also call ImGui_ImplGLUT_InstallFuncs() to get all the other functions installed for us,
    // otherwise it is possible to install our own functions and call the imgui_impl_glut.h functions ourselves.
    glutDisplayFunc(glut_display_func);
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGLUT_Init();
    ImGui_ImplGLUT_InstallFuncs();
    ImGui_ImplOpenGL2_Init();

    //start glut event loop
    glutMainLoop();
    return 0;
    bezierMode = true;
    bSplineMode = false;
    addMode = false;
    modifyMode = false;
    deleteMode = false;
}

/*initialize gl stufff*/
void init()
{
    //set clear color (Default background to white)
    glClearColor(0.0, 0.0, 0.0, 0.0);
    //glLineWidth(1.0f);
    //checks for OpenGL errors
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, grid_width, 0, grid_height, -1, 1);
    check();
}

void my_display_code()
{
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    // if (show_demo_window)
    //     ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        static int f = 0;
        static int f2 = 0;
        static int counter = 0;

        ImGui::Begin("2D Curve Editing Gui");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("Please select one of below to operate");               // Display some text (you can use a format strings too)
        // ImGui::Checkbox("Bezir", &bezierMode);      // Edit bools storing our window open/close state
        static int c = 0;
        ImGui::RadioButton("Bezier", &c, 0); 
        ImGui::RadioButton("B-Spline", &c, 1); 
        if(c == 0){
            bSplineMode = false;
            bezierMode = true;
        } 
        if(c == 1){
            bezierMode = false;
            bSplineMode = true;
        } 

        static int e = 0;
        ImGui::RadioButton("Add", &e, 0); ImGui::SameLine();
        ImGui::RadioButton("Delete", &e, 1); ImGui::SameLine();
        ImGui::RadioButton("Modify", &e, 2);
        if(e == 0){
            addMode = true;
            deleteMode = false;
            modifyMode = false;
        } 
        if(e == 1){
            addMode = false;
            deleteMode = true;
            modifyMode = false;
        } 
        if(e == 2) {
            addMode = false;
            deleteMode = false;
            modifyMode = true;
        }

        if(c == 0){
            if (ImGui::SliderInt("n", &f, 0, 80))            // Edit 1 float using a slider from 0.0f to 1.0f
        {
            glutSetWindow(mainWindow);
            glutPostRedisplay();
            glutSetWindow(guiWindow);
            gloT = f;
        }

        }else if(c ==1){
            ImGui::SliderInt("u0", &f2, 0, 10);
            ImGui::SliderInt("u1", &f2, 0, 10);
            ImGui::SliderInt("u2", &f2, 0, 10);
            ImGui::SliderInt("u3", &f2, 0, 10);

        //     if (ImGui::SliderInt("u", &f2, 0, 10))            // Edit 2 float using a slider from 0.0f to 1.0f u0 - un
        // {
        //     glutSetWindow(mainWindow);
        //     glutPostRedisplay();
        //     glutSetWindow(guiWindow);
        //     // gloT = f2;
        // }

        }


        

        // ImGui::Checkbox("B-Spline", &bSplineMode);
        
        
        // ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
        if (ImGui::Button("Clear All")){
            clicked.clear();
            glutSetWindow(mainWindow);
            glutPostRedisplay();
            glutSetWindow(guiWindow);
        }                            // Buttons return true when clicked (most widgets return true when edited/activated)
            
        // ImGui::SameLine();
        // ImGui::Text("counter = %d", counter);

        ImGui::End();
    }
}


void glut_display_func()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGLUT_NewFrame();

    my_display_code();
    // Rendering
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();
    glViewport(0, 0, (GLsizei)io.DisplaySize.x, (GLsizei)io.DisplaySize.y);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    //glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound, but prefer using the GL3+ code.
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

    glutSwapBuffers();
    glutPostRedisplay();
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
Coord deCasteljau(std::vector<Coord> B , float u){
    Coord q[B.size()];
	int i, k;
	for (i=0; i<B.size(); ++i) {
		q[i].x = B[i].x;
		q[i].y = B[i].y;
	}
	for (k=1; k<B.size(); ++k) {
		for (i=0; i<(B.size()-k); ++i) {
			q[i].x = (1.0-u)*q[i].x + u*q[i+1].x;
			q[i].y = (1.0-u)*q[i].y + u*q[i+1].y;
		}
	}
	return q[0];
}


//B-Spline
// Coord deBoor(int k, int n, float u, std::vector<Coord> B){
//     Coord q[clicked.size()];

//     for (int i=0; i<B.size(); ++i) {
// 		q[i].x = B[i].x;
// 		q[i].y = B[i].y;
// 	}
//     int vertexCount = clicked.size();
//     int inum = 0; //u is in which [uI,uI+1]
//     for(int j=1; j< k-1; j++){
//         for(int i = inum-(k-1); inum <inum - j; i++){
//             q[i].x = (uValues[i+k]-u)/(uValues[i+k]-uValues[i+j]) * (q[i].x) + (u-uValues[i+j])/(uValues(i+k)-uValues[i+j])*q[i+1].x;
//             q[i].y = (uValues[i+k]-u)/(uValues[i+k]-uValues[i+j]) * (q[i-1].y) + (u-uValues[i+j])/(uValues(i+k)-uValues[i+j])*q[i-1].y;
//         }
//     }


// }

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

    std::vector<Coord> B;
    float step , iq;
    step = 1.0f / gloT;
	for (iq = 0.0f; iq <= 1.0; iq += step) {
		B.push_back(deCasteljau(clicked, iq));
	}

    if(clicked.size() > 1){
        for(int i = 1; i < clicked.size(); i++){
        Coord vA = clicked[i-1];
        Coord vB = clicked[i];
        drawLine(vA.x/grid_width,vA.y/grid_width,vB.x/grid_width,vB.y/grid_width);
        }
    }

    if(B.size()>1){
        for(int i = 1; i < B.size(); i++){
        Coord vA = B[i-1];
        Coord vB = B[i];
        drawLine(vA.x/grid_width,vA.y/grid_width,vB.x/grid_width,vB.y/grid_width);
        }
    }
    B.clear();
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

        default:
            //prints out which key the user hit
            printf("User hit the \"%c\" key\n", ch);
            break;
    }
    //redraw the scene after keyboard input
    glutPostRedisplay();
}

void reshape(int width, int height)
{
    /*set up projection matrix to define the view port*/
    //update the ne window width and height
    WIN_WIDTH = width;
    WIN_HEIGHT = height;
    
    //creates a rendering area across the window
    glViewport(0, 0, width, height);
    // up an orthogonal projection matrix so that
    // the pixel space is mapped to the grid space
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, grid_width, 0, grid_height, -10, 10);
    
    //clear the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //set pixel size based on width, if the aspect ratio
    //changes this hack won't work as well
    pixel_size = width / (float)grid_width;
    
    //set pixel size relative to the grid cell size
    glPointSize(pixel_size);
    //check for opengl errors
    check();
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
    if(addMode){
        if(clicked.size()==0 ){
        clicked.push_back(newPoint);
        uValues.push_back(0.5);
        }
        else if(clicked.back().x != newPoint.x && clicked.back().y != newPoint.y){
        clicked.push_back(newPoint);
        uValues.push_back(0.5);
        }
    }
    if(deleteMode){
        //find the line & delete
        int index = findNearest(clicked, newPoint);
        std::cout<<"Deleted "<<clicked[index].x <<" "<<clicked[index].y<<std::endl;
        clicked.erase(clicked.begin() + index);
        
    }

    if(modifyMode){
        //change the 
    //Function finds the nearest vertex
        int index = findNearest(clicked, newPoint);
        if(modifyMode){
            std::cout<<"Modified "<<clicked[index].x <<" "<<clicked[index].y<<std::endl;
            clicked[index].x = newPoint.x;
            clicked[index].y = newPoint.y;
        }
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
int findNearest(std::vector<Coord> points, Coord cur){
    float distance = 999;
    int min = 0;
    float tempdis;
    for(int i=0;i<points.size();i++){
        tempdis = sqrt(pow((points[i].x - cur.x),2) + pow((points[i].y - cur.y),2));
        if(tempdis < distance){
            distance = tempdis;
            min = i;
        }
    }
    return min;
}
//gets called when the curser moves accross the scene
void motion(int x, int y)
{
    //redraw the scene after mouse movement
    //1-find nearest point
    //2-set it to x,y
    // Coord cur((float)x/WIN_WIDTH,(WIN_HEIGHT - (float)y) / WIN_HEIGHT);
    // //Function finds the nearest vertex
    // int index = findNearest(clicked, cur);
    // if(modifyMode){
    //     clicked[index].x = x;
    //     clicked[index].y = y;
        
    // }
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



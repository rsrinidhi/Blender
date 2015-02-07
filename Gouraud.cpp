// ch01.cpp : Defines the entry point for the console application.
//
//////////////////////////////////////////////////////////
//  triangles.cpp  from the OpenGL Red Book   Chapter 1
//////////////////////////////////////////////////////////

#include <iostream>
#include <math.H>

using namespace std;

#include "vgl.h"
#include "LoadShaders.h"

const GLuint Triangles = 0, NumVAOs = 1, NewVAOs=2, CircleVAO =3;
const GLuint ArrayBuffer=0, NumBuffers=1, NewBuffers=2, CircleBuffer=3;
const GLuint vPosition=0, vColor=1;
const int MENU_POLYMODE=2, MENU_WIRE_FRAME=3,MENU_CIRCLE_GEOMETRY=4,MENU_TRIANGLE_TOGGLE=5,MENU_GOURAUD_TOGGLE=6,MENU_CIRCLE_TOGGLE=7,MENU_EXIT=8;

GLuint VAOs[CircleVAO];
GLuint Buffers[CircleBuffer];

GLuint program,program1;

const GLuint NumVertices = 9;
const GLint numTriangles=41;
GLuint steps;
/** Structure initialization here**/

struct VertexData {
    GLubyte color[4];
    GLfloat position[4];
} vertices[NumVertices], circle[42];

/////////////////////////////////////////////////////
//  int
/////////////////////////////////////////////////////
void init(void) {
 
    /**Circle part of the code here**/
  
       for(int i=0;i<=numTriangles;i++){
        circle[i].color[0]=0;
        circle[i].color[1]=0;
        circle[i].color[2]=255;
        circle[i].color[3]=0;
        circle[i].position[0]= 0.2 *cos(2*3.14*i/numTriangles);
        circle[i].position[1]= 0.2 *sin(2*3.14*i/numTriangles);
    }
    
    glGenVertexArrays(CircleVAO,VAOs);
    glBindVertexArray(VAOs[NewVAOs]);
    
    glGenBuffers(CircleBuffer,Buffers);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[NewBuffers]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(circle),circle,GL_STATIC_DRAW);
    
    //glBufferData(GL_ARRAY_BUFFER,sizeof(circle),NULL,GL_STREAM_DRAW);
    
    ShaderInfo  shaders[] = {
        { GL_VERTEX_SHADER, "triangles.vert" },
        { GL_FRAGMENT_SHADER, "triangles.frag" },
        { GL_NONE, NULL }
    };
    
    program = LoadShaders(shaders);
    
    glVertexAttribPointer(vColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexData),BUFFER_OFFSET(0));
    glVertexAttribPointer(vPosition, 2, GL_FLOAT,GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(sizeof(circle[0].color)));
    
    glEnableVertexAttribArray(vColor);
    glEnableVertexAttribArray(vPosition);
    
    /** Triangle definition here **/
    
    VertexData vertices[NumVertices] ={
        {{ 0,     0,   255, 0},{-0.90, -0.9 }}, // Triangle 1
        {{ 0,     0,   255, 0},{  0.85, -0.9 }},
        {{ 0,     0,   255, 0},{ -0.90,  0.85 }},
        {{ 0,     0,   255, 0},{ 0.90, -0.85 }},// Triangle 2
        {{ 0,     0,   255, 0},{0.90,  0.90 }},
        {{ 0,     0,   255, 0},{ -0.85, 0.90 }},
        {{ 255,   0,   0, 255 }, { 0.00, 0.50 }},  // Triangle 3
        {{   0, 0,   255, 255 }, {  -0.50, -0.50 }},
        {{   0,   255, 0, 255 }, { 0.50,  -0.50 }}
    };
    
    
    /** 2 blue triangles part here**/
    
    glGenVertexArrays(NumVAOs, VAOs);
    glBindVertexArray(VAOs[Triangles]);
    
    glGenBuffers(NumBuffers, Buffers);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
    glBufferData(GL_ARRAY_BUFFER, 6,vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(vColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0,BUFFER_OFFSET(0));
    glVertexAttribPointer(vPosition, 2, GL_FLOAT,GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(sizeof(vertices[0].color)));
    
    glEnableVertexAttribArray(vColor);
    glEnableVertexAttribArray(vPosition );
    
    /** Gouraud part of the code here**/
    
    glGenVertexArrays(NewVAOs, VAOs);
    glBindVertexArray(VAOs[NumVAOs]);
    
    glGenBuffers(NewBuffers, Buffers);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[NumBuffers]);
    glBufferData(GL_ARRAY_BUFFER, (sizeof(vertices)-6),vertices, GL_STATIC_DRAW);
    
   ShaderInfo shaders1[] = {
        { GL_VERTEX_SHADER, "gouraud.vert" },
        { GL_FRAGMENT_SHADER, "gouraud.frag" },
        { GL_NONE, NULL }
    };
    
    program1 = LoadShaders(shaders1);
    
    glVertexAttribPointer(vColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexData),BUFFER_OFFSET(0));
    glVertexAttribPointer(vPosition, 2, GL_FLOAT,GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(sizeof(vertices[0].color)));
    
    glEnableVertexAttribArray(vColor);
    glEnableVertexAttribArray(vPosition);
    
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    
    
}

////////////////////////////////////////////////////////////////////
//	display
////////////////////////////////////////////////////////////////////
void display ()
{
	glClear( GL_COLOR_BUFFER_BIT );
    
    cout<<"Reaches here 7"<<endl;
    
    glUseProgram(program);
	glBindVertexArray( VAOs[Triangles] );
    glDrawArrays( GL_TRIANGLES, 0, NumVertices );
    
    cout<<"Reaches here 8"<<endl;
   
    glUseProgram(program1);
    glBindVertexArray( VAOs[NumVAOs] );
    glDrawArrays( GL_TRIANGLES, 0, NumVertices );
    
    cout<<"Reaches here 9"<<endl;
    
    glUseProgram(program);
    glBindVertexArray(VAOs[NewVAOs]);
    glDrawArrays(GL_TRIANGLE_FAN,0,numTriangles);
    
    cout<<"Reaches here 10"<<endl;
    
	glFlush();
}

////////////////////////////////////////////////////////////////////
//	SelectFromMenu(int id)
////////////////////////////////////////////////////////////////////

void SelectFromMenu(int id)
{
    switch(id)
    {
        case MENU_POLYMODE:
        {
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            break;
        }
            
        case MENU_WIRE_FRAME:
        {
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
            break;

        }
            
        case MENU_CIRCLE_GEOMETRY:
        {
            float radius;
            int steps;
            
            cout<<"Enter the radius of the circle : ";
            cin>>radius;
            
            cout<<"Enter the number of steps:";
            cin>>steps;
            
            for(int i=0;i<=steps;i++){
                circle[i].color[0]=0;
                circle[i].color[1]=0;
                circle[i].color[2]=255;
                circle[i].color[3]=0;
                
                circle[i].position[0]= radius *cos(2*3.14*i/steps);
                circle[i].position[1]= radius *sin(2*3.14*i/steps);
            }
            
            cout<<"Reaches here 1"<<endl;
            
            glBindVertexArray(VAOs[NewVAOs]);
            
            glGenBuffers(CircleBuffer,Buffers);
            glBindBuffer(GL_ARRAY_BUFFER, Buffers[NewBuffers]);
            glBufferData(GL_ARRAY_BUFFER,steps,circle,GL_STREAM_DRAW);
            
            cout<<"Reaches here 2"<<endl;
            
            ShaderInfo  shaders[] = {
                { GL_VERTEX_SHADER, "triangles.vert" },
                { GL_FRAGMENT_SHADER, "triangles.frag" },
                { GL_NONE, NULL }
            };
            
            program = LoadShaders( shaders );
            
            cout<<"Reaches here 3"<<endl;
            
            glVertexAttribPointer(vColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexData),BUFFER_OFFSET(0));
            glVertexAttribPointer(vPosition, 2, GL_FLOAT,GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(sizeof(circle[0].color)));
             
            glEnableVertexAttribArray(vColor);
            glEnableVertexAttribArray(vPosition);
            
            cout<<"Reaches here 4"<<endl;
            
            break;
            
        }
        
        case MENU_TRIANGLE_TOGGLE:
        {
                GLfloat time = (GLfloat)clock() / (GLfloat)CLOCKS_PER_SEC;
                glUniform3f(vColor, (sin(time * 4.0f) + 1.0f) / 2.0f, 0.0f, 0.0f);
            
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                break;
            
        }
            
        case MENU_EXIT:
        {
         
            exit(0);
            break;
        }
    }
    
       cout<<"Reaches here 5"<<endl;
       glutPostRedisplay();
       cout<<"Reaches here 6"<<endl;
}


////////////////////////////////////////////////////////////////////
//	display
////////////////////////////////////////////////////////////////////
void keyboard(unsigned char ch, int x,int y){
    
    switch(ch){
       /* case 'c':
        case 'C':*/
        
        case 's':
        case 'S':
                {
                   SelectFromMenu(MENU_POLYMODE);
                   break;
                }
        
        case 'w':
        case 'W':
                {
                    SelectFromMenu(MENU_WIRE_FRAME);
                    break;
                }
        
        case 'g':
        case 'G':
                {
                    SelectFromMenu(MENU_CIRCLE_GEOMETRY);
                    break;
                }
        
        case 'x':
        case 'X':
                {
                    SelectFromMenu(MENU_TRIANGLE_TOGGLE);
                    break;
                }
        
        case 'y':
        case 'Y':
                {
                    SelectFromMenu(MENU_GOURAUD_TOGGLE);
                    break;
                }
            
        case 'z':
        case 'Z':
                {
                    SelectFromMenu(MENU_CIRCLE_TOGGLE);
                    break;
                }
        
        case 'q':
        case 'Q':
        case 27: exit(0);
                break;
    }
}

////////////////////////////////////////////////////////////////////////
//	BuildMenu
////////////////////////////////////////////////////////////////////////
int BuildMenu(void){
    
    int menu;
    
    menu= glutCreateMenu(SelectFromMenu);
    
    //glutAddMenuEntry ("Press C for specifying the color of the triangle and circle\tl");
    glutAddMenuEntry ("Press S for rendering the object as solid surface ",MENU_POLYMODE);
    glutAddMenuEntry ("Press W for wirefram display ",MENU_WIRE_FRAME);
    //glutAddMenuEntry ("Press G for generating the geometry\tl");
    glutAddMenuEntry ("Press X for toggling display of two triangles\tl", MENU_TRIANGLE_TOGGLE);
    //glutAddMenuEntry ("Press Y for toggling display of gouraud triangles\tl");
    //glutAddMenuEntry ("Press Z for toggling display of circle\tl");
    glutAddMenuEntry ("Press Q/ESC to quit the program \tEsc",MENU_EXIT);

    return menu;
}
////////////////////////////////////////////////////////////////////////
//	main
////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    char ch;
	glutInit( &argc, argv );

    #ifdef __APPLE__
    glutInitDisplayMode( GLUT_3_2_CORE_PROFILE|GLUT_RGBA);
    
    #else
    glutInitContextVersion( 4, 3 );
    glutInitContextProfile( GLUT_CORE_PROFILE );
    glutInitDisplayMode( GLUT_RGBA);
    #endif
    
    glutInitWindowSize( 512, 512 );
	glutCreateWindow( "My first Program");

    
	glewExperimental = GL_TRUE;	// added for glew to work!
	if ( glewInit() )
	{
		cerr << "Unable to initialize GLEW ... exiting" << endl;
		exit (EXIT_FAILURE );
	}

	// code from OpenGL 4 Shading Language cookbook, second edition
	const GLubyte *renderer = glGetString( GL_RENDERER );
	const GLubyte *vendor = glGetString( GL_VENDOR );
	const GLubyte *version = glGetString( GL_VERSION );
	const GLubyte *glslVersion = glGetString (GL_SHADING_LANGUAGE_VERSION );
	GLint major, minor;
	glGetIntegerv( GL_MAJOR_VERSION, &major );
	glGetIntegerv( GL_MINOR_VERSION, &minor );

	cout << "GL Vendor            :" << vendor << endl;
	cout << "GL Renderer          :" << renderer  << endl;
	cout << "GL Version (string)  :" << version << endl;
	cout << "GL Version (integer) :" << major << " " << minor << endl;
	cout << "GLSL Version         :" << glslVersion << endl;

   
	
    
    glutDisplayFunc( display );
    
    glutKeyboardFunc(keyboard);
    BuildMenu();
    glutAttachMenu (GLUT_RIGHT_BUTTON);
    
    init();
    
	glutMainLoop();

	return 0;
}


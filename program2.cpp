// ch01.cpp : Defines the entry point for the console application.
//
//////////////////////////////////////////////////////////
//  triangles.cpp  from the OpenGL Red Book   Chapter 1
//////////////////////////////////////////////////////////

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <istream>
#include "glm/glm/glm.hpp"
#include "glm/glm/mat4x4.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "glm/glm/gtx/rotate_vector.hpp"

using namespace std;

#include "vgl.h"
#include "LoadShaders.h"

/** Variable declarations **/
const GLuint vPosition=0, vnormal=1, vambient=2, vdiffuse=3;
const int MENU_RESET=1, MENU_POLYMODE=2, MENU_WIRE_FRAME=3,MENU_KEY_UP=4,MENU_KEY_DOWN=5,MENU_CAMERA_DOWN=6,MENU_CAMERA_UP=7,MENU_FC_OPP=8,MENU_FC_UP=9,MENU_ROTATE_CCW=10,MENU_ROTATE_CW=11,MENU_EXIT=12,MENU_KEY_LEFT=13,MENU_KEY_RIGHT=14,MENU_BLEND=15,MENU_MULT=16,MENU_ADD=17;
static int flag=0;

GLuint VAOs[10];
GLuint Buffers[10];
GLuint program;

GLfloat width=5.0f;

//Reshape function
float aspect;
GLint current_width;
GLint current_height;

//Eye Position
GLfloat maxX=0.0f;
GLfloat maxY=0.0f;
GLfloat maxZ=0.0f;

//Focal point
GLfloat midX=0.0f;
GLfloat midY=0.0f;
GLfloat midZ=0.0f;

GLfloat angle = 0.0f;
 int size=0;

bool animating=false;

/** Structure initialization here**/
struct
{
    GLint model_matrix;
    GLint view_matrix;
    GLint projection_matrix;
    GLint light_position;
    GLint condition;
} render_scene_uniforms;


struct{
    
    glm::mat4 scaleMatrix;
    glm::mat4 transMatrix;
    glm::mat4 rotateMatrix;
    glm::mat4 modelMatrix;
    
    glm::mat4 OrderMatrix[10];
    
    glm::vec3* VerticesArray;
    glm::vec3* NormalArray;
    glm::vec3 *KaArray;
    glm::vec3 *KdArray;
    
    int size;
    int number;
    
}VertexData[10];

/** Matrices initialization **/
glm::vec3 eyepoint;
glm::vec3 focalpoint;
glm::vec3 viewUp;

glm::vec3 light_vector;
glm::mat4 view_matrix;
glm::mat4 Projection;

glm::vec3 move1;
glm::vec3 move2;
glm::vec3 move3;
glm::vec3 x1;
glm::vec3 y;
glm::vec3 z1;


/** Blender **/

static int num_blend_funcs;
static int num_add_funcs;
static int num_mult_funcs;
static int blendMode;

static const GLenum blend_func[] =
{
    GL_ZERO,
    GL_ONE,
    GL_SRC_COLOR,
    GL_ONE_MINUS_SRC_COLOR,
    GL_DST_COLOR,
    GL_ONE_MINUS_DST_COLOR,
    GL_SRC_ALPHA,
    GL_ONE_MINUS_SRC_ALPHA,
    GL_DST_ALPHA,
    GL_ONE_MINUS_DST_ALPHA,
    GL_CONSTANT_COLOR,
    GL_ONE_MINUS_CONSTANT_COLOR,
    GL_CONSTANT_ALPHA,
    GL_ONE_MINUS_CONSTANT_ALPHA,
    GL_SRC_ALPHA_SATURATE,
    GL_SRC1_COLOR,
    GL_ONE_MINUS_SRC1_COLOR,
    GL_SRC1_ALPHA,
    GL_ONE_MINUS_SRC1_ALPHA
};


static const GLenum additive[] =
{
    GL_ONE,
    GL_SRC_COLOR,
    GL_SRC_ALPHA,
    GL_ONE_MINUS_SRC_COLOR,
    GL_ONE_MINUS_SRC_ALPHA
};

static const GLenum multiplicative[] =
{
    GL_ZERO,
    GL_SRC_COLOR,
    GL_SRC_ALPHA,
    GL_ONE_MINUS_SRC_COLOR,
    GL_ONE_MINUS_SRC_ALPHA
};


static const GLenum ABuffer[]=
{
    GL_DST_COLOR,
    GL_ONE_MINUS_DST_COLOR,
    GL_DST_ALPHA,
    GL_ONE_MINUS_DST_ALPHA,
    GL_CONSTANT_COLOR,
    GL_ONE_MINUS_CONSTANT_COLOR,
    GL_CONSTANT_ALPHA,
    GL_ONE_MINUS_CONSTANT_ALPHA,
    GL_SRC_ALPHA_SATURATE,
    GL_SRC1_COLOR,
    GL_ONE_MINUS_SRC1_COLOR,
    GL_SRC1_ALPHA,
    GL_ONE_MINUS_SRC1_ALPHA
};

int i=0,j=0;
char *PathName=NULL;
/////////////////////////////////////////////////////
//  int
/////////////////////////////////////////////////////

int LoadOBJ(char* path,glm::vec3 *VerticesArray,glm::vec3 *NormalArray,glm::vec3 *KaArray,glm::vec3 *KdArray)
{
    int k=0,l=0;
    
    glm::vec3* vArray= new glm::vec3[10000000];
    glm::vec3* nArray=new glm::vec3[10000000];
    int *fvArray=new int[10000000];
    int *fnArray=new int[10000000];
    glm::vec3 color1;
    glm::vec3 diffuse;
    
    int i=0,j=0,m=0,p=0,n=0, ct=0;
    FILE * file = fopen(path, "r");
    
    if( file == NULL ){
        printf("Impossible to open the file ! Are you in the right path ?\n");
        getchar();
        return 0;
    }
    
    while( 1 ){
        
        char lineHeader[128];
        char fileName[128];
        
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break;
        
        if(strcmp (lineHeader,"mtllib")==0)
        {
            fscanf(file,"%s.mtl", &fileName);
        }
        
        if ( strcmp( lineHeader, "v" ) == 0 ){
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            vArray[i]= vertex;
            
            i++;
            
        }
        
        else if (strcmp( lineHeader, "vt" ) == 0 ){
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y );
            uv.y = -uv.y;
            
        }
        
        else if ( strcmp( lineHeader, "vn" ) == 0 ){
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
            nArray[j]= normal;
            j++;
        }
        
        else if ( strcmp( lineHeader, "f" ) == 0 ){
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int match=0;
            fpos_t pos;
            
            fgetpos(file,&pos);
            match=fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
            
            if(match==9){
            fvArray[k++]=vertexIndex[0];
            KaArray[m++]=color1;
            KdArray[p++]=diffuse;
            fvArray[k++]=vertexIndex[1];
            KaArray[m++]=color1;
            KdArray[p++]=diffuse;
            fvArray[k++]=vertexIndex[2];
            KaArray[m++]=color1;
            KdArray[p++]=diffuse;

            
            fnArray[l++]=normalIndex[0];
            fnArray[l++]=normalIndex[1];
            fnArray[l++]=normalIndex[2];
                
                
                
            n=1;
            }
            
            if(match!=9){
                fsetpos(file,&pos);
                match=fscanf(file, "%d %d %d\n", &vertexIndex[0], &vertexIndex[1],&vertexIndex[2]);
                if(match==3){
                fvArray[k++]=vertexIndex[0];
                KaArray[m++]=color1;
                KdArray[p++]=diffuse;
                fvArray[k++]=vertexIndex[1];
                KaArray[m++]=color1;
                KdArray[p++]=diffuse;
                fvArray[k++]=vertexIndex[2];
                KaArray[m++]=color1;
                KdArray[p++]=diffuse;
                    
                n=2;
                
            }}
            
            if(match!=9 && match!=3){
                fsetpos(file,&pos);
                match=fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0],&normalIndex[0],&vertexIndex[1], &normalIndex[1],&vertexIndex[2], &normalIndex[2]);
                
                if(match==6){
                fvArray[k++]=vertexIndex[0];
                KaArray[m++]=color1;
                KdArray[p++]=diffuse;
                fvArray[k++]=vertexIndex[1];
                KaArray[m++]=color1;
                KdArray[p++]=diffuse;
                fvArray[k++]=vertexIndex[2];
                KaArray[m++]=color1;
                KdArray[p++]=diffuse;
                
                fnArray[l++]=normalIndex[0];
                fnArray[l++]=normalIndex[1];
                fnArray[l++]=normalIndex[2];
                    
                
                    
                n=3;
                }}
            
            if(match!=9 && match!=3 && match!=6){
                fsetpos(file,&pos);
                match=fscanf(file, "%d/%d %d/%d %d/%d\n", &vertexIndex[0],&uvIndex[0],&vertexIndex[1], &uvIndex[1],&vertexIndex[2], &uvIndex[2]);
                fvArray[k++]=vertexIndex[0];
                KaArray[m++]=color1;
                KdArray[p++]=diffuse;
                fvArray[k++]=vertexIndex[1];
                KaArray[m++]=color1;
                KdArray[p++]=diffuse;
                fvArray[k++]=vertexIndex[2];
                KaArray[m++]=color1;
                KdArray[p++]=diffuse;
                    
                n=4;
                }
        }
        
            else if( strcmp( lineHeader, "usemtl" ) == 0 )
            {
                    char color[128];
                    char previous[128]=" ";
                
                    fscanf(file, "%s\n",color);
                
                    char *s=strtok(path,"/");
                    strcat(s,"/");
                    strcat(s,fileName);
                
                    FILE * file1 = fopen(s, "r");
                
                    if( file1 == NULL ){
                    printf("Impossible to open the file ! Are you in the right path ?\n");
                    getchar();
                    return 0;
                    }
                
                    while(1){
                        
                        char lineHeader2[128];
                        
                        int res1 = fscanf(file1, "%s", lineHeader2);
                        if (res1 == EOF)
                        break;
                        
                        if(strcmp(lineHeader2,color)==0)
                            strcpy(previous,color);
                        
                            if(strcmp(lineHeader2,"Ka")==0)
                            {
                                    if(strcmp(previous,color)==0){
                                    fscanf(file1,"%f %f %f",&color1.x,&color1.y,&color1.z);
                                    }
                            }
                        
                            if(strcmp(lineHeader2,"Kd")==0)
                            {
                                if(strcmp(previous,color)==0){
                                fscanf(file1,"%f %f %f",&diffuse.x,&diffuse.y,&diffuse.z);
                                strcpy(previous,"void");
                                flag=1;
                                }
                            }
                        
                        }
            }

       }
    
    for(int a=0;a<k;a++)
    {
        VerticesArray[a]=vArray[fvArray[a]-1];
    }
    
    if(j>0)
    {
      
        for(int b=0;b<k;b++)
        {
            NormalArray[b]=nArray[fnArray[b]-1];
           
        }
    }
    
        else if(j==0){
        for(int b=0;b<k;b+=3)
        {
            glm::vec3 edge1=vArray[fvArray[b+1]-1]- vArray[fvArray[b]-1];
            glm::vec3 edge2=vArray[fvArray[b+2]-1]- vArray[fvArray[b]-1];
            glm::vec3 tnormal=glm::normalize(glm::cross(edge1,edge2));
       
            NormalArray[b].x=tnormal.x;
            NormalArray[b].y=tnormal.y;
            NormalArray[b].z=tnormal.z;
        
            NormalArray[b+1].x=tnormal.x;
            NormalArray[b+1].y=tnormal.y;
            NormalArray[b+1].z=tnormal.z;
        
            NormalArray[b+2].x=tnormal.x;
            NormalArray[b+2].y=tnormal.y;
            NormalArray[b+2].z=tnormal.z;
        }
    }

    for(int b=0;b<k;b++)
    {
        if(flag==0)
        {
            KaArray[b]=glm::vec3(0.1f, 0.1f, 0.1f);
            KdArray[b]=glm::vec3(0.9f, 0.9f, 0.9f);
        }
    }
    
    
    for(int b=0;b<k;b++)
    {
        if(VerticesArray[b].x>maxX)
            maxX=VerticesArray[b].x;
        
        if(VerticesArray[b].y>maxY)
            maxY=VerticesArray[b].y;
        
        if(VerticesArray[b].z>maxZ)
            maxZ=VerticesArray[b].z;
        
        midX+=VerticesArray[b].x;
        midY+=VerticesArray[b].y;
        midZ+=VerticesArray[b].z;
    }
    
    midX=midX/k;
    midY=midY/k;
    midZ=midZ/k;
    
   delete vArray;
   delete nArray;
   delete fvArray;
   delete fnArray;
    
   return k;
    
}
/////////////////////////////////////////////////////
//  void
/////////////////////////////////////////////////////

void read(char* path){
    
    
    FILE * file = fopen(path, "r");
    
    if( file == NULL ){
        printf("Impossible to open the file ! Are you in the right path ?\n");
        getchar();
        return;
    }
        
        while( 1 ){
            
            char lineHeader[128];
            char fileName[128];
            
            int res = fscanf(file, "%s", lineHeader);
            if (res == EOF)
                break;
            
            if(strcmp (lineHeader,"obj")==0)
            {
                fscanf(file,"%s/%s.obj", &fileName);
                VertexData[size].VerticesArray= new glm::vec3[10000000];
                VertexData[size].NormalArray=new glm::vec3[10000000];
                VertexData[size].KaArray=new glm::vec3[10000000];
                VertexData[size].KdArray=new glm::vec3[10000000];
                
                VertexData[size].size=LoadOBJ(fileName,VertexData[size].VerticesArray,VertexData[size].NormalArray,VertexData[size].KaArray,VertexData[size].KdArray);
                
                VertexData[size].scaleMatrix=glm::mat4(1.0f);
                VertexData[size].transMatrix=glm::mat4(1.0f);
                VertexData[size].rotateMatrix=glm::mat4(1.0f);
                VertexData[size].modelMatrix=glm::mat4(1.0f);
                size++;
                VertexData[size-1].number=0;
            }
            
            else if(strcmp(lineHeader,"t")==0)
            {
                glm::vec3 translate;
                fscanf(file,"%f %f %f\n", &translate.x,&translate.y,&translate.z);
                VertexData[size-1].transMatrix=glm::translate(glm::mat4(1.0f),translate);
                VertexData[size-1].OrderMatrix[VertexData[size-1].number]=VertexData[size-1].transMatrix;
                VertexData[size-1].number++;
                
            }
            
            else if(strcmp(lineHeader,"s")==0)
            {
                glm::vec3 scale;
                fscanf(file,"%f %f %f\n", &scale.x,&scale.y,&scale.z);
                VertexData[size-1].scaleMatrix=glm::scale(glm::mat4(1.0f),scale);
                VertexData[size-1].OrderMatrix[VertexData[size-1].number]=VertexData[size-1].scaleMatrix;
                VertexData[size-1].number+=1;
            }
            
            else if(strcmp(lineHeader,"rx")==0)
            {
                glm::vec3 rotate;
                fscanf(file,"%f\n", &rotate.x);
                VertexData[size-1].rotateMatrix=glm::rotate(glm::mat4(1.0f),rotate.x*3.14159f/180.f,glm::vec3(1.0f,0.0f,0.0f));
                VertexData[size-1].OrderMatrix[VertexData[size-1].number]=VertexData[size-1].rotateMatrix;
                VertexData[size-1].number++;
            }
            
            else if(strcmp(lineHeader,"ry")==0)
            {
                glm::vec3 rotate;
                fscanf(file,"%f\n", &rotate.y);
                VertexData[size-1].rotateMatrix=glm::rotate(glm::mat4(1.0f),rotate.y*3.14159f/180.f,glm::vec3(0.0f,1.0f,0.0f));
                VertexData[size-1].OrderMatrix[VertexData[size-1].number]=VertexData[size-1].rotateMatrix;
                VertexData[size-1].number++;
            }
            
            else if(strcmp(lineHeader,"rz")==0)
            {
                glm::vec3 rotate;
                fscanf(file,"%f\n", &rotate.z);
                VertexData[size-1].rotateMatrix=glm::rotate(glm::mat4(1.0f),rotate.z*3.14159f/180.f,glm::vec3(0.0f,0.0f,1.0f));
                VertexData[size-1].OrderMatrix[VertexData[size-1].number]=VertexData[size-1].rotateMatrix;
                VertexData[size-1].number++;
            }

        }
}

/////////////////////////////////////////////////////
//  int
/////////////////////////////////////////////////////
void init(void) {
    
    
     /****Shader loading****/
     ShaderInfo  shaders[] = {
     { GL_VERTEX_SHADER, "viewer.vert" },
     { GL_FRAGMENT_SHADER, "viewer.frag" },
     { GL_NONE, NULL }
     };
    
    program = LoadShaders(shaders);
    
    /****Uniform variables declarations****/
    render_scene_uniforms.model_matrix = glGetUniformLocation(program, "model_matrix");
    render_scene_uniforms.view_matrix = glGetUniformLocation(program, "view_matrix");
    render_scene_uniforms.projection_matrix = glGetUniformLocation(program, "projection_matrix");
    render_scene_uniforms.light_position = glGetUniformLocation(program, "light_position");
    render_scene_uniforms.condition = glGetUniformLocation(program,"condition");
    
    /**********  MVP  ************/

    //Perspective matrix
    Projection=glm::frustum(-1.0f,1.0f,-1.0f,1.0f,3.0f,100.f);
    /******Transformations end here*****/
    
    //Light Vector Calculation
    light_vector=glm::vec3(0.0f,0.0f,1.0f);
    
    //View matrix
    eyepoint=glm::vec3(3*maxX,3*maxY,1.0*maxZ);
    focalpoint=glm::vec3(midX,midY,midZ);
    viewUp=glm::vec3(0.0f,0.0f,1.0f);
    view_matrix = glm::lookAt(eyepoint,focalpoint,viewUp);
    
    //Buffer initialization
    for(int p=size-1;p>=0;p--){
        
    glGenVertexArrays(p+1, VAOs);
    glBindVertexArray( VAOs[p]);
    
    glGenBuffers(p+1, Buffers );
    glBindBuffer( GL_ARRAY_BUFFER, Buffers[p] );
    
    glBufferData( GL_ARRAY_BUFFER,4*VertexData[p].size*sizeof(glm::vec3),NULL,GL_STATIC_DRAW );
    glBufferSubData(GL_ARRAY_BUFFER, 0, VertexData[p].size*sizeof(glm::vec3),VertexData[p].VerticesArray);
    glBufferSubData(GL_ARRAY_BUFFER, VertexData[p].size*sizeof(glm::vec3), VertexData[p].size*sizeof(glm::vec3),VertexData[p].NormalArray);
    glBufferSubData(GL_ARRAY_BUFFER, 2*VertexData[p].size*sizeof(glm::vec3), VertexData[p].size*sizeof(glm::vec3),VertexData[p].KaArray);
    glBufferSubData(GL_ARRAY_BUFFER, 3*VertexData[p].size*sizeof(glm::vec3), VertexData[p].size*sizeof(glm::vec3),VertexData[p].KdArray);
    
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vPosition);
    
    glVertexAttribPointer( vnormal, 3, GL_FLOAT, GL_FALSE, 0,(const GLvoid *)(VertexData[p].size*sizeof(glm::vec3)));
    glEnableVertexAttribArray(vnormal);
    
    glVertexAttribPointer(vambient, 3, GL_FLOAT, GL_FALSE, 0,(const GLvoid *)(2*VertexData[p].size*sizeof(glm::vec3)));
    glEnableVertexAttribArray(vambient);
    
    glVertexAttribPointer(vdiffuse, 3, GL_FLOAT, GL_FALSE, 0,(const GLvoid *)(3*VertexData[p].size*sizeof(glm::vec3)));
    glEnableVertexAttribArray(vdiffuse);
    
    }
    
    /***Blender***/
    num_blend_funcs = sizeof(blend_func) / sizeof(blend_func[0]);
    num_add_funcs = sizeof(additive)/sizeof(additive[0]);
    num_mult_funcs= sizeof(multiplicative)/sizeof(multiplicative[0]);
    
    /****Buffering initializations****/
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    glClearDepth(1.0f);
    glClear(GL_DEPTH_BUFFER_BIT);
}

////////////////////////////////////////////////////////////////////
//	display
////////////////////////////////////////////////////////////////////

void display ()
{
    glClearColor(0.0,0.0,0.0,0.0);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glUseProgram(program);
    
    //Sending the uniform variable value to the shader
    
    glUniformMatrix4fv(render_scene_uniforms.view_matrix, 1, GL_FALSE, glm::value_ptr(view_matrix));
    glUniformMatrix4fv(render_scene_uniforms.projection_matrix, 1, GL_FALSE, glm::value_ptr(Projection));
    glUniform3fv(render_scene_uniforms.light_position, 1, glm::value_ptr(light_vector));
    glProgramUniform1f(program,render_scene_uniforms.condition,false);
    
    glLineWidth(width);

    for(int p=0;p<size;p++)
    {

        VertexData[p].modelMatrix=glm::mat4(1.0f);

        for(int i=0;i<VertexData[p].number;i++)
        {
            VertexData[p].modelMatrix=VertexData[p].OrderMatrix[i] * VertexData[p].modelMatrix;
        }
        
        glUniformMatrix4fv(render_scene_uniforms.model_matrix,1,GL_FALSE,glm::value_ptr(VertexData[p].modelMatrix));
        glBindVertexArray(VAOs[p] );
        glDrawArrays( GL_TRIANGLES, 0, VertexData[p].size);

    }

	glFlush();
}


////////////////////////////////////////////////////////////////////
//	SpinDisplay
////////////////////////////////////////////////////////////////////
void spinDisplay ()
{

    glProgramUniform1f(program,render_scene_uniforms.condition,true);
    
    if(blendMode==MENU_BLEND){
    if(i==num_blend_funcs-1)
    {
        j+=1;i=0;
    }
    
    else if(j==num_blend_funcs-1 && i==num_blend_funcs-1)
    {
        cout<<"Starting over again"<<endl;
        j=0;i=0;
    }
    
    else
    i+=1;
        
         glBlendFunc(blend_func[i],blend_func[j]);
    }
    
    else if(blendMode==MENU_MULT){
        if(num_mult_funcs-1==i)
        {
            i=0;
        }
        
        else
        {
            i+=1;
        }
        
        //glBlendFunc(multiplicative[i],GL_DST_COLOR);
        
        
        if(strcmp(PathName,"ObjFiles/ControlFile12.txt")==0)
           glBlendFunc(multiplicative[i],multiplicative[4]);
           
        else
          glBlendFunc(multiplicative[i],multiplicative[3]);
    }
    

    else if(blendMode==MENU_ADD){
        if(num_add_funcs-1==i)
        {
            i=0;
        }
        else
        {
            i+=1;
        }
    
        glBlendFunc(additive[i],additive[0]);
    }
    
        for(int p=0;p<size;p++)
        {
        
        VertexData[p].modelMatrix=glm::mat4(1.0f);
        
        for(int i=0;i<VertexData[p].number;i++)
        {
            
            VertexData[p].modelMatrix=VertexData[p].OrderMatrix[i] * VertexData[p].modelMatrix;
        }
        
        glUniformMatrix4fv(render_scene_uniforms.model_matrix,1,GL_FALSE,glm::value_ptr(VertexData[p].modelMatrix));
        glBindVertexArray(VAOs[p] );
        glDrawArrays( GL_TRIANGLES, 0, VertexData[p].size);
        
    }
    glutPostRedisplay();
}

////////////////////////////////////////////////////////////////////
//	Timer_func
////////////////////////////////////////////////////////////////////

void timer_func(int n)
{
    
    spinDisplay();
    
    if(animating)
    glutTimerFunc(n, timer_func, n);
}


////////////////////////////////////////////////////////////////////
//	reshape
////////////////////////////////////////////////////////////////////
void Reshape(int width, int height)
{
    
    current_width = width;
    current_height=height;
    
    aspect = float(height) / float(width);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glViewport(0, 0 , width, height);

    glm::frustum(-1.0f,1.0f,-aspect,aspect,0.5f,1000.0f);
    
    glMatrixMode(GL_MODELVIEW);
    
}
////////////////////////////////////////////////////////////////////
//	SelectFromMenu(int id)
////////////////////////////////////////////////////////////////////

void SelectFromMenu(int id)
{
    switch(id)
    {
        case MENU_BLEND:
        {
            /**Blender**/
            glEnable(GL_POINT_SMOOTH);
            glEnable(GL_LINE_SMOOTH);
            glEnable(GL_POLYGON_SMOOTH);
            
            glEnable(GL_BLEND);
            glDisable(GL_CULL_FACE);
            glDisable(GL_DEPTH_TEST);
            glDepthMask(GL_FALSE);
            
            i=0,j=0;
            glBlendColor(0.2f, 0.5f, 0.7f, 0.5f);
            static const GLfloat one = 1.0f;
            glClearBufferfv(GL_DEPTH, 0, &one);
            
            if(!animating)
                animating=true;
            else
                animating=false;
            
            blendMode=MENU_BLEND;
            timer_func(700);
            
            break;
        }
         
        case MENU_ADD:
        {
            /**Blender**/
            glEnable(GL_POINT_SMOOTH);
            glEnable(GL_LINE_SMOOTH);
            glEnable(GL_POLYGON_SMOOTH);
            
            glEnable(GL_BLEND);
            glDisable(GL_CULL_FACE);
            glDisable(GL_DEPTH_TEST);
            
            i=0,j=0;
            glBlendColor(0.2f, 0.5f, 0.7f, 0.5f);
            static const GLfloat one = 1.0f;
            glClearBufferfv(GL_DEPTH, 0, &one);
            
            blendMode=MENU_ADD;
            
            if(!animating)
                animating=true;
            else
                animating=false;
            
            timer_func(700);
            break;
        }

        case MENU_MULT:
        {
            /**Blender**/
            glEnable(GL_POINT_SMOOTH);
            glEnable(GL_LINE_SMOOTH);
            glEnable(GL_POLYGON_SMOOTH);
            
            glEnable(GL_BLEND);
            glDisable(GL_CULL_FACE);
            glDisable(GL_DEPTH_TEST);
            
            i=0,j=0;
            glBlendColor(0.2f, 0.5f, 0.7f, 0.5f);
            static const GLfloat one = 1.0f;
            glClearBufferfv(GL_DEPTH, 0, &one);
            
            blendMode=MENU_MULT;
            
            if(!animating)
                animating=true;
            else
                animating=false;
            
            timer_func(700);
            break;
        }

        
        case MENU_RESET:
        {
            eyepoint=glm::vec3(3*maxX,3*maxY,1.0*maxZ);
            focalpoint=glm::vec3(midX,midY,midZ);
            viewUp=glm::vec3(0.0f,0.0f,1.0f);
            view_matrix = glm::lookAt(eyepoint,focalpoint,viewUp);
            angle=0.0f;
            move1=glm::vec3(0.0f,0.0f,0.0f);
            move2=glm::vec3(0.0f,0.0f,0.0f);
            move3=glm::vec3(0.0f,0.0f,0.0f);
            
            break;
        }
        case MENU_POLYMODE:
        {
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            
            break;
        }
            
        case MENU_WIRE_FRAME:
        {
            cout<<"Enter the line width:";
            cin>>width;
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
            glLineWidth(width);
            
            break;
        }
        
        case MENU_KEY_LEFT:
        {
            if(angle<0.0f)
                angle=-angle;
            angle+=1.0f;
            
            eyepoint=glm::rotateZ(eyepoint,angle*3.14159f/180.f);
            focalpoint=glm::rotateZ(focalpoint, angle*3.14159f/180.f);
            view_matrix = glm::lookAt(eyepoint,focalpoint,viewUp);

            
            break;
        }
            
        case MENU_KEY_RIGHT:
        {
            if(angle>0.0f)
            angle=-angle;
            angle-=1.0f;
            
            eyepoint=glm::rotateZ(eyepoint,angle*3.14159f/180.f);
            focalpoint=glm::rotateZ(focalpoint, angle*3.14159f/180.f);
            view_matrix = glm::lookAt(eyepoint,focalpoint,viewUp);
            
            break;
        }
            
        case MENU_ROTATE_CCW:
        {
            angle-=1.0f;
            viewUp=glm::rotateZ((focalpoint-eyepoint),angle*3.14159f/180.f);
            view_matrix = glm::lookAt(eyepoint,focalpoint,viewUp);
            
            break;
        }
            
        case MENU_ROTATE_CW:
        {
            
            angle+=1.0f;
            viewUp=glm::rotateZ((focalpoint-eyepoint),angle*3.14159f/180.f);
            view_matrix = glm::lookAt(eyepoint,focalpoint,viewUp);

            break;
        }
            
        case MENU_KEY_DOWN:
        {
            move1= glm::normalize(focalpoint-eyepoint);
            eyepoint-=move1;
            focalpoint-=move1;
            view_matrix = glm::lookAt(eyepoint,focalpoint,viewUp);
            
            break;
        }
            
        case MENU_KEY_UP:
        {
            move1=glm::normalize(focalpoint-eyepoint);
            eyepoint+=move1;
            focalpoint+=move1;
            view_matrix = glm::lookAt(eyepoint,focalpoint,viewUp);
            
            break;
        }
            
        case MENU_CAMERA_DOWN:
        {
            move2=glm::normalize(viewUp);
            eyepoint=eyepoint-move2;
            view_matrix = glm::lookAt(eyepoint,focalpoint,viewUp);

            break;
        }
            
        case MENU_CAMERA_UP:
        {
            move2=glm::normalize(viewUp);
            eyepoint=eyepoint+move2;
            view_matrix = glm::lookAt(eyepoint,focalpoint,viewUp);

            break;
        }
            
        case MENU_FC_OPP:
        {
            move3=glm::normalize(viewUp);
            focalpoint=focalpoint+move3;
            view_matrix = glm::lookAt(eyepoint,focalpoint,viewUp);

            break;
        }
            
        case MENU_FC_UP:
        {
            move3=glm::normalize(viewUp);
            focalpoint=focalpoint-move3;
            view_matrix = glm::lookAt(eyepoint,focalpoint,viewUp);

            break;
        }
            
        case MENU_EXIT:
        {
            exit(0);
            
            break;
        }

    }
    
       glutPostRedisplay();
}

////////////////////////////////////////////////////////////////////
//	Special keys
////////////////////////////////////////////////////////////////////
void specialKeys(int key,int x,int y)
{
    switch(key){
        case GLUT_KEY_UP:
        {
            SelectFromMenu(MENU_KEY_UP);
            break;
        }
        case GLUT_KEY_DOWN:
        {
            SelectFromMenu(MENU_KEY_DOWN);
            break;
        }
            
        case GLUT_KEY_LEFT:
        {
            SelectFromMenu(MENU_KEY_LEFT);
            break;
        }
        
        case GLUT_KEY_RIGHT:
        {
            SelectFromMenu(MENU_KEY_RIGHT);
            break;
        }

    }
}

////////////////////////////////////////////////////////////////////
//	keyboard
////////////////////////////////////////////////////////////////////
void keyboard(unsigned char ch, int x,int y){
    
    switch(ch){
        case 'b':
        case 'B':
                {
                    SelectFromMenu(MENU_BLEND);
                    break;
                }
            
        case 'a':
        case 'A':
        {
            SelectFromMenu(MENU_ADD);
            break;
        }
            
        case 'm':
        case 'M':
        {
            SelectFromMenu(MENU_MULT);
            break;
        }


        case 'r':
        case 'R':
                {
                    SelectFromMenu(MENU_RESET);
                    break;
                }
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
        
        case 'c':
        case 'C':
                {
                    SelectFromMenu(MENU_CAMERA_DOWN);
                    break;
                }
            
        case 'V':
        case 'v':
                {
                    SelectFromMenu(MENU_CAMERA_UP);
                    break;
                }
        case 'd':
        case 'D':
                {
                    SelectFromMenu(MENU_FC_OPP);
                    break;
                }
            
        case 'f':
        case 'F':
                {
                    SelectFromMenu(MENU_FC_UP);
                    break;
                }
        
        case 'z':
        case 'Z':
                {
                    SelectFromMenu(MENU_ROTATE_CCW);
                    break;
                }
        
        case 'x':
        case 'X':
                {
                    SelectFromMenu(MENU_ROTATE_CW);
                    break;
                }
        
        case 'q':
        case 'Q':
        case 27: exit(0);
                break;
            
        default: cout<<"Input Entry Invalid!!! Please try again!!!"<<endl;

    }
}

////////////////////////////////////////////////////////////////////////
//	BuildMenu
////////////////////////////////////////////////////////////////////////
int BuildMenu(void){
    
    int menu;
    
    menu= glutCreateMenu(SelectFromMenu);

    glutAddMenuEntry ("Press S for rendering the object as solid surface ",MENU_POLYMODE);
    glutAddMenuEntry ("Press W for wireframe display ",MENU_WIRE_FRAME);
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
    glutInitDisplayMode( GLUT_3_2_CORE_PROFILE|GLUT_RGBA|GLUT_DEPTH);
    
    #else
    glutInitContextVersion( 4, 3 );
    glutInitContextProfile( GLUT_CORE_PROFILE );
    glutInitDisplayMode( GLUT_RGBA |GLUT_DEPTH);
    glEnable(GL_DEPTH_TEST);
    #endif
    
    glutInitWindowSize( 512, 512 );
	glutCreateWindow( "Blender");

    
	glewExperimental = GL_TRUE;	// added for glew to work!
	if ( glewInit() )
	{
		cerr << "Unable to initialize GLEW ... exiting" << endl;
		exit (EXIT_FAILURE );
	}
    
    PathName=argv[2];
    read(argv[2]);
    init();
    
    cout<<endl;
    cout<< "****************************** MENU ***************************"<<endl;
    cout<< "2. Enter 'r/R' to reset"<<endl;
    cout<< "2. Enter 's/S' to render all the objects as solid surfaces"<<endl;
    cout<< "3. Enter 'w/W' to render all the objects in wireframe"<<endl;
    
    glutReshapeFunc(Reshape);
    glutDisplayFunc( display );
    
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(keyboard);
    BuildMenu();
    glutAttachMenu (GLUT_RIGHT_BUTTON);
    
	glutMainLoop();

	return 0;
}


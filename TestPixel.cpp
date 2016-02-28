#include <iostream>
#include "Vector3.hpp"
#include "Vector3.cpp"
#include "Vector4.hpp"
#include "Vector4.cpp"
#include "Matrices.h"
#include "GLUT/glut.h"
#include "math.h"
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <vector>
//#include "FileReader.cpp"
#define F_PATH "queen"

using namespace std;




Vector3 c(5,0.1,5);     //the position of the camera  5,0,5 queen     cow 1,0,10
Vector3 c1(-c.x,-c.y,-c.z);
Vector3 lookUpPoint(0,0,0);
Vector3 w;

/***use the keyboard to control the camera ***/
void changeCamera(int key,int x,int y)
{
    switch (key) {
        case GLUT_KEY_RIGHT: //right arrow
            c.x=c.x+0.1;
            break;
        case GLUT_KEY_LEFT: //left arrow
            c.x=c.x-0.1;
            break;
        case GLUT_KEY_UP: //up arrow
            c.y=c.y+0.1;
            break;
        case GLUT_KEY_DOWN: //down arrow
            c.y=c.y-0.1;
            break;
        case 'w':
            c.z=c.z+0.1;
            break;
        case 's':
            c.z=c.z-0.1;
            break;
        case 27:
            exit(0);
        default:
            break;
    }
    glutPostRedisplay();
}

/***Matrix for View***/
Matrix4 View(){
    Vector3 u;
    Vector3 v;
    
    Vector3 temp(0,1,0);
    
    
    w=lookUpPoint-c;
    w.normalize();
    
    u=temp.crossProduct(w);
    u.normalize();
    
    v=w.crossProduct(u);
    v.normalize();
    
    Matrix4 rotation;
    rotation.identity();
    
    
    rotation.setColumn(0, u);
    rotation.setColumn(1, v);
    rotation.setColumn(2, w);
    
    
    Matrix4 translation;
    
    
    translation.setRow(3,c1);
    
    Matrix4 view;
    view=rotation*translation;
    
    return view;
}

/***Matrix for Pers***/
Matrix4 Pers(){
    float d=1.0;
    float f=200.0;
    float h=1.0;
    
    float temp_a=d/h;
    float temp_b=f/(f-d);
    float temp_c=-d*f/(f-d);
    
    Matrix4 pers;
    
    
    
    pers.set(temp_a, 0, 0, 0,
             0,temp_a, 0, 0,
             0,0,temp_b,temp_c,
             0,0, 1,0);
    
    return pers;
}
/***Matrix for Mode***/
Matrix4 Model(){
    Matrix4 model;
    model.set(1, 0, 0, 0,
              0, 1, 0, 0,
              0, 0, 1, 0,
              0, 0, 0, 1);
    
    
    return model;
}
/***Multiply all the Matrix***/
Matrix4 Integrated_Matrix(){
    Matrix4 igm;
    Matrix4 view=View();
    Matrix4 pers=Pers();
    Matrix4 model=Model();
    
    igm=view*pers*model;
    
    return igm;
}

//the method use to color the pixel
void draw_pixel(float x,float y,float red,float green,float blue)
{
    glPointSize(1);
    glBegin(GL_POINTS);
     glColor3f(red, green, blue);
     glVertex2f((float)x/1000.0,(float)y/1000.0);
    glEnd();
    
    glFlush();
}

//the scan-line method
void edgedetect(float x1,float y1,float x2,float y2,float *le,float *re,int *flag,int *overindex)
{
    float temp,x,mx;
    float i;
    *overindex=0;//y1<0,y2>0时，y2的x下标是紧接着y1的，但实质上应该从0开始，所以像素描点的时候要减去y1绝对值后的最大x坐标
    
    x1=x1*1000;
    x2=x2*1000;
    y1=y1*1000;
    y2=y2*1000;
    if(y1>y2)
    {
        temp=x1,x1=x2,x2=temp;
        temp=y1,y1=y2,y2=temp;
    }
    
    if(y1==y2)
        mx=x2-x1;
    else
        mx=(x2-x1)/(y2-y1);
    
    x=x1;
    
    cout<<"y1:"<<y1<<endl;
    
    /**
        It is simple to use the index of the array to represent the y value of a pixel, Howver, some y values are negative, The key is how to deal with negative value.
        Simply solve the problem by coloring ever polygon and clear the color buffer. The index represent the y value and the value of the array represents the x value.
        Three conditions, all y values of the polygon are negative. Then use the fabs to process. All y values are positive, just store it. Some y values are negative, some
        are positive. The use a value to make a shift with those values.
     **/
    if(y1<0&&y2<=0)
    {
        *flag=1;
    }
    
    if(y1<0&&y2>0)
    {
        *flag=2;
    }
    
    
    if(y1>=0)
    {
        for(i=y1;i<=y2;i++)
        {
            if(x<le[(int)i])
            {
                le[(int)i]=x;
            }
            if(re[(int)i]==0||x>re[(int)i]){
                re[(int)i]=x;
            }
            
            x+=mx;
        }
    }
    
    
    if(y1<0&&y2<=0)
    {
        for(i=y1;i<=y2;i++)
        {
            if(x<le[abs((int)i)])
            {
                le[abs((int)i)]=x;
            }
            if(re[abs((int)i)]==0||x>re[abs((int)i)]){
                re[abs((int)i)]=x;
            }
            
            x+=mx;
        }
    }
    
    if(y1<0&&y2>0)
    {
        for(i=y1;i<0;i++)
        {
            if(x<le[abs((int)i)])
            {
                le[abs((int)i)]=x;
            }
            if(re[abs((int)i)]==0||x>re[abs((int)i)]){
                re[abs((int)i)]=x;
            }
            x+=mx;
            *overindex=abs((int)y1);
        }
        
        for(i=0;i<=y2;i++)
        {
            if(x<le[(int)i+*overindex])
            {
                le[(int)i+*overindex]=x;
            }
            if(re[(int)i+*overindex]==0||x>re[(int)i+*overindex]){
                re[(int)i+*overindex]=x;
            }
            
            x+=mx;
        }
        
    }
    
}

/***Use to change the coordinates of the vertices and draw the Graph ***/
void Change_Vertex(void){
    /** Initial the zBuffer
     **/
    vector<vector<float>> zBuffer;
    vector<float> temp_zBuffer;
    for(int i=0;i<2000;i++)
    {
        temp_zBuffer.push_back(10000.0);
    }
    for(int j=0;j<2000;j++){
        zBuffer.push_back(temp_zBuffer);
    }
    
    glClearColor(1.0, 1.0, 1.0,0.0);
    
    
    
    /*********It is for file reading**********************/
    ifstream f(F_PATH);
    int numberOfVertices;
    int numberOfPolygons;
    int edegeOfPolyons;
    string temp;
    
    if(!f)
    {
        cout<<"No File!"<<endl;
        return ;
    }
    string s;
    
    
    getline(f, s);
    
    istringstream is(s);
    
    is>>temp;
    is>>temp;
    numberOfVertices=atoi(temp.c_str());
    
    is>>temp;
    numberOfPolygons=atoi(temp.c_str());
    
    
    
    float vertex_list[numberOfVertices][4];
    float edege_list[numberOfPolygons][20];
//    float vertex_list[100000][4];
//    float edege_list[100000][20];
    
    
    for(int i=0;i<numberOfVertices;i++){
        getline(f, s);
        istringstream is(s);
        is>>temp;
        vertex_list[i][0]=atof(temp.c_str());
        is>>temp;
        vertex_list[i][1]=atof(temp.c_str());
        is>>temp;
        vertex_list[i][2]=atof(temp.c_str());
        vertex_list[i][3]=1;
    }
    
    
    
    for(int i=0;i<=numberOfPolygons-1;i++){
        getline(f,s);
        istringstream is(s);
        is>>temp;
        edegeOfPolyons=atoi(temp.c_str());
        
        int j=0;
        for(j;j<=edegeOfPolyons-1;j++){
            is>>temp;
            edege_list[i][j]=atoi(temp.c_str());
        }
        
        for(j;j<=19;j++){
            edege_list[i][j]=-1;
        }
    }
    
    
    /**********file read ending *************************/
    
    //draw the 3D Graph
    
    Matrix4 finalMatrix=Integrated_Matrix();
    int range=(sizeof(vertex_list)/sizeof(vertex_list[0][0])/4);
    int edege_range=sizeof(edege_list)/sizeof(edege_list[0][0])/20-1;//
    float end_Vertex[range][3];
    
    Vector4 m;
    Vector4 tempVector;
    
    w=c-lookUpPoint;
    w.normalize();
    
    for(int i=0;i<range;i++){
        m.x=vertex_list[i][0];
        m.y=vertex_list[i][1];
        m.z=vertex_list[i][2];
        m.w=vertex_list[i][3];
        tempVector=finalMatrix*m;
        tempVector.x=tempVector.x/tempVector.w;
        tempVector.y=tempVector.y/tempVector.w;
        tempVector.z=tempVector.z/tempVector.w;
        
        
        
        end_Vertex[i][0]=tempVector.x;
        end_Vertex[i][1]=tempVector.y;
        end_Vertex[i][2]=tempVector.z;//tempVector.z;
        
    }
    
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    int tag=0;
    /*%%%%*/
    float le[1000],re[1000],tempZ;
    int i,j;      int flag;/*determine the negative value*/
    int overIndex; //when y1<0 and y2>0, try to make the fabs(y1),Howver,every y2 valus becomes y2+fabs(y1),so try this is the value of fabs(y1)
    
    for(i=0;i<1000;i++)
        le[i]=1000,re[i]=0;
    
    
    
    /*%%%%*/
    srand((unsigned)time(NULL));
    
    for(int x=0;x<=edege_range;++x)
    {
        flag=0;
        /**It is for hiding back face*****/
        Vector3 temp;
        Vector3 temp_a(vertex_list[(int)edege_list[x][1]-1][0]-vertex_list[(int)edege_list[x][0]-1][0],vertex_list[(int)edege_list[x][1]-1][1]-vertex_list[(int)edege_list[x][0]-1][1],vertex_list[(int)edege_list[x][1]-1][2]-vertex_list[(int)edege_list[x][0]-1][2]);
        Vector3 temp_b(vertex_list[(int)edege_list[x][2]-1][0]-vertex_list[(int)edege_list[x][1]-1][0],vertex_list[(int)edege_list[x][2]-1][1]-vertex_list[(int)edege_list[x][1]-1][1],vertex_list[(int)edege_list[x][2]-1][2]-vertex_list[(int)edege_list[x][1]-1][2]);
        Vector3 normal_vector=temp_a.crossProduct(temp_b);
        temp.x=vertex_list[(int)edege_list[x][0]-1][0]-c.x;
        temp.y=vertex_list[(int)edege_list[x][0]-1][1]-c.y;
        temp.z=vertex_list[(int)edege_list[x][0]-1][2]-c.z;
        
        if(normal_vector.dot(temp)<0){
            continue;
        }
        /***end for hiding back face***/
        
        /***use the OPENGL function to disply the graph***/
        
        
 //       glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_LINES);
        //glBegin(GL_POINTS);
        int j;
        for(j=0;j<19;j++)
        {
            if(edege_list[x][j+1]<0){  //when reach the end of the edege array
                break;
            }
            
            glVertex3fv(end_Vertex[(int)edege_list[x][j]-1]);
            glVertex3fv(end_Vertex[(int)edege_list[x][j+1]-1]);
            
            
            edgedetect(end_Vertex[(int)edege_list[x][j]-1][0],end_Vertex[(int)edege_list[x][j]-1][1],end_Vertex[(int)edege_list[x][j+1]-1][0],end_Vertex[(int)edege_list[x][j+1]-1][1],le,re,&flag,&overIndex);
        }
        glVertex3fv(end_Vertex[(int)edege_list[x][j]-1]);
        glVertex3fv(end_Vertex[(int)edege_list[x][0]-1]);
        
            edgedetect(end_Vertex[(int)edege_list[x][j]-1][0],end_Vertex[(int)edege_list[x][j]-1][1],end_Vertex[(int)edege_list[x][0]-1][0],end_Vertex[(int)edege_list[x][0]-1][1],le,re,&flag,&overIndex);
        tempZ=end_Vertex[(int)edege_list[x][j]-1][2]*1000;
        glEnd();

        cout<<"flag:"<<flag<<endl;
        cout<<"tempZ:"<<tempZ<<endl;
        
        
        float red=(float)rand()/RAND_MAX;
        float green=(float)rand()/RAND_MAX;
        float blue=(float)rand()/RAND_MAX;
//        cout<<"red:"<<red<<"green:"<<green<<"blue:"<<blue<<endl;
        
        for(j=0;j<1000;j++)
        {
            if(le[j]<=re[j])
            {
                if(flag==0)
                {
                    for(i=le[j];i<re[j];i++)
                    {
                        if(tempZ<=zBuffer[(int)i+1000][(int)j+1000])
                        {
                            draw_pixel(i,j,red,green,blue);
                            zBuffer[(int)i+1000][(int)j+1000]=tempZ;
                        }
                    }
                }
                if(flag==1)
                {
                    for(i=le[j];i<re[j];i++)
                    {
                        if(tempZ<=zBuffer[(int)i+1000][(int)-j+1000])
                        {
                            draw_pixel(i,-j,red,green,blue);
                            zBuffer[(int)i+1000][(int)-j+1000]=tempZ;
                        }
                    }
                }
                if(flag==2)
                {
                    if(j<overIndex)
                    {
                        for(i=le[j];i<re[j];i++)
                        {
                            if(tempZ<=zBuffer[(int)i+1000][(int)-j+1000])
                            {
                                draw_pixel(i,-j,red,green,blue);
                                zBuffer[(int)i+1000][(int)-j+1000]=tempZ;
                            }
                        }
                    }
                    else{
                        for(i=le[j];i<re[j];i++)
                        {
                            if(tempZ<=zBuffer[(int)i+1000][(int)j-overIndex+1000])
                            {
                                draw_pixel(i,j-overIndex,red,green,blue);
                                zBuffer[(int)i+1000][(int)j-overIndex+1000]=tempZ;
                            }
                        }
                    }
                    
                }
            }
        }
        for(i=0;i<1000;i++)
            le[i]=1000,re[i]=0;
    }
    
    
    glFlush();
    
    
}


void init()
{
    glClearColor(1.0,1.0,1.0,1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0,1000.0,0.0,1000.0);
}



int main(int argc,char **argv)
{
    /* code */
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(1000,1000);
    glutCreateWindow("It is a 3D Graph");
    glutDisplayFunc(Change_Vertex);
    glutSpecialFunc(changeCamera);
    // init();
    glutMainLoop();
    
    return 0;
}

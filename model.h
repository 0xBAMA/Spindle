//******************************************************************************
//  Program: Interactive Computer Graphics Project 1
//
//  Author: Jon Baker
//  Email: jb239812@ohio.edu
//
//  Description: This file contains the definitions of the classes used to
//    represent the shapes in this project. There are three different ones,
//    each of which declares their own VAO and buffer, compile their own shaders
//    and manage their own uniform variables. When the display() function is
//    called from the display callback in main(), glUseProgram() is called,
//    along with all the associated bindings of the buffers and vertex
//    attributes.
//
//  Date: 20 September 2019
//******************************************************************************


#include <vector>
#include <iostream>
using std::cout;
using std::endl;

#include <random>



// #include "Angel.h" //needed on the prime machines. (is it?)


//**********************************************

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>


// GLUT
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>


// Shader Compilation
#include "shaders/Shader.h"

//**********************************************



#define GLM_FORCE_SWIZZLE
#include "glm/glm.hpp" //general vector types
#include "glm/gtc/matrix_transform.hpp" // for glm::ortho
#include "glm/gtc/type_ptr.hpp" //to send matricies gpu-side


//******************************************************************************
//  Function: planetest
//
//  Purpose:
//      Return true if the test point is below the plane. Return false if the
//      test point is below the plane. Above and below are determined with
//      respect to the normal specified by plane_norm. This is used to confirm
//      that computed normals are actually pointing outwards, by testing a
//      point that is known to be inside the shape against the computed normal.
//      If the point is below the plane specified by plane_norm and plane_point,
//      we know that that normal will be valid. Otherwise, it needs to be
//      inverted.
//
//  Parameters:
//      plane_point - the point from which the normal will originate
//      plane_norm - what direction is 'up'?
//      test_point - you want to know if this is above or below the plane
//
//  Preconditions:
//      plane_norm must be given as a nonzero vector
//
//  Postconditions:
//      true or false is returned to tell the user the restult of their query
//
//******************************************************************************
bool planetest(glm::vec3 plane_point, glm::vec3 plane_norm, glm::vec3 test_point)
{
  double result, a, b, c, x, x1, y, y1, z, z1;

  a  =  plane_norm.x;   b  =  plane_norm.y;  c  =  plane_norm.z;
  x  =  test_point.x;   y  =  test_point.y;  z  =  test_point.z;
  x1 = plane_point.x;   y1 = plane_point.y;  z1 = plane_point.z;

  //equation of a plane is:
    // a (x-x1) + b (y-y1) + c (z-z1) = 0;

  result = a * (x-x1) + b * (y-y1) + c * (z-z1);

  return (result < 0) ? true:false;
}


//******************************************************************************
//  Class: BallModel
//
//  Purpose:  To represent a ball on the GPU, and everything that goes along
//        with displaying this ball to the user.
//
//  Functions:
//
//    Constructor:
//        Takes no arguments, calls generate_points() to create geometry. Then
//        buffers all this data to the GPU memory.
//
//    Setters:
//        Used to update the values of the uniform variables.
//
//    Generate Points:
//        Creates an icosahedron, subdivides the faces once, normalizes and
//        scales them to make an icosphere. This data is used to populate the
//        vectors containing points, normals, and color data.
//
//    Display:
//        Makes sure the correct shader is being used, that the correct buffers
//        are bound, that the vertex attributes are set up, and that all the
//        latest values of the uniform variables are sent to the GPU.
//******************************************************************************


class BallModel
{
public:

  BallModel();

  void display();

  void set_time(int tin) {time = tin;}
  void set_proj(glm::mat4 pin) {proj = pin;}

private:
  GLuint vao;
  GLuint buffer;

  GLuint shader_program;

//VERTEX ATTRIB LOCATIONS
  GLuint vPosition;
  GLuint vNormal;
  GLuint vColor;

//UNIFORM LOCATIONS
  GLuint uTime;
  GLuint uProj;

//VALUES OF THOSE UNIFORMS
  int time;
  glm::mat4 proj;

  void generate_points();

  std::vector<glm::vec3> points;    //add the 1.0 w value in the shader
  std::vector<glm::vec3> normals;
  std::vector<glm::vec3> colors;
};

  //****************************************************************************
  //  Function: BallModel Constructor
  //
  //  Purpose:
  //    Calls generate_points() and then sets up everything related to the GPU
  //****************************************************************************


BallModel::BallModel()
{

  //initialize all the vectors
  points.clear();
  colors.clear();
  normals.clear();

  //fill those vectors with geometry
  generate_points();


//SETTING UP GPU STUFF
  //VAO
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  //BUFFER, SEND DATA
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);

  int num_bytes_points = sizeof(glm::vec3) * points.size();
  int num_bytes_normals = sizeof(glm::vec3) * normals.size();
  int num_bytes_colors = sizeof(glm::vec3) * colors.size();

  glBufferData(GL_ARRAY_BUFFER, num_bytes_points + num_bytes_normals + num_bytes_colors, NULL, GL_STATIC_DRAW);

  glBufferSubData(GL_ARRAY_BUFFER, 0, num_bytes_points, &points[0]);
  glBufferSubData(GL_ARRAY_BUFFER, num_bytes_points, num_bytes_normals, &normals[0]);
  glBufferSubData(GL_ARRAY_BUFFER, num_bytes_points + num_bytes_normals, num_bytes_colors, &colors[0]);

  //SHADERS (COMPILE, USE)


  // shader_program = InitShader("shaders/vSphere.glsl", "shaders/fSphere.glsl");

  Shader s("shaders/vSphere.glsl", "shaders/fSphere.glsl");

  shader_program = s.Program;

  glUseProgram(shader_program);

  //VERTEX ATTRIB AND UNIFORM LOCATIONS

  // Initialize the vertex position attribute from the vertex shader
  vPosition = glGetAttribLocation(shader_program, "vPosition");
  glEnableVertexAttribArray(vPosition);
  glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, ((GLvoid*) (0)));

  vNormal = glGetAttribLocation(shader_program, "vNormal");
  glEnableVertexAttribArray(vNormal);
  glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, ((GLvoid*) (sizeof(glm::vec3) * points.size())));

  vColor = glGetAttribLocation(shader_program, "vColor");
  glEnableVertexAttribArray(vColor);
  glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, ((GLvoid*) (sizeof(glm::vec3) * (points.size() + normals.size()))));


  uTime = glGetUniformLocation(shader_program, "t");
  glUniform1i(uTime, time);

  uProj = glGetUniformLocation(shader_program, "proj");
  proj = glm::ortho(-1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f);
  glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(proj));

}

  //****************************************************************************
  //  Function: BallModel::generate_points()
  //
  //  Purpose:
  //    This function produces all the data for representing this object.
  //****************************************************************************

void BallModel::generate_points()
{
  //GENERATING GEOMETRY

    double phi = (1 + std::sqrt(5.0))/2.0;  //golden ratio, used to compute icosahedron
    glm::vec3 a,b,c,d,e,f,g,h,i,j,k,l;  //store the vectors representing the verticies

  //This is based on the idea that the points of a regular icosahedron lie on the
  //points defined by three mutually orthogonal golden rectangles that share a
  //center point at the origin i.e. these rectanges are abcd, efgh and ijkl
    a = glm::vec3(  0,  1,  phi); e = glm::vec3(  1,  phi,  0); i = glm::vec3(  phi,  0,  1);
    b = glm::vec3(  0,  1, -phi); f = glm::vec3( -1, -phi,  0); j = glm::vec3(  phi,  0, -1);
    c = glm::vec3(  0, -1,  phi); g = glm::vec3( -1,  phi,  0); k = glm::vec3( -phi,  0,  1);
    d = glm::vec3(  0, -1, -phi); h = glm::vec3(  1, -phi,  0); l = glm::vec3( -phi,  0, -1);

    typedef struct tri_t{
      glm::vec3 p0, p1, p2;
      glm::vec3 norm;
    } triangle;

    std::vector<triangle> trivec;

    trivec.resize(20);

    //the faces of this shape are defined as follows:
    trivec[ 0].p0 = a; trivec[ 0].p1 = g; trivec[ 0].p2 = e; trivec[ 0].norm = glm::cross(a-g,g-e);//AGE
    trivec[ 1].p0 = a; trivec[ 1].p1 = i; trivec[ 1].p2 = e; trivec[ 1].norm = glm::cross(a-i,i-e);//AIE
    trivec[ 2].p0 = a; trivec[ 2].p1 = c; trivec[ 2].p2 = i; trivec[ 2].norm = glm::cross(a-c,c-i);//ACI
    trivec[ 3].p0 = a; trivec[ 3].p1 = c; trivec[ 3].p2 = k; trivec[ 3].norm = glm::cross(a-c,c-k);//ACK
    trivec[ 4].p0 = a; trivec[ 4].p1 = g; trivec[ 4].p2 = k; trivec[ 4].norm = glm::cross(a-g,g-k);//AGK
    trivec[ 5].p0 = l; trivec[ 5].p1 = b; trivec[ 5].p2 = g; trivec[ 5].norm = glm::cross(l-b,b-g);//LBG
    trivec[ 6].p0 = l; trivec[ 6].p1 = g; trivec[ 6].p2 = k; trivec[ 6].norm = glm::cross(l-g,g-k);//LGK
    trivec[ 7].p0 = l; trivec[ 7].p1 = f; trivec[ 7].p2 = k; trivec[ 7].norm = glm::cross(l-f,f-k);//LFK
    trivec[ 8].p0 = l; trivec[ 8].p1 = d; trivec[ 8].p2 = f; trivec[ 8].norm = glm::cross(l-d,d-f);//LDF
    trivec[ 9].p0 = l; trivec[ 9].p1 = d; trivec[ 9].p2 = b; trivec[ 9].norm = glm::cross(l-d,d-b);//LDB
    trivec[10].p0 = k; trivec[10].p1 = f; trivec[10].p2 = c; trivec[10].norm = glm::cross(k-f,f-c);//KFC
    trivec[11].p0 = f; trivec[11].p1 = h; trivec[11].p2 = c; trivec[11].norm = glm::cross(f-h,h-c);//FHC
    trivec[12].p0 = h; trivec[12].p1 = i; trivec[12].p2 = c; trivec[12].norm = glm::cross(h-i,i-c);//HIC
    trivec[13].p0 = e; trivec[13].p1 = j; trivec[13].p2 = i; trivec[13].norm = glm::cross(e-j,j-i);//EJI
    trivec[14].p0 = b; trivec[14].p1 = g; trivec[14].p2 = e; trivec[14].norm = glm::cross(b-g,g-e);//BGE
    trivec[15].p0 = f; trivec[15].p1 = h; trivec[15].p2 = d; trivec[15].norm = glm::cross(f-h,h-d);//FHD
    trivec[16].p0 = d; trivec[16].p1 = h; trivec[16].p2 = j; trivec[16].norm = glm::cross(d-h,h-j);//DHJ
    trivec[17].p0 = d; trivec[17].p1 = b; trivec[17].p2 = j; trivec[17].norm = glm::cross(d-b,b-j);//DBJ
    trivec[18].p0 = b; trivec[18].p1 = j; trivec[18].p2 = e; trivec[18].norm = glm::cross(b-j,j-e);//BJE
    trivec[19].p0 = h; trivec[19].p1 = i; trivec[19].p2 = j; trivec[19].norm = glm::cross(h-i,i-j);//HIJ

    for (int i = 0; i < 20; i++)
    {
      //is the origin 'under' the plane containing the triangle? if not, correct
      trivec[i].norm = planetest(trivec[i].p0, trivec[i].norm, glm::vec3(0.0f, 0.0f, 0.0f)) ? trivec[i].norm : (-1.0f * trivec[i].norm);
    }



    //generate the points for the ball
    //these are triangles, making up the faces of an icosahedron

  //random number generation
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(0.35f, 0.75f);
    //example usage for random number generation:
    // for (int i=0; i<16; ++i)
    //         std::cout << dist(mt) << "\n";

    glm::vec3 p0t, p1t, p2t, nt; //temporary storage

    float rad_scale = 0.45f; //used to scale the radius

    for(auto tri : trivec)
    {
      //each triangle is three points - we are going to want to make it into 4 triangles,
      //by finding the midpoint of each edge.

      /* i.e.
           /\                 /\
          /  \               /__\
         /    \   becomes   /\  /\
        /______\           /__\/__\   */

      //the color actually holds the vector to the middle of the triangle - this
      //stores the value of the normal of the triangle. Also, the alpha channel is
      //a random number which will be used to control the animation.
      //details on what this means are in the vertex shader and the writeup

      //first - made of p0 and the other two edges midpoints.
      p0t = rad_scale * glm::normalize(tri.p0);
      p1t = rad_scale * glm::normalize((tri.p0+tri.p2)/2.0f);
      p2t = rad_scale * glm::normalize((tri.p0+tri.p1)/2.0f);
      nt = glm::normalize(tri.norm);

      points.push_back(p0t);
      normals.push_back(nt);
      colors.push_back(glm::vec3(0.1f, dist(mt), 0.3f));

      points.push_back(p1t);
      normals.push_back(nt);
      colors.push_back(glm::vec3(0.1f, dist(mt), 0.3f));

      points.push_back(p2t);
      normals.push_back(nt);
      colors.push_back(glm::vec3(0.1f, dist(mt), 0.3f));

      //second - made of p1 and the other two edges midpoints
      p0t = rad_scale * glm::normalize(tri.p1);
      p1t = rad_scale * glm::normalize((tri.p1+tri.p0)/2.0f);
      p2t = rad_scale * glm::normalize((tri.p1+tri.p2)/2.0f);

      points.push_back(p0t);
      normals.push_back(nt);
      colors.push_back(glm::vec3(0.1f, dist(mt), 0.3f));

      points.push_back(p1t);
      normals.push_back(nt);
      colors.push_back(glm::vec3(0.1f, dist(mt), 0.3f));

      points.push_back(p2t);
      normals.push_back(nt);
      colors.push_back(glm::vec3(0.1f, dist(mt), 0.3f));

      //third - made of p2 and the other two edges midpoints
      p0t = rad_scale * glm::normalize(tri.p2);
      p1t = rad_scale * glm::normalize((tri.p2+tri.p1)/2.0f);
      p2t =  rad_scale * glm::normalize((tri.p2+tri.p0)/2.0f);

      points.push_back(p0t);
      normals.push_back(nt);
      colors.push_back(glm::vec3(0.1f, dist(mt), 0.3f));

      points.push_back(p1t);
      normals.push_back(nt);
      colors.push_back(glm::vec3(0.1f, dist(mt), 0.3f));

      points.push_back(p2t);
      normals.push_back(nt);
      colors.push_back(glm::vec3(0.1f, dist(mt), 0.3f));

      //fourth - made of the three edges midpoints
      p0t = rad_scale * glm::normalize((tri.p2+tri.p0)/2.0f);
      p1t = rad_scale * glm::normalize((tri.p1+tri.p0)/2.0f);
      p2t = rad_scale * glm::normalize((tri.p1+tri.p2)/2.0f);

      points.push_back(p0t);
      normals.push_back(nt);
      colors.push_back(glm::vec3(0.1f, dist(mt), 0.3f));

      points.push_back(p1t);
      normals.push_back(nt);
      colors.push_back(glm::vec3(0.1f, dist(mt), 0.3f));

      points.push_back(p2t);
      normals.push_back(nt);
      colors.push_back(glm::vec3(0.1f, dist(mt), 0.3f));
    }
}



  //****************************************************************************
  //  Function: BallModel::display()
  //
  //  Purpose:
  //    This function does all the setup for the buffers and uniforms and then
  //    issues a draw call for the geometry representing this object
  //****************************************************************************

void BallModel::display()
{
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glUseProgram(shader_program);

  glEnableVertexAttribArray(vPosition);
  glEnableVertexAttribArray(vNormal);
  glEnableVertexAttribArray(vColor);

  glUniform1i(uTime, time);
  glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(proj));

  glDrawArrays(GL_TRIANGLES, 0, 240); // there are 240 verticies for the 80 tris
}

//******************************************************************************
//  Class: CageModel
//
//  Purpose:  To represent the skeleton of the ball on the GPU, and everything
//        that goes along with displaying this part of the ball to the user.
//
//  Functions:
//
//    Constructor:
//        Takes no arguments, calls generate_points() to create geometry. Then
//        buffers all this data to the GPU memory.
//
//    Setters:
//        Used to update the values of the uniform variables.
//
//    Generate Points:
//        Creates an icosahedron, subdivides the faces once, normalizes and
//        scales them to make an icosphere. This data is used to populate the
//        vectors that hold the points and color data.
//
//    Display:
//        Makes sure the correct shader is being used, that the correct buffers
//        are bound, that the vertex attributes are set up, and that all the
//        latest values of the uniform variables are sent to the GPU.
//******************************************************************************

class CageModel
{
public:

  CageModel();

  void display();

  void set_time(int tin) {time = tin;}
  void set_proj(glm::mat4 pin) {proj = pin;}

private:
  GLuint vao;
  GLuint buffer;

  GLuint shader_program;

//VERTEX ATTRIB LOCATIONS
  GLuint vPosition;
  GLuint vColor;
  GLuint vNormal;

//UNIFORM LOCATIONS
  GLuint uTime;
  GLuint uProj;

//VALUES OF THOSE UNIFORMS
  int time;
  glm::mat4 proj;

  void generate_points();

  std::vector<glm::vec3> points;    //add the 1.0 w value in the shader
  std::vector<glm::vec3> colors;
};



  //****************************************************************************
  //  Function: CageModel Constructor
  //
  //  Purpose:
  //    Calls generate_points() and then sets up everything related to the GPU
  //****************************************************************************


CageModel::CageModel()
{
  //initialize all the vectors
  points.clear();
  colors.clear();

  //fill those vectors with geometry
  generate_points();


  //SETTING UP GPU STUFF
    //VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //BUFFER, SEND DATA
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    int num_bytes_points = sizeof(glm::vec3) * points.size();
    int num_bytes_colors = sizeof(glm::vec3) * colors.size();

    glBufferData(GL_ARRAY_BUFFER, num_bytes_points + num_bytes_colors, NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, num_bytes_points, &points[0]);
    glBufferSubData(GL_ARRAY_BUFFER, num_bytes_points, num_bytes_colors, &colors[0]);

    //SHADERS (COMPILE, USE)
    // shader_program = InitShader("shaders/vCage.glsl", "shaders/fCage.glsl");

    Shader s("shaders/vCage.glsl", "shaders/fCage.glsl");

    shader_program = s.Program;

    glUseProgram(shader_program);

    //VERTEX ATTRIB AND UNIFORM LOCATIONS

    // Initialize the vertex position attribute from the vertex shader
    vPosition = glGetAttribLocation(shader_program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, ((GLvoid*) (0)));

    vColor = glGetAttribLocation(shader_program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, ((GLvoid*) (sizeof(glm::vec3) * points.size())));


    uTime = glGetUniformLocation(shader_program, "t");
    glUniform1i(uTime, time);

    uProj = glGetUniformLocation(shader_program, "proj");
    proj = glm::ortho(-1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(proj));


}

  //****************************************************************************
  //  Function: CageModel::generate_points()
  //
  //  Purpose:
  //    This function produces all the data for representing this object.
  //****************************************************************************

void CageModel::generate_points()
{
  //GENERATING GEOMETRY

    double phi = (1 + std::sqrt(5.0))/2.0;  //golden ratio, used to compute icosahedron
    glm::vec3 a,b,c,d,e,f,g,h,i,j,k,l;  //store the vectors representing the verticies

  //This is based on the idea that the points of a regular icosahedron lie on the
  //points defined by three mutually orthogonal golden rectangles that share a
  //center point at the origin i.e. these rectanges are abcd, efgh and ijkl
    a = glm::vec3(  0,  1,  phi); e = glm::vec3(  1,  phi,  0); i = glm::vec3(  phi,  0,  1);
    b = glm::vec3(  0,  1, -phi); f = glm::vec3( -1, -phi,  0); j = glm::vec3(  phi,  0, -1);
    c = glm::vec3(  0, -1,  phi); g = glm::vec3( -1,  phi,  0); k = glm::vec3( -phi,  0,  1);
    d = glm::vec3(  0, -1, -phi); h = glm::vec3(  1, -phi,  0); l = glm::vec3( -phi,  0, -1);

    typedef struct tri_t{
      glm::vec3 p0, p1, p2;
      glm::vec3 norm;
    } triangle;

    std::vector<triangle> trivec;

    trivec.resize(20);

    //the faces of this shape are defined as follows:
    trivec[ 0].p0 = a; trivec[ 0].p1 = g; trivec[ 0].p2 = e; trivec[ 0].norm = glm::cross(a-g,g-e);//AGE
    trivec[ 1].p0 = a; trivec[ 1].p1 = i; trivec[ 1].p2 = e; trivec[ 1].norm = glm::cross(a-i,i-e);//AIE
    trivec[ 2].p0 = a; trivec[ 2].p1 = c; trivec[ 2].p2 = i; trivec[ 2].norm = glm::cross(a-c,c-i);//ACI
    trivec[ 3].p0 = a; trivec[ 3].p1 = c; trivec[ 3].p2 = k; trivec[ 3].norm = glm::cross(a-c,c-k);//ACK
    trivec[ 4].p0 = a; trivec[ 4].p1 = g; trivec[ 4].p2 = k; trivec[ 4].norm = glm::cross(a-g,g-k);//AGK
    trivec[ 5].p0 = l; trivec[ 5].p1 = b; trivec[ 5].p2 = g; trivec[ 5].norm = glm::cross(l-b,b-g);//LBG
    trivec[ 6].p0 = l; trivec[ 6].p1 = g; trivec[ 6].p2 = k; trivec[ 6].norm = glm::cross(l-g,g-k);//LGK
    trivec[ 7].p0 = l; trivec[ 7].p1 = f; trivec[ 7].p2 = k; trivec[ 7].norm = glm::cross(l-f,f-k);//LFK
    trivec[ 8].p0 = l; trivec[ 8].p1 = d; trivec[ 8].p2 = f; trivec[ 8].norm = glm::cross(l-d,d-f);//LDF
    trivec[ 9].p0 = l; trivec[ 9].p1 = d; trivec[ 9].p2 = b; trivec[ 9].norm = glm::cross(l-d,d-b);//LDB
    trivec[10].p0 = k; trivec[10].p1 = f; trivec[10].p2 = c; trivec[10].norm = glm::cross(k-f,f-c);//KFC
    trivec[11].p0 = f; trivec[11].p1 = h; trivec[11].p2 = c; trivec[11].norm = glm::cross(f-h,h-c);//FHC
    trivec[12].p0 = h; trivec[12].p1 = i; trivec[12].p2 = c; trivec[12].norm = glm::cross(h-i,i-c);//HIC
    trivec[13].p0 = e; trivec[13].p1 = j; trivec[13].p2 = i; trivec[13].norm = glm::cross(e-j,j-i);//EJI
    trivec[14].p0 = b; trivec[14].p1 = g; trivec[14].p2 = e; trivec[14].norm = glm::cross(b-g,g-e);//BGE
    trivec[15].p0 = f; trivec[15].p1 = h; trivec[15].p2 = d; trivec[15].norm = glm::cross(f-h,h-d);//FHD
    trivec[16].p0 = d; trivec[16].p1 = h; trivec[16].p2 = j; trivec[16].norm = glm::cross(d-h,h-j);//DHJ
    trivec[17].p0 = d; trivec[17].p1 = b; trivec[17].p2 = j; trivec[17].norm = glm::cross(d-b,b-j);//DBJ
    trivec[18].p0 = b; trivec[18].p1 = j; trivec[18].p2 = e; trivec[18].norm = glm::cross(b-j,j-e);//BJE
    trivec[19].p0 = h; trivec[19].p1 = i; trivec[19].p2 = j; trivec[19].norm = glm::cross(h-i,i-j);//HIJ

    //generate the points for the ball
    //these are triangles, making up the faces of an icosahedron

  //random number generation
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(0.3f, 0.75f);
    //example usage for random number generation:
    // for (int i=0; i<16; ++i)
    //         std::cout << dist(mt) << "\n";

    glm::vec3 p0t, p1t, p2t; //temporary storage

    float rad_scale = 0.45f; //used to scale the radius

    //generate the points for the ball's cage
    //these are the outlines of the trianglular faces of the ball
    // they are, however, translated outwards, slightly, with respect to the
    // surface of the ball, and there are a number of layered copies


    std::uniform_real_distribution<float> dist2(-0.1f, 0.1f);


    for (int i = 0; i < 3; i++)
    {
      rad_scale *= 1.01f; //move outwards slightly

      //points
      for(auto tri : trivec)
      {
        p0t = rad_scale * glm::normalize(tri.p0);
        p1t = rad_scale * glm::normalize((tri.p0+tri.p2)/2.0f);
        p2t = rad_scale * glm::normalize((tri.p0+tri.p1)/2.0f);

        //three sides of the triangle
        points.push_back(p0t);
        points.push_back(p1t);

        points.push_back(p1t);
        points.push_back(p2t);

        points.push_back(p0t);
        points.push_back(p2t);

        p0t = rad_scale * glm::normalize(tri.p1);
        p1t = rad_scale * glm::normalize((tri.p1+tri.p0)/2.0f);
        p2t = rad_scale * glm::normalize((tri.p1+tri.p2)/2.0f);

        points.push_back(p0t);
        points.push_back(p1t);

        points.push_back(p1t);
        points.push_back(p2t);

        points.push_back(p0t);
        points.push_back(p2t);

        p0t = rad_scale * glm::normalize(tri.p2);
        p1t = rad_scale * glm::normalize((tri.p2+tri.p1)/2.0f);
        p2t =  rad_scale * glm::normalize((tri.p2+tri.p0)/2.0f);

        points.push_back(p0t);
        points.push_back(p1t);

        points.push_back(p1t);
        points.push_back(p2t);

        points.push_back(p0t);
        points.push_back(p2t);

        p0t = rad_scale * glm::normalize((tri.p2+tri.p0)/2.0f);
        p1t = rad_scale * glm::normalize((tri.p1+tri.p0)/2.0f);
        p2t = rad_scale * glm::normalize((tri.p1+tri.p2)/2.0f);

        points.push_back(p0t);
        points.push_back(p1t);

        points.push_back(p1t);
        points.push_back(p2t);

        points.push_back(p0t);
        points.push_back(p2t);

      }


      // colors
      for(int j = 0; j < 480; j++)
      {
        switch(i)
        {
          case 0: //first pass  - like a grey blue
            colors.push_back(glm::vec3(0.5f + dist2(mt), 0.5f + dist2(mt), 0.9f + dist2(mt)));
            break;
          case 1: //second pass - similar to the background
            colors.push_back(glm::vec3((1/phi) + dist2(mt), (1/phi) + dist2(mt), (1/phi) + dist2(mt)));
            break;
          case 2: //third pass  - sort of a yellow
            colors.push_back(glm::vec3(0.85f + dist2(mt), 0.9f + dist2(mt), 0.0f));
            break;
        }
      }
    }
}


  //****************************************************************************
  //  Function: CageModel::display()
  //
  //  Purpose:
  //    This function does all the setup for the buffers and uniforms and then
  //    issues a draw call for the geometry representing this object
  //****************************************************************************

void CageModel::display()
{
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glUseProgram(shader_program);

  glEnableVertexAttribArray(vPosition);
  glEnableVertexAttribArray(vColor);

  glUniform1i(uTime, time);
  glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(proj));

  //there are 80 triangles, each have 3 sides
  //with this information, we can derive that there are 240 lines per layer, 480 points

  glLineWidth(4.0f);
  glDrawArrays(GL_LINES, 0, 480);

  glLineWidth(2.0f);
  glDrawArrays(GL_LINES, 480, 640);

  glLineWidth(1.0f);
  glDrawArrays(GL_LINES, 640, 800); //I'm doing something wrong here, it should be 3*480=1440

}



//******************************************************************************
//  Class: SerpinskiModel
//
//  Purpose:  To represent the serpinski model inside the ball on the GPU, and
//        everything that goes along with displaying this part of the ball to
//        the user.
//
//  Functions:
//
//    Constructor:
//        Takes no arguments, calls generate_points() to create geometry. Then
//        buffers all this data to the GPU memory.
//
//    Setters:
//        Used to update the values of the uniform variables.
//
//    Generate Points:
//        Uses the algorithm discussed in class to generate points on a
//        3d serpinski gasket.
//
//    Display:
//        Makes sure the correct shader is being used, that the correct buffers
//        are bound, that the vertex attributes are set up, and that all the
//        latest values of the uniform variables are sent to the GPU.
//******************************************************************************

class SerpinskiModel
{
public:

  SerpinskiModel();

  void display();

  void set_time(int tin) {time = tin;}
  void set_proj(glm::mat4 pin) {proj = pin;}


private:
  GLuint vao;
  GLuint buffer;

  GLuint shader_program;

//VERTEX ATTRIB LOCATIONS
  GLuint vPosition;
  GLuint vColor;
  GLuint vNormal;

//UNIFORM LOCATIONS
  GLuint uTime;
  GLuint uProj;

//VALUES OF THOSE UNIFORMS
  int time;
  glm::mat4 proj;

  void generate_points();

  std::vector<glm::vec3> points;    //add the 1.0 w value in the shader
};



  //****************************************************************************
  //  Function: SerpinskiModel Constructor
  //
  //  Purpose:
  //    Calls generate_points() and then sets up everything related to the GPU
  //****************************************************************************


SerpinskiModel::SerpinskiModel()
{
  //initialize all the vectors
  points.clear();

  //fill those vectors with geometry
  generate_points();


  //SETTING UP GPU STUFF
    //VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //BUFFER, SEND DATA
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    int num_bytes_points = sizeof(glm::vec3) * points.size();

    glBufferData(GL_ARRAY_BUFFER, num_bytes_points, NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, num_bytes_points, &points[0]);

    //SHADERS (COMPILE, USE)
    // shader_program = InitShader("shaders/vSerp.glsl", "shaders/fSerp.glsl");

    Shader s("shaders/vSerp.glsl", "shaders/fSerp.glsl");

    shader_program = s.Program;

    glUseProgram(shader_program);

    //VERTEX ATTRIB AND UNIFORM LOCATIONS

    // Initialize the vertex position attribute from the vertex shader
    vPosition = glGetAttribLocation(shader_program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, ((GLvoid*) (0)));

    uTime = glGetUniformLocation(shader_program, "t");
    glUniform1i(uTime, time);

    uProj = glGetUniformLocation(shader_program, "proj");
    proj = glm::ortho(-1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(proj));


}

  //****************************************************************************
  //  Function: SerpinskiModel::generate_points()
  //
  //  Purpose:
  //    This function produces all the data for representing this object.
  //****************************************************************************

void SerpinskiModel::generate_points()
{
  //GENERATING GEOMETRY

  float scale = 0.25f;

  //INITIAL VERTICIES
  glm::vec3 initial_verticies[4] = { scale * glm::vec3(1.0f, 1.0f, 1.0f),
    scale * glm::vec3(-1.0f, -1.0f,  1.0f),
    scale * glm::vec3(-1.0f,  1.0f, -1.0f),
    scale * glm::vec3( 1.0f, -1.0f, -1.0f)
  };

  points.push_back(glm::vec3(0.15f, 0.15f, 0.15f));   //initial point

  for(int i = 1; i < 20001; i++)
  {
    int j = rand() % 4; //pick a random vertex

    //compute the halfway point between the selected vertex and previous point
    points.push_back((points[i-1] + initial_verticies[j]) / 2.0f);
  }

}




  //****************************************************************************
  //  Function: SerpinskiModel::display()
  //
  //  Purpose:
  //    This function does all the setup for the buffers and uniforms and then
  //    issues a draw call for the geometry representing this object
  //****************************************************************************

void SerpinskiModel::display()
{
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glUseProgram(shader_program);

  glEnableVertexAttribArray(vPosition);

  glUniform1i(uTime, time);
  glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(proj));

  //there are 80 triangles, each have 3 sides
  //with this information, we can derive that there are 240 lines per layer, 480 points

  glPointSize(1.25f);
  glDrawArrays(GL_POINTS, 0, 20000);
}

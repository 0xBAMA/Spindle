// Jon Baker - the Spindle

//  the Spindle has four parts - the ball, the cage, the shaft, and the end caps.
// these are drawn as three separate pices, as they are defined using different
// primitive types.

#include <vector>

#include "Angel.h"

#include "glm/glm.hpp"
//general vector types



void init()
{

  std::vector<glm::vec4> colors;    //I'm using alpha
  std::vector<glm::vec3> points;    //add the 1.0 in the shader

  int index = 0;


  // the ball has 20 faces, subdivided once -
  //    80 triangles, 320 vec3s
  // the ball's cage has the same number of triangles, but each gets 3 lines,
  // since I'm not doing anything fancy to eliminate duplicates -
  //   5*80 triangles, i.e. 1200 lines, which is 2400 vec3s
  // the spindle consists of stacked triangles - TBD - plus 8 triangles on each
  // end, so the known part right now is
  //   4*2 triangles, i.e. 24 vec3s

  // the serpinski action consists of 10 sets of 100 points.
  //    that's just 1000 points.

  //so total you've got 320 + 2400 + 24 + 1000 = 3744 points.

  colors.resize(3744);
  points.resize(3744);

  // Create a vertex array object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create and initialize a buffer object
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);

  // First, we create an empty buffer of the correct size
  glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_STATIC_DRAW);





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
  } triangle;

  std::vector<triangle> trivec;

  trivec.resize(20);

  //the faces of this shape are defined as follows:
  //AGE
    trivec[0].p0 = a; trivec[0].p1 = g; trivec[0].p2 = e;
  //AIE
    trivec[1].p0 = a; trivec[1].p1 = i; trivec[1].p2 = e;
  //ACI
    trivec[2].p0 = a; trivec[2].p1 = c; trivec[2].p2 = i;
  //ACK
  //AGK
  //LBG
  //LGK
  //LFK
  //LDF
  //LDB
  //KFC
  //FHC
  //HIC
  //EJI
  //BGE
  //FHD
  //DHJ
  //DBJ
  //BJE
  //HIJ



  //generate the points for the ball
  //these are triangles, making up the faces of an icosahedron



  //generate the points for the ball's cage
  //these are the outlines of the trianglular faces of the black ball
  // they are, however, translated outwards, slightly, and there are 5 layered copies



  //generate the points for the spindles
  //these are the triangles that make up the connection between the ball and the
  // end cap, including the triangles that surround the serpinski action



  //generate the points for the serpinski action
  //this is just a few iterations of the algorithm that we covered in class
  //to generate some points in the shape. Because this element of the model
  //is kept relatively small, I am only going to use 100 points for each
  //as part of the animation, there is an index that is used in the display
  //function that tells it where to start drawing points (array index).
  //basically glDrawArrays(GL_TRIANGLES, serp_base + (rand()%10)*100, 100);















  // Next, we load the real data in parts.  We need to specify the
  //   correct byte offset for placing the color data after the point
  //   data in the buffer.  Conveniently, the byte offset we need is
  //   the same as the size (in bytes) of the points array, which is
  //   returned from "sizeof(points)".
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points[0]);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors[0]);














  // Load shaders and use the resulting shader program
  GLuint program = InitShader("shaders/vSpindle.glsl", "shaders/fSpindle.glsl");
  glUseProgram(program);


  // Initialize the vertex position attribute from the vertex shader
  GLuint vPosition = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(vPosition);
  glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, ((GLvoid*) (0)));

  // Likewise, initialize the vertex color attribute.  Once again, we
  //    need to specify the starting offset (in bytes) for the color
  //    data.  Just like loading the array, we use "sizeof(points)"
  //    to determine the correct value.
  GLuint vColor = glGetAttribLocation(program, "vColor");
  glEnableVertexAttribArray(vColor);
  glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, ((GLvoid*) (sizeof(points))));

















  glEnable(GL_DEPTH_TEST);

  glClearColor(1/phi, 1/phi, 1/phi, 1.0); // grey background
}

//----------------------------------------------------------------------------

extern "C" void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // glDrawArrays(GL_TRIANGLES, 0, NumVertices);
  glFlush();
}

//----------------------------------------------------------------------------

extern "C" void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case 033:
    exit(EXIT_SUCCESS);
    break;
  }
  glutPostRedisplay();
}

//----------------------------------------------------------------------------

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(512, 512);
  glutCreateWindow("Simple GLSL example");

  glewInit();

  init();

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);

  glutMainLoop();
  return(EXIT_SUCCESS);
}

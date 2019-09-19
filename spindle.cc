// Jon Baker - the Spindle

//  the Spindle has four parts - the ball, the cage, the shaft, and the end caps.
// these are drawn as three separate pices, as they are defined using different
// primitive types.

#include <vector>
#include <iostream>
using std::cout;
using std::endl;

#include <random>



#include "Angel.h"

#include "glm/glm.hpp" //general vector types
#include "glm/gtc/matrix_transform.hpp" // for glm::ortho
#include "glm/gtc/type_ptr.hpp" //to send matricies gpu-side


int animation_time = 0;
GLint time_loc;

glm::mat4 projection;
GLint proj_loc;

void init()
{

  std::vector<glm::vec4> colors;    //I'm using alpha
  std::vector<glm::vec3> points;    //add the 1.0 w value in the shader

  int index = 0;


  // the ball has 20 faces, subdivided once -
  //    80 triangles, 240 vec3s
  // the ball's cage has the same number of triangles, but each gets 3 lines,
  // since I'm not doing anything fancy to eliminate duplicates -
  //   5*80 triangles, i.e. 1200 lines, which is 2400 vec3s
  // the spindle consists of stacked triangles - TBD - plus 8 triangles on each
  // end, so the known part right now is
  //   4*2 triangles, i.e. 24 vec3s

  // the serpinski action consists of 10 sets of 100 points.
  //    that's just 1000 points.

  //so total you've got 240 + 2400 + 24 + 1000 = 3664 points.

  colors.resize(3664);
  points.resize(3664);







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
  //AGE
    trivec[ 0].p0 = a; trivec[ 0].p1 = g; trivec[ 0].p2 = e; trivec[ 0].norm = glm::cross(a-g,g-e);
  //AIE
    trivec[ 1].p0 = a; trivec[ 1].p1 = i; trivec[ 1].p2 = e; trivec[ 1].norm = glm::cross(a-i,i-e);
  //ACI
    trivec[ 2].p0 = a; trivec[ 2].p1 = c; trivec[ 2].p2 = i; trivec[ 2].norm = glm::cross(a-c,c-i);
  //ACK
    trivec[ 3].p0 = a; trivec[ 3].p1 = c; trivec[ 3].p2 = k; trivec[ 3].norm = glm::cross(a-c,c-k);
  //AGK
    trivec[ 4].p0 = a; trivec[ 4].p1 = g; trivec[ 4].p2 = k; trivec[ 4].norm = glm::cross(a-g,g-k);
  //LBG
    trivec[ 5].p0 = l; trivec[ 5].p1 = b; trivec[ 5].p2 = g; trivec[ 5].norm = glm::cross(l-b,b-g);
  //LGK
    trivec[ 6].p0 = l; trivec[ 6].p1 = g; trivec[ 6].p2 = k; trivec[ 6].norm = glm::cross(l-g,g-k);
  //LFK
    trivec[ 7].p0 = l; trivec[ 7].p1 = f; trivec[ 7].p2 = k; trivec[ 7].norm = glm::cross(l-f,f-k);
  //LDF
    trivec[ 8].p0 = l; trivec[ 8].p1 = d; trivec[ 8].p2 = f; trivec[ 8].norm = glm::cross(l-d,d-f);
  //LDB
    trivec[ 9].p0 = l; trivec[ 9].p1 = d; trivec[ 9].p2 = b; trivec[ 9].norm = glm::cross(l-d,d-b);
  //KFC
    trivec[10].p0 = k; trivec[10].p1 = f; trivec[10].p2 = c; trivec[10].norm = glm::cross(k-f,f-c);
  //FHC
    trivec[11].p0 = f; trivec[11].p1 = h; trivec[11].p2 = c; trivec[11].norm = glm::cross(f-h,h-c);
  //HIC
    trivec[12].p0 = h; trivec[12].p1 = i; trivec[12].p2 = c; trivec[12].norm = glm::cross(h-i,i-c);
  //EJI
    trivec[13].p0 = e; trivec[13].p1 = j; trivec[13].p2 = i; trivec[13].norm = glm::cross(e-j,j-i);
  //BGE
    trivec[14].p0 = b; trivec[14].p1 = g; trivec[14].p2 = e; trivec[14].norm = glm::cross(b-g,g-e);
  //FHD
    trivec[15].p0 = f; trivec[15].p1 = h; trivec[15].p2 = d; trivec[15].norm = glm::cross(f-h,h-d);
  //DHJ
    trivec[16].p0 = d; trivec[16].p1 = h; trivec[16].p2 = j; trivec[16].norm = glm::cross(d-h,h-j);
  //DBJ
    trivec[17].p0 = d; trivec[17].p1 = b; trivec[17].p2 = j; trivec[17].norm = glm::cross(d-b,b-j);
  //BJE
    trivec[18].p0 = b; trivec[18].p1 = j; trivec[18].p2 = e; trivec[18].norm = glm::cross(b-j,j-e);
  //HIJ
    trivec[19].p0 = h; trivec[19].p1 = i; trivec[19].p2 = j; trivec[19].norm = glm::cross(h-i,i-j);



  //generate the points for the ball
  //these are triangles, making up the faces of an icosahedron

//random number generation
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> dist(1.0, 30.0);
  //example usage for random number generation:
  // for (int i=0; i<16; ++i)
  //         std::cout << dist(mt) << "\n";

  glm::vec3 p0t, p1t, p2t, nt; //temporary storage
  glm::vec4 temp_col;

  float rad_scale = 0.45f; //used to scale the radius

  for(auto tri : trivec)
  {
    // cout << tri.p0.x << " " << tri.p0.y << " " << tri.p0.z << endl;
    // cout << tri.p1.x << " " << tri.p1.y << " " << tri.p1.z << endl;
    // cout << tri.p2.x << " " << tri.p2.y << " " << tri.p2.z << endl;
    // cout << endl;

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



    //old
    // temp_col = glm::vec4((tri.p0 + tri.p1 + tri.p2) / 3.0f, dist(mt));


    //first - made of p0 and the other two edges midpoints.
    p0t = rad_scale * glm::normalize(tri.p0);
    p1t = rad_scale * glm::normalize((tri.p0+tri.p2)/2.0f);
    p2t = rad_scale * glm::normalize((tri.p0+tri.p1)/2.0f);
    nt = rad_scale * glm::normalize(tri.norm);

    temp_col = glm::vec4(nt, dist(mt));

    // temp_col = glm::vec4((p0t + p1t + p2t)/3.0f, dist(mt));  //center point and a random number

    // cout << "p0t: " << p0t.x << " " << p0t.y << " " << p0t.z << endl;
    // cout << "p1t: " << p1t.x << " " << p1t.y << " " << p1t.z << endl;
    // cout << "p2t: " << p2t.x << " " << p2t.y << " " << p2t.z << endl;
    // cout << "col: " << temp_col.r << " " << temp_col.g << " " << temp_col.b << " " << temp_col.a << endl;
    // cout << endl;

    points[index] = p0t;
    colors[index] = temp_col;
    index++;
    points[index] = p1t;
    temp_col = glm::vec4(nt, dist(mt));
    colors[index] = temp_col;
    index++;
    points[index] = p2t;
    temp_col = glm::vec4(nt, dist(mt));
    colors[index] = temp_col;
    index++;


    //second - made of p1 and the other two edges midpoints
    p0t = rad_scale * glm::normalize(tri.p1);
    p1t = rad_scale * glm::normalize((tri.p1+tri.p0)/2.0f);
    p2t = rad_scale * glm::normalize((tri.p1+tri.p2)/2.0f);
    //
    // temp = glm::cross(p0t-p1t, p1t-p2t);
    // temp = 0.1f * rad_scale * glm::normalize(temp);
    //
    // temp_col = glm::vec4(temp, dist(mt));
    // temp_col = glm::vec4((p0t + p1t + p2t)/3.0f, dist(mt));  //center point and a random number

    // cout << "p0t: " << p0t.x << " " << p0t.y << " " << p0t.z << endl;
    // cout << "p1t: " << p1t.x << " " << p1t.y << " " << p1t.z << endl;
    // cout << "p2t: " << p2t.x << " " << p2t.y << " " << p2t.z << endl;
    // cout << "col: " << temp_col.r << " " << temp_col.g << " " << temp_col.b << " " << temp_col.a << endl;
    // cout << endl;

    points[index] = p0t;
    temp_col = glm::vec4(nt, dist(mt));
    colors[index] = temp_col;
    index++;
    points[index] = p1t;
    temp_col = glm::vec4(nt, dist(mt));
    colors[index] = temp_col;
    index++;
    points[index] = p2t;
    temp_col = glm::vec4(nt, dist(mt));
    colors[index] = temp_col;
    index++;


    //third - made of p2 and the other two edges midpoints
    p0t = rad_scale * glm::normalize(tri.p2);
    p1t = rad_scale * glm::normalize((tri.p2+tri.p1)/2.0f);
    p2t =  rad_scale * glm::normalize((tri.p2+tri.p0)/2.0f);


    // temp_col = glm::vec4((p0t + p1t + p2t)/3.0f, dist(mt));  //center point and a random number

    // cout << "p0t: " << p0t.x << " " << p0t.y << " " << p0t.z << endl;
    // cout << "p1t: " << p1t.x << " " << p1t.y << " " << p1t.z << endl;
    // cout << "p2t: " << p2t.x << " " << p2t.y << " " << p2t.z << endl;
    // cout << "col: " << temp_col.r << " " << temp_col.g << " " << temp_col.b << " " << temp_col.a << endl;
    // cout << endl;

    points[index] = p0t;
    temp_col = glm::vec4(nt, dist(mt));
    colors[index] = temp_col;
    index++;
    points[index] = p1t;
    temp_col = glm::vec4(nt, dist(mt));
    colors[index] = temp_col;
    index++;
    points[index] = p2t;
    temp_col = glm::vec4(nt, dist(mt));
    colors[index] = temp_col;
    index++;


    //fourth - made of the three edges midpoints
    p0t = rad_scale * glm::normalize((tri.p2+tri.p0)/2.0f);
    p1t = rad_scale * glm::normalize((tri.p1+tri.p0)/2.0f);
    p2t = rad_scale * glm::normalize((tri.p1+tri.p2)/2.0f);

    // temp_col = glm::vec4((p0t + p1t + p2t)/3.0f, dist(mt));  //center point and a random number

    // cout << "p0t: " << p0t.x << " " << p0t.y << " " << p0t.z << endl;
    // cout << "p1t: " << p1t.x << " " << p1t.y << " " << p1t.z << endl;
    // cout << "p2t: " << p2t.x << " " << p2t.y << " " << p2t.z << endl;
    // cout << "col: " << temp_col.r << " " << temp_col.g << " " << temp_col.b << " " << temp_col.a << endl;
    // cout << endl;

    points[index] = p0t;
    temp_col = glm::vec4(nt, dist(mt));
    colors[index] = temp_col;
    index++;
    points[index] = p1t;
    temp_col = glm::vec4(nt, dist(mt));
    colors[index] = temp_col;
    index++;
    points[index] = p2t;
    temp_col = glm::vec4(nt, dist(mt));
    colors[index] = temp_col;
    index++;

  }



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













  // Create a vertex array object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create and initialize a buffer object
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);


  int num_bytes_points = sizeof(glm::vec3) * points.size();
  int num_bytes_colors = sizeof(glm::vec4) * colors.size();

  cout << " points is " << num_bytes_points << " bytes" << endl;
  cout << " colors is " << num_bytes_colors << " bytes" << endl;

  // First, we create an empty buffer of the correct size
  // glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_STATIC_DRAW);
  glBufferData(GL_ARRAY_BUFFER, num_bytes_points + num_bytes_colors, NULL, GL_STATIC_DRAW);


  // Next, we load the real data in parts.  We need to specify the
  //   correct byte offset for placing the color data after the point
  //   data in the buffer.  Conveniently, the byte offset we need is
  //   the same as the size (in bytes) of the points array, which is
  //   returned from "sizeof(points)".



  // glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), &points[0]);
  // glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), &colors[0]);

  glBufferSubData(GL_ARRAY_BUFFER, 0, num_bytes_points, &points[0]);
  glBufferSubData(GL_ARRAY_BUFFER, num_bytes_points, num_bytes_colors, &colors[0]);













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






  //uniform int used to keep track of time

  time_loc = glGetUniformLocation(program, "t");
  glUniform1i(time_loc, animation_time);

  //orthographic projection matrix so I can handle non-square windows

  proj_loc = glGetUniformLocation(program, "proj");
  projection = glm::ortho(-1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f);
  glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(projection));














  glEnable(GL_DEPTH_TEST);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glClearColor(1/phi, 1/phi, 1/phi, 1.0); // grey background
}

//----------------------------------------------------------------------------

extern "C" void display()
{
  animation_time++;
  glUniform1i(time_loc, animation_time);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //draw the ball
  glDrawArrays(GL_TRIANGLES, 0, 240); // there are 240 verticies for the 80 tris

  //draw the lines around the ball

  //draw the triangles for the spindle

  //draw the points for the serpinski action





  glFlush();
  glutSwapBuffers();
  glutPostRedisplay();

}

//----------------------------------------------------------------------------

extern "C" void reshape(int width, int height)
{
  // projection = glm::ortho();

  glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(projection));

  cout << "width is " << width << " and height is " << height << endl;

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
  glutInitDisplayMode(GLUT_MULTISAMPLE | GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  // glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // no MSAA

  glutInitWindowSize(512, 512);
  glutCreateWindow("Spindle");

  glewInit();

  init();

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);

  glutMainLoop();
  return(EXIT_SUCCESS);
}

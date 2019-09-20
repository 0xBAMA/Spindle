//******************************************************************************
//  Program: Interactive Computer Graphics Project 1
//
//  Author: Jon Baker
//  Email: jb239812@ohio.edu
//
//  Description: This file contains GLUT specific code to open a window and
//       allow interaction. It instantiates models declared in model.h
//
//  Date: 20 September 2019
//******************************************************************************

#include "model.h"

int animation_time = 0;

//the models
BallModel*       ball;
CageModel*       cage;
SerpinskiModel*  serp;

void init()
{
  ball = new BallModel();
  cage = new CageModel();
  // serp = new SerpinskiModel();

  glEnable(GL_DEPTH_TEST);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_LINE_SMOOTH);

  double phi = (1 + std::sqrt(5.0))/2.0;  //golden ratio, used to compute icosahedron
  glClearColor(1/phi, 1/phi, 1/phi, 1.0); // grey background
}

//----------------------------------------------------------------------------

extern "C" void display()
{

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  animation_time++;

  ball->set_time(animation_time);
  cage->set_time(animation_time);
  // serp->set_time(animation_time);

  ball->display();
  cage->display();
  // serp->display();



  // //draw the ball
  // glUseProgram(ball_program);
  // glEnableVertexAttribArray(vPosition_ball);
  // glEnableVertexAttribArray(vColor_ball);
  // glUniform1i(time_loc_ball, animation_time);
  //
  //
  // glDrawArrays(GL_TRIANGLES, 0, 240); // there are 240 verticies for the 80 tris
  //
  //
  //
  //
  //
  // //draw the lines around the ball
  // glUseProgram(cage_program);
  // glEnableVertexAttribArray(vPosition_cage);
  // glEnableVertexAttribArray(vColor_cage);
  // glUniform1i(time_loc_cage, animation_time);
  //
  //
  //
  // //there are 80 triangles, each have 3 sides
  // //with this information, we can derive that there are 240 lines per layer, 480 points
  // glLineWidth(4.0f);
  // glDrawArrays(GL_LINES, 240, 480);
  //
  // glLineWidth(3.0f);
  // glDrawArrays(GL_LINES, 480, 720);
  //
  // glLineWidth(2.0f);
  // glDrawArrays(GL_LINES, 720, 960);
  //
  // glLineWidth(1.0f);
  // glDrawArrays(GL_LINES, 960, 1200);
  //








  //draw the points for the serpinski action





  glFlush();
  glutSwapBuffers();
  glutPostRedisplay();

}

//----------------------------------------------------------------------------
//
// extern "C" void reshape(int width, int height)
// {
//   // projection = glm::ortho();
//
//   // glUniformMatrix4fv(proj_loc_ball, 1, GL_FALSE, glm::value_ptr(projection));
//   // glUniformMatrix4fv(proj_loc_cage, 1, GL_FALSE, glm::value_ptr(projection));
//   //
//   // cout << "width is " << width << " and height is " << height << endl;
//
// }

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

  glutInitWindowSize(800, 800);
  glutCreateWindow("GLUT");

  glewInit();

  init();

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  // glutReshapeFunc(reshape);

  glutMainLoop();
  return(EXIT_SUCCESS);
}

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
  serp = new SerpinskiModel();

  glEnable(GL_DEPTH_TEST);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_LINE_SMOOTH);

  // double phi = (1 + std::sqrt(5.0))/2.0;  //golden ratio, used to compute icosahedron
  // glClearColor(1/phi, 1/phi, 1/phi, 1.0); // grey background

  glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
}

//----------------------------------------------------------------------------

extern "C" void display()
{

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  animation_time++;

  ball->set_time(animation_time);
  cage->set_time(animation_time);
  serp->set_time(animation_time);

  ball->display();
  cage->display();
  serp->display();

  glFlush();
  glutSwapBuffers();
  glutPostRedisplay();

}

//----------------------------------------------------------------------------

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


  glutInitContextVersion( 4, 5 );
	glutInitContextProfile( GLUT_CORE_PROFILE );

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

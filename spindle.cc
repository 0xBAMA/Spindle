//******************************************************************************
//  Program: Interactive Computer Graphics Project 1
//
//  Author: Jon Baker
//  Email: jb239812@ohio.edu
//
//  Description: This file contains GLUT specific code to open a window and
//       allow interaction. It instantiates model classes declared in model.h
//
//  Date: 20 September 2019
//******************************************************************************

#include "model.h"

int animation_time = 0;

//the models
BallModel*       ball;
CageModel*       cage;
SerpinskiModel*  serp;

//should you draw the models?
bool drawball = true;
bool drawcage = true;
bool drawserp = true;

bool rotate = true;
int temp_time = animation_time;

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


  if(rotate)
  {
    animation_time++;
    ball->set_time(animation_time);
    cage->set_time(animation_time);
    serp->set_time(animation_time);
  }

  if(drawball)
    ball->display();

  if(drawcage)
    cage->display();

  if(drawserp)
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


  case 'a':
    ball->toggle_depthcolor();
    break;
  case 's':
    cage->toggle_depthcolor();
    break;




  case 'z':
    //toggle drawing of ball
    drawball = !drawball;
    break;
  case 'x':
    //toggle drawing of cage
    drawcage = !drawcage;
    break;
  case 'c':
    //toggle drawing of serpinski action
    drawserp = !drawserp;
    break;
  case 'v':
    //stop or start the rotation;
    temp_time = animation_time;
    rotate = !rotate;
    break;
  case 'b':
    ball->toggle_scanlines();
    break;
  }
  glutPostRedisplay();
}

//----------------------------------------------------------------------------


// not working on laptop - probably an incompatibility with i3 display manager
//
// static int menu_id;
// static int submenu_id;
//
// extern "C" void menu(int choice)
// {
//   cout << choice << endl;
// }
//
// void create_menu()
// {
//   //this is some example code from https://www.programming-techniques.com/2012/05/glut-tutorial-creating-menus-and-submenus-in-glut.html
//   submenu_id = glutCreateMenu(menu);
//
//   glutAddMenuEntry("Sphere", 2);
//   glutAddMenuEntry("Cone", 3);
//   glutAddMenuEntry("Torus", 4);
//   glutAddMenuEntry("Teapot", 5);
//
//   menu_id = glutCreateMenu(menu);
//
//   glutAddMenuEntry("Clear", 1);
//   glutAddSubMenu("Draw", submenu_id);
//   glutAddMenuEntry("Quit", 0);
//
//   glutAttachMenu(GLUT_RIGHT_BUTTON);
// }


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

  // create_menu();

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  // glutReshapeFunc(reshape);

  glutMainLoop();
  return(EXIT_SUCCESS);
}

Jon Baker
9/20/2019

  First, I have zipped everything relevant here, because I'm using a vector
library that has 300-odd files in it. I saw the note that you shouldn't be
submitting more than 20 at a time, so I just went ahead and used
  'tar czvf project1.gz ./*' to compress it into one, plus this readme.

  Anyways, I implemented 3 separate classes which model different elements of
what is being displayed. Each of these classes creates a VAO, a buffer, compiles
shader programs specific to that class, and sets up vertex attributes. When
they are instantiated in the main file, they each generate all the geometry and
other data associated with each model and send it to the GPU. When this is done,
and all the callbacks are registered, we enter the GLUT main loop and begin to
render the shapes. In the display callback, each of these classes' display
functions are called - these functions call glUseProgram for that class's
shader, bind the relevant array object and buffer, and issue the draw calls
which are associated with that object.

  Each of these classes has a different number of vertex attributes, and the
shaders implement unique logic in each of them. For example, in the vSphere and
fSphere shaders, it implements rotation and scaling along the normal of the
triangular faces, in the vertex shader, and then scales the color by the
fragment depth in the fragment shader. This creates the outer layer of moving
triangles which can be seen in the animation. Next is the cage around this ball,
which uses the same set of verticies, but renders them as lines instead of
triangles. There are a few copies of this data, layered over one another, to
give a sort of stratified look to the cage around the ball. The last part of the
animation is based on the algorithm we covered in class to generate points that
lie on the 3d Serpinski gasket - this object slowly rotates and scales with a
sine function, but in addition to this, it also uses the built in variable
gl_VertexID to color the points in the order that they were produced by that
algorithm. By this, I mean that there is an input t, representing the number
of frames that have been rendered, essentially clock ticks, which I am then
modding by the number of points that were generated. If gl_VertexID is less than
the result of this mod operation, it is colored yellow instead of black - in
this way, it sort of shows the order in which those points were generated.
Because it is based on mod, it will cycle when this time value becomes greater
than the number of points.

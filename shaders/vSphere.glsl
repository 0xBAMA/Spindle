#version 330

in  vec3 vPosition;
in  vec3 vNormal;
in  vec3 vColor;
out vec4 color;

uniform int t;
uniform mat4 proj;


float ball_rot;
float spin_rot;


//thanks to Neil Mendoza via http://www.neilmendoza.com/glsl-rotation-about-an-arbitrary-axis/
mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;

    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}


void main()
{

  vec3 rot_axis = vec3(1.0f, 1.0f, 0.0f);


  //gl_Position = rotationMatrix(rot_axis, 0.01f * t) * proj * vec4(vPosition+(((t%50)/5)*0.1*vColor.xyz), 1.0);



  float scale = 0.055 * sin(0.05*t);


  //vColor.xyz is the displacement vector - add it to the position before rotation
  vec3 vPosition_local = (scale * vNormal) + vPosition;

  gl_Position = rotationMatrix(rot_axis, 0.0075f * t) * proj * vec4(vPosition_local, 1.0f);


  //color = vec4(gl_VertexID/500.0f + 0.3f, gl_VertexID/500.0f + 0.32f, gl_VertexID/500.0f + 0.3f, 1.0f);

  color = vec4(vColor, 0.3f);

}

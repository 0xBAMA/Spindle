#version 330

in  vec3 vPosition;
out vec4 color;

uniform int t;
uniform mat4 proj;



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

  vec3 vPosition_local = (0.995 + 0.005 * sin(0.3*t)) * vPosition;

  gl_Position = (rotationMatrix(rot_axis, 0.00075f * t) * proj * vec4(vPosition_local, 1.0f));

  if(gl_VertexID < (t % 20000))
    color = vec4(1.0f, 0.75f, 0.3 * sin(0.01*t), 1.0f);
  else
    color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

#version 330

in  vec3 vPosition;
in  vec4 vColor;
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

  //if(vColor.a < (t % 50))
    //gl_Position = proj * rotationMatrix(rot_axis, 0.01f * t) * vec4(vPosition, 1.0);
  //else
    gl_Position = proj * rotationMatrix(rot_axis, 0.01f * t) * vec4(vPosition+vColor.xyz, 1.0);

  color = vec4(gl_VertexID/500.0f, gl_VertexID/500.0f, gl_VertexID/500.0f, 1.0f);

}

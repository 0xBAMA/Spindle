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

  float scale = 0.08 * sin(0.005*t) + 0.088;

  vec3 vPosition_local = (scale * vNormal) + vPosition;

  gl_Position = proj * rotationMatrix(rot_axis, 0.0075f * t) * vec4(vPosition_local, 1.0f);

  color = vec4(vColor, 1.0f);

}

#version 330
varying  vec4 color;

void
main()
{
    //gl_FragColor = vec4(vec2(0.8f-gl_FragCoord.z), 0.1f, 1.0f);
    gl_FragColor = color;
}

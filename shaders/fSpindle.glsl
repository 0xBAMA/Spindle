#version 330
varying  vec4 color;

void
main()
{
    //gl_FragColor = color;

    gl_FragColor = vec4(vec2(1.0f-gl_FragCoord.z), 0.0f, 1.0f);
}

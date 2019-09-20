#version 330
varying  vec4 color;

void
main()
{
    //gl_FragColor = vec4(gl_FragCoord.z*(gl_FragCoord.x/800.0), gl_FragCoord.z*(gl_FragCoord.y/800.0), gl_FragCoord.z, 1.0f);

    gl_FragColor = color;

    //gl_FragColor = (vec4(1.0f-gl_FragCoord.z, 0.3f, 0.0f, 1.0f) + color)/2.0;
}

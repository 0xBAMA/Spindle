#version 330
varying  vec4 color;

void
main()
{
    //gl_FragColor = vec4(vec2(0.8f-gl_FragCoord.z), 0.1f, 1.0f);


    //fcmod3 is used to draw something along the lines of scanlines
    //add a uniform to allow this to be toggled - do this in the glut right
    //click menu

    int fcmod3 = int(gl_FragCoord.y) % 3;

    if(fcmod3 != 0)
      gl_FragColor = vec4(color.rgb * (0.8f-gl_FragCoord.z), 1.0f);
    else
      discard;
}

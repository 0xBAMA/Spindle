#version 330
varying  vec4 color;

uniform int scanlines;   //should you blank out every third line?
uniform int depthcolor; //should you darken deeper fragments?

void
main()
{
    //gl_FragColor = vec4(vec2(0.8f-gl_FragCoord.z), 0.1f, 1.0f); //older



    if(depthcolor == 1)
      gl_FragColor = vec4(color.rgb * (0.8f-gl_FragCoord.z), 1.0f);
    else
      gl_FragColor = color;

    //fcymod3 is used to draw something along the lines of scanlines
    // int fcxmod2 = int(gl_FragCoord.x) % 2;
    int fcymod3 = int(gl_FragCoord.y) % 3;

    if((scanlines == 1) && (fcymod3 == 0)) //add  '&& (fcxmod2 == 0)' for x based stuff
    {
      discard;
    }
}

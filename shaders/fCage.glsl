#version 330
varying  vec4 color;

uniform int depthcolor; //should you darken deeper fragments?

void
main()
{

    if(depthcolor == 1)
      gl_FragColor = vec4(color.rgb * (1.0f-gl_FragCoord.z), 1.0f);
    else
      gl_FragColor = color;

}

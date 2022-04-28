//#version 120

varying vec4 color;

void main()
{
	gl_FragColor = color;
	gl_FragColor.a = dot(color.rgb, vec3(0.299, 0.587, 0.114));
//	gl_FragColor.rgb = color.rgb;
//	gl_FragColor.a = 1.0;
}

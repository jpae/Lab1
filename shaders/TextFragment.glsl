uniform sampler2D uTexUnit;

varying vec2 UV;

void main() {
   gl_FragColor = texture2D( uTexUnit, UV );

   float d = abs(gl_FragColor.r - gl_FragColor.b);
   if (d > 0.6)
      discard;
   d = abs(gl_FragColor.r - gl_FragColor.g);
   if (d > 0.6)
      discard;

   // gl_FragColor = vec4(1, 1, 1, 1);
}
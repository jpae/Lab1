uniform sampler2D uTexUnit;

varying vec2 vTexCoord;
varying vec3 vColor;

void main(void) {
  vec4 BaseColor = vec4(vColor.x, vColor.y, vColor.z, 1);
  vec4 texColor1 = texture2D(uTexUnit, vTexCoord);

  gl_FragColor = texColor1;
}


attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec2 aTexCoord;

uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;
uniform vec2 uWinScale;

varying vec3 vColor;
varying vec2 vTexCoord;

void main() {
  vec4 vPosition;

  /* First model transforms */
  gl_Position = uProjMatrix*uViewMatrix*uModelMatrix*vec4(aPosition, 1);

  /* a color that could be blended - or be shading */
  vColor = vec3(0.56, 0.3, 0.1);
  /* pass through the texture coordinates to be interpolated */
  vTexCoord = aTexCoord;
}

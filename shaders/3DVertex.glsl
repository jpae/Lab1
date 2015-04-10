attribute vec4 aPosition;
attribute vec3 aNormal;
uniform mat4 uProjMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

uniform vec3 uLightPos;

uniform vec3 UaColor;
uniform vec3 UdColor;
uniform vec3 UsColor;
uniform float Ushine;
uniform int uShadeModel;
uniform int uShowNormal;

varying vec3 vNormal;
varying vec4 vWorldSpace;

#define M_PI 3.1415926535897932384626433832795

void main()
{
    vWorldSpace = uViewMatrix * uModelMatrix * aPosition;
    gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * aPosition;
        
    
    vNormal = (uViewMatrix * uModelMatrix * vec4(aNormal, 0)).xyz;
}

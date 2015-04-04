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

uniform vec3 uBend;

varying vec3 vNormal;
varying vec3 vColor;
varying vec4 vWorldSpace;

#define M_PI 3.1415926535897932384626433832795

void main()
{
    vec4 pos = aPosition, old_pos = aPosition;
    vec3 nor = aNormal, old_nor = aNormal;
    if (uBend.z != 0.0) {
        // Twist the track. The bigger the z value, the closer to uBend.z
        float theta = aPosition.z * uBend.z - M_PI / 2.0;
        
        pos.y = cos(theta) * old_pos.x - sin(theta) * old_pos.y;
        pos.x = sin(theta) * old_pos.x + cos(theta) * old_pos.y;
        nor.y = cos(theta) * old_nor.x - sin(theta) * old_nor.y;
        nor.x = sin(theta) * old_nor.x + cos(theta) * old_nor.y;
        
        old_pos = pos;
        old_nor = nor;
    }
    if (uBend.y != 0.0) {
        // If we want z = [0, 1] to bend along theta = [0, uBend]
        // then the "track" must be a segment length 1 across a circle
        // So 2PIr * (uBend / 2PI) = uBend * r == 1
        float radius = 1.0 / uBend.y;
        // Position along the curve
        float theta = aPosition.z * uBend.y;
        // Position outside or inside the bend
        radius += aPosition.x;
        
        // Compute the turn!
        pos.x = cos(theta) * radius - 1.0 / uBend.y;
        pos.z = sin(theta) * radius;

        nor.x = cos(theta) * old_nor.x - sin(theta) * old_nor.z;
        nor.z = sin(theta) * old_nor.x + cos(theta) * old_nor.z;
        
        old_pos = pos;
        old_nor = nor;
    }
    if (uBend.x != 0.0) {
        float radius = 1.0 / uBend.x - old_pos.y;
        float theta = aPosition.z * uBend.x;
        
        pos.z = sin(theta) * radius;
        pos.y = 1.0 / uBend.x - cos(theta) * radius;
        nor.y = cos(theta) * old_nor.y + sin(theta) * old_nor.z;
        nor.z = -sin(theta) * old_nor.y + cos(theta) * old_nor.z;
        
        old_pos = pos;
        old_nor = nor;
    }
    
    vWorldSpace = uViewMatrix * uModelMatrix * pos;
    gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * pos;
        
    
    vNormal = (uViewMatrix * uModelMatrix * vec4(nor, 0)).xyz;
}

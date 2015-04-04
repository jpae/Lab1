varying vec3 vNormal;
varying vec3 vColor;
varying vec4 vWorldSpace;

uniform vec3 uLightPos;

uniform vec3 UaColor;
uniform vec3 UdColor;
uniform vec3 UsColor;
uniform float Ushine;

uniform int uShadeModel;
uniform int uShowNormal;

void main()
{
    vec3 lightVector = normalize(uLightPos - vWorldSpace.xyz);
        
    float Id = dot(normalize(vNormal), lightVector);
    float Is = pow(dot(normalize(vNormal), normalize(lightVector + vWorldSpace.xyz)), Ushine);
    
    gl_FragColor = vec4(Is * UsColor + Id * UdColor + UaColor, 1);
}

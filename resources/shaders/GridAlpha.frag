#version 330 core

in float vertexDistance;
in vec4 vColor;

// Ouput data
out vec4 color;

void main(){
    float fogFactor;
    float fog_coord;
    float end_fog = 50.0;

    fogFactor = (end_fog - vertexDistance) * (1.0 / end_fog);
    fogFactor = clamp(fogFactor, 0.0, 1.0);
    color = mix(vec4(vColor.xyz, 0.0), vec4(vColor.xyz, 1.0), fogFactor);
}
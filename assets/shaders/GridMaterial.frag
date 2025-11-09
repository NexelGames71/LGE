#version 330 core

in vec3 v_WorldPos;
in vec3 v_Normal;
in vec3 v_Color;

out vec4 FragColor;

uniform vec3 u_GridColor1;
uniform vec3 u_GridColor2;
uniform float u_GridSize;
uniform float u_GridThickness;
uniform vec3 u_BaseColor;
uniform float u_GridIntensity;
uniform vec3 u_ViewPos;

// Grid material similar to Unreal Engine's grid material
void main() {
    vec3 worldPos = v_WorldPos;
    
    // Calculate grid lines
    vec2 grid = abs(fract(worldPos.xz / u_GridSize - 0.5) - 0.5) / fwidth(worldPos.xz / u_GridSize);
    float gridLine = min(grid.x, grid.y);
    float gridMask = 1.0 - min(gridLine / u_GridThickness, 1.0);
    
    // Mix grid colors
    vec3 gridColor = mix(u_GridColor1, u_GridColor2, gridMask);
    
    // Apply base color tint
    vec3 finalColor = mix(u_BaseColor, gridColor, u_GridIntensity);
    
    // Simple lighting (normal facing camera gets brighter)
    vec3 normal = normalize(v_Normal);
    vec3 viewDir = normalize(u_ViewPos - worldPos);
    float fresnel = pow(1.0 - dot(normal, viewDir), 2.0);
    finalColor += fresnel * 0.1;
    
    // Apply vertex color tint
    finalColor *= v_Color;
    
    FragColor = vec4(finalColor, 1.0);
}



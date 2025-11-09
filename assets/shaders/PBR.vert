#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Color;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in vec2 a_TexCoord;
layout(location = 4) in vec3 a_Tangent;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;
uniform mat4 u_View;

out vec3 v_Color;
out vec3 v_Normal;
out vec3 v_FragPos;
out vec2 v_TexCoord;
out vec3 v_Tangent;
out vec3 v_ViewDir;

void main()
{
    // Transform vertex position by model matrix first, then view-projection
    vec4 worldPos = u_Model * vec4(a_Position, 1.0);
    gl_Position = u_ViewProjection * worldPos;
    
    v_Color = a_Color;
    v_FragPos = vec3(worldPos);
    
    // Transform normal and tangent to world space
    mat3 normalMatrix = mat3(transpose(inverse(u_Model)));
    v_Normal = normalize(normalMatrix * a_Normal);
    v_Tangent = normalize(normalMatrix * a_Tangent);
    
    v_TexCoord = a_TexCoord;
    
    // Calculate view direction in world space
    vec3 viewPos = vec3(inverse(u_View) * vec4(0.0, 0.0, 0.0, 1.0));
    v_ViewDir = normalize(viewPos - v_FragPos);
}


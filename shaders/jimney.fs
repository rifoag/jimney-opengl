#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TextCoord;
in vec3 FragPos;

uniform sampler2D ourTexture;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;


void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = vec3(1.0);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 result = (ambient + diff) * objectColor;
    // FragColor = texture(ourTexture, TextCoord) * ourColor;
    FragColor = vec4(result, 1.0);
}

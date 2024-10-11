#version 330 core
out vec4 FragColor;
in vec2 TexCord;
in float label;
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform sampler2D texture5;
uniform sampler2D texture6;
void main()
{
    if(label == 0.0f)
        FragColor = texture(texture0, TexCord);
    else if(label == 1.0f)
        FragColor = texture(texture1, TexCord);
    else if(label == 2.0f)
        FragColor = texture(texture2, TexCord);
    else if(label == 3.0f)
        FragColor = texture(texture3, TexCord);
    else if(label == 4.0f)
        FragColor = texture(texture4, TexCord);
    else if(label == 5.0f)
        FragColor = texture(texture5, TexCord);
    else
        FragColor = texture(texture6, TexCord);
}

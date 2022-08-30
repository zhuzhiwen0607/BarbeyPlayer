//uniform sampler2D qt_Texture0;
//varying highp vec4 qt_TexCoord0;

#version 330 core
in vec4 otex;
uniform sampler2D textureY;
uniform sampler2D textureUV;

void main(void)
{
    vec3 yuv;
    vec3 rgb;

    yuv.x = texture2D(textureY, otex.st).r;
    yuv.y = texture2D(textureUV, otex.st).r - 0.5;
    yuv.z = texture2D(textureUV, otex.st).g - 0.5;

    rgb = mat3( 1.164, 1.164, 1.164,
                0.0, -0.392, 2.017,
                1.596, -0.813, 0.0) * yuv;

    gl_FragColor = vec4(rgb, 1);
}

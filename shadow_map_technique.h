#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShadowMapTechnique : public Technique {

public:
    ShadowMapTechnique();
    virtual bool Init();
    void SetWVP(const glm::mat4& WVP);
    void SetTextureUnit(unsigned int TextureUnit);

private:

    GLuint m_WVPLocation;
    GLuint m_textureLocation;
};

static const char* pVS2 = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
layout (location = 0) in vec3 Position;                                             \n\
layout (location = 1) in vec2 TexCoord;                                             \n\
layout (location = 2) in vec3 Normal;                                               \n\
                                                                                    \n\
uniform mat4 gWVP;                                                                  \n\
                                                                                    \n\
out vec2 TexCoordOut;                                                               \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    gl_Position = gWVP * vec4(Position, 1.0);                                       \n\
    TexCoordOut = TexCoord;                                                         \n\
}";

static const char* pFS2 = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
in vec2 TexCoordOut;                                                                \n\
uniform sampler2D gShadowMap;                                                       \n\
                                                                                    \n\
out vec4 FragColor;                                                                 \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    float Depth = texture(gShadowMap, TexCoordOut).x;                               \n\
    Depth = 1.0 - (1.0 - Depth) * 25.0;                                             \n\
    FragColor = vec4(Depth);                                                        \n\
}";

ShadowMapTechnique::ShadowMapTechnique()
{
}

bool ShadowMapTechnique::Init()
{
    if (!Technique::Init()) {
        return false;
    }

    if (!AddShader(GL_VERTEX_SHADER, pVS2)) {
        return false;
    }

    if (!AddShader(GL_FRAGMENT_SHADER, pFS2)) {
        return false;
    }

    if (!Finalize()) {
        return false;
    }

    m_WVPLocation = GetUniformLocation("gWVP");
    m_textureLocation = GetUniformLocation("gShadowMap");

    if (m_WVPLocation == 0xFFFFFFFF ||
        m_textureLocation == 0xFFFFFFFF) {
        return false;
    }

    return true;
}

void ShadowMapTechnique::SetWVP(const glm::mat4& WVP)
{
    glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, glm::value_ptr(WVP));
}

void ShadowMapTechnique::SetTextureUnit(unsigned int TextureUnit)
{
    glUniform1i(m_textureLocation, TextureUnit);
}

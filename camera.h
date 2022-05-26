#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/geometric.hpp>
#include <glm/gtx/rotate_vector.hpp>

const static float STEP_SCALE = 0.1f;
const static int MARGIN = 10;

class Camera
{
public:

    Camera();

    Camera(const glm::vec3& Pos, const glm::vec3& Target, const glm::vec3& Up);

    Camera(int WindowWidth, int WindowHeight);

    Camera(int WindowWidth, int WindowHeight, const glm::vec3& Pos, const glm::vec3& Target, const glm::vec3& Up);

    bool OnKeyboard(int Key);

    void OnMouse(int x, int y);

    void OnRender();

    const glm::vec3& GetPos() const
    {
        return m_pos;
    }

    const glm::vec3& GetTarget() const
    {
        return m_target;
    }

    const glm::vec3& GetUp() const
    {
        return m_up;
    }

private:

    void Init();
    void Update();

    glm::vec3 m_pos;
    glm::vec3 m_target;
    glm::vec3 m_up;

    int m_windowWidth;
    int m_windowHeight;

    float m_AngleH;
    float m_AngleV;


    glm::ivec2 m_mousePos;
};


Camera::Camera(int WindowWidth, int WindowHeight) :
    m_windowWidth(WindowWidth),
    m_windowHeight(WindowHeight),
    m_pos(glm::vec3(0.0f, 0.0f, 0.0f)),
    m_target(glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f))),
    m_up(glm::vec3(0.0f, 1.0f, 0.0f))
{
    Init();
    //m_target = glm::normalize(m_target);
}


Camera::Camera(int WindowWidth, int WindowHeight, const glm::vec3& Pos, const glm::vec3& Target, const glm::vec3& Up) :
    m_windowWidth(WindowWidth),
    m_windowHeight(WindowHeight),
    m_pos(Pos),
    m_target(glm::normalize(Target)),
    //m_target(Target),
    m_up(glm::normalize(Up))
{
    //m_target = glm::normalize(m_target);
    Init();
}


void Camera::Init()
{
    glm::vec3 HTarget(m_target[0], 0.0, m_target[2]);
    HTarget = glm::normalize(HTarget);

    if (HTarget[2] >= 0.0f)
    {
        if (HTarget[0] >= 0.0f)
        {
            m_AngleH = 360.0f - glm::degrees(asin(HTarget[2]));
        }
        else
        {
            m_AngleH = 180.0f + glm::degrees(asin(HTarget[2]));
        }
    }
    else
    {
        if (HTarget[0] >= 0.0f)
        {
            m_AngleH = glm::degrees(asin(-HTarget[2]));
        }
        else
        {
            m_AngleH = 90.0f + glm::degrees(asin(-HTarget[2]));
        }
    }

    m_AngleV = -glm::degrees(asin(m_target[1]));

    m_mousePos[0] = m_windowWidth / 2;
    m_mousePos[1] = m_windowHeight / 2;

    glutWarpPointer(m_mousePos[0], m_mousePos[0]);
}


bool Camera::OnKeyboard(int Key)
{
    bool Ret = false;

    switch (Key) {

    case GLUT_KEY_UP:
    {
        m_pos += (m_target * STEP_SCALE);
        Ret = true;
    }
    break;

    case GLUT_KEY_DOWN:
    {
        m_pos -= (m_target * STEP_SCALE);
        Ret = true;
    }
    break;

    case GLUT_KEY_LEFT:
    {
        glm::vec3 Left = glm::cross(m_target, m_up);
        Left = glm::normalize(Left);
        Left *= STEP_SCALE;
        m_pos += Left;
        Ret = true;
    }
    break;

    case GLUT_KEY_RIGHT:
    {
        glm::vec3 Right = glm::cross(m_up, m_target);
        Right = glm::normalize(Right);
        Right *= STEP_SCALE;
        m_pos += Right;
        Ret = true;
    }
    break;
    }

    return Ret;
}


void Camera::OnMouse(int x, int y)
{
    if ((x == m_mousePos[0]) && (y == m_mousePos[1])) return;

    const int DeltaX = x - m_mousePos[0];
    const int DeltaY = y - m_mousePos[1];

    m_AngleH += (float)DeltaX / 20.0f;
    m_AngleV += (float)DeltaY / 20.0f;

    Update();
    glutWarpPointer(m_mousePos[0], m_mousePos[1]);
}


void Camera::OnRender()
{
    bool ShouldUpdate = false;

    if (ShouldUpdate) {
        Update();
    }
}

void Camera::Update()
{
    const glm::vec3 Vaxis(0.0f, 1.0f, 0.0f);

    // Rotate the view vector by the horizontal angle around the vertical axis
    glm::vec3 View(1.0f, 0.0f, 0.0f);
    View = glm::rotate(View, glm::radians(m_AngleH), Vaxis);
    View = glm::normalize(View);

    // Rotate the view vector by the vertical angle around the horizontal axis
    glm::vec3 Haxis = glm::cross(Vaxis, View);
    Haxis = glm::normalize(Haxis);
    View = glm::rotate(View, glm::radians(this->m_AngleV), Haxis);

    m_target = View;
    m_target = glm::normalize(m_target);

    m_up = glm::cross(m_target, Haxis);
    m_up = glm::normalize(m_up);
}
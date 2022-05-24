#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/geometric.hpp>
#include <glm/gtx/rotate_vector.hpp>

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

    bool m_OnUpperEdge;
    bool m_OnLowerEdge;
    bool m_OnLeftEdge;
    bool m_OnRightEdge;

    glm::ivec2 m_mousePos;
};

Camera::Camera()
{
    m_pos = glm::vec3(0.0f, 0.0f, 0.0f);
    m_target = glm::vec3(0.0f, 0.0f, 1.0f);
    m_up = glm::vec3(0.0f, 1.0f, 0.0f);
}

Camera::Camera(const glm::vec3& Pos, const glm::vec3& Target, const glm::vec3& Up)
{
    m_pos = Pos;
    m_target = Target;
    glm::normalize(m_target);
    m_up = Up;
    glm::normalize(m_up);
}

const static float STEP_SCALE = 0.1f;
const static int MARGIN = 10;

Camera::Camera(int WindowWidth, int WindowHeight) :
    m_windowWidth(WindowWidth),
    m_windowHeight(WindowHeight),
    m_pos(glm::vec3(0.0f, 0.0f, 0.0f)),
    m_target(glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f))),
    m_up(glm::vec3(0.0f, 1.0f, 0.0f))
{
    Init();
}


Camera::Camera(int WindowWidth, int WindowHeight, const glm::vec3& Pos, const glm::vec3& Target, const glm::vec3& Up) :
    m_windowWidth(WindowWidth),
    m_windowHeight(WindowHeight),
    m_pos(Pos),
    m_target(glm::normalize(Target)),
    m_up(glm::normalize(Up))
{
    Init();
}


void Camera::Init()
{
    glm::vec3 HTarget(m_target.x, 0.0, m_target.z);
    glm::normalize(HTarget);

    if (HTarget.z >= 0.0f)
    {
        if (HTarget.x >= 0.0f)
        {
            m_AngleH = 360.0f - glm::degrees(asin(HTarget.z));
        }
        else
        {
            m_AngleH = 180.0f + glm::degrees(asin(HTarget.z));
        }
    }
    else
    {
        if (HTarget.x >= 0.0f)
        {
            m_AngleH = glm::degrees(asin(-HTarget.z));
        }
        else
        {
            m_AngleH = 90.0f + glm::degrees(asin(-HTarget.z));
        }
    }

    m_AngleV = -glm::degrees(asin(m_target.y));

    m_OnUpperEdge = false;
    m_OnLowerEdge = false;
    m_OnLeftEdge = false;
    m_OnRightEdge = false;
    m_mousePos[0] = m_windowWidth / 2;
    m_mousePos[1] = m_windowHeight / 2;

    glutWarpPointer(m_mousePos[0], m_mousePos[1]);
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
        glm::normalize(Left);
        Left *= STEP_SCALE;
        m_pos += Left;
        Ret = true;
    }
    break;

    case GLUT_KEY_RIGHT:
    {
        glm::vec3 Right = glm::cross(m_up, m_target);
        glm::normalize(Right);
        Right *= STEP_SCALE;
        m_pos += Right;
        Ret = true;
    }
    break;
    }

    return Ret;
}


/*void Camera::OnMouse(int x, int y)
{
    const int DeltaX = x - m_mousePos[0];
    const int DeltaY = y - m_mousePos[1];

    m_mousePos[0] = x;
    m_mousePos[1] = y;

    m_AngleH += (float)DeltaX / 20.0f;
    m_AngleV += (float)DeltaY / 20.0f;

    if (DeltaX == 0) {
        if (x <= MARGIN) {
            //    m_AngleH -= 1.0f;
            m_OnLeftEdge = true;
        }
        else if (x >= (m_windowWidth - MARGIN)) {
            //    m_AngleH += 1.0f;
            m_OnRightEdge = true;
        }
    }
    else {
        m_OnLeftEdge = false;
        m_OnRightEdge = false;
    }

    if (DeltaY == 0) {
        if (y <= MARGIN) {
            m_OnUpperEdge = true;
        }
        else if (y >= (m_windowHeight - MARGIN)) {
            m_OnLowerEdge = true;
        }
    }
    else {
        m_OnUpperEdge = false;
        m_OnLowerEdge = false;
    }

    Update();
}


void Camera::OnRender()
{
    bool ShouldUpdate = false;

    if (m_OnLeftEdge) {
        m_AngleH -= 0.1f;
        ShouldUpdate = true;
    }
    else if (m_OnRightEdge) {
        m_AngleH += 0.1f;
        ShouldUpdate = true;
    }

    if (m_OnUpperEdge) {
        if (m_AngleV > -90.0f) {
            m_AngleV -= 0.1f;
            ShouldUpdate = true;
        }
    }
    else if (m_OnLowerEdge) {
        if (m_AngleV < 90.0f) {
            m_AngleV += 0.1f;
            ShouldUpdate = true;
        }
    }

    if (ShouldUpdate) {
        Update();
    }
}*/

void Camera::OnMouse(int x, int y)
{
    if ((x == m_mousePos.x) && (y == m_mousePos.y)) return;

    const int DeltaX = x - m_mousePos.x;
    const int DeltaY = y - m_mousePos.y;

    m_AngleH += (float)DeltaX / 20.0f;
    m_AngleV += (float)DeltaY / 20.0f;

    Update();
    glutWarpPointer(m_mousePos.x, m_mousePos.y);
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
    glm::normalize(View);

    // Rotate the view vector by the vertical angle around the horizontal axis
    glm::vec3 Haxis = glm::cross(Vaxis, View);
    glm::normalize(Haxis);
    View = glm::rotate(View, glm::radians(this->m_AngleV), Haxis);

    m_target = View;
    glm::normalize(m_target);

    m_up = glm::cross(m_target, Haxis);
    glm::normalize(m_up);
}
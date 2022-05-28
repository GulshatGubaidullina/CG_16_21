#include <glm/glm.hpp>
#define _USE_MATH_DEFINES
#include <math.h> // ÷èñëî pi

#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)


class Pipeline
{
public:
    Pipeline() : m_scale(glm::vec3(1.0f, 1.0f, 1.0f)),
        m_worldPos(glm::vec3(0.0f, 0.0f, 0.0f)),
        m_rotateInfo(glm::vec3(0.0f, 0.0f, 0.0f)),
        m_transformation(glm::mat4()),
        mPersProj(),
        m_camera(),
        m_WVPtransformation(glm::mat4()),
        m_WorldTransformation(glm::mat4())
    {}

    void scale(float ScaleX, float ScaleY, float ScaleZ)
    {
        m_scale[0] = ScaleX;
        m_scale[1] = ScaleY;
        m_scale[2] = ScaleZ;
    }

    void worldPos(float x, float y, float z)
    {
        m_worldPos[0] = x;
        m_worldPos[1] = y;
        m_worldPos[2] = z;
    }

    void rotate(float RotateX, float RotateY, float RotateZ)
    {
        m_rotateInfo[0] = RotateX;
        m_rotateInfo[1] = RotateY;
        m_rotateInfo[2] = RotateZ;
    }

    void perspectiveProj(float FOV, float width, float height, float zNear, float zFar) {
        mPersProj.FOV = FOV;
        mPersProj.width = width;
        mPersProj.height = height;
        mPersProj.zNear = zNear;
        mPersProj.zFar = zFar;
    }

    void SetCamera(const glm::vec3& Pos, const glm::vec3& Target, const glm::vec3& Up)
    {
        m_camera.Pos = Pos;
        m_camera.Target = Target;
        m_camera.Up = Up;
    }
    const glm::mat4& GetWVPTrans();
    const glm::mat4& GetWorldTrans();

    const glm::mat4* GetTrans();

private:
    void initScaleTransform(glm::mat4& m) const;
    void initRotateTransform(glm::mat4& m) const;
    void initTranslationTransform(glm::mat4& m) const;
    void SetPerspectiveProj(glm::mat4& m) const;

    glm::vec3 m_scale;
    glm::vec3 m_worldPos;
    glm::vec3 m_rotateInfo;

    struct {
        float FOV;
        float width;
        float height;
        float zNear;
        float zFar;
    }  mPersProj;

    struct {
        glm::vec3 Pos;
        glm::vec3 Target;
        glm::vec3 Up;
    } m_camera;

    glm::mat4 m_transformation;
    glm::mat4 m_WVPtransformation;
    glm::mat4 m_WorldTransformation;
};

void Pipeline::initScaleTransform(glm::mat4& m) const {
    m[0][0] = m_scale[0]; m[0][1] = 0.0f;       m[0][2] = 0.0f;       m[0][3] = 0.0f;
    m[1][0] = 0.0f;       m[1][1] = m_scale[1]; m[1][2] = 0.0f;       m[1][3] = 0.0f;
    m[2][0] = 0.0f;       m[2][1] = 0.0f;       m[2][2] = m_scale[2]; m[2][3] = 0.0f;
    m[3][0] = 0.0f;       m[3][1] = 0.0f;       m[3][2] = 0.0f;       m[3][3] = 1.0f;
}

void Pipeline::initRotateTransform(glm::mat4& m) const
{
    glm::mat4 rx, ry, rz;

    const float x = ToRadian(m_rotateInfo[0]);
    const float y = ToRadian(m_rotateInfo[1]);
    const float z = ToRadian(m_rotateInfo[2]);

    rx[0][0] = 1.0f; rx[0][1] = 0.0f;    rx[0][2] = 0.0f;     rx[0][3] = 0.0f;
    rx[1][0] = 0.0f; rx[1][1] = cosf(x); rx[1][2] = -sinf(x); rx[1][3] = 0.0f;
    rx[2][0] = 0.0f; rx[2][1] = sinf(x); rx[2][2] = cosf(x);  rx[2][3] = 0.0f;
    rx[3][0] = 0.0f; rx[3][1] = 0.0f;    rx[3][2] = 0.0f;     rx[3][3] = 1.0f;

    ry[0][0] = cosf(y); ry[0][1] = 0.0f; ry[0][2] = -sinf(y); ry[0][3] = 0.0f;
    ry[1][0] = 0.0f;    ry[1][1] = 1.0f; ry[1][2] = 0.0f;     ry[1][3] = 0.0f;
    ry[2][0] = sinf(y); ry[2][1] = 0.0f; ry[2][2] = cosf(y);  ry[2][3] = 0.0f;
    ry[3][0] = 0.0f;    ry[3][1] = 0.0f; ry[3][2] = 0.0f;     ry[3][3] = 1.0f;

    rz[0][0] = cosf(z); rz[0][1] = -sinf(z); rz[0][2] = 0.0f; rz[0][3] = 0.0f;
    rz[1][0] = sinf(z); rz[1][1] = cosf(z);  rz[1][2] = 0.0f; rz[1][3] = 0.0f;
    rz[2][0] = 0.0f;    rz[2][1] = 0.0f;     rz[2][2] = 1.0f; rz[2][3] = 0.0f;
    rz[3][0] = 0.0f;    rz[3][1] = 0.0f;     rz[3][2] = 0.0f; rz[3][3] = 1.0f;

    m = rz * ry * rx;
}

void Pipeline::initTranslationTransform(glm::mat4& m) const {
    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = m_worldPos[0];
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = m_worldPos[1];
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = m_worldPos[2];
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}
void Pipeline::SetPerspectiveProj(glm::mat4& m) const {
    const float ar = mPersProj.width / mPersProj.height;
    const float zNear = mPersProj.zNear;
    const float zFar = mPersProj.zFar;
    const float zRange = zNear - zFar;
    const float tanHalfFOV = tanf(ToRadian(mPersProj.FOV / 2.0f));

    m[0][0] = 1.0f / (tanHalfFOV * ar);
    m[0][1] = 0.0f;
    m[0][2] = 0.0f;
    m[0][3] = 0.0;
    m[1][0] = 0.0f;
    m[1][1] = 1.0f / tanHalfFOV;
    m[1][2] = 0.0f;
    m[1][3] = 0.0;
    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = (-zNear - zFar) / zRange;
    m[2][3] = 2.0f * zFar * zNear / zRange;
    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = 1.0f;
    m[3][3] = 0.0;
}

glm::mat4 MatrixMultiplicationIdk(glm::mat4 first, glm::mat4 second)
{
    glm::mat4 Ret;

    for (unsigned int i = 0; i < 4; i++) {
        for (unsigned int j = 0; j < 4; j++) {
            Ret[i][j] = first[i][0] * second[0][j] +
                first[i][1] * second[1][j] +
                first[i][2] * second[2][j] +
                first[i][3] * second[3][j];
        }
    }

    return Ret;
}

const glm::mat4* Pipeline::GetTrans()
{
    glm::mat4 scaleTrans, rotateTrans, translationTrans, persProjTrans, CameraTranslationTrans, CameraRotateTrans;

    //ScaleTrans.InitScaleTransform(m_scale.x, m_scale.y, m_scale.z);
    //RotateTrans.InitRotateTransform(m_rotateInfo.x, m_rotateInfo.y, m_rotateInfo.z);
    //TranslationTrans.InitTranslationTransform(m_worldPos.x, m_worldPos.y, m_worldPos.z);
    //CameraTranslationTrans.InitTranslationTransform(-m_camera.Pos.x, -m_camera.Pos.y, -m_camera.Pos.z);
    //CameraRotateTrans.InitCameraTransform(m_camera.Target, m_camera.Up);
    //PersProjTrans.InitPersProjTransform(m_persProj.FOV, m_persProj.Width, m_persProj.Height, m_persProj.zNear, m_persProj.zFar);

    // initscaletransform
    scaleTrans[0][0] = m_scale[0]; scaleTrans[0][1] = 0.0f;   scaleTrans[0][2] = 0.0f;   scaleTrans[0][3] = 0.0f;
    scaleTrans[1][0] = 0.0f;   scaleTrans[1][1] = m_scale[1]; scaleTrans[1][2] = 0.0f;   scaleTrans[1][3] = 0.0f;
    scaleTrans[2][0] = 0.0f;   scaleTrans[2][1] = 0.0f;   scaleTrans[2][2] = m_scale[2]; scaleTrans[2][3] = 0.0f;
    scaleTrans[3][0] = 0.0f;   scaleTrans[3][1] = 0.0f;   scaleTrans[3][2] = 0.0f;   scaleTrans[3][3] = 1.0f;

    //initrotatetransform
    glm::mat4 rx, ry, rz;

    const float x = ToRadian(m_rotateInfo[0]);
    const float y = ToRadian(m_rotateInfo[1]);
    const float z = ToRadian(m_rotateInfo[2]);

    rx[0][0] = 1.0f; rx[0][1] = 0.0f;    rx[0][2] = 0.0f;     rx[0][3] = 0.0f;
    rx[1][0] = 0.0f; rx[1][1] = cosf(x); rx[1][2] = -sinf(x); rx[1][3] = 0.0f;
    rx[2][0] = 0.0f; rx[2][1] = sinf(x); rx[2][2] = cosf(x);  rx[2][3] = 0.0f;
    rx[3][0] = 0.0f; rx[3][1] = 0.0f;    rx[3][2] = 0.0f;     rx[3][3] = 1.0f;

    ry[0][0] = cosf(y); ry[0][1] = 0.0f; ry[0][2] = -sinf(y); ry[0][3] = 0.0f;
    ry[1][0] = 0.0f;       ry[1][1] = 1.0f; ry[1][2] = 0.0f;        ry[1][3] = 0.0f;
    ry[2][0] = sinf(y); ry[2][1] = 0.0f; ry[2][2] = cosf(y);  ry[2][3] = 0.0f;
    ry[3][0] = 0.0f;       ry[3][1] = 0.0f; ry[3][2] = 0.0f;        ry[3][3] = 1.0f;

    rz[0][0] = cosf(z); rz[0][1] = -sinf(z); rz[0][2] = 0.0f; rz[0][3] = 0.0f;
    rz[1][0] = sinf(z); rz[1][1] = cosf(z);  rz[1][2] = 0.0f; rz[1][3] = 0.0f;
    rz[2][0] = 0.0f;    rz[2][1] = 0.0f;     rz[2][2] = 1.0f; rz[2][3] = 0.0f;
    rz[3][0] = 0.0f;    rz[3][1] = 0.0f;     rz[3][2] = 0.0f; rz[3][3] = 1.0f;

    rotateTrans = rz * ry * rx;

    // inittranslationform
    translationTrans[0][0] = 1.0f; translationTrans[0][1] = 0.0f; translationTrans[0][2] = 0.0f; translationTrans[0][3] = this->m_worldPos[0];
    translationTrans[1][0] = 0.0f; translationTrans[1][1] = 1.0f; translationTrans[1][2] = 0.0f; translationTrans[1][3] = this->m_worldPos[1];
    translationTrans[2][0] = 0.0f; translationTrans[2][1] = 0.0f; translationTrans[2][2] = 1.0f; translationTrans[2][3] = this->m_worldPos[2];
    translationTrans[3][0] = 0.0f; translationTrans[3][1] = 0.0f; translationTrans[3][2] = 0.0f; translationTrans[3][3] = 1.0f;
    // inittranslationform2
    CameraTranslationTrans[0][0] = 1.0f; CameraTranslationTrans[0][1] = 0.0f; CameraTranslationTrans[0][2] = 0.0f; CameraTranslationTrans[0][3] = -this->m_camera.Pos[0];
    CameraTranslationTrans[1][0] = 0.0f; CameraTranslationTrans[1][1] = 1.0f; CameraTranslationTrans[1][2] = 0.0f; CameraTranslationTrans[1][3] = -this->m_camera.Pos[1];
    CameraTranslationTrans[2][0] = 0.0f; CameraTranslationTrans[2][1] = 0.0f; CameraTranslationTrans[2][2] = 1.0f; CameraTranslationTrans[2][3] = -this->m_camera.Pos[2];
    CameraTranslationTrans[3][0] = 0.0f; CameraTranslationTrans[3][1] = 0.0f; CameraTranslationTrans[3][2] = 0.0f; CameraTranslationTrans[3][3] = 1.0f;

    // initcameratransform
    glm::vec3 N = m_camera.Target;
    glm::normalize(N);
    glm::vec3 U = m_camera.Up;
    glm::normalize(U);
    U = glm::cross(U, N);
    glm::vec3 V = glm::cross(N, U);

    CameraRotateTrans[0][0] = U.x;   CameraRotateTrans[0][1] = U.y;   CameraRotateTrans[0][2] = U.z;   CameraRotateTrans[0][3] = 0.0f;
    CameraRotateTrans[1][0] = V.x;   CameraRotateTrans[1][1] = V.y;   CameraRotateTrans[1][2] = V.z;   CameraRotateTrans[1][3] = 0.0f;
    CameraRotateTrans[2][0] = N.x;   CameraRotateTrans[2][1] = N.y;   CameraRotateTrans[2][2] = N.z;   CameraRotateTrans[2][3] = 0.0f;
    CameraRotateTrans[3][0] = 0.0f;  CameraRotateTrans[3][1] = 0.0f;  CameraRotateTrans[3][2] = 0.0f;  CameraRotateTrans[3][3] = 1.0f;

    //initpersproj kavo
    const float ar = this->mPersProj.width / this->mPersProj.height;
    const float zRange = this->mPersProj.zNear - this->mPersProj.zFar;
    const float tanHalfFOV = tanf(ToRadian(this->mPersProj.FOV / 2.0f));

    persProjTrans[0][0] = 1.0f / (tanHalfFOV * ar); persProjTrans[0][1] = 0.0f;              persProjTrans[0][2] = 0.0f;                                                     persProjTrans[0][3] = 0.0;
    persProjTrans[1][0] = 0.0f;                     persProjTrans[1][1] = 1.0f / tanHalfFOV; persProjTrans[1][2] = 0.0f;                                                     persProjTrans[1][3] = 0.0;
    persProjTrans[2][0] = 0.0f;                     persProjTrans[2][1] = 0.0f;              persProjTrans[2][2] = (-this->mPersProj.zNear - this->mPersProj.zFar) / zRange; persProjTrans[2][3] = 2.0f * this->mPersProj.zFar * this->mPersProj.zNear / zRange;
    persProjTrans[3][0] = 0.0f;                     persProjTrans[3][1] = 0.0f;              persProjTrans[3][2] = 1.0f;                                                     persProjTrans[3][3] = 0.0;


    //initScaleTransform(scaleTrans);
    //initRotateTransform(rotateTrans);
    //initTranslationTransform(translationTrans);
    //SetPerspectiveProj(persProjTrans);

    // glm íå óìååò óìíîæàòü ìàòðèöû, ïîýòîìó íàïèñàë ñâîþ ôóíêöèþ
    m_transformation = MatrixMultiplicationIdk(persProjTrans, CameraRotateTrans);
    m_transformation = MatrixMultiplicationIdk(m_transformation, CameraTranslationTrans);
    m_transformation = MatrixMultiplicationIdk(m_transformation, translationTrans);
    m_transformation = MatrixMultiplicationIdk(m_transformation, rotateTrans);
    m_transformation = MatrixMultiplicationIdk(m_transformation, scaleTrans);
    return &m_transformation;
}

const glm::mat4& Pipeline::GetWorldTrans()
{
    glm::mat4 ScaleTrans, RotateTrans, TranslationTrans;

    ScaleTrans[0][0] = m_scale[0]; ScaleTrans[0][1] = 0.0f;       ScaleTrans[0][2] = 0.0f;       ScaleTrans[0][3] = 0.0f;
    ScaleTrans[1][0] = 0.0f;       ScaleTrans[1][1] = m_scale[1]; ScaleTrans[1][2] = 0.0f;       ScaleTrans[1][3] = 0.0f;
    ScaleTrans[2][0] = 0.0f;       ScaleTrans[2][1] = 0.0f;       ScaleTrans[2][2] = m_scale[2]; ScaleTrans[2][3] = 0.0f;
    ScaleTrans[3][0] = 0.0f;       ScaleTrans[3][1] = 0.0f;       ScaleTrans[3][2] = 0.0f;       ScaleTrans[3][3] = 1.0f;

    glm::mat4 rx, ry, rz;

    const float x = glm::radians(m_rotateInfo[0]);
    const float y = glm::radians(m_rotateInfo[1]);
    const float z = glm::radians(m_rotateInfo[2]);

    rx[0][0] = 1.0f; rx[0][1] = 0.0f; rx[0][2] = 0.0f; rx[0][3] = 0.0f;
    rx[1][0] = 0.0f; rx[1][1] = cosf(x); rx[1][2] = -sinf(x); rx[1][3] = 0.0f;
    rx[2][0] = 0.0f; rx[2][1] = sinf(x); rx[2][2] = cosf(x); rx[2][3] = 0.0f;
    rx[3][0] = 0.0f; rx[3][1] = 0.0f; rx[3][2] = 0.0f; rx[3][3] = 1.0f;

    ry[0][0] = cosf(y); ry[0][1] = 0.0f; ry[0][2] = -sinf(y); ry[0][3] = 0.0f;
    ry[1][0] = 0.0f; ry[1][1] = 1.0f; ry[1][2] = 0.0f; ry[1][3] = 0.0f;
    ry[2][0] = sinf(y); ry[2][1] = 0.0f; ry[2][2] = cosf(y); ry[2][3] = 0.0f;
    ry[3][0] = 0.0f; ry[3][1] = 0.0f; ry[3][2] = 0.0f; ry[3][3] = 1.0f;

    rz[0][0] = cosf(z); rz[0][1] = -sinf(z); rz[0][2] = 0.0f; rz[0][3] = 0.0f;
    rz[1][0] = sinf(z); rz[1][1] = cosf(z); rz[1][2] = 0.0f; rz[1][3] = 0.0f;
    rz[2][0] = 0.0f; rz[2][1] = 0.0f; rz[2][2] = 1.0f; rz[2][3] = 0.0f;
    rz[3][0] = 0.0f; rz[3][1] = 0.0f; rz[3][2] = 0.0f; rz[3][3] = 1.0f;

    //RotateTrans = rz * ry * rx;
    RotateTrans = MatrixMultiplicationIdk(rz, ry);
    RotateTrans = MatrixMultiplicationIdk(RotateTrans, rz);

    TranslationTrans[0][0] = 1.0f; TranslationTrans[0][1] = 0.0f; TranslationTrans[0][2] = 0.0f; TranslationTrans[0][3] = m_worldPos[0];
    TranslationTrans[1][0] = 0.0f; TranslationTrans[1][1] = 1.0f; TranslationTrans[1][2] = 0.0f; TranslationTrans[1][3] = m_worldPos[1];
    TranslationTrans[2][0] = 0.0f; TranslationTrans[2][1] = 0.0f; TranslationTrans[2][2] = 1.0f; TranslationTrans[2][3] = m_worldPos[2];
    TranslationTrans[3][0] = 0.0f; TranslationTrans[3][1] = 0.0f; TranslationTrans[3][2] = 0.0f; TranslationTrans[3][3] = 1.0f;

    //m_WorldTransformation = TranslationTrans * RotateTrans * ScaleTrans;
    m_WorldTransformation = MatrixMultiplicationIdk(TranslationTrans, RotateTrans);
    m_WorldTransformation = MatrixMultiplicationIdk(m_WorldTransformation, ScaleTrans);
    return m_WorldTransformation;
}

const glm::mat4& Pipeline::GetWVPTrans()
{
    GetWorldTrans();

    glm::mat4 CameraTranslationTrans, CameraRotateTrans, PersProjTrans;

    CameraTranslationTrans[0][0] = 1.0f; CameraTranslationTrans[0][1] = 0.0f; CameraTranslationTrans[0][2] = 0.0f; CameraTranslationTrans[0][3] = -m_camera.Pos[0];
    CameraTranslationTrans[1][0] = 0.0f; CameraTranslationTrans[1][1] = 1.0f; CameraTranslationTrans[1][2] = 0.0f; CameraTranslationTrans[1][3] = -m_camera.Pos[1];
    CameraTranslationTrans[2][0] = 0.0f; CameraTranslationTrans[2][1] = 0.0f; CameraTranslationTrans[2][2] = 1.0f; CameraTranslationTrans[2][3] = -m_camera.Pos[2];
    CameraTranslationTrans[3][0] = 0.0f; CameraTranslationTrans[3][1] = 0.0f; CameraTranslationTrans[3][2] = 0.0f; CameraTranslationTrans[3][3] = 1.0f;

    glm::vec3 N = m_camera.Target;
    N = glm::normalize(N);
    glm::vec3 U = m_camera.Up;
    U = glm::normalize(U);
    U = glm::cross(U, N);
    glm::vec3 V = glm::cross(N, U);

    CameraRotateTrans[0][0] = U[0];   CameraRotateTrans[0][1] = U[1];   CameraRotateTrans[0][2] = U[2];   CameraRotateTrans[0][3] = 0.0f;
    CameraRotateTrans[1][0] = V[0];   CameraRotateTrans[1][1] = V[1];   CameraRotateTrans[1][2] = V[2];   CameraRotateTrans[1][3] = 0.0f;
    CameraRotateTrans[2][0] = N[0];   CameraRotateTrans[2][1] = N[1];   CameraRotateTrans[2][2] = N[2];   CameraRotateTrans[2][3] = 0.0f;
    CameraRotateTrans[3][0] = 0.0f;  CameraRotateTrans[3][1] = 0.0f;  CameraRotateTrans[3][2] = 0.0f;  CameraRotateTrans[3][3] = 1.0f;

    const float ar = mPersProj.width / mPersProj.height;
    const float zRange = mPersProj.zNear - mPersProj.zFar;
    const float tanHalfFOV = tanf(ToRadian(mPersProj.FOV / 2.0f));

    PersProjTrans[0][0] = 1.0f / (tanHalfFOV * ar); PersProjTrans[0][1] = 0.0f;            PersProjTrans[0][2] = 0.0f;          PersProjTrans[0][3] = 0.0;
    PersProjTrans[1][0] = 0.0f;                   PersProjTrans[1][1] = 1.0f / tanHalfFOV; PersProjTrans[1][2] = 0.0f;          PersProjTrans[1][3] = 0.0;
    PersProjTrans[2][0] = 0.0f;                   PersProjTrans[2][1] = 0.0f;            PersProjTrans[2][2] = (-mPersProj.zNear - mPersProj.zFar) / zRange; PersProjTrans[2][3] = 2.0f * mPersProj.zFar * mPersProj.zNear / zRange;
    PersProjTrans[3][0] = 0.0f;                   PersProjTrans[3][1] = 0.0f;            PersProjTrans[3][2] = 1.0f;          PersProjTrans[3][3] = 0.0;

    //m_WVPtransformation = PersProjTrans * CameraRotateTrans * CameraTranslationTrans * m_WorldTransformation;
    m_WVPtransformation = MatrixMultiplicationIdk(PersProjTrans, CameraRotateTrans);
    m_WVPtransformation = MatrixMultiplicationIdk(m_WVPtransformation, CameraTranslationTrans);
    m_WVPtransformation = MatrixMultiplicationIdk(m_WVPtransformation, m_WorldTransformation);
    return m_WVPtransformation;
}
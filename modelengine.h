#ifndef MODELENGINE_H
#define MODELENGINE_H

#include <QGLFunctions>
#include <QGLShaderProgram>

struct QOpenGLTriangle3D
{
QVector3D p1 = QVector3D(0, 0, 0);
QVector3D p2 = QVector3D(0, 0, 0);
QVector3D p3 = QVector3D(0, 0, 0);
QVector3D p1Normal = QVector3D(0, 0, 0);
QVector3D p2Normal = QVector3D(0, 0, 0);
QVector3D p3Normal = QVector3D(0, 0, 0);
QVector2D p1UV = QVector2D(0, 0);
QVector2D p2UV = QVector2D(0, 0);
QVector2D p3UV = QVector2D(0, 0);
};

class ModelEngine : public QGLFunctions
{
public:
    ModelEngine();
    virtual ~ModelEngine();

    void init();
    void loadModel(const QString &fileName);
    void drawModel(QGLShaderProgram *program);

private:
    void initModel();

    QVector<QOpenGLTriangle3D> m_triangles;
    GLuint m_vboId[2];
};

#endif // MODELENGINE_H

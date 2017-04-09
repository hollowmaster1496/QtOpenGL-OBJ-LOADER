#ifndef QGLBEGIN_H
#define QGLBEGIN_H

#include <QtWidgets>
#include<qopengl.h>
#include<QTextStream>


class QGLBegin : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit QGLBegin(QWidget *parent = 0);
    void setRotation(float x, float y, float z);
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);



private:
    void FreeRenderData();
    void GetMatrixTransform(QMatrix4x4 matrixVertex,
                            QVector<QOpenGLTriangle3D> triangles);

    static float* GenerateVertexBuffer(const QVector<QOpenGLTriangle3D> triangles);
    static void GenerateNormalsBuffer(const QVector<QOpenGLTriangle3D> triangles, float* coords);
    static void ReleaseVertexBuffer(float* buffer);

    float xrot,yrot,zrot;

    QVector<QOpenGLTriangle3D> m_triangles;
    GLuint m_vertexes;
    GLuint m_normals;
    QOpenGLShaderProgram m_shaderProgram;
    int m_coordVertex;
    int m_coordNormal;
    int m_matrixVertex;
    int m_matrixNormal;
    int m_colorFragment;

    QMatrix4x4  m_matrixRotate;

};

#endif // QGLBEGIN_H

#ifndef QGLBEGIN_H
#define QGLBEGIN_H

#include "modelengine.h"

#include <QtWidgets>
#include<qopengl.h>
#include<QTextStream>

class ModelEngine;

class QGLBegin : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit QGLBegin(QWidget *parent = 0);
    ~QGLBegin();
    void setRotation(float x, float y, float z);
    void openFile(const QString &fileName);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    void initShaders();
    void initTextures();



private:
    void FreeRenderData();
    void GetMatrixTransform(QMatrix4x4 matrixVertex,
                            QVector<QOpenGLTriangle3D> triangles);

    QGLShaderProgram m_shaderProgram;
    ModelEngine model;

    GLuint texture;

    QMatrix4x4 projection;

    float xrot,yrot,zrot;

};

#endif // QGLBEGIN_H

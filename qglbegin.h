#ifndef QGLBEGIN_H
#define QGLBEGIN_H

#include <QtWidgets>
#include<qopengl.h>
#include<QTextStream>

struct QOpenGLTriangle3D
{
QVector3D p1;
QVector3D p2;
QVector3D p3;
QVector3D p1Normal;
QVector3D p2Normal;
QVector3D p3Normal;
QVector2D p1UV;
QVector2D p2UV;
QVector2D p3UV;
};


class QGLBegin : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit QGLBegin(QWidget *parent = 0);
    void setRotation(float x, float y, float z);
    void parseObjFile(const QString &fileName,
                      QStringList &comments,
                      QVector<QOpenGLTriangle3D> &triangles);
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);



private:
    float xrot,yrot,zrot;

};

#endif // QGLBEGIN_H

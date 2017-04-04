/* Credits: Amin Ahmadi
 * http://amin-ahmadi.com/2016/05/24/opengl-qt-beginners-example-project/
 */

#include "qglbegin.h"

QGLBegin::QGLBegin(QWidget *parent) : QOpenGLWidget(parent)
{
    xrot = yrot = zrot = 0.0;
}

void QGLBegin::initializeGL()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void QGLBegin::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0, 0.0, -7.0);
    glRotatef(xrot, 1.0, 0.0, 0.0);
    glRotatef(yrot, 0.0, 1.0, 0.0);
    glRotatef(zrot, 0.0, 0.0, 1.0);

    glBegin(GL_QUADS);
    glNormal3d(0,0,+1);
    glVertex3d(-1,-1,0);
    glVertex3d(-1,1,0);
    glVertex3d(1,1,0);
    glVertex3d(1,-1,0);
    glEnd();

}

void QGLBegin::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2, +2, -2, +2, 1.0, 15.0);
    glMatrixMode(GL_MODELVIEW);
}

void QGLBegin::setRotation(float x, float y, float z)
{
    xrot = x;
    yrot = y;
    zrot = z;
    update();
}


void QGLBegin::parseObjFile(const QString &fileName,
                QStringList &comments,
                QVector<QOpenGLTriangle3D> &triangles)
{
    comments.clear();
    triangles.clear();

    QFile file(fileName);
    if(file.exists())
    {
        qDebug() << "blahs";
        printf("file exists \n");

        if(file.open(QFile::ReadOnly | QFile::Text))
        {
            QVector<QVector3D> v, vn;
            QVector<QVector2D> vt;

            printf("File open \n");

            while(!file.atEnd())
            {

                QString line = file.readLine().trimmed();
                QStringList lineParts = line.split('\\s+');
                if(lineParts.count() > 0)
                {

                    // if it’s a comment
                    if(lineParts.first() == '#')
                    {
                        comments.append(line.remove(0, 1).trimmed());
                    }

                    // if it’s a vertex position (v)
                    else if(lineParts.first() == "v")
                    {
                        v.append(QVector3D(lineParts.at(1).toFloat(),
                        lineParts.at(2).toFloat(),
                        lineParts.at(3).toFloat()));
                    }

                    // if it’s a normal (vn)
                    else if(lineParts.first() == "vn")
                    {
                        vn.append(QVector3D(lineParts.at(1).toFloat(),
                        lineParts.at(2).toFloat(),
                        lineParts.at(3).toFloat()));
                    }

                    // if it’s a texture (vt)
                    else if(lineParts.first() == "vt")
                    {
                        vt.append(QVector2D(lineParts.at(1).toFloat(),
                        lineParts.at(2).toFloat()));
                    }

                    // if it’s face data (f)
                    // there’s an assumption here that faces are all triangles
                    else if(lineParts.first() == "f")
                    {
                       QOpenGLTriangle3D triangle;

                        // get points from v array
                        triangle.p1 = v.at(lineParts.at(1).split('//').at(0).toInt() - 1);
                        triangle.p2 = v.at(lineParts.at(2).split('//').at(0).toInt() - 1);
                        triangle.p3 = v.at(lineParts.at(3).split('//').at(0).toInt() - 1);

                        if(vt.count() > 0) // check if really there are any UV coords
                        {
                            triangle.p1UV = vt.at(lineParts.at(1).split('//').at(1).toInt() - 1);
                            triangle.p2UV = vt.at(lineParts.at(2).split('//').at(1).toInt() - 1);
                            triangle.p3UV = vt.at(lineParts.at(3).split('//').at(1).toInt() - 1);
                        }

                        // get normals from vn array
                        triangle.p1Normal = vn.at(lineParts.at(1).split('//').at(2).toInt() - 1);
                        triangle.p2Normal = vn.at(lineParts.at(2).split('//').at(2).toInt() - 1);
                        triangle.p3Normal = vn.at(lineParts.at(3).split('//').at(2).toInt() - 1);

                        triangles.append(triangle);
                    }

                }
            }

            file.close();

            printf("Triangles \n", triangles.data()->p1);
        }


    }
}

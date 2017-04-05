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
   /* glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
    glDisable(GL_CULL_FACE);

    // Get the Qt object which allows to operate with buffers
    QOpenGLFunctions funcs(QOpenGLContext::currentContext());
    GLuint handle;

    // Create the buffer handle
    funcs.glGenBuffers(1, &handle);
    // Select buffer by its handle (so we’ll use this buffer
    // further)
    funcs.glBindBuffer(GL_ARRAY_BUFFER, handle);

    // Copy data into the buffer. Being copied,
    // source data is not used any more and can be released
    funcs.glBufferData(GL_ARRAY_BUFFER,
        sizeof m_triangles,
        src_data,
        GL_STATIC_DRAW);
    // Tell the program we’ve finished with the handle
    funcs.glBindBuffer(GL_ARRAY_BUFFER, 0); */

    QOpenGLWidget::initializeGL();


    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
    glDisable(GL_CULL_FACE);
}

void QGLBegin::paintGL()
{
  /*  QOpenGLFunctions funcs(QOpenGLContext::currentContext());
    // Vertex data
    glEnableClientState(GL_VERTEX_ARRAY);
    funcs.glBindBuffer(GL_ARRAY_BUFFER, m_triangles);

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
    glEnd(); */

    // Erase the scene
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Check whether the model exists
    if((0 == m_vertexes) || (0 == m_normals))
    {
        return;
    }

    // Set Projection mode
    glMatrixMode(GL_PROJECTION);

    // Clear transformation parameters
    glLoadIdentity();

    // Get model transformation matrix
    QMatrix4x4 matrixVertex;
    GetMatrixTransform(matrixVertex, m_triangles);


    // Vertex data
    glEnableClientState(GL_VERTEX_ARRAY);
    funcs.glBindBuffer(GL_ARRAY_BUFFER, m_vertexes);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    funcs.glVertexAttribPointer(m_coordVertex, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Normal data
    glEnableClientState(GL_NORMAL_ARRAY);
    funcs.glBindBuffer(GL_ARRAY_BUFFER, m_normals);
    glNormalPointer(GL_FLOAT, 0, 0);
    funcs.glEnableVertexAttribArray(m_coordNormal);
    funcs.glVertexAttribPointer(m_coordNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Draw frame
    glDrawArrays(GL_TRIANGLES, 0, (3 * m_triangles.count()));

    funcs.glDisableVertexAttribArray(m_coordNormal);
    funcs.glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

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

            qDebug() << "Triangles \n" << triangles.data()->p1;
            qDebug() << "Triangles \n" << triangles.data()->p2;
            qDebug() << "Triangles \n" << triangles.data()->p3;

            qDebug() << "Triangles \n" << triangles.data()->p1UV;
            qDebug() << "Triangles \n" << triangles.data()->p2UV;
            qDebug() << "Triangles \n" << triangles.data()->p3UV;

            qDebug() << "Triangles \n" << triangles.data()->p1Normal;
            qDebug() << "Triangles \n" << triangles.data()->p2Normal;
            qDebug() << "Triangles \n" << triangles.data()->p3Normal;
        }


    }

    float* coords = GenerateVertexBuffer(triangles);
    if(coords != 0)
    {
        QOpenGLFunctions funcs(QOpenGLContext::currentContext());
        funcs.glGenBuffers(1, &m_vertexes);
        funcs.glGenBuffers(1, &m_normals);
        if((m_vertexes != 0) && (m_normals != 0))
        {
            funcs.glBindBuffer(GL_ARRAY_BUFFER, m_vertexes);
            funcs.glBufferData(GL_ARRAY_BUFFER,
                (3 * 3 * triangles.count() * sizeof(float)),
                coords,
                GL_STATIC_DRAW);

            GenerateNormalsBuffer(triangles, coords);
            funcs.glBindBuffer(GL_ARRAY_BUFFER, m_normals);
            funcs.glBufferData(GL_ARRAY_BUFFER,
                (3 * 3 * triangles.count() * sizeof(float)),
                coords,
                GL_STATIC_DRAW);

            funcs.glBindBuffer(GL_ARRAY_BUFFER, 0);
            qDebug() << "Loaded Buffer Data";
        }
        else
        {
            FreeRenderData();
            qDebug() << "Freed Render Data";
        }
        ReleaseVertexBuffer(coords);
    }
}


void QGLBegin::FreeRenderData()
{
    QOpenGLFunctions funcs(QOpenGLContext::currentContext());
    if(m_vertexes != 0)
    {
        funcs.glDeleteBuffers(1, &m_vertexes);
        m_vertexes = 0;
    }
}

void QGLBegin::GetMatrixTransform(QMatrix4x4 matrixVertex,
                        QVector<QOpenGLTriangle3D> triangles)
{
    matrixVertex.setToIdentity();

     QMatrix4x4 matrixScaleScreen;
     double dimMin = static_cast<double>(qMin(width(), height()));
     float scaleScreenVert = static_cast<float>(dimMin /
         static_cast<double>(height()));
     float scaleScreenHorz = static_cast<float>(dimMin /
         static_cast<double>(width()));
     matrixScaleScreen.scale(scaleScreenHorz, scaleScreenVert, 1.0f);

     QMatrix4x4 matrixCenter;
     float centerX, centerY, centerZ;
     centerX = 0.0f; centerY = 0.0f; centerZ = 1.0f;
     matrixCenter.translate(-centerX, -centerY, -centerZ);

     QMatrix4x4 matrixScale;
     matrixScale.scale(1.0f);


     QMatrix4x4 matrixTranslateScene;
     matrixTranslateScene.translate(0.0f, 0.0f, -0.5f);

     matrixVertex = matrixScaleScreen * matrixTranslateScene * matrixScale * m_matrixRotate * matrixCenter;


}

float* QGLBegin::GenerateVertexBuffer(const QVector<QOpenGLTriangle3D> triangles)
{
    const unsigned int triangleCount = triangles.count();
    float* pointCoord = new float[3 * 3 * triangleCount];

    if(pointCoord != 0)
    {
        const unsigned int* indexes = new unsigned int[triangleCount * 3];
        const float* pointCoordSrc = pointCoord;
        float* coord = pointCoord;
        for(unsigned int triangle = 0 ; triangle < triangleCount ; ++triangle)
        {
            for(unsigned int vertex = 0 ; vertex < 3 ; ++vertex, ++indexes)
            {
                const float* coordSrc = (pointCoordSrc + (3 * (*indexes)));
                (*(coord++)) = (*(coordSrc++));
                (*(coord++)) = (*(coordSrc++));
                (*(coord++)) = (*(coordSrc++));
            }
        }
    }

    return pointCoord;
}


void QGLBegin::GenerateNormalsBuffer(const QVector<QOpenGLTriangle3D> triangles, float* coords)
{
    const unsigned int triangleCount = triangles.count();
    const unsigned int* indexes =  new unsigned int[triangleCount * 3];
    const float* points =  new float[3 * 3 * triangleCount];
    for(unsigned int triangle = 0 ; triangle < triangleCount ; ++triangle, indexes += 3)
    {
        const float* pointCoord1 = (points + (3 * indexes[0]));
        const float* pointCoord2 = (points + (3 * indexes[1]));
        const float* pointCoord3 = (points + (3 * indexes[2]));

        QVector3D normal(QVector3D::crossProduct(
            QVector3D((pointCoord2[0] - pointCoord1[0]), (pointCoord2[1] - pointCoord1[1]), (pointCoord2[2] - pointCoord1[2])),
            QVector3D((pointCoord3[0] - pointCoord1[0]), (pointCoord3[1] - pointCoord1[1]), (pointCoord3[2] - pointCoord1[2])))
                .normalized());

        (*(coords++)) = normal[0];
        (*(coords++)) = normal[1];
        (*(coords++)) = normal[2];
        (*(coords++)) = normal[0];
        (*(coords++)) = normal[1];
        (*(coords++)) = normal[2];
        (*(coords++)) = normal[0];
        (*(coords++)) = normal[1];
        (*(coords++)) = normal[2];
    }
}


void QGLBegin::ReleaseVertexBuffer(float* buffer)
{
    delete [] buffer;
}

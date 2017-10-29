#include "modelengine.h"

#include <QVector2D>
#include <QVector3D>
#include <QFile>

ModelEngine::ModelEngine()
{
    //init();
    loaded = false;
}


ModelEngine::~ModelEngine()
{
    glDeleteBuffers(2, m_vboId);
}


void ModelEngine::init()
{
    //initializeGLFunctions();
    //makeCurrent();
    initializeOpenGLFunctions();

    // Generate VBO
    glGenBuffers(2, m_vboId);

}


void ModelEngine::loadModel(const QString &fileName)
{
    QStringList comments;
    m_triangles.clear();

    QFile file(fileName);
    if(file.exists())
    {
        qDebug() << "blahs";
        printf("file exists \n");


        //initializeGLFunctions();
        //makeCurrent();
        initializeOpenGLFunctions();

        // Generate VBO
        glGenBuffers(2, m_vboId);


        if(file.open(QFile::ReadOnly | QFile::Text))
        {
            QVector<QVector3D> v, vn;
            QVector<QVector2D> vt;

            printf("File open \n");

            while(!file.atEnd())
            {

                qDebug() << "In the loop";

                QString line = file.readLine().trimmed();
                QList<QString> lineParts = line.split(' ');

                qDebug() << "lineParts " << lineParts;

                if(lineParts.count() > 0)
                {
                    qDebug() << "lineparts first == " << lineParts.first();
                    // if it’s a comment
                    if(lineParts.first() == '#')
                    {
                        qDebug() << "lineparts first == '#' ";
                        comments.append(line.remove(0, 1).trimmed());
                    }

                    // if it’s a vertex position (v)
                    else if(lineParts.first() == "v")
                    {
                        qDebug() << "lineparts first == 'v' ";
                        v.append(QVector3D(lineParts.at(1).toFloat(),
                                           lineParts.at(2).toFloat(),
                                           lineParts.at(3).toFloat()));
                    }

                    // if it’s a normal (vn)
                    else if(lineParts.first() == "vn")
                    {
                        qDebug() << "lineparts first == 'vn' ";
                        vn.append(QVector3D(lineParts.at(1).toFloat(),
                                            lineParts.at(2).toFloat(),
                                            lineParts.at(3).toFloat()));
                    }

                    // if it’s a texture (vt)
                    else if(lineParts.first() == "vt")
                    {
                        qDebug() << "lineparts first == 'vt' ";
                        vt.append(QVector2D(lineParts.at(1).toFloat(),
                        lineParts.at(2).toFloat()));
                    }

                    // if it’s face data (f)
                    // there’s an assumption here that faces are all triangles
                    else if(lineParts.first() == "f")
                    {
                       QOpenGLTriangle3D triangle;

                       qDebug() << "lineparts first == 'f' ";
                       qDebug() << "v: " << v;
                       //qDebug() << "p1 index" << lineParts.at(1).split('//').value(0).toInt() - 1;
                       //qDebug() << "p2 index" << lineParts.at(1).split("//").at(1);

                        // get points from v array
                        triangle.p1 = v.at(lineParts.at(1).split("//").value(0).toInt() - 1);
                        qDebug() << "p1 found";
                        triangle.p2 = v.at(lineParts.at(2).split("//").at(0).toInt() - 1);
                        qDebug() << "p2 found";
                        triangle.p3 = v.at(lineParts.at(3).split("//").at(0).toInt() - 1);

                        qDebug() << "vn" << vn;
                        qDebug() << "vt" << vt;

                        /*if(vt.count() > 0) // check if really there are any UV coords
                        {
                            triangle.p1UV = vt.at(lineParts.at(1).split("//").at(1).toInt() - 1);
                            triangle.p2UV = vt.at(lineParts.at(2).split("//").at(1).toInt() - 1);
                            triangle.p3UV = vt.at(lineParts.at(3).split("//").at(1).toInt() - 1);
                        }*/

                        // get normals from vn array
                        /*triangle.p1Normal = vn.at(lineParts.at(1).split("//").at(1).toInt() - 1);
                        triangle.p2Normal = vn.at(lineParts.at(2).split("//").at(1).toInt() - 1);
                        triangle.p3Normal = vn.at(lineParts.at(3).split("//").at(1).toInt() - 1); */

                        m_triangles.append(triangle);
                        qDebug() << "Triangles count" << m_triangles.count();
                    }

                }
            }

            file.close();

            // Initialize model geometry
            initModel();
        }

    }

    loaded = true;
}


void ModelEngine::initModel()
{

    // Indices to select which vertices are drawn
    /*GLushort indices[] = {
        0,  1,  2,  3,  3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
         4,  4,  5,  6,  7,  7, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
         8,  8,  9, 10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
        12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
        16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
        20, 20, 21, 22, 23      // Face 5 - triangle strip (v20, v21, v22, v23)
    };*/

    GLushort *indices;
    indices = new GLushort[m_triangles.size() * 3];
    for (int i = 0; i < m_triangles.size(); i++)
    {
        indices[3*i+0] = 3*i;
        indices[3*i+1] = 3*i+1;
        indices[3*i+2] = 3*i+2;
    }


    glBindBuffer(GL_ARRAY_BUFFER, m_vboId[0]);
    glBufferData(GL_ARRAY_BUFFER, m_triangles.size() * sizeof(QOpenGLTriangle3D), m_triangles.constData(), GL_STATIC_DRAW);

    // Transfer index data to VBO 1
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboId[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_triangles.size() * sizeof(GLushort), indices, GL_STATIC_DRAW);

}


void ModelEngine::drawModel(QGLShaderProgram *program)
{
    // Tell OpenGL which VBOs to use
    glBindBuffer(GL_ARRAY_BUFFER, m_vboId[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboId[1]);

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(QOpenGLTriangle3D), (const void *)offset);

    // Offset for texture coordinate
    offset += 6*sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(QOpenGLTriangle3D), (const void *)offset);

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLE_STRIP, 3*m_triangles.size(), GL_UNSIGNED_SHORT, 0);
}

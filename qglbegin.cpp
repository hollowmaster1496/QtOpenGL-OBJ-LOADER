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

    m_shaderProgram.addShaderFromSourceCode(QOpenGLShader::Vertex,
        QString::fromUtf8(
            "#version 400\r\n"
            "\r\n"
            "layout (location = 0) in vec3 coordVertexes;\r\n"
            "layout (location = 1) in vec3 coordNormals;\r\n"
            "flat out float lightIntensity;\r\n"
            "\r\n"
            "uniform mat4 matrixVertex;\r\n"
            "uniform mat4 matrixNormal;\r\n"
            "\r\n"
            "void main()\r\n"
            "{\r\n"
            "   gl_Position = matrixVertex * vec4(coordVertexes, 1.0);\r\n"
            "   lightIntensity = abs((matrixNormal * vec4(coordNormals, 1.0)).z);\r\n"
            "}"));
    m_shaderProgram.addShaderFromSourceCode(QOpenGLShader::Fragment,
        QString::fromUtf8(
            "#version 400\r\n"
            "\r\n"
            "flat in float lightIntensity;\r\n"
            "\r\n"
            "layout (location = 0) out vec4 FragColor;\r\n"
            "uniform vec3 fragmentColor;\r\n"
            "\r\n"
            "void main()\r\n"
            "{\r\n"
            "    FragColor = vec4(fragmentColor * lightIntensity, 1.0);\r\n"
            "}"));
    m_shaderProgram.link();
    m_shaderProgram.bind();

    m_coordVertex = m_shaderProgram.attributeLocation(QString::fromUtf8("coordVertexes"));
    m_coordNormal = m_shaderProgram.attributeLocation(QString::fromUtf8("coordNormals"));
    m_matrixVertex = m_shaderProgram.uniformLocation(QString::fromUtf8("matrixVertex"));
    m_matrixNormal = m_shaderProgram.uniformLocation(QString::fromUtf8("matrixNormal"));
    m_colorFragment = m_shaderProgram.uniformLocation(QString::fromUtf8("fragmentColor"));

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

    // Set Shader Program object' parameters
    m_shaderProgram.setUniformValue(m_matrixVertex, matrixVertex);
    m_shaderProgram.setUniformValue(m_matrixNormal, m_matrixRotate);
    QColor fragmentColor(0x00FF00);
    m_shaderProgram.setUniformValue(m_colorFragment,
        static_cast<GLfloat>(fragmentColor.red()) / 256.0f,
        static_cast<GLfloat>(fragmentColor.green()) / 256.0f,
        static_cast<GLfloat>(fragmentColor.blue()) / 256.0f);

    QOpenGLFunctions funcs(QOpenGLContext::currentContext());

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

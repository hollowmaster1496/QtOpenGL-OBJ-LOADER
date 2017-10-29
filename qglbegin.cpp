/* Credits: Amin Ahmadi
 * http://amin-ahmadi.com/2016/05/24/opengl-qt-beginners-example-project/
 */

#include "qglbegin.h"

QGLBegin::QGLBegin(QWidget *parent) : QOpenGLWidget(parent)
{
    xrot = yrot = zrot = 0.0; 
}

QGLBegin::~QGLBegin()
{

}


void QGLBegin::openFile(const QString &fileName)
{
    model.loadModel(fileName);
}


void QGLBegin::initializeGL()
{

    QOpenGLWidget::initializeGL();

    /*m_shaderProgram.addShaderFromSourceCode(QOpenGLShader::Vertex,
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
    */

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
           f->glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    initShaders();
    initTextures();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    //glShadeModel(GL_FLAT);
    //glDisable(GL_CULL_FACE);


}


void QGLBegin::paintGL()
{
    /* // Erase the scene
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
    QColor fragmentColor(0xFFFFFF);
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
    glDisableClientState(GL_NORMAL_ARRAY); */

    qDebug() << "paintGL()";

    // Clear color
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
            f->glClear(GL_COLOR_BUFFER_BIT);

    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, 0.0);
    matrix.rotate(QQuaternion(1, xrot, yrot, zrot));

    // Set modelview-projection matrix
    //m_shaderProgram.setUniformValue("mvp_matrix", projection * matrix);

    // Use texture unit 0 which contains cube.png
    //m_shaderProgram.setUniformValue("texture", 0);

    // Set modelview-projection matrix
    m_shaderProgram.setUniformValue("mvp_matrix", projection * matrix);

    // Use texture unit 0 which contains cube.png
    m_shaderProgram.setUniformValue("texture", 0);

    // Draw cube geometry
    if(model.loaded)
        model.drawModel(&m_shaderProgram);  /*This line is resulting in issue 3 on first run*/

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
     centerX = 0.0f; centerY = 0.0f; centerZ = -1.0f;
     matrixCenter.translate(-centerX, -centerY, -centerZ);

     QMatrix4x4 matrixScale;
     matrixScale.scale(1.0f);


     QMatrix4x4 matrixTranslateScene;
     matrixTranslateScene.translate(0.0f, 0.0f, -0.5f);

     QMatrix4x4 matrixRotateScene;
     matrixRotateScene.rotate(QQuaternion(1, xrot, yrot, zrot));

     matrixVertex = matrixScaleScreen * matrixTranslateScene * matrixScale * matrixRotateScene * matrixCenter;

}


void QGLBegin::initShaders()
{
    // Override system locale until shaders are compiled
    setlocale(LC_NUMERIC, "C");

    qDebug() << "initshaders";


    // Compile vertex shader
    if (!m_shaderProgram.addShaderFromSourceFile(QGLShader::Vertex, ":/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!m_shaderProgram.addShaderFromSourceFile(QGLShader::Fragment, ":/fshader.glsl"))
        close();

    // Link shader pipeline
    if (!m_shaderProgram.link())
        close();

    // Bind shader pipeline for use
    if (!m_shaderProgram.bind())
        close();

    // Restore system locale
    setlocale(LC_ALL, "");
}


void QGLBegin::initTextures()
{
    // Load cube.png image
    glEnable(GL_TEXTURE_2D);
    //texture = bindTexture(QImage(":/cube.png"));

    //glBindTexture(QImage(":/cube.png"), texture);

    // Set nearest filtering mode for texture minification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // Set bilinear filtering mode for texture magnification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}



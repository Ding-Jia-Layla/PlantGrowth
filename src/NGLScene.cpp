#include <QMouseEvent>
#include <QGuiApplication>
#include <ngl/Transformation.h>
#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <ngl/Util.h>
#include <iostream>


NGLScene::NGLScene()
{
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  setTitle("Vegetables");
}
NGLScene::~NGLScene()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
}
void NGLScene::resizeGL(int _w , int _h)
{
    std::cout<<"resizeGL"<<std::endl;
  m_win.width  = static_cast<int>( _w * devicePixelRatio() );
  m_win.height = static_cast<int>( _h * devicePixelRatio() );
  m_project = ngl::perspective(45.0f,static_cast<float>(_w)/_h,0.1f,100.0f);
}
//constexpr  char *ColourShader = "ColourShader";
void NGLScene::initializeGL()
{
    std::cout<<"initializeGL"<<std::endl;
    // we must call that first before any other GL commands to load and link the
  // gl commands from the lib, if that is not done program will crash
  ngl::NGLInit::initialize();
  glClearColor(0.7f, 0.7f, 0.7f, 1.0f);			   // Grey Background
  glPointSize(4.0);
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);

  //ctor
  m_frac = std::make_unique<FractalSystem>(ngl::Vec3(0.0f,1.0f,0.0f),ngl::Vec3(0.0f,0.0f,0.0f));
  m_view = ngl::lookAt({0,15,15},{0,0,0},{0,1,0});
  ngl::VAOPrimitives::createLineGrid("floor",10,10,10);
    m_frac->addGeneration();
    m_frac->generatePath();
    startTimer(10);
    std::string m_objFileName("mesh/Cylinder.obj");
    m_mesh.reset(new ngl::Obj(m_objFileName));
    m_mesh->createVAO();
}
void NGLScene::paintGL()
{
    //std::cout<<"paintGL calling"<<std::endl;
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_win.width,m_win.height);
    auto rotX = ngl::Mat4::rotateX(m_win.spinXFace);
    auto rotY = ngl::Mat4::rotateY(m_win.spinYFace);
    auto mouseRotation = rotX * rotY;
    mouseRotation.m_m[3][0] = m_modelPos.m_x;
    mouseRotation.m_m[3][1] = m_modelPos.m_y;
    mouseRotation.m_m[3][2] = m_modelPos.m_z;
    ngl::ShaderLib::loadShader("VegetablesShader","shaders/VVertex.glsl","shaders/VFragment.glsl");
    ngl::ShaderLib::use("VegetablesShader");
    ngl::ShaderLib::setUniform("MVP",m_project*m_view*mouseRotation);
    ngl::ShaderLib::setUniform("PlantColor",0.5f,0.0f,1.0f,1.0f);
    m_frac->renderVAO();

    ngl::ShaderLib::use(ngl::nglColourShader);
    ngl::ShaderLib::setUniform("Colour",0.6f, 0.6f, 0.6f, 1.0f);
    ngl::ShaderLib::setUniform("MVP",m_project * m_view*mouseRotation);
    ngl::VAOPrimitives::draw("floor");
    m_mesh->draw();


}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  // escape key to quite
  case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
  case Qt::Key_Space :
      m_win.spinXFace=0;
      m_win.spinYFace=0;
      m_modelPos.set(ngl::Vec3::zero());
  break;
  default : break;
  }
  // finally update the GLWindow and re-draw
   update();
}
void NGLScene::timerEvent(QTimerEvent *_event)
{
    if(!m_animate)
        return;
    static float t=0.0f;

//all of each plant
//    for(size_t i=1; plant.size(); ++i)
//    {
//        plant[i].m_x+=sinf(t*plant[i].m_y)*0.001f;
//        plant[i].m_z+=cosf(t*plant[i].m_y)*0.001f;
//
//    }

    t+=0.05f;
    update();
}



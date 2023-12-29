#include "FractalSystem.h"
#include <ngl/VAOFactory.h>
#include <iostream>
#include <ngl/Random.h>
#include "NGLScene.h"
#include <ngl/SimpleIndexVAO.h>
#include <cmath>
#include <limits>
#include <ngl/Transformation.h>
bool State::operator==(const State &ref) const
{
    return(this->dir==ref.dir);
}

FractalSystem::FractalSystem(ngl::Vec3 _dir, ngl::Vec3 _pos)
{
//    std::uniform_int_distribution<> numLines(2,5);
//    ngl::Random::addIntGenerator("numLines",numLines);
    currentState.dir = _dir;
    currentState.pos = _pos;
    //m_vao = ngl::VAOFactory::createVAO(ngl::simpleIndexVAO,GL_LINE_STRIP);

    m_vao = ngl::VAOFactory::createVAO(ngl::simpleIndexVAO,GL_LINE_STRIP);
}
void FractalSystem::addGeneration()
{
    std::cout<<"add rules"<<std::endl;
    //must add all happened elements
    g.setGrammarName("test");
    char letterA = 'F';
    std::string wordA = "F";
    g.addGeneration(letterA,wordA);
//    g.addGeneration('X',"F[FX]F");
//START POINT EXISTED
//store(0)<point[index]> go(1) store(1) go(2) back(1) go(3) back(0) go(4)
    g.addGeneration('X',"F[+[[[X]+F]++F]+++F");
    g.setStart("X");
    //TODO:iterate calling shouldn't in here.
    g.iterate(1);


}
/*
+ Turn left by angle δ, using rotation matrix RU(δ).
− Turn right by angle δ, using rotation matrix RU(−δ).
& Pitch down by angle δ, using rotation matrix RL(δ).
∧ Pitch up by angle δ, using rotation matrix RL(−δ).
\ Roll left by angle δ, using rotation matrix RH(δ).
/ Roll right by angle δ, using rotation matrix RH(−δ).
| Turn around, using rotation matrix RU(180◦).

 */
//TODO:add random function to select one of the grammar.
State FractalSystem::getCurrentStates()
{
    return currentState;
}

void FractalSystem::generatePath() {
    std::cout<<"forming the path"<<std::endl;

    //default constructor
    ngl::Transformation tx;
    tx.setRotation(-10,0,0);

    stateStack.push_back(currentState);
    std::cout<<g.getResult()<<std::endl;
    //get the current state
    //using grammar generate the trunk: trunk.pos = currentState.pos
    for (size_t i = 0; i < g.getResult().size(); i++) {
        char ch = g.getResult()[i];
        trunk temp_trunk;
        switch (ch) {
            case 'F':
                temp_trunk.startPos = currentState.pos;
                currentState.pos.m_x += currentState.dir.m_x * distance;
                currentState.pos.m_y += currentState.dir.m_y * distance;
                currentState.pos.m_z += currentState.dir.m_z * distance;
                temp_trunk.endPos = currentState.pos;
                m_tree.push_back(temp_trunk);
                break;
                //[ ] are just to manage the conditions
            case '[':
                //add this state
                stateStack.push_back(currentState);
                break;
            case ']':
                //When bifurcating, the starting point should always remain at the same location
                //to get this one, need to delete the new one after add it.
                currentState = stateStack.at(stateStack.size() - 1);
                stateStack.pop_back();
                //currentState.dir = ngl::Vec3{-1.5f, 2.0f, 0.0f};
                break;
            case '+':
               currentState.dir = ngl::Vec4{currentState.dir,1.0}*tx.getMatrix();
               std::cout<<currentState.dir.m_x
                        <<currentState.dir.m_y
                        <<currentState.dir.m_z
                        <<std::endl;
               break;


            default:
                break;
        }
    }

}
//TODO:renderVAO must be in the generatePath()
void FractalSystem::renderVAO() {
    std::vector<GLuint> index;
    m_points.clear();
    index.clear();

    //green
    ngl::Vec3 baseColour(0.0f,1.0f,0.0f);
    //yellow
    ngl::Vec3 tipColour(1.0f, 1.0f,0.1f);
    GLuint idx=0;
    GLuint restart = std::numeric_limits<GLuint>::max() - 1;
    int restartCount = 0;
    //add all points of each branch
    for (auto t: m_tree) {
        m_points.push_back(t.startPos);
        m_points.push_back(baseColour);
        index.push_back(idx++);
        m_points.push_back(t.endPos);
        m_points.push_back(tipColour);
        index.push_back(idx++);
        index.push_back(restart);
    }

    for (auto dx : index)
    {
        if (dx == restart)
        {
            restartCount++;
        }
    }
        std::cout<<"Breaking down Count: "<<restartCount<<
             "total points: "<<m_points.size()<<std::endl;

    //TODO: 放在GL的paintGL里之后就不停地产生了新的点points一直疯狂增加
    m_vao->bind();
    glPrimitiveRestartIndex(restart);
    glEnable(GL_PRIMITIVE_RESTART);
    m_vao->setData(ngl::SimpleIndexVAO::VertexData(
            m_points.size()*sizeof(ngl::Vec3),
            m_points[0].m_x,
            index.size(), &index[0],
            GL_UNSIGNED_INT));
    //how to locate each vertex to set all attributes of each vertex
    //each point in the array has a position and a color, a single point occupies the size of 2 ngl::Vec3.
    m_vao->setVertexAttributePointer(0, 3, GL_FLOAT, sizeof (ngl::Vec3)*2, 0);
    m_vao->setVertexAttributePointer(1, 3, GL_FLOAT, sizeof(ngl::Vec3)*2, 3);
    //16 points have 4 segments 8 positions and 8 color
    m_vao->setNumIndices(m_points.size());
    m_vao->draw();
    m_vao->unbind();
    glDisable(GL_PRIMITIVE_RESTART);
}

//TODO:B-Spline
//std::vector<ngl::Vec3> FractalSystem::Line(ngl::Vec3 _sPoint, ngl::Vec3 _ePoint) {
//    m_points.push_back(_sPoint);
//    //number of points
//
//    int numLines = ngl::Random::getIntFromGeneratorName("numLines");
//    auto height = _ePoint.m_y-_sPoint.m_y;
//    auto width = _ePoint.m_x-_sPoint.m_x;
////the length of each step between the start point and the end point
//    float stepY=height/static_cast<float>(numLines);
//    float stepX=width/static_cast<float>(numLines);
//
//    auto pos = _sPoint;
//    for(int i=0; i<numLines; ++i)
//    {
//        float t = static_cast<float>(i) / numLines;  // Parameterized value between 0 and 1
//        float angle = t * 2.0f * M_PI;
//        // Full rotation (2*pi) based on parameter
//        float radius = 1.0f;  // Adjust the radius of the twisting motion
//        float twistedZ = pos.m_z + radius * sin(angle);  // Twist in the Z-axis
//        pos.m_y +=stepY;
//        pos.m_x += stepX;
//        pos.m_z += twistedZ;
//        m_points.push_back(pos);
//    }
//    return m_points;
//    }
















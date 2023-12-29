#include "FractalSystem.h"

#include <iostream>
#include <ngl/Random.h>
#include "NGLScene.h"

#include <cmath>
#include <limits>
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
    g.addGeneration('X',"F[F[FX]F]F");
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
//                Line(temp_trunk.startPos,temp_trunk.endPos);
//                for(auto p:m_points)
//                {
//                    temp_trunk.addPosToTrunk(p);
//                }
                m_tree.push_back(temp_trunk);
                break;
                //[ ] are just to manage the conditions
            case '[':
                //add this state
                currentState.dir = ngl::Vec3{1.0f, 1.0f, 0.0f};
                stateStack.push_back(currentState);
                break;
            case ']':
                //When bifurcating, the starting point should always remain at the same location
                //to get this one, need to delete the new one after add it.
                currentState = stateStack.at(stateStack.size() - 1);
                stateStack.pop_back();
                currentState.dir = ngl::Vec3{-1.5f, 2.0f, 0.0f};
                break;
            default:
                break;
        }
    }
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
















#include<Arduino.h>

//Prevent multiple definitions
#ifndef VECTOR_H
#define VECTOR_H

class VectorP;
class Vector2;

class VectorP
{
    public:
        float angle = 0;
        float length = 0;

        VectorP(float newAngle, float newLength){
            angle = newAngle;
            length = newLength;
        }
        VectorP operator * (float const &factor) {
            return VectorP(angle, length * factor);
        }
        VectorP operator / (float const &factor) {
            return VectorP(angle, length / factor);
        }


        VectorP normalized(){
            VectorP vect(angle,length);
            length = 1;
            return vect;
        }
        VectorP toRadians(){
            VectorP vect(angle,length);
            vect.angle *= PI/180;
            return vect;
        }
        VectorP toDegrees(){
            VectorP vect(angle,length);
            vect.angle *= 180/PI;
            return vect;
        }

        operator Vector2();
};
class Vector2
{
    public:
        float x = 0;
        float y = 0;

        Vector2(float newX, float newY){
            x = newX;
            y = newY;
        }
        Vector2(){
            
        }
        Vector2 operator + (Vector2 const &obj) {
            Vector2 vect(0,0);
            vect.x = x + obj.x;
            vect.y = y + obj.y;
            return vect;
        }
        Vector2 operator - (Vector2 const &obj) {
            Vector2 vect(0,0);
            vect.x = x - obj.x;
            vect.y = y - obj.y;
            return vect;
        }
        Vector2 operator * (float const &factor) {
            Vector2 vect(0,0);
            vect.x = x * factor;
            vect.y = y * factor;
            return vect;
        }
        Vector2 operator / (float const &factor) {
            Vector2 vect(0,0);
            vect.x = x / factor;
            vect.y = y / factor;
            return vect;
        }

        Vector2 normalized(){
            Vector2 vect(x,y);
            float length = sqrt(x*x + y*y);
            vect.x /= length;
            vect.y /= length;
            return vect;
        }

        operator VectorP();
};

VectorP::operator Vector2()
{
  Vector2 obj(0,0);
  obj.y = sin(angle) * length;
  obj.x = cos(angle) * length;
  return obj;   
}

Vector2::operator VectorP()
{
  VectorP obj(0,0);
  obj.length = sqrt(x*x+y*y);
  obj.angle = atan2(y, x);
  return obj;
   
}

#endif
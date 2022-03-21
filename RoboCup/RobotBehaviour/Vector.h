#include<Arduino.h>
class Vector2
{
    public:
        float x = 0;
        float y = 0;

        Vector2(float newX, float newY){
            x = newX;
            y = newY;
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

        void normalize(){
            float length = sqrt(x*x + y*y);
            x /= length;
            y /= length;
        }
};
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
        Vector2 ToVector2(){
            return Vector2(cos(angle * PI / 180), sin(angle * PI / 180)) * length;
        }

        void normalize(){
            length = 1;
        }
};
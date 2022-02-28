
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

#include <bits/stdc++.h>
using namespace std;

int main() {
    float middle_x = 0.45, middle_z = 0.0;
    float radius = 0.20;

    float leftmost_x = middle_x - radius;
    float rightmost_x = middle_x + radius;
    float x = leftmost_x;
    float len;
    for (x = leftmost_x; x <= rightmost_x; x = x + 0.05){
        len = x - middle_x;
        float z  = sqrt(abs(radius*radius - len*len));
        cout << "x : "  << x << "  z : "  << z << endl;
    }

    len = x - middle_x;
    float z  = sqrt(abs(radius*radius - len*len));
    cout << "x : "  << x << "  z : "  << z << endl;

}
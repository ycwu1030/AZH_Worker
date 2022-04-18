#ifndef AZH_BARYCENTER_H_
#define AZH_BARYCENTER_H_

struct Point {
    double x;
    double y;
};

class Triangle {
public:
    Triangle();
    Triangle(Point x1, Point x2, Point x3);

    void Get_Barycentric_Coordinate(const Point &P, double *lams);
    Point &P(int i);
    Point P0, P1, P2;
    // * Convention for P0, P1, P2
    // * Either:
    // * P1 --- P2
    // *  |   /
    // *  |  /
    // *  | /
    // * P0
    // * Or:
    // *        P2
    // *      / |
    // *     /  |
    // *    /   |
    // * P0 --- P1
};

#endif  // AZH_BARYCENTER_H_

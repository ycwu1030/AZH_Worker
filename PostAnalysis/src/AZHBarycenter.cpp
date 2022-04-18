#include "AZHBarycenter.h"

#include <algorithm>
#include <vector>

Triangle::Triangle() : P0{0, 0}, P1{0, 1}, P2{1, 1} {}

bool PointComp(const Point &p1, const Point &p2) { return (p1.y <= p2.y && p1.x <= p2.x); }

Triangle::Triangle(Point x1, Point x2, Point x3) {
    std::vector<Point> vP{x1, x2, x3};
    std::sort(vP.begin(), vP.end(), PointComp);
    P0 = vP[0];
    P1 = vP[1];
    P2 = vP[2];
}

Point &Triangle::P(int i) {
    switch (i) {
        case 0:
            return P0;
        case 1:
            return P1;
        case 2:
            return P2;
        default:
            return P0;
    }
}

void Triangle::Get_Barycentric_Coordinate(const Point &P, double *lams) {
    double area_2 = P0.x * (P1.y - P2.y) + P1.x * (P2.y - P0.y) + P2.x * (P0.y - P1.y);
    lams[0] = ((P1.y - P2.y) * (P.x - P2.x) + (P2.x - P1.x) * (P.y - P2.y)) / area_2;
    lams[1] = ((P2.y - P0.y) * (P.x - P2.x) + (P0.x - P2.x) * (P.y - P2.y)) / area_2;
    lams[2] = 1.0 - lams[0] - lams[1];
}

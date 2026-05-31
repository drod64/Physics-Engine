#ifndef SPE_COLLISIONDETECTOR_H
#define SPE_COLLISIONDETECTOR_H
#include <cmath>
#include <vector>
#include <SGE/entities/Entity.h>
#include <SPE/Vec2.h>

class CollisionDetector {
public:
    /**
     * A narrow phase collision detection algorithm for a pair of potentially colliding objects.
     * This function basically implements the SAT (Separating Axis Theorem) algorithm.
     * @param object1 the first object
     * @param object2 the second object
     * @return true if the objects are colliding, false otherwise
     */
    static bool isColliding(sge::Entity& object1, sge::Entity& object2);

private:
    /**
     * Helper function which grabs all the normals (unit vectors perpendicular to the edges of the shape).
     * The only needed information to calculate this is all vertices (points) of the shape.
     * Steps implemented in this function...
     * 1. Calculate the edge vectors (or simply edges) of the shape
     * 2. Get the orthogonal vectors of the edges
     * 3. Convert each orthogonal vector into a normal (unit vector form)
     * @param vertices the vertices of the shape
     * @return a std::vector<sge::Vec2> containing the normals (unit vectors perpendicular to the edges) of the shape
     */
    static std::vector<sge::Vec2> getNormals(const std::vector<sge::Vec2>& vertices);

    /**
     * Helper function which grabs all the vertices (points) of the GameObject object.
     * @param component the GameObject to get the vertices from
     * @return a std::vector<sge::Vec2> containing all the vertices of the GameObject object
     */
    static std::vector<sge::Vec2> getVertices(const sge::Entity& component);

    /**
     * This function projects vertices1 and vertices2 (the points of both the GameObject objects)
     * onto the passed in collection of normals.
     * @param vertices1 expected to be the vertices of the first GameObject object
     * @param vertices2 expected to be the vertices of the second GameObject object
     * @param normals a collection of normals calculated beforehand from one of the GameObject objects
     * @param unitVector after all vertices are projected onto all normals, unitVector will be updated to represent the direction of the collision
     * @param minimumOverlap this value will be updated to represent the smallest overlap found during the projections
     * @return true if a separating axis was found, false otherwise
     */
    static bool hasSeparatingAxis(const std::vector<sge::Vec2>& vertices1,
                                const std::vector<sge::Vec2>& vertices2,
                                const std::vector<sge::Vec2>& normals,
                                sge::Vec2& unitVector, float& minimumOverlap);

    /**
     * Returns the scalar necessary to multiply to vector u in order to get the projection of v onto u.
     * @param v the vector which will be projected
     * @param u the vector we want to project onto
     * @return a scalar value which, when multiplied to vector u will give the projection of v onto u
     */
    static float dot(const sge::Vec2& v, const sge::Vec2& u);

    /**
     * Helper function which turns a vector into its orthogonal form.
     * All it really does is rotate the vector by positive 90 degrees.
     * @param v the vector to rotate by 90 degrees
     */
    static void makeOrthogonal(sge::Vec2& v);

    /**
     * Helper function which normalizes a vector, essentially turning it into a unit vector.
     * @param v the vector to normalize
     */
    static void makeNormal(sge::Vec2& v);

    /**
     * Returns the magnitude of the passed in vector.
     * @param v the vector to get the magnitude from
     * @return the magnitude of v
     */
    static float magnitude(const sge::Vec2& v);
};

#endif //SGE_COLLISIONDETECTOR_H
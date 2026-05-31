#ifndef SM_VEC3_H
#define SM_VEC3_H
#include <SM/Precision.h>

namespace sm {

class Vec3 {
public:
    real x;
    real y;
    real z;

    /**
     * Default constructor.
     */
    Vec3();

    /**
     * Explicit constructor.
     * @param x the x-axis position
     * @param y the y-axis position
     * @param z the z-axis position
     */
    Vec3(real x, real y, real z);

    /**
     * Inverts the contents of the Vector3.
     */
    void invert();

    /**
     * Calculates the magnitude of the Vector3.
     * @return the magnitude (length) of the Vector3
     */
    real magnitude() const;

    /**
     * Calculates the squared magnitude of the Vector3.
     * @return the squared magnitude (length) of the Vector3
     */
    real sqrMagnitude() const;

    /**
     * Normalizes the Vector3.
     */
    void normalize();
    
    ////////////////////////////////////////
    //   Modifying Overloaded Operators   //
    ////////////////////////////////////////
    Vec3& operator = (const Vec3 &vec);
    Vec3& operator *= (real scalar);
    Vec3& operator += (const Vec3 &vec);
    Vec3& operator -= (const Vec3 &vec);
    Vec3& operator %= (const Vec3 & vec);
    
    ////////////////////////////////////////////
    //   Non-Modifying Overloaded Operators   //
    ////////////////////////////////////////////
    Vec3 operator * (const real scalar) const;
    Vec3 operator + (const Vec3 &vec) const;
    Vec3 operator - (const Vec3 &vec) const;
    Vec3 operator % (const Vec3 &vec) const;

    /**
     * Updates this Vector3 by multiplying its components with the components of another Vec3.
     * @param vec the other Vec3 to perform the operation with
     * @return this
     */
    Vec3& componentProductUpdate(const Vec3 &vec);

    /**
     * Updates this Vector3 by adding a scaled Vector3 to itself.
     * @param vec the Vec3 to add
     * @param scalar the scalar to apply to the Vec3 argument
     * @return this
     */
    Vec3& addScaledVector(const Vec3 &vec, real scalar);

    /**
     * Returns the dot product between this Vec3 and another Vec3.
     * @param vec the other Vec3 to perform the operation with
     * @return a floating-point scalar
     */
    real dotProduct(const Vec3 &vec) const;

    /**
     * Calculates the component product between this Vec3 and another Vec3.
     * @param vec the other Vec3 to perform the operation with
     * @return a brand new Vec3 representing the component product between both Vec3s
     */
    Vec3 componentProduct(const Vec3 &vec) const;

    /**
     * Calcualtes the vector product between this Vec3 and another Vec3.
     * @param vec the other Vec3 to perform the operation with
     * @return a brand new Vec3 representing the vector product between both Vec3s
     */
    Vec3 vectorProduct(const Vec3 &vec) const;

private:
    float _pad;
};

} // namespace sm

#endif // SM_VEC3_H
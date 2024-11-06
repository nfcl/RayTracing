#ifndef _VEC3_H_
#define _VEC3_H_

class Vec3 {
public:
    double element[3];
    Vec3(double x = 0, double y = 0, double z = 0){
        element[0] = x;
        element[1] = y;
        element[2] = z;
    }
    double x() const {
        return element[0];
    }
    double y() const {
        return element[1];
    }
    double z() const {
        return element[2];
    }
    double operator[](const int index) const {
        return element[index];
    }
    double& operator[](const int index) {
        return element[index];
    }
    Vec3 operator-() const { 
        return Vec3{ -element[0], -element[1], -element[2] };
    }
    Vec3 operator+(const Vec3& other) const { 
        return Vec3{ element[0] + other.element[0], element[1] + other.element[1], element[2] + other.element[2] }; 
    }
    Vec3 operator-(const Vec3& other) const { 
        return Vec3{ element[0] - other.element[0], element[1] - other.element[1], element[2] - other.element[2] }; 
    }
    Vec3 operator*(const double value) const {
        return Vec3{ element[0] * value, element[1] * value, element[2] * value };
    }
    Vec3 operator*(const Vec3& other) const {
        return Vec3{ element[0] * other.element[0], element[1] * other.element[1], element[2] * other.element[2] };
    }
    Vec3 operator/(const double value) const {
        return Vec3{ element[0] / value, element[1] / value, element[2] / value };
    }
    Vec3 operator/(const Vec3& other) const {
        return Vec3{ element[0] / other.element[0], element[1] / other.element[1], element[2] / other.element[2] };
    }
    Vec3& operator+=(const Vec3& other) {
        element[0] += other.element[0];
        element[1] += other.element[1];
        element[2] += other.element[2];
        return *this;
    }
    Vec3& operator-=(const Vec3& other) {
        element[0] -= other.element[0];
        element[1] -= other.element[1];
        element[2] -= other.element[2];
        return *this;
    }
    Vec3& operator*=(const double value) {
        element[0] *= value;
        element[1] *= value;
        element[2] *= value;
        return *this;
    }
    Vec3& operator*=(const Vec3& other) {
        element[0] *= other.element[0];
        element[1] *= other.element[1];
        element[2] *= other.element[2];
        return *this;
    }
    Vec3& operator/=(const double value) {
        element[0] /= value;
        element[1] /= value;
        element[2] /= value;
        return *this;
    }
    Vec3& operator/=(const Vec3& other) {
        element[0] /= other.element[0];
        element[1] /= other.element[1];
        element[2] /= other.element[2];
        return *this;
    }
    double length() const {
        return std::sqrt(length_squared());
    }
    double length_squared() const {
        return element[0] * element[0] + element[1] * element[1] + element[2] * element[2];
    }
    Vec3 normalized() const {
        return *this / this->length();
    }
    static Vec3 random() {
        return Vec3(random_double(), random_double(), random_double());
    }
    static Vec3 random(double min, double max) {
        return Vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }
    static Vec3 random_unit() {
        while(true) {
            Vec3 p = Vec3::random();
            double length = p.length_squared();
            if(1e-160 < length && length <= 1){
                return p / sqrt(length);
            }
        }
    }
    static double dot(const Vec3& left, const Vec3& right) {
        return left.element[0] * right.element[0] + left.element[1] * right.element[1] + left.element[2] * right.element[2];
    }
    static Vec3 cross(const Vec3& left, const Vec3& right) {
        return Vec3{ 
            left.element[1] * right.element[2] - left.element[2] * right.element[1],
            left.element[2] * right.element[0] - left.element[0] * right.element[2],
            left.element[0] * right.element[1] - left.element[1] * right.element[0]
        };
    }
    static Vec3 random_on_hemisphere(const Vec3& normal) {
        Vec3 unit_vec3 = Vec3::random_unit();
        if(Vec3::dot(unit_vec3, normal) > 0.0) {
            return unit_vec3;
        }
        else{
            return -unit_vec3;
        }
    }
    bool near_zero() const {
        double s = 1e-8;
        return (std::fabs(element[0]) < s) && (std::fabs(element[1]) < s) && (std::fabs(element[2]) < s);
    }
};

using Point3 = Vec3;

inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
    return out << "{ " << v.element[0] << " , " << v.element[1] << " , " << v.element[2] << " }";
}

inline Vec3 operator*(const double t, const Vec3& v) {
    return Vec3{ v.element[0] * t, v.element[1] * t, v.element[2] * t };
}

inline Vec3 operator/(const double t, const Vec3& v) {
    return Vec3{ v.element[0] / t, v.element[1] / t, v.element[2] / t };
}

inline Vec3 reflect(const Vec3& v, const Vec3& n) {
    return v - 2 * Vec3::dot(v, n) * n;
}

inline Vec3 refract(const Vec3& uv, const Vec3& n, double etai_over_etat) {
    double cos_theta = std::fmin(Vec3::dot(-uv, n), 1.0);
    Vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    Vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

inline Vec3 random_in_unit_disk() {
    double x = random_double(-1, 1);
    double ylength = std::sqrt(1 - x * x);
    return Vec3(x, random_double(-ylength, ylength));
}

#endif
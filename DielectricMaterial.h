#ifndef _DIELECTRICMATERIAL_H_
#define _DIELECTRICMATERIAL_H_

#include "Material.h"

class DielectricMaterial : public Material {
public:
	DielectricMaterial(double refraction_index) : refraction_index(refraction_index) {}
	bool scatter(const Ray& r_in, const HitInfos& infos, Color& attenuation, Ray& scattered) const override {
		attenuation = Color(1.0, 1.0, 1.0);
		double ri = infos.front_face ? (1.0 / refraction_index) : refraction_index;
		
		Vec3 unit_direction = r_in.direction().normalized();
		double cos_theta = std::fmin(Vec3::dot(-unit_direction, infos.normal), 1.0);
		double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

		bool cannot_refract = (ri * sin_theta) > 1.0;
		Vec3 direction;

		if (cannot_refract) {
			direction = reflect(unit_direction, infos.normal);
		}
		else {
			direction = refract(unit_direction, infos.normal, ri);
		}

		scattered = Ray(infos.p, direction);
		return true;
	}
private:
	double refraction_index;
};

#endif
#include "main.h"

#include "Camera.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Material.h"
#include "LambertianMaterial.h"
#include "MetalMaterial.h"
#include "DielectricMaterial.h"

int main() {

    HittableList world;

    auto R = std::cos(pi / 4);

    auto material_ground = make_shared<LambertianMaterial>(Color(0.8, 0.8, 0.0));
    auto material_center = make_shared<LambertianMaterial>(Color(0.1, 0.2, 0.5));
    auto material_left = make_shared<DielectricMaterial>(1.50);
    auto material_bubble = make_shared<DielectricMaterial>(1.00 / 1.50);
    auto material_right = make_shared<MetalMaterial>(Color(0.8, 0.6, 0.2), 1.0);

    world.Add(make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.Add(make_shared<Sphere>(Point3(0.0, 0.0, -1.2), 0.5, material_center));
    world.Add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.Add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.4, material_bubble));
    world.Add(make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));

    Camera camera;

    camera.aspect_ratio             = 1920.0 / 1080.0;
    camera.image_width              = 480;
    camera.samples_per_pixel        = 100;
    camera.max_reflect_depth        = 50;

    camera.vfov         = 20;
    camera.lookfrom     = Point3(-2, 2, 1);
    camera.lookat       = Point3(0, 0, -1);
    camera.vup          = Vec3(0, 1, 0);

    camera.Render(world);

}
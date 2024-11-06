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

    auto ground_Material = make_shared<LambertianMaterial>(Color(0.5, 0.5, 0.5));
    world.Add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_Material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            Point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> Sphere_Material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color::random() * Color::random();
                    Sphere_Material = make_shared<LambertianMaterial>(albedo);
                    world.Add(make_shared<Sphere>(center, 0.2, Sphere_Material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    Sphere_Material = make_shared<MetalMaterial>(albedo, fuzz);
                    world.Add(make_shared<Sphere>(center, 0.2, Sphere_Material));
                }
                else {
                    // glass
                    Sphere_Material = make_shared<DielectricMaterial>(1.5);
                    world.Add(make_shared<Sphere>(center, 0.2, Sphere_Material));
                }
            }
        }
    }

    auto Material1 = make_shared<DielectricMaterial>(1.5);
    world.Add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, Material1));

    auto Material2 = make_shared<LambertianMaterial>(Color(0.4, 0.2, 0.1));
    world.Add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, Material2));

    auto Material3 = make_shared<MetalMaterial>(Color(0.7, 0.6, 0.5), 0.0);
    world.Add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, Material3));

    Camera camera;

    camera.aspect_ratio         = 16.0 / 9.0;
    camera.image_width          = 1200;
    camera.samples_per_pixel    = 500;
    camera.max_depth            = 50;

    camera.vfov         = 20;
    camera.lookfrom     = Point3(13, 2, 3);
    camera.lookat       = Point3(0, 0, 0);
    camera.vup          = Vec3(0, 1, 0);

    camera.defocus_angle    = 0.6;
    camera.focus_dist       = 10.0;

    camera.Render(world);

}
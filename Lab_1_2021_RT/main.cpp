#include <iostream>
#include <vector>
#include <random>
#include "LightSource.h"
#include "LiteMath.h"
#include "Geometry.h"
#include "Camera.h"

using namespace HydraLiteMath;
struct Scene 
{
    std::vector<std::shared_ptr<GeoObject>> geoObjects;
    std::vector<std::shared_ptr<LightSource>> lightSources;
};

void RenderScene(uint32_t w, uint32_t h, uint32_t num_samples, const std::vector<std::shared_ptr<GeoObject>> &scene, const Camera &cam, const std::string &filename)
{
  auto  background_color = float3(2.05f, 1.33f, 0.63f);
  auto  film = std::make_unique<Film>(w, h, num_samples);
  auto  tracer = std::make_unique<SimpleRT>(16, background_color);
  float invWidth  = 1.0f / float(w);
  float invHeight = 1.0f / float(h);

  for (int y = 0; y < h; ++y)
  {
    for (int x = 0; x < w; ++x)
    {
      float3 pixel_color = float3(0.0f, 0.0f, 0.0f);

      for (int s = 0; s < num_samples; ++s)
      {
        Ray ray = cam.genRay(w, h, x, h - y); //генерируем луч из камеры через текущий пиксель
        pixel_color += tracer->TraceRay(ray, scene, 0); //трассируем луч и получаем цвет
      }
      pixel_color /= film->num_samples;      // усредняем полученные цвета
      pixel_color *= cam.getExposureTime();  // умножаем на время экспонирования сенсора - выдержка виртуальной камеры
      film->SetPixelColor(x, y, pixel_color); //записываем цвет на виртуальный сенсор
    }
  }
  film->SaveImagePPM(filename); //сохраняем картинку
}

void create_scene()
{
  Scene  myScene;
  float3        eye   (0.0f, 2.0f, 20.0f);
  float3        lookat(0.0f, 2.0f, 0.0f);
  float3        up    (0.0f, 1.0f, 0.0f);
  float         field_of_view = 90.0f;
  unsigned int  w = 640;
  unsigned int  h = 480;
  Camera        cam(eye, lookat, up, field_of_view, float(w) / float(h));

  auto lightSource = std::make_shared<DirectlightSource>(float3(+5.0f, -10.0f, -14.0f), float3(1.0f, 1.0f, 1.0f), float(1.0f));
  myScene.lightSources.push_back(lightSource);

 
  auto plane = std::make_shared<Plane>(float3(+0.0f, +0.0f, +0.0f), float3(0.0f, 1.0f, 0.0f), new IdealMirror(float3(222 / float(255), 184 / float(255), 135 / float(255)))); 
  myScene.geoObjects.push_back(plane);

  auto sphere = std::make_shared<Sphere>(float3(-1.5f, +2.5f, +5.0f), float(1.5f), new IdealMirror(float3(255 / float(255), 191 / float(255), 255 / float(255))));
  auto parallel = std::make_shared<Parallel>(float3(-10.0f, +0.0f, -2.0f), float3(-5.0f, +7.0f, +5.0f), new IdealMirror(float3(0.0f, 0.5f, 1.0f)));
  auto square = std::make_shared<Square>(float3(-0.5f, +2.0f, 0.0f), float(5.0f), new IdealMirror(float3(250 / float(255), 0 / float(255), 100 / float(255))));
  auto tr = std::make_shared<Triangle>(float3(+8.0f, +0.0f, +5.0f), float3(8.0f, +7.0f, 0.0f), float3(+7.0f, +0.0f, -8.0f), new IdealMirror(float3(250 / float(255), 69 / float(255), 0 / float(255))));
  myScene.geoObjects.push_back(tr);
  myScene.geoObjects.push_back(parallel);
  myScene.geoObjects.push_back(square);
  myScene.geoObjects.push_back(sphere);

  RenderScene(w, h, 1, myScene.geoObjects, cam,  "MYpicture");
}
/////////////////////////////////////////


int main()
{
  create_scene();

  return 0;
}

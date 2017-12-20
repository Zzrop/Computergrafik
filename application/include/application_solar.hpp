#ifndef APPLICATION_SOLAR_HPP
#define APPLICATION_SOLAR_HPP

#include <vector>

#include "application.hpp"
#include "model.hpp"
#include "structs.hpp"
#include "texture_loader.hpp"

// gpu representation of model
class ApplicationSolar : public Application {
 public:
  // allocate and initialize objects
  ApplicationSolar(std::string const& resource_path);
  // free allocated objects
  ~ApplicationSolar();

  //uploading the normal and model matrix
  void upload_planet_transforms(Planet const& temp_planet) const;
  //uploading moon to planet
  void upload_moon_transforms(Planet const& temp_planet) const;
  // update uniform locations and values
  void uploadUniforms();
  // update projection matrix
  void updateProjection();

  void upload_Quad() const;
  // react to key input
  void keyCallback(int key, int scancode, int action, int mods);
  //handle delta mouse movement input
  void mouseCallback(double pos_x, double pos_y);

  void color_planets(Color const& rgb) const;

  void render_skysphere()const;
  // draw all objects
  void render() const;


 protected:

  void initializeSky();
  void initializeGeometryStars();
  void initializeShaderPrograms();
  void initializeGeometry();
  void initializeTextures();
  void initializeFramebuffer();
  void initializeQuad();
  void updateView();

  // cpu representation of model
  model_object sky_object;
  model_object star_object;
  model_object planet_object;
  model_object quad;
  model quadi;
  std::map<std::string, Planet> planets;
  std::map<std::string, Color> Planet_Colors;
  std::vector<pixel_data>Planet_Textures;
  pixel_data sky_sphere_texture;
  std::vector<float> Stars;
  std::vector<float> Quad_vector= {-1.0f,-1.0f,0.0f,0.0f,0.0f,1.0f,-1.0f,0.0f,1.0f,0.0f,-1.0f,1.0f,0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,1.0f,1.0f};
  int Stars_num;
  std::vector<GLuint>texture_object_container;
  GLuint sky_sphere_tex_obj;
  GLuint framebuffer_tex_obj;
  GLuint rb_handle;
  GLuint fbo_handle;
  bool horizont_bool;
  bool vert_bool;
  bool grey_bool;
  bool blur_bool;


};

#endif

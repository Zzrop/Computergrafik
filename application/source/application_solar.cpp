#include "application_solar.hpp"
#include "launcher.hpp"

#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"

#include <glbinding/gl/gl.h>
// use gl definitions from glbinding
using namespace gl;

//dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

ApplicationSolar::ApplicationSolar(std::string const& resource_path)
 :Application{resource_path}
 ,planet_object{}
 ,planets{}
 ,Planet_Colors{}
 ,Stars{}

{
  Planet Sun(1.0f, 0.0f, 0.0f);
  Planet Merkur(0.383f, 3.012f, 0.5f);
  Planet Venus(0.950f, 1.177f, 0.723f);
  Planet Erde(1.0f, 1.0f, 1.0f);
  Planet Mars(0.583f, 0.53f, 1.524f);
  Planet Jupiter(10.97f, 0.084f, 4.2f);
  Planet Saturn(9.14f, 0.0339f, 6.54f);
  Planet Uranus(3.98f, 0.0119f, 8.19f);
  Planet Neptun(3.87f, 0.006f, 9.1f);
  planets["Sun"] = Sun;
  planets["Merkur"] = Merkur;
  planets["Venus"] = Venus;
  planets["Erde"] = Erde;
  planets["Mars"] = Mars;
  planets["Jupiter"] = Jupiter;
  planets["Saturn"] = Saturn;
  planets["Uranus"] = Uranus;
  planets["Neptun"] = Neptun;

  Color Sun_c(1.0f,1.0f,0.0f);
  Color Merkur_c(0.863f,0.863f,0.863f);
  Color Venus_c(0.957f,0.643f,0.376f);
  Color Erde_c(0.0f,0.0f,1.0f);
  Color Mars_c(1.0f,0.0f,0.0f);
  Color Jupiter_c(0.275f,0.510f,0.706f);
  Color Saturn_c(0.502f,0.502f,0.0f);
  Color Uranus_c(0.118f,0.565f,1.0f);
  Color Neptun_c(0.0f,0.749f,1.0f);
  Color Mond_c(0.863f,0.863f,0.863f);

  Planet_Colors["Sun"] = Sun_c;
  Planet_Colors["Merkur"] = Merkur_c;
  Planet_Colors["Venus"] = Venus_c;
  Planet_Colors["Erde"] = Erde_c;
  Planet_Colors["Mars"] = Mars_c;
  Planet_Colors["Jupiter"] = Jupiter_c;
  Planet_Colors["Saturn"] = Saturn_c;
  Planet_Colors["Uranus"] = Uranus_c;
  Planet_Colors["Neptun"] = Neptun_c;
  Planet_Colors["Mond"] = Mond_c;

  //Default-Planeten-Farbe


  pixel_data Sky_tex = texture_loader::file("../resources/textures/sky_sphere.png");
  pixel_data Sun_tex = texture_loader::file("../resources/textures/sunmap.png");
  pixel_data Merkur_tex = texture_loader::file("../resources/textures/mercurymap.png");
  pixel_data Venus_tex = texture_loader::file("../resources/textures/venusmap.png");
  pixel_data Erde_tex = texture_loader::file("../resources/textures/earthmap1k.png");
  pixel_data Mond_tex = texture_loader::file("../resources/textures/moonmap1k.png");
  pixel_data Mars_tex = texture_loader::file("../resources/textures/marsmap1k.png");
  pixel_data Jupiter_tex = texture_loader::file("../resources/textures/jupitermap.png");
  pixel_data Saturn_tex = texture_loader::file("../resources/textures/saturnmap.png");
  pixel_data Uranus_tex = texture_loader::file("../resources/textures/uranusmap.png");
  pixel_data Neptun_tex = texture_loader::file("../resources/textures/neptunemap.png");


  sky_sphere_texture = Sky_tex;

  Planet_Textures.push_back(Erde_tex);
  Planet_Textures.push_back(Mond_tex);
  Planet_Textures.push_back(Jupiter_tex);
  Planet_Textures.push_back(Mars_tex);
  Planet_Textures.push_back(Merkur_tex);
  Planet_Textures.push_back(Neptun_tex);
  Planet_Textures.push_back(Saturn_tex);
  Planet_Textures.push_back(Sun_tex);
  Planet_Textures.push_back(Uranus_tex);
  Planet_Textures.push_back(Venus_tex);


  //Erstellung eines Sterne-Vektors mit positions und farbangaben
  Stars_num = 2000;
  for (int i = 0; i < Stars_num; ++i)
  {
    float pos_x = (1.0f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(20-1))))-10.0f;
    float pos_y = (1.0f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(20-1))))-10.0f;
    float pos_z = (1.0f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(20-1))))-10.0f;
    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    //float r = 1.0;
    float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    //float g = 0.0;
    float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    //float b = 0.0;

    Stars.push_back(pos_x);
    Stars.push_back(pos_y);
    Stars.push_back(pos_z);
    Stars.push_back(r);
    Stars.push_back(g);
    Stars.push_back(b);

  }


//  std::cout << "Im working!";

  initializeTextures();
  initializeSky();
  initializeGeometry();
  initializeGeometryStars();
  initializeShaderPrograms();
  initializeQuad();




}

void ApplicationSolar::color_planets(Color const& rgb) const {

//  glUniform3f(m_shaders.at("planet").u_locs.at("PlanetColor"), rgb.r, rgb.g, rgb.b);

}

void ApplicationSolar::initializeSky(){
  model sky_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL | model::TEXCOORD);


  // generate vertex array object
  glGenVertexArrays(1, &sky_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(sky_object.vertex_AO);

  // generate generic buffer
  glGenBuffers(1, &sky_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, sky_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sky_model.data.size(), sky_model.data.data(), GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, sky_model.vertex_bytes, sky_model.offsets[model::POSITION]);
  // activate third attribute on gpu
  glEnableVertexAttribArray(1);
  // third attribute is 2 floats with no offset & stride
  glVertexAttribPointer(1, model::TEXCOORD.components, model::TEXCOORD.type, GL_FALSE, sky_model.vertex_bytes, sky_model.offsets[model::TEXCOORD]);



   // generate generic buffer
  glGenBuffers(1, &sky_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sky_object.element_BO);
  // configure currently bound array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * sky_model.indices.size(), sky_model.indices.data(), GL_STATIC_DRAW);

  // store type of primitive to draw
  sky_object.draw_mode = GL_TRIANGLES;
  // transfer number of indices to model object
  sky_object.num_elements = GLsizei(sky_model.indices.size());
}

void ApplicationSolar::upload_Quad()const{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glUseProgram(m_shaders.at("quad").handle);
  glActiveTexture(GL_TEXTURE0);

  int color_sampler_location = glGetUniformLocation(m_shaders.at("quad").handle, "ColorTex");
  glBindTexture(GL_TEXTURE_2D, framebuffer_tex_obj);
  glUniform1i(color_sampler_location,0);

  glBindVertexArray(quad.vertex_AO);
  glDrawArrays(quad.draw_mode, 0, quad.num_elements);


}


void ApplicationSolar::initializeQuad(){
  glGenVertexArrays(1, &quad.vertex_AO);

  glBindVertexArray(quad.vertex_AO);

  glGenBuffers(1, &quad.vertex_BO);

  glBindBuffer(GL_ARRAY_BUFFER, quad.vertex_BO);

  glBufferData(GL_ARRAY_BUFFER, (sizeof(float)*20), Quad_vector.data(), GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,(5*sizeof(float)),0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,(5*sizeof(float)),(void*)3);
  // activate first attribute on gpu

  quad.draw_mode = GL_TRIANGLE_STRIP;

  quad.num_elements = GLsizei(Quad_vector.size()/5);


}

void ApplicationSolar::initializeTextures(){
  int k = 0;
  for (auto i: Planet_Textures)
  {
  glActiveTexture(GL_TEXTURE0);
  GLuint texture_object;
  glGenTextures(1, &texture_object);
  glBindTexture(GL_TEXTURE_2D, texture_object);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, i.channels, i.width, i.height, 0,
              i.channels, i.channel_type, i.ptr());
  texture_object_container.push_back(texture_object);
  k++;
  }
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &sky_sphere_tex_obj);
  glBindTexture(GL_TEXTURE_2D, sky_sphere_tex_obj);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, sky_sphere_texture.channels, sky_sphere_texture.width, sky_sphere_texture.height, 0,
              sky_sphere_texture.channels, sky_sphere_texture.channel_type, sky_sphere_texture.ptr());


  std::cout << "working?" <<"\n";
}

void ApplicationSolar::render_skysphere()const {
  glDepthMask(GL_FALSE);
  glUseProgram(m_shaders.at("sky").handle);

  glActiveTexture(GL_TEXTURE0);

  int color_sampler_location = glGetUniformLocation(m_shaders.at("sky").handle, "ColorTex");
  glBindTexture(GL_TEXTURE_2D, sky_sphere_tex_obj);
  glUniform1i(color_sampler_location,0);

  glBindVertexArray(sky_object.vertex_AO);
  glDrawElements(sky_object.draw_mode, sky_object.num_elements, model::INDEX.type, NULL);
  glDepthMask(GL_TRUE);

}

void ApplicationSolar::initializeFramebuffer(){
  glGenRenderbuffers(1, &rb_handle);
  glBindRenderbuffer(GL_RENDERBUFFER, rb_handle);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 800, 600);

  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &framebuffer_tex_obj);
  glBindTexture(GL_TEXTURE_2D, framebuffer_tex_obj);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

  glGenFramebuffers(1, &fbo_handle);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_handle);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, framebuffer_tex_obj, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb_handle);
  GLenum draw_buffers[1] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1 , draw_buffers);
  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) { std::cout << "Frambuffer läuft nicht " << '\n';}
}


void ApplicationSolar::render() const {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_handle);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  render_skysphere();
  // bind shader to upload uniforms
  glUseProgram(m_shaders.at("planet").handle);

  int k = 0;

  for (auto i: planets)
  {
//    std::cout <<i.first << " " << k <<" \n";
    int color_sampler_location = glGetUniformLocation(m_shaders.at("planet").handle, "ColorTex");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_object_container[k]);
    glUniform1i(color_sampler_location, 0);

    upload_planet_transforms(i.second);
    color_planets(Planet_Colors.at(i.first));
    // bind the VAO to draw
    glBindVertexArray(planet_object.vertex_AO);
    // draw bound vertex array using bound shader
    glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);

    k++;
    if (i.first == "Erde")
    {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture_object_container[k]);
      glUniform1i(color_sampler_location, 0);

      upload_moon_transforms(i.second);
      color_planets(Planet_Colors.at("Mond"));

      glBindVertexArray(planet_object.vertex_AO);

      // draw bound vertex array using bound shader
      glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);
      k++;
    }


  }

  glUseProgram(m_shaders.at("stars").handle);

  glBindVertexArray(star_object.vertex_AO);

  glDrawArrays(star_object.draw_mode, 0, Stars_num);


}

void ApplicationSolar::upload_moon_transforms(Planet const& host)const{
  //std::cout<<"Mooooon!\n";
  glm::fmat4 model_matrix = glm::scale(glm::fmat4{}, glm::fvec3{host.size, host.size, host.size});
  model_matrix = glm::rotate(model_matrix, float(glfwGetTime()*host.rotation_speed), glm::fvec3{0.0f, 1.0f, 0.0f});
  model_matrix = glm::translate(model_matrix, glm::fvec3{0.0f, 0.0f, host.distance});

  Planet Mond(2.0f, 9.815f, 1.0f); //Vorsicht! Anpassungen im planeten-struct vervielfachen sich!
  model_matrix = glm::scale(model_matrix, glm::fvec3{Mond.size, Mond.size, Mond.size});
  model_matrix = glm::rotate(model_matrix, float(glfwGetTime()*Mond.rotation_speed), glm::fvec3{0.0f, 1.0f, 0.0f});
  model_matrix = glm::translate(model_matrix, glm::fvec3{0.0f, 0.0f, Mond.distance});
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                     1, GL_FALSE, glm::value_ptr(model_matrix));

  // extra matrix for normal transformation to keep them orthogonal to surface
  glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                     1, GL_FALSE, glm::value_ptr(normal_matrix));
}

void ApplicationSolar::upload_planet_transforms(Planet const& temp_planet)const {


  glm::fmat4 model_matrix = glm::scale(glm::fmat4{}, glm::fvec3{temp_planet.size, temp_planet.size, temp_planet.size});
  model_matrix = glm::rotate(model_matrix, float(glfwGetTime()*temp_planet.rotation_speed), glm::fvec3{0.0f, 1.0f, 0.0f});
  model_matrix = glm::translate(model_matrix, glm::fvec3{0.0f, 0.0f, temp_planet.distance});
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                     1, GL_FALSE, glm::value_ptr(model_matrix));

  // extra matrix for normal transformation to keep them orthogonal to surface
  glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                     1, GL_FALSE, glm::value_ptr(normal_matrix));
//  glUniform3f(m_shaders.at("planet").u_locs.at("PlanetColor"), 1.0, 0.0, 0.0);
}

void ApplicationSolar::updateView() {
  // vertices are transformed in camera space, so camera transform must be inverted
  glUseProgram(m_shaders.at("planet").handle);
  glm::fmat4 view_matrix = glm::inverse(m_view_transform);
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));

  glUseProgram(m_shaders.at("stars").handle);

  glUniformMatrix4fv(m_shaders.at("stars").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));

  glUseProgram(m_shaders.at("sky").handle);

  glUniformMatrix4fv(m_shaders.at("sky").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));
}

void ApplicationSolar::updateProjection() {
  // upload matrix to gpu
  glUseProgram(m_shaders.at("planet").handle);
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));

  glUseProgram(m_shaders.at("stars").handle);

  glUniformMatrix4fv(m_shaders.at("stars").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));
  glUseProgram(m_shaders.at("sky").handle);
  glUniformMatrix4fv(m_shaders.at("sky").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));
}

// update uniform locations
void ApplicationSolar::uploadUniforms() {
  updateUniformLocations();

  // bind new shader
  glUseProgram(m_shaders.at("planet").handle);
  updateView();
  updateProjection();
}

// handle key input
void ApplicationSolar::keyCallback(int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_W &&( action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, -0.1f});
    updateView();
  }
  else if (key == GLFW_KEY_S &&( action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, 0.1f});
    updateView();
  }
  else if (key == GLFW_KEY_O &&( action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, -0.1f, 0.0f});
    updateView();
  }
  else if (key == GLFW_KEY_L &&( action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.1f, 0.0f});
    updateView();
  }
  else if (key == GLFW_KEY_A &&( action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{-0.1f, 0.0f, 0.0f});
    updateView();
  }
  else if (key == GLFW_KEY_D &&( action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.1f, 0.0f, 0.0f});
    updateView();
  }
}

//handle delta mouse movement input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {
  // mouse handling
  m_view_transform = glm::rotate(m_view_transform, float(pos_x)/100, glm::fvec3{0.0f , -1.0f, 0.0f});
  updateView();
  m_view_transform = glm::rotate(m_view_transform, float(pos_y)/100, glm::fvec3{1.0f , 0.0f, 0.0f});
  updateView();
}

// load shader programs
void ApplicationSolar::initializeShaderPrograms() {
  m_shaders.emplace("stars", shader_program{m_resource_path + "shaders/stars.vert",
                                          m_resource_path + "shaders/stars.frag"});
  m_shaders.at("stars").u_locs["ViewMatrix"] = -1;
  m_shaders.at("stars").u_locs["ProjectionMatrix"] = -1;
  // store shader program objects in container
  m_shaders.emplace("planet", shader_program{m_resource_path + "shaders/simple.vert",
                                           m_resource_path + "shaders/simple.frag"});
  // request uniform locations for shader program
  m_shaders.at("planet").u_locs["NormalMatrix"] = -1;
  m_shaders.at("planet").u_locs["ModelMatrix"] = -1;
  m_shaders.at("planet").u_locs["ViewMatrix"] = -1;
  m_shaders.at("planet").u_locs["ProjectionMatrix"] = -1;
//  m_shaders.at("planet").u_locs["PlanetColor"] = -1;
//  m_shaders.at("planet").u_locs["colorTex"] = -1;
//  m_shaders.at("planet").u_locs["rendermode"] = -1;

  m_shaders.emplace("sky", shader_program{m_resource_path + "shaders/sky.vert",
                                           m_resource_path + "shaders/sky.frag"});
  m_shaders.at("sky").u_locs["ViewMatrix"] = -1;
  m_shaders.at("sky").u_locs["ProjectionMatrix"] = -1;


  m_shaders.emplace("quad", shader_program{m_resource_path + "shaders/quad.vert",
                                           m_resource_path + "shaders/quad.frag"});

  m_shaders.at("quad").u_locs["ColorTex"] = -1;


}

// load models
void ApplicationSolar::initializeGeometry() {
  model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL | model::TEXCOORD);


  // generate vertex array object
  glGenVertexArrays(1, &planet_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(planet_object.vertex_AO);

  // generate generic buffer
  glGenBuffers(1, &planet_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, planet_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * planet_model.data.size(), planet_model.data.data(), GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::POSITION]);
  // activate second attribute on gpu
  glEnableVertexAttribArray(1);
  // second attribute is 3 floats with no offset & stride
  glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::NORMAL]);
  // activate third attribute on gpu
  glEnableVertexAttribArray(2);
  // third attribute is 2 floats with no offset & stride
  glVertexAttribPointer(2, model::TEXCOORD.components, model::TEXCOORD.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::TEXCOORD]);



   // generate generic buffer
  glGenBuffers(1, &planet_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planet_object.element_BO);
  // configure currently bound array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * planet_model.indices.size(), planet_model.indices.data(), GL_STATIC_DRAW);

  // store type of primitive to draw
  planet_object.draw_mode = GL_TRIANGLES;
  // transfer number of indices to model object
  planet_object.num_elements = GLsizei(planet_model.indices.size());
}

  void ApplicationSolar::initializeGeometryStars(){

    glGenVertexArrays(1, &star_object.vertex_AO);

    glBindVertexArray(star_object.vertex_AO);

    glGenBuffers(1, &star_object.vertex_BO);

    glBindBuffer(GL_ARRAY_BUFFER, star_object.vertex_BO);

    glBufferData(GL_ARRAY_BUFFER, (sizeof(float)*(Stars.size())), Stars.data(), GL_STATIC_DRAW);

    // activate first attribute on gpu
    glEnableVertexAttribArray(0);
    // first attribute is 3 floats with no offset & stride
    glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE,(6*sizeof(float)),0);
    // activate first attribute on gpu
    glEnableVertexAttribArray(1);
    // first attribute is 3 floats with no offset & stride
    //glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE,(3*sizeof(float)),(3*sizeof(float)));
    glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE,(6*sizeof(float)),(GLvoid*)uintptr_t(sizeof(float)*3));

    star_object.draw_mode = GL_POINTS;




  }


ApplicationSolar::~ApplicationSolar() {
  glDeleteBuffers(1, &planet_object.vertex_BO);
  glDeleteBuffers(1, &star_object.vertex_BO);
  glDeleteBuffers(1, &planet_object.element_BO);
  glDeleteVertexArrays(1, &planet_object.vertex_AO);
  glDeleteVertexArrays(1, &star_object.vertex_AO);
}

// exe entry point
int main(int argc, char* argv[]) {
  Launcher::run<ApplicationSolar>(argc, argv);
}

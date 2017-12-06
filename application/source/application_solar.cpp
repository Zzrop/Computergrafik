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
  Planet Merkur(0.383f, 3.012f, 0.387f);
  Planet Venus(0.950f, 1.177f, 0.723f);
  Planet Erde(1.0f, 1.0f, 1.0f);
  Planet Mars(0.583f, 0.53f, 1.524f);
  Planet Jupiter(10.97f, 0.084f, 5.2f);
  Planet Saturn(9.14f, 0.0339f, 9.54f);
  Planet Uranus(3.98f, 0.0119f, 19.19f);
  Planet Neptun(3.87f, 0.006f, 30.1f);
  planets["Sun"] = Sun;
  planets["Merkur"] = Merkur;
  planets["Venus"] = Venus;
  planets["Erde"] = Erde;
  planets["Mars"] = Mars;
  planets["Jupiter"] = Jupiter;
  planets["Saturn"] = Saturn;
  planets["Uranus"] = Uranus;
  planets["Neptun"] = Neptun;

  //Default-Planeten-Farbe
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


  pixel_data Sun_tex = texture_loader::file("../resources/textures/sunmap.png");
/*  pixel_data Merkur_tex = texture_loader::file("mercurymap.png");
  pixel_data Venus_tex = texture_loader::file("venusmap.png");
  pixel_data Erde_tex = texture_loader::file("earthmap1k.png");
  pixel_data Mars_tex = texture_loader::file("marsmap1k.png");
  pixel_data Jupiter_tex = texture_loader::file("jupitermap");
  pixel_data Saturn_tex = texture_loader::file("saturnmap.png");
  pixel_data Uranus_tex = texture_loader::file("uranusmap.png");
  pixel_data Neptun_tex = texture_loader::file("neptunemap.png");
  pixel_data Mond_tex = texture_loader::file("moonmap1k.png");*/

  Planet_Textures["Sun"] = Sun_tex;
/*  Planet_Textures["Merkur"] = Merkur_tex;
  Planet_Textures["Venus"] = Venus_tex;
  Planet_Textures["Erde"] = Erde_tex;
  Planet_Textures["Mars"] = Mars_tex;
  Planet_Textures["Jupiter"] = Jupiter_tex;
  Planet_Textures["Uranus"] = Uranus_tex;
  Planet_Textures["Neptun"] = Neptun_tex;
  Planet_Textures["Mond"] = Mond_tex;*/

  glActiveTexture(GL_TEXTURE0);
  GLuint texture_object;
  glGenTextures(1, &texture_object);
  glBindTexture(GL_TEXTURE_2D, texture_object);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, Sun_tex.channels, Sun_tex.width, Sun_tex.height, 0,
              Sun_tex.channels, Sun_tex.channel_type, Sun_tex
              .ptr());

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

//    float* stars = Stars.data();


/*    for (int i = 0; i<6000;++i)
    {
        std::cout << stars[i] << std::endl;
    }*/
/*
  for (std::vector<float>::const_iterator i = Stars.begin(); i != Stars.end(); ++i)
      std::cout << *i << ' ';*/
  std::cout << "Im working!";

  initializeGeometry();
  initializeGeometryStars();
  //initializeTextures();
  initializeShaderPrograms();



}

/*void ApplicationSolar::load_planets() const {



}*/

void ApplicationSolar::color_planets(Color const& rgb) const {

  glUniform3f(m_shaders.at("planet").u_locs.at("PlanetColor"), rgb.r, rgb.g, rgb.b);

}
/*
void ApplicationSolar::initializeTextures()const{
  glActiveTexture(GL_TEXTURE0);
  GLint texture_object;
  glGenTextures(1, &texture_object);
  glBindTexture(GL_TEXTURE_2D, texture_object);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0,"jpg", , 500, 0,
              input_channels, "jpg", data_ptr);
}*/
/*
void ApplicationSolar::uploadtextures(int unit){

  GLint texture_object;


}
*/
void ApplicationSolar::render() const {


  // bind shader to upload uniforms
  glUseProgram(m_shaders.at("planet").handle);


  for (auto i: planets)
  {
//    std::cout <<Planet_Colors[i.first].r;
    int color_sampler_location = glGetUniformLocation(m_shaders.at("planet").handle, "ColorTex");

    glUniform1i(color_sampler_location, 0);
    upload_planet_transforms(i.second);
    color_planets(Planet_Colors.at(i.first));
    // bind the VAO to draw
    glBindVertexArray(planet_object.vertex_AO);

    // draw bound vertex array using bound shader
    glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);
    if (i.first == "Erde")
    {
      upload_moon_transforms(i.second);
      color_planets(Planet_Colors.at("Mond"));

      glBindVertexArray(planet_object.vertex_AO);

      // draw bound vertex array using bound shader
      glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);
    }

  }

  glUseProgram(m_shaders.at("stars").handle);

  glBindVertexArray(star_object.vertex_AO);

  glDrawArrays(star_object.draw_mode, 0, Stars_num);


/*
  glm::fmat4 model_matrix = glm::rotate(glm::fmat4{}, float(glfwGetTime()), glm::fvec3{0.0f, 1.0f, 0.0f});
  model_matrix = glm::scale(model_matrix, glm::fvec3{0.5f, 0.5f, 0.5f});
  model_matrix = glm::translate(model_matrix, glm::fvec3{0.0f, 0.0f, -1.0f});
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                     1, GL_FALSE, glm::value_ptr(model_matrix));

  // extra matrix for normal transformation to keep them orthogonal to surface
  glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                     1, GL_FALSE, glm::value_ptr(normal_matrix));
*/
/*
  // bind the VAO to draw
  glBindVertexArray(planet_object.vertex_AO);

  // draw bound vertex array using bound shader
  glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);
*/
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

}

void ApplicationSolar::updateProjection() {
  // upload matrix to gpu
  glUseProgram(m_shaders.at("planet").handle);
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));

  glUseProgram(m_shaders.at("stars").handle);

  glUniformMatrix4fv(m_shaders.at("stars").u_locs.at("ProjectionMatrix"),
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
  if (key == GLFW_KEY_W && action == GLFW_PRESS) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, -0.1f});
    updateView();
  }
  else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, 0.1f});
    updateView();
  }
}

//handle delta mouse movement input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {
  // mouse handling
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
  m_shaders.at("planet").u_locs["PlanetColor"] = -1;
  m_shaders.at("planet").u_locs["colorTex"] = -1;
//  m_shaders.at("planet").u_locs["TexCoord"] = -1;




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

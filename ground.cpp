#include <GL/glew.h>
#include <stdlib.h>

#include <vector>

#include "ground.h"
#include "sim.h"

using namespace std;

Ground::Ground(const string& texture):
  shader("grnd.vert", "grnd.frag"), tex(texture)
{
  int rows = tex.get_width(), cols = tex.get_height();

  vec3* ground_points = new vec3[rows*cols];
  std::vector<unsigned> indices;

  for (int j = 0; j < rows; j++) {
    for (int i = 0; i < cols; i++) {
      ground_points[i+j*cols] = vec3(
        (-(rows/2)+i)*32.0,
        tex.intensity_at(i+j*cols) * 100.0,
        (-(cols/2)+j)*32.0);
    }
  }

  unsigned n = 0;
  int cs = cols * 2;
  int rs = rows - 1;
  for (int j = 0; j < rs; j++) {
    for (int i = 0; i < cs; i++) {
      int t = i + j * cs;
      if (i == cs - 1) {
        indices.push_back(n);
      } else {
        indices.push_back(n);
        if (t % 2 == 0) {
          n += cols;
        } else if (j % 2 == 0) {
          n -= cols - 1;
        } else {
          n -= cols + 1;
        }
      }
    }
  }

  n_indicies = indices.size();

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER,
    sizeof(vec3) * rows * cols, ground_points, GL_STATIC_DRAW);

  delete ground_points;

  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
     n_indicies * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glUseProgram(shader.get_id());
  shader.bind_attribute("coord", 0);
  glEnableVertexAttribArray(shader.get_attribute(0));
  glVertexAttribPointer(
    shader.get_attribute(0), 3, GL_FLOAT, false, 0, (void*)(0));

  shader.bind_uniform("mv", 0);
  shader.bind_uniform("proj", 1);
}

Ground::~Ground() {
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ibo);
  glDeleteVertexArrays(1, &vao);
}

void Ground::draw() {
  glBindVertexArray(vao);
  glUseProgram(shader.get_id());

  glUniformMatrix4fv(shader.get_uniform(0), 1, false, 
    Sim::get_view_matrix().as_array());
  glUniformMatrix4fv(shader.get_uniform(1), 1, false, 
    Sim::get_projection_matrix().as_array());

  glBindVertexArray(vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glDrawElements(GL_TRIANGLE_STRIP, n_indicies, GL_UNSIGNED_INT, (void*)(0));
}
module;

#include "util/gl.h"

module shaders:program;

import debug;

using namespace GL;

ProgramObject::ProgramObject(const std::initializer_list<Shader> &shaders)
    : ID{glCreateProgram()} {
  for (auto &shader : shaders)
    glAttachShader(ID, shader.ID);
  glLinkProgram(ID);
  for (auto &shader : shaders)
    glDetachShader(ID, shader.ID);
}
ProgramObject ::~ProgramObject() {
  if (ID)
    glDeleteProgram(ID);
}
ProgramObject::ProgramObject(ProgramObject &&o) : ID{o.ID} { o.ID = 0; };
ProgramObject &ProgramObject::operator=(ProgramObject &&o) {
  ID = o.ID;
  o.ID = 0;
  return *this;
}
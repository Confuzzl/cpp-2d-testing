module;

#include "util/gl.h"

module query_object;

using namespace GL;

QueryObject::QueryObject() { glGenQueries(1, &ID); }
QueryObject::~QueryObject() {
  if (ID)
    glDeleteQueries(1, &ID);
}
QueryObject::QueryObject(QueryObject &&o) : ID{o.ID} { o.ID = 0; }
QueryObject &QueryObject::operator=(QueryObject &&o) {
  ID = o.ID;
  o.ID = 0;
  return *this;
}
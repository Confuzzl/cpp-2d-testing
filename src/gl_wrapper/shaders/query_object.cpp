module;

#include "util/gl.h"

module query_object;

using namespace GL;

QueryObject::QueryObject() { glGenQueries(1, &ID); }
QueryObject::~QueryObject() {
  if (ID != -1)
    glDeleteQueries(1, &ID);
}
QueryObject::QueryObject(QueryObject &&o) : ID{o.ID} { o.ID = -1; }
QueryObject &QueryObject::operator=(QueryObject &&o) {
  ID = o.ID;
  o.ID = -1;
  return *this;
}
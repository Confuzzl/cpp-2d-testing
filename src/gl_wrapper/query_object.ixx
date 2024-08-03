module;

#include "util/gl.h"

export module query_object;

export namespace GL {
struct QueryObject {
  GLuint ID;

  QueryObject();
  ~QueryObject();
  QueryObject(const QueryObject &) = delete;
  QueryObject(QueryObject &&);
  QueryObject &operator=(const QueryObject &) = delete;
  QueryObject &operator=(QueryObject &&);
};
} // namespace GL
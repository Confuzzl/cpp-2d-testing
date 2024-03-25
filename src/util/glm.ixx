export module glm;

export import <glm/glm.hpp>;
export import <glm/gtc/matrix_transform.hpp>;
export import <glm/gtc/type_ptr.hpp>;
export import <glm/gtx/string_cast.hpp>;
export import <glm/gtx/matrix_interpolation.hpp>;
export import <glm/gtx/norm.hpp>;

export glm::vec2 cwPerp(const glm::vec2 &v) { return {v.y, -v.x}; }
export glm::vec2 ccwPerp(const glm::vec2 &v) { return {-v.y, v.x}; }

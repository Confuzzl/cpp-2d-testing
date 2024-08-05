/// @ref gtx_log_base

namespace glm
{
	template<typename genType>
	GLM_FUNC_QUALIFIER genType log(genType const& x, genType const& Base)
	{
		return glm::log(x) / glm::log(Base);
	}

	template<length_t L, typename T, qualifier Q>
	GLM_FUNC_QUALIFIER vec<L, T, Q> log(vec<L, T, Q> const& x, vec<L, T, Q> const& Base)
	{
		return glm::log(x) / glm::log(Base);
	}
}//namespace glm

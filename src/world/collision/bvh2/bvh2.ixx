export module bvh2;

import <list>;

export struct BoundingBox;

export namespace collision {
struct BoundingVolumeHierarchy {
	enum CONSTRUCTION_SCHEME {TOP_DOWN, BOTTOM_UP, INCREMENTAL};
	static constexpr CONSTRUCTION_SCHEME SCHEME = INCREMENTAL;

	using Object = BoundingBox *;
	using ObjList = std::list<Object>;
	using ObjListIter = ObjList::iterator;

	ObjList objects;

	BoundingVolumeHierarchy() = default;
};
}
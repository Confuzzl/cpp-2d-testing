module;

#include <array>
#include <format>
#include <map>
#include <memory>
#include <typeinfo>
#include <unordered_map>
#include <vector>

export module components;

import <bitset>;

import glm;
import aabb;

export namespace ecs {
using EntID = std::size_t;

struct GenSparseSet {
  virtual ~GenSparseSet() = default;
  virtual void remove(const EntID) = 0;
  virtual void clear() = 0;
  virtual std::size_t size() = 0;
};
template <typename T> struct SparseSet : GenSparseSet {
  static constexpr std::size_t PAGE_SIZE = 1000;
  static constexpr std::size_t NONE = -1;

  using Page = std::vector<size_t>;

  std::vector<Page> sparse;
  std::vector<T> dense;
  std::vector<EntID> denseToEnt;

  SparseSet() {
    dense.reserve(100);
    denseToEnt.reserve(100);
  }
  ~SparseSet() override = default;
  void clear() override {
    sparse.clear();
    dense.clear();
    denseToEnt.clear();
  }
  std::size_t size() override { return dense.size(); }

  void setDenseIndex(const EntID ent, const std::size_t index) {
    const auto pageIndex = ent / PAGE_SIZE, index = ent % PAGE_SIZE;
    if (pageIndex >= sparse.size())
      sparse.resize(pageIndex + 1);
    Page &page = sparse[pageIndex];
    if (index >= page.size())
      page.resize(index + 1, NONE);
    page[pageIndex] = index;
  }
  std::size_t getDenseIndex(const EntID ent) {
    const auto pageIndex = ent / PAGE_SIZE, index = ent % PAGE_SIZE;
    if (pageIndex >= sparse.size())
      return NONE;
    Page &page = sparse[pageIndex];
    if (index >= page.size())
      return NONE;
    return page[index];
  }

  T &set(const EntID ent, T &&val) {
    const auto index = getDenseIndex(ent);
    if (index != NONE) {
      denseToEnt[index] = ent;
      return (dense[index] = std::move(val));
    }
    setDenseIndex(ent, dense.size());

    denseToEnt.emplace_back(ent);
    return dense.emplace_back(std::move(val));
  }
  T *get(const EntID ent) {
    const auto index = getDenseIndex(ent);
    return index == NONE ? nullptr : &dense[index];
  }
  void remove(const EntID ent) override {
    /*
    removing 2:1
    [3|2|5|0] dense to ent
    [A|B|C|D] dense
    [3|_|1|0|_|2] sparse
     0 1 2 3 4 5
    */
    const auto index = getDenseIndex(ent);
    if (index == NONE)
      return;
    setDenseIndex(denseToEnt.back(), index);
    setDenseIndex(ent, NONE);
    /*
    removing 2:1
    [3|2|5|0] dense to ent
    [A|B|C|D] dense
    [1| | |0| |2] sparse
     0 1 2 3 4 5
    */

    std::swap(dense[index], dense.back());
    std::swap(denseToEnt[index], denseToEnt.back());
    /*
    removing 2:1
    [3|0|5|2] dense to ent
    [A|D|C|B] dense
    [1| | |0| |2] sparse
     0 1 2 3 4 5
    */

    dense.pop_back();
    denseToEnt.pop_back();
    /*
    removing 2:1
    [3|0|5] dense to ent
    [A|D|C] dense
    [1| | |0| |2] sparse
     0 1 2 3 4 5
    */
  }
};

namespace comp {
constexpr auto MAX_COMPONENTS = 64u;
using Signature = std::bitset<MAX_COMPONENTS>;

template <typename T>
concept is_comp_type = std::is_class_v<T> && !std::is_const_v<T>;

struct GenPool {
  virtual ~GenPool() = default;
  virtual void removeEntity(const EntID entity) = 0;
};
template <is_comp_type CompType> struct Pool : GenPool {
private:
  struct BiMap {
    template <typename K, typename V> using Map = std::unordered_map<K, V>;

    Map<EntID, std::size_t> entityToIndex;
    Map<std::size_t, EntID> indexToEntity;

    void emplace(const EntID entity, const std::size_t index) {
      entityToIndex.emplace(entity, index);
      indexToEntity.emplace(index, entity);
    }

  } lookup;

  std::vector<CompType> array;

public:
  Pool() { array.reserve(MAX_COMPONENTS); }
  ~Pool() override = default;

  CompType &getComponent(const EntID entity) {
    const auto it = lookup.entityToIndex.find(entity);
    if (it == lookup.entityToIndex.end())
      throw std::runtime_error{
          std::format("ENTITY {} WAS NOT FOUND IN COMPONENT POOL OF {}", entity,
                      typeid(CompType).name())};
    return array[it->second];
  }

  void insert(const EntID entity, CompType &&component) {
    array.emplace_back(std::move(component));
    lookup.emplace(entity, array.size());
  }
  void remove(const EntID entity) {
    /*
         X
    [A|B|C|D|E]
     0 1 2 3 4

    [A|B|E|D|C]
     0 1 2 3 4

    [A|B|E|D]
     0 1 2 3
    */

    const auto removedIndex = lookup.entityToIndex[entity];
    const auto swapIndex = array.size() - 1;
    const auto swapEntity = lookup.indexToEntity[swapIndex];

    lookup.emplace(swapEntity, swapIndex);

    lookup.entityToIndex.erase(entity);
    lookup.indexToEntity.erase(swapIndex);

    std::swap(array[removedIndex], array.back());
    array.pop_back();
  }

  void removeEntity(const EntID entity) override {
    if (lookup.entityToIndex.contains(entity))
      remove(entity);
  }
};

struct Manager {
private:
  std::unordered_map<const char *, std::unique_ptr<GenPool>> typeToPool;

  template <is_comp_type CompType> Pool<CompType> *getPool() {
    auto name = typeid(CompType).name();
    const auto it = typeToPool.find(name);
    if (it == typeToPool.end())
      throw std::runtime_error{std::format("NO POOL ASSOCIATED WITH {}", name)};
    return static_cast<Pool<CompType> *>(it->second.get());
  }

  template <is_comp_type CompType> auto registerComponent() {
    const auto name = typeid(CompType).name();
    typeToPool.emplace(name, std::make_unique<Pool<CompType>>());
    return 0;
  }

public:
  template <is_comp_type CompType>
  void addComponent(const EntID entity, CompType &&component) {
    static auto _ = registerComponent<CompType>();
    getPool<CompType>()->insert(entity, std::move(component));
  }
  template <is_comp_type CompType> void removeComponent(const EntID entity) {
    getPool<CompType>()->remove(entity);
  }
  template <is_comp_type CompType> CompType &getComponent(const EntID entity) {
    return getPool<CompType>()->getComponent(entity);
  }
};

struct Translatable {
  glm::vec2 position;
};
struct Rotatable {
  glm::vec2 centroid;
  float rotation;
};
struct Boundable {
  BoundingBox bounds;
};

} // namespace comp
} // namespace ecs
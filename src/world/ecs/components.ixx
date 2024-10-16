module;

#include <array>
#include <format>
#include <map>
#include <memory>
#include <typeinfo>
#include <unordered_map>

export module components;

// import <array>;
import <vector>;
import <bitset>;
import <stack>;

import glm;
import aabb;

static auto counter = 0u;

export namespace ecs {
using EntID = std::size_t;

struct GenSparseSet {
  virtual ~GenSparseSet() = default;
  virtual void remove(const EntID) = 0;
  virtual void clear() = 0;
  virtual std::size_t size() const = 0;
};
template <typename T> struct SparseSet : GenSparseSet {
  static constexpr std::size_t PAGE_SIZE = 1000;
  static constexpr std::size_t NONE = -1;

private:
  using Page = std::vector<size_t>;

  std::vector<Page> sparse;
  std::vector<T> dense;
  std::vector<EntID> denseToEnt;

  void setDenseIndex(const EntID ent, const std::size_t val) {
    const auto pageIndex = ent / PAGE_SIZE, index = ent % PAGE_SIZE;
    if (pageIndex >= sparse.size())
      sparse.resize(pageIndex + 1);
    Page &page = sparse[pageIndex];
    if (index >= page.size())
      page.resize(index + 1, NONE);
    page[pageIndex] = val;
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

public:
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
  std::size_t size() const override { return dense.size(); }
  bool empty() const { return size() == 0; }
  const auto &data() const { return dense; }

  template <typename U> T &set(const EntID ent, U &&val) {
    const auto index = getDenseIndex(ent);
    if (index != NONE) {
      denseToEnt[index] = ent;
      return (dense[index] = std::forward<U>(val));
    }
    setDenseIndex(ent, dense.size());

    denseToEnt.emplace_back(ent);
    return dense.emplace_back(std::forward<U>(val));
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

struct Manager {
private:
  static constexpr EntID NULL_ENT = -1;
  static constexpr std::size_t MAX_COMPONENTS = 64u;
  using Signature = std::bitset<MAX_COMPONENTS>;

  template <typename T> auto getComponentBitIndex() {
    static auto id = counter++;
    return id;
  }
  template <typename T> bool getComponentBit(const Signature sig) {
    return sig.test(getComponentBitIndex<T>());
  }
  template <typename T> void setComponentBit(Signature &sig, const bool val) {
    sig.set(getComponentBitIndex<T>(), val);
  }

  template <typename T> T &getComponent(const EntID ent) {
    auto comp = getComponentPool<T>().get(ent);
    if (comp)
      return *comp;
    throw std::runtime_error{
        std::format("{} NOT A COMPONENT OF {}", typeid(T).name(), ent)};
  }

  SparseSet<Signature> entitySignatures;
  std::vector<std::unique_ptr<GenSparseSet>> componentPools;
  std::unordered_map<Signature, SparseSet<EntID>> groups;
  std::stack<EntID> entityStack;

  template <typename T> SparseSet<T> &registerPool() {
    return componentPools.emplace_back(std::make_unique<SparseSet<T>>());
  }
  template <typename T> SparseSet<T> &getComponentPool() {
    static SparseSet<T> &pool = registerPool<T>();
    return pool;
  }

  Signature *getSignature(const EntID ent) { return entitySignatures.get(ent); }
  template <typename... Ts> Signature getSignature() {
    Signature out;
    (setComponentBit<Ts>(out, true), ...);
    return out;
  }

  SparseSet<EntID> &getGroup(const Signature sig) {
    auto it = groups.find(sig);
    if (it != groups.end())
      return it->second;
    throw std::runtime_error{
        std::format("NO GROUP MATCHING SIGNATURE {}", sig.to_ullong())};
  }

  void removeFromGroup(const Signature signature, const EntID ent) {
    if (signature.none() || ent == NULL_ENT)
      return;

    auto it = groups.find(signature);
    if (it == groups.end())
      return;
    auto &group = it->second;
    group.remove(ent);
    if (group.empty())
      groups.erase(it);
  }
  void addToGroup(const Signature signature, const EntID ent) {
    if (signature.none() || ent == NULL_ENT)
      return;
    groups.emplace(signature, SparseSet<EntID>{}).first->second.set(ent, ent);
  }

public:
  Manager() { componentPools.reserve(MAX_COMPONENTS); }

  EntID newEntity() {
    static EntID counter = 0;
    EntID out = NULL_ENT;
    if (entityStack.empty()) {
      out = counter++;
    } else {
      out = entityStack.top();
      entityStack.pop();
    }
    entitySignatures.set(out, Signature{});
    return out;
  }

  void deleteEntity(const EntID ent) {
    if (ent == NULL_ENT)
      return;
    const auto sig = getSignature(ent);
    if (!sig)
      return;
    const auto signature = *sig;
    removeFromGroup(signature, ent);
    for (auto i = 0u; i < MAX_COMPONENTS; i++) {
      if (!signature[i])
        continue;
      componentPools[i]->remove(ent);
    }
    entitySignatures.remove(ent);
    entityStack.emplace(ent);
  }

  template <typename... Ts> auto getViewIDs() {
    std::vector<EntID> out;
    out.reserve(10);
    const auto sig = getSignature<Ts>();
    for (const auto &[signature, group] : groups) {
      if ((signature & sig) != sig)
        continue;
      out.insert(out.end(), group.data().begin(), group.data().end());
    }
    return out;
  }
  template <typename... Ts> auto getViewComponents() {
    std::vector<std::tuple<EntID, Ts...>> out;
    out.reserve(10);
    const auto sig = getSignature<Ts>();
    for (const auto &[signature, group] : groups) {
      if ((signature & sig) != sig)
        continue;
      for (const EntID ent : group)
        out.emplace_back(ent, getComponent<Ts>(ent)...);
    }
    return out;
  }
};

namespace comp {

template <typename T>
concept is_comp_type = std::is_class_v<T> && !std::is_const_v<T>;

// struct GenPool {
//   virtual ~GenPool() = default;
//   virtual void removeEntity(const EntID entity) = 0;
// };
// template <is_comp_type CompType> struct Pool : GenPool {
// private:
//   struct BiMap {
//     template <typename K, typename V> using Map = std::unordered_map<K, V>;
//
//     Map<EntID, std::size_t> entityToIndex;
//     Map<std::size_t, EntID> indexToEntity;
//
//     void emplace(const EntID entity, const std::size_t index) {
//       entityToIndex.emplace(entity, index);
//       indexToEntity.emplace(index, entity);
//     }
//
//   } lookup;
//
//   std::vector<CompType> array;
//
// public:
//   Pool() { array.reserve(MAX_COMPONENTS); }
//   ~Pool() override = default;
//
//   CompType &getComponent(const EntID entity) {
//     const auto it = lookup.entityToIndex.find(entity);
//     if (it == lookup.entityToIndex.end())
//       throw std::runtime_error{
//           std::format("ENTITY {} WAS NOT FOUND IN COMPONENT POOL OF {}",
//           entity,
//                       typeid(CompType).name())};
//     return array[it->second];
//   }
//
//   void insert(const EntID entity, CompType &&component) {
//     array.emplace_back(std::move(component));
//     lookup.emplace(entity, array.size());
//   }
//   void remove(const EntID entity) {
//     /*
//          X
//     [A|B|C|D|E]
//      0 1 2 3 4
//
//     [A|B|E|D|C]
//      0 1 2 3 4
//
//     [A|B|E|D]
//      0 1 2 3
//     */
//
//     const auto removedIndex = lookup.entityToIndex[entity];
//     const auto swapIndex = array.size() - 1;
//     const auto swapEntity = lookup.indexToEntity[swapIndex];
//
//     lookup.emplace(swapEntity, swapIndex);
//
//     lookup.entityToIndex.erase(entity);
//     lookup.indexToEntity.erase(swapIndex);
//
//     std::swap(array[removedIndex], array.back());
//     array.pop_back();
//   }
//
//   void removeEntity(const EntID entity) override {
//     if (lookup.entityToIndex.contains(entity))
//       remove(entity);
//   }
// };
//
// struct Manager {
// private:
//   std::unordered_map<const char *, std::unique_ptr<GenPool>> typeToPool;
//
//   template <is_comp_type CompType> Pool<CompType> *getPool() {
//     auto name = typeid(CompType).name();
//     const auto it = typeToPool.find(name);
//     if (it == typeToPool.end())
//       throw std::runtime_error{std::format("NO POOL ASSOCIATED WITH {}",
//       name)};
//     return static_cast<Pool<CompType> *>(it->second.get());
//   }
//
//   template <is_comp_type CompType> auto registerComponent() {
//     const auto name = typeid(CompType).name();
//     typeToPool.emplace(name, std::make_unique<Pool<CompType>>());
//     return 0;
//   }
//
// public:
//   template <is_comp_type CompType>
//   void addComponent(const EntID entity, CompType &&component) {
//     static auto _ = registerComponent<CompType>();
//     getPool<CompType>()->insert(entity, std::move(component));
//   }
//   template <is_comp_type CompType> void removeComponent(const EntID entity) {
//     getPool<CompType>()->remove(entity);
//   }
//   template <is_comp_type CompType> CompType &getComponent(const EntID entity)
//   {
//     return getPool<CompType>()->getComponent(entity);
//   }
// };

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
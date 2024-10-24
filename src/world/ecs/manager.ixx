export module ecs_manager;

import <unordered_map>;
import <vector>;
import <bitset>;
import <stack>;
import <utility>;
import <typeinfo>;

import glm;
import aabb;
import debug;

import ecs_component;

static std::size_t counter = 0u;

// https://github.com/chrischristakis/seecs
export namespace ecs {
using EntID = std::size_t;

struct GenSparseSet {
  virtual ~GenSparseSet() = default;
  virtual void remove(const EntID) = 0;
  // virtual void clear() = 0;
  // virtual std::size_t size() const = 0;
};
template <typename T> struct SparseSet : GenSparseSet {
private:
  static constexpr std::size_t PAGE_SIZE = 1000;
  static constexpr std::size_t NONE = -1;

  using Page = std::vector<size_t>;

  std::vector<Page> pages;
  std::vector<T> dense;
  std::vector<EntID> denseToEnt;

  void setDenseIndex(const EntID ent, const std::size_t val) {
    const auto pageIndex = ent / PAGE_SIZE, index = ent % PAGE_SIZE;
    if (pageIndex >= pages.size()) {
      pages.resize(pageIndex + 1);
    }
    Page &page = pages[pageIndex];
    if (index >= page.size())
      page.resize(index + 1, NONE);
    page[index] = val;
  }
  std::size_t getDenseIndex(const EntID ent) {
    const auto pageIndex = ent / PAGE_SIZE, index = ent % PAGE_SIZE;
    if (pageIndex >= pages.size())
      return NONE;
    Page &page = pages[pageIndex];
    if (index >= page.size())
      return NONE;
    return page[index];
  }

public:
  SparseSet() {}
  ~SparseSet() override = default;
  void clear() /*override*/ {
    pages.clear();
    dense.clear();
    denseToEnt.clear();
  }
  void reserve(const std::size_t dense, const std::size_t pages = 1) {
    this->dense.reserve(dense);
    this->denseToEnt.reserve(dense);
    this->pages.reserve(pages);
  }

  std::size_t size() const /*override*/ { return dense.size(); }
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
  bool contains(const EntID ent) { return getDenseIndex(ent) != NONE; }
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

template <typename T> void specializeAdd(T &comp) {}
template <typename T> void specializeRemove(T &comp) {}

struct Manager {
private:
  static constexpr std::size_t SIGNATURE_RESERVE = 20;
  static constexpr std::size_t COMPONENT_POOL_RESERVE = 20;
  static constexpr std::size_t GROUP_RESERVE = 50;

  static constexpr EntID NULL_ENT = -1;
  static constexpr std::size_t MAX_COMPONENTS = 64u;
  using Signature = std::bitset<MAX_COMPONENTS>;

  SparseSet<Signature> entitySignatures;
  std::vector<std::unique_ptr<GenSparseSet>> componentPools;
  std::unordered_map<Signature, SparseSet<EntID>> groups;
  std::stack<EntID> entityStack;

  auto registerID() {
    if (counter + 1 == MAX_COMPONENTS)
      throw std::runtime_error{"REGISTERING TOO MANY COMPONENTS"};
    return counter++;
  }
  template <typename T> SparseSet<T> &registerPool() {
    auto pool = std::make_unique<SparseSet<T>>();
    pool->reserve(COMPONENT_POOL_RESERVE);
    return static_cast<SparseSet<T> &>(
        *componentPools.emplace_back(std::move(pool)));
  }
  template <typename T> int registerDebug(const std::size_t id) {
    println("{} registered at {}", typeid(T).name(), id);
    return 0;
  }
  template <typename T> auto registerInfo() {
    static auto id = registerID();
    static SparseSet<T> &pool = registerPool<T>();
    static auto debug = registerDebug<T>(id);
    return std::make_pair(id, &pool);
  }

  template <typename T> auto getComponentBitIndex() {
    return registerInfo<T>().first;
  }
  template <typename T> bool getComponentBit(const Signature sig) {
    return sig.test(getComponentBitIndex<T>());
  }
  template <typename T> void setComponentBit(Signature &sig, const bool val) {
    sig.set(getComponentBitIndex<T>(), val);
  }

  template <typename T> SparseSet<T> &getComponentPool() {
    return *registerInfo<T>().second;
  }
  template <typename T> T &getComponent(const EntID ent) {
    if (auto comp = getComponentPool<T>().get(ent); comp)
      return *comp;
    throw std::runtime_error{
        std::format("{} NOT A COMPONENT OF {}", typeid(T).name(), ent)};
  }

  Signature *getSignature(const EntID ent) { return entitySignatures.get(ent); }
  template <typename... Ts> Signature getSignature() {
    Signature out;
    (setComponentBit<Ts>(out, true), ...);
    return out;
  }

  SparseSet<EntID> &getGroup(const Signature sig) {
    if (auto it = groups.find(sig); it != groups.end())
      return it->second;
    throw std::runtime_error{
        std::format("NO GROUP MATCHING SIGNATURE {}", sig.to_ullong())};
  }

  void removeFromGroup(const Signature signature, const EntID ent) {
    if (signature.none() || ent == NULL_ENT)
      return;
    getGroup(signature).remove(ent);
  }
  void addToGroup(const Signature signature, const EntID ent) {
    if (signature.none() || ent == NULL_ENT)
      return;
    auto [it, inserted] = groups.try_emplace(signature, SparseSet<EntID>{});
    if (inserted)
      it->second.reserve(GROUP_RESERVE);
    it->second.set(ent, ent);
  }

public:
  Manager() {
    entitySignatures.reserve(SIGNATURE_RESERVE);
    componentPools.reserve(MAX_COMPONENTS);
  }

  template <typename T> void add(const EntID ent, T &&comp) {
    if (ent == NULL_ENT)
      return;
    const auto sig = getSignature(ent);
    if (!sig)
      return;
    auto &pool = getComponentPool<T>();
    if (!pool.contains(ent)) {
      auto &signature = *sig;
      removeFromGroup(signature, ent);
      setComponentBit<T>(signature, true);
      addToGroup(signature, ent);
    }
    specializeAdd<T>(pool.set(ent, std::move(comp)));
    // if (pool.contains(ent)) {
    //   pool.set(ent, std::move(comp));
    //   return;
    // }
    // auto &signature = *sig;
    // removeFromGroup(signature, ent);
    // setComponentBit<T>(signature, true);
    // addToGroup(signature, ent);
    // pool.set(ent, std::move(comp));
  }
  template <typename... Ts> void add(const EntID ent, Ts &&...comps) {
    (add<Ts>(ent, std::move(comps)), ...);
  }
  template <typename T> void remove(const EntID ent) {
    if (ent == NULL_ENT)
      return;
    const auto sig = getSignature(ent);
    if (!sig)
      return;
    auto &pool = getComponentPool<T>();
    auto comp = pool.get(ent);
    if (!comp)
      return;
    specializeRemove<T>(*comp);
    auto &signature = *sig;
    removeFromGroup(signature, ent);
    setComponentBit<T>(signature, false);
    addToGroup(signature, ent);
    pool.remove(ent);
  }
  template <typename... Ts> void remove(const EntID ent) {
    (remove<Ts>(ent), ...);
  }

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
  template <typename... Ts> EntID newEntity(Ts &&...comps) {
    const auto out = newEntity();
    add<Ts...>(out, std::forward<Ts>(comps)...);
    return out;
  }

  void deleteEntity(EntID &ent) {
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
    ent = 0;
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
    const auto sig = getSignature<Ts...>();
    for (const auto &[signature, group] : groups) {
      if ((signature & sig) != sig)
        continue;
      for (const EntID ent : group.data())
        out.emplace_back(ent, getComponent<Ts>(ent)...);
    }
    return out;
  }
};
} // namespace ecs
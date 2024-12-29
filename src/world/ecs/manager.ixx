export module ecs_manager;

import <unordered_map>;
import <vector>;
import <bitset>;
import <stack>;
import <utility>;
import <typeinfo>;
import <format>;

import glm;
import aabb;
import debug;

import ecs_component;

// export namespace ecs {
// struct Signature {
//   std::size_t data = 0;
//
//   void set(const std::size_t i, const bool val) {
//     data = (data & ~(1ull << i)) | (static_cast<std::size_t>(val) << i);
//   }
//
//   Signature operator&(const Signature that) const { return {data &
//   that.data}; } Signature operator|(const Signature that) const { return
//   {data | that.data}; }
//
//   bool operator==(const Signature that) const { return data == that.data; }
//   bool operator!=(const Signature that) const { return data != that.data; }
//
//   bool none() const { return data == 0; }
//
//   unsigned long long to_ullong() const { return data; }
//
//   bool operator[](const std::size_t i) const { return (data >> i) & 1; }
// };
// } // namespace ecs
//
// template <> struct std::hash<ecs::Signature> {
//   std::size_t operator()(const ecs::Signature &sig) const noexcept {
//     return std::hash<std::size_t>{}(sig.data);
//   }
// };

// https://github.com/chrischristakis/seecs
export namespace ecs {
using EntID = std::size_t;
constexpr EntID NULL_ENT = -1;

constexpr std::size_t MAX_COMPONENTS = 64u;
using Signature = std::bitset<MAX_COMPONENTS>;

struct GenSparseSet {
  virtual ~GenSparseSet() = default;
  virtual void remove(const EntID) = 0;
};
template <typename T> struct SparseSet : GenSparseSet {
private:
  // static constexpr bool IS_SIG_OR_ENT_ID_TYPE =
  //     std::same_as<T, Signature> || std::same_as<T, EntID>;

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
  void clear() {
    pages.clear();
    dense.clear();
    denseToEnt.clear();
  }
  void reserve(const std::size_t dense, const std::size_t pages = 1) {
    this->dense.reserve(dense);
    this->denseToEnt.reserve(dense);
    this->pages.reserve(pages);
  }

  std::size_t size() const { return dense.size(); }
  bool empty() const { return size() == 0; }
  const auto &data() const { return dense; }

  template <typename U> T &set(const EntID ent, U &&val) {
    const auto index = getDenseIndex(ent);
    if (index != NONE) {
      denseToEnt[index] = ent;
      T &component = (dense[index] = std::forward<U>(val));
      // onAdd<T>(ent, component);
      return component;
    }
    setDenseIndex(ent, dense.size());

    denseToEnt.emplace_back(ent);
    T &component = dense.emplace_back(std::forward<U>(val));
    // onAdd<T>(ent, component);
    return component;
  }
  T *get(const EntID ent) {
    const auto index = getDenseIndex(ent);
    return index == NONE ? nullptr : &dense[index];
  }
  bool contains(const EntID ent) { return getDenseIndex(ent) != NONE; }
  void remove(const EntID ent) override {

    const auto index = getDenseIndex(ent);
    if (index == NONE)
      return;
    setDenseIndex(denseToEnt.back(), index);
    setDenseIndex(ent, NONE);

    std::swap(dense[index], dense.back());
    // if constexpr (IS_SIG_OR_ENT_ID_TYPE) {
    // } else {
    //   T &component = dense[index];
    //   // onRemove<T>(ent, component);
    //   std::swap(component, dense.back());
    // }
    std::swap(denseToEnt[index], denseToEnt.back());

    dense.pop_back();
    denseToEnt.pop_back();
  }
};

struct Manager {
private:
  static constexpr std::size_t SIGNATURE_RESERVE = 20;
  static constexpr std::size_t COMPONENT_POOL_RESERVE = 20;
  static constexpr std::size_t GROUP_RESERVE = 50;
  static constexpr std::size_t VIEW_RESERVE = 10;

  SparseSet<Signature> entitySignatures;
  std::vector<std::unique_ptr<GenSparseSet>> componentPools;
  std::unordered_map<Signature, SparseSet<EntID>> groups;
  std::stack<EntID> entityStack;

  std::size_t registerID();
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

  template <typename T> std::size_t getComponentBitIndex() {
    return registerInfo<T>().first;
  }
  template <typename T> bool getComponentBit(const Signature sig) {
    return sig[getComponentBitIndex<T>()];
  }
  template <typename T> void setComponentBit(Signature &sig, const bool val) {
    sig.set(getComponentBitIndex<T>(), val);
  }

  template <typename T> SparseSet<T> &getComponentPool() {
    return *registerInfo<T>().second;
  }

  Signature *signaturePointerFrom(const EntID ent);
  template <typename... Ts> Signature getSignature() {
    Signature out;
    (setComponentBit<Ts>(out, true), ...);
    return out;
  }

  SparseSet<EntID> &getGroup(const Signature sig);

  void removeFromGroup(const Signature signature, const EntID ent);
  void addToGroup(const Signature signature, const EntID ent);

public:
  Manager();

  template <typename T> T &getComponentUnchecked(const EntID ent) {
    return *getComponentPool<T>().get(ent);
  }
  template <typename T> T &getComponent(const EntID ent) {
    if (auto comp = getComponentPool<T>().get(ent); comp)
      return *comp;
    throw std::runtime_error{
        std::format("{} NOT A COMPONENT OF {}", typeid(T).name(), ent)};
  }

  template <typename T> T *add(const EntID ent, T &&comp) {
    if (ent == NULL_ENT)
      return nullptr;
    const auto sig = signaturePointerFrom(ent);
    if (!sig)
      return nullptr;
    auto &pool = getComponentPool<T>();
    if (!pool.contains(ent)) {
      auto &signature = *sig;
      removeFromGroup(signature, ent);
      setComponentBit<T>(signature, true);
      addToGroup(signature, ent);
    }
    T &c = pool.set(ent, std::move(comp));
    return &c;
  }
  template <typename... Ts>
  std::tuple<Ts *...> add(const EntID ent, Ts &&...comps) {
    return {add<Ts>(ent, std::move(comps))...};
  }
  template <typename T> void remove(const EntID ent) {
    if (ent == NULL_ENT)
      return;
    const auto sig = signaturePointerFrom(ent);
    if (!sig)
      return;
    auto &pool = getComponentPool<T>();
    T *comp = pool.get(ent);
    if (!comp)
      return;
    auto &signature = *sig;
    removeFromGroup(signature, ent);
    setComponentBit<T>(signature, false);
    addToGroup(signature, ent);
    pool.remove(ent);
  }
  template <typename... Ts> void remove(const EntID ent) {
    (remove<Ts>(ent), ...);
  }

  EntID newEntity();
  template <typename... Ts>
  std::tuple<EntID, Ts *...> newEntity(Ts &&...comps) {
    const auto id = newEntity();
    return {id, add<Ts>(id, std::forward<Ts>(comps))...};
  }

  void deleteEntity(EntID &ent);

private:
  template <bool exact, typename... Ts> auto viewIDs() {
    std::vector<EntID> out;
    out.reserve(VIEW_RESERVE);
    const auto sig = getSignature<Ts...>();
    for (const auto &[signature, group] : groups) {
      if constexpr (exact) {
        if (signature != sig)
          continue;
      } else {
        if ((signature & sig) != sig)
          continue;
      }
      out.insert(out.end(), group.data().begin(), group.data().end());
    }
    return out;
  }

public:
  template <typename... Ts> auto viewIDs() { return viewIDs<false, Ts...>; }
  template <typename... Ts> auto viewExactIDs() { return viewIDs<true, Ts...>; }

private:
  template <bool exact, typename... Ts> auto viewComponents() {
    std::vector<std::tuple<EntID, Ts *...>> out;
    out.reserve(VIEW_RESERVE);
    const auto sig = getSignature<Ts...>();
    for (const auto &[signature, group] : groups) {
      if constexpr (exact) {
        if (signature != sig)
          continue;
      } else {
        if ((signature & sig) != sig)
          continue;
      }
      for (const EntID ent : group.data())
        out.emplace_back(ent, &getComponent<Ts>(ent)...);
    }
    return out;
  }

public:
  template <typename... Ts> auto viewComponents() {
    return viewComponents<false, Ts...>();
  }
  template <typename... Ts> auto viewExactComponents() {
    return viewComponents<true, Ts...>();
  }
};
} // namespace ecs
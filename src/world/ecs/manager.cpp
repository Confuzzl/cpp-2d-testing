module ecs_manager;

using namespace ecs;

std::size_t Manager::registerID() {
  static std::size_t counter = 0;
  if (counter + 1 == MAX_COMPONENTS)
    throw std::runtime_error{"REGISTERING TOO MANY COMPONENTS"};
  return counter++;
}

Manager::Signature *Manager::signaturePointerFrom(const EntID ent) {
  return entitySignatures.get(ent);
}

SparseSet<EntID> &Manager::getGroup(const Signature sig) {
  if (auto it = groups.find(sig); it != groups.end())
    return it->second;
  throw std::runtime_error{
      std::format("NO GROUP MATCHING SIGNATURE {}", sig.to_ullong())};
}

void Manager::removeFromGroup(const Signature signature, const EntID ent) {
  if (signature.none() || ent == NULL_ENT)
    return;
  getGroup(signature).remove(ent);
}
void Manager::addToGroup(const Signature signature, const EntID ent) {
  if (signature.none() || ent == NULL_ENT)
    return;
  auto [it, inserted] = groups.try_emplace(signature, SparseSet<EntID>{});
  if (inserted)
    it->second.reserve(GROUP_RESERVE);
  it->second.set(ent, ent);
}

Manager::Manager() {
  entitySignatures.reserve(SIGNATURE_RESERVE);
  componentPools.reserve(MAX_COMPONENTS);
}

EntID Manager::newEntity() {
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

void Manager::deleteEntity(EntID &ent) {
  if (ent == NULL_ENT)
    return;
  const auto sig = signaturePointerFrom(ent);
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
  ent = NULL_ENT;
}
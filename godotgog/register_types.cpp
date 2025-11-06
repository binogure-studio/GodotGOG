#include "core/config/engine.h"
#include "register_types.h"

#include "godotgog.h"
#include "godotgog_userstats.h"
#include "godotgog_friends.h"

void initialize_godotgog_module(ModuleInitializationLevel p_level) {

  if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
    return;
  }

  GDREGISTER_CLASS(GodotGOG);
  GDREGISTER_CLASS(GodotGOGUserstats);
  GDREGISTER_CLASS(GodotGOGFriends);

  Engine::get_singleton()->add_singleton(Engine::Singleton("GodotGOG", GodotGOG::get_singleton()));
  Engine::get_singleton()->add_singleton(Engine::Singleton("GodotGOGUserstats", GodotGOGUserstats::get_singleton()));
  Engine::get_singleton()->add_singleton(Engine::Singleton("GodotGOGFriends", GodotGOGFriends::get_singleton()));
}

void uninitialize_godotgog_module(ModuleInitializationLevel p_level) {

  if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
    return;
  }

  GodotGOG::reset_singleton();
  GodotGOGUserstats::reset_singleton();
  GodotGOGFriends::reset_singleton();
}

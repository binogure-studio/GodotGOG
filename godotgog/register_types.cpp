#include "globals.h"
#include "object_type_db.h"
#include "register_types.h"

#include "godotgog.h"
#include "godotgog_userstats.h"
#include "godotgog_friends.h"

void register_godotgog_types() {
  Globals::get_singleton()->add_singleton(Globals::Singleton("GodotGOG", GodotGOG::get_singleton()));
  Globals::get_singleton()->add_singleton(Globals::Singleton("GodotGOGUserstats", GodotGOGUserstats::get_singleton()));
  Globals::get_singleton()->add_singleton(Globals::Singleton("GodotGOGFriends", GodotGOGFriends::get_singleton()));

  ObjectTypeDB::register_virtual_type<GodotGOG>();
  ObjectTypeDB::register_virtual_type<GodotGOGUserstats>();
  ObjectTypeDB::register_virtual_type<GodotGOGFriends>();
}

void unregister_godotgog_types() {
  GodotGOG::reset_singleton();
  GodotGOGUserstats::reset_singleton();
  GodotGOGFriends::reset_singleton();
}

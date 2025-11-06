#include "godotgog_friends.h"

GodotGOGFriends *GodotGOGFriends::singleton = NULL;

GodotGOGFriends::GodotGOGFriends() {
  singleton = this;
}

GodotGOGFriends *GodotGOGFriends::get_singleton() {
  if (GodotGOGFriends::singleton == NULL) {
    GodotGOGFriends::singleton = new GodotGOGFriends();
  }

  return GodotGOGFriends::singleton;
}

GodotGOGFriends::~GodotGOGFriends() { singleton = NULL; }

void GodotGOGFriends::reset_singleton() {
  galaxy::api::Shutdown();

  delete GodotGOGFriends::singleton;

  GodotGOGFriends::singleton = NULL;
}

bool GodotGOGFriends::isFriendsReady() { return galaxy::api::Friends() != NULL; }

bool GodotGOGFriends::get_friend_avatar(uint64_t userID) {
  GOG_FAIL_COND_V(!isFriendsReady(), false);

  galaxy::api::Friends()->RequestUserInformation(galaxy::api::GalaxyID::FromRealID(galaxy::api::GalaxyID::ID_TYPE_USER, userID), (galaxy::api::AvatarType) AVATAR_LARGE, this);
  if (galaxy::api::GetError()) {
    printf("GOG Galaxy: Failed to request user informations: %s\n", galaxy::api::GetError()->GetMsg());

    return false;
  }

  return true;
}

String GodotGOGFriends::get_friend_persona_name(uint64_t userID) {
  GOG_FAIL_COND_V(!isFriendsReady(), "");

  galaxy::api::GalaxyID galaxyID = galaxy::api::GalaxyID(userID);

  return galaxy::api::Friends()->GetFriendPersonaName(galaxyID);
}

String GodotGOGFriends::get_persona_name() {
  GOG_FAIL_COND_V(!isFriendsReady(), "");

  return galaxy::api::Friends()->GetPersonaName();
}

int GodotGOGFriends::get_persona_state() {
  GOG_FAIL_COND_V(!isFriendsReady(), 0);

  // Enum
  // PERSONA_STATE_OFFLINE User is not currently logged on.
  // PERSONA_STATE_ONLINE  User is logged on.
  return galaxy::api::Friends()->GetPersonaState();
}

// Listeners
void GodotGOGFriends::OnUserInformationRetrieveSuccess(galaxy::api::GalaxyID userID) {
  int width = 184;
  PackedByteArray data;
  data.resize(width * width * 4);

  galaxy::api::Friends()->GetFriendAvatarImageRGBA(userID, (galaxy::api::AvatarType) AVATAR_LARGE, data.ptrw(), data.size());

  call_deferred("emit_signal", "avatar_loaded", userID.GetRealID(), width, data);
}

void GodotGOGFriends::OnUserInformationRetrieveFailure(galaxy::api::GalaxyID userID, FailureReason failureReason) {

  call_deferred("emit_signal", "avatar_failed", userID.GetRealID(), failureReason);
}

void GodotGOGFriends::_bind_methods() {
  //Friends
  ClassDB::bind_method(D_METHOD("get_persona_name"), &GodotGOGFriends::get_persona_name);
  ClassDB::bind_method(D_METHOD("get_persona_state"), &GodotGOGFriends::get_persona_state);
  ClassDB::bind_method(D_METHOD("get_friend_persona_name", "user_id"), &GodotGOGFriends::get_friend_persona_name);
  ClassDB::bind_method(D_METHOD("get_friend_avatar", "user_id"), &GodotGOGFriends::get_friend_avatar);

  ADD_SIGNAL(MethodInfo("avatar_loaded",
    PropertyInfo(Variant::INT, "size"),
    PropertyInfo(Variant::INT, "steamID"),
    PropertyInfo(Variant::ARRAY, "avatar")
  ));

  ADD_SIGNAL(MethodInfo("avatar_failed",
    PropertyInfo(Variant::INT, "userID"),
    PropertyInfo(Variant::INT, "failureReason")
  ));

  BIND_CONSTANT(AVATAR_NONE);
  BIND_CONSTANT(AVATAR_SMALL);
  BIND_CONSTANT(AVATAR_MEDIUM);
  BIND_CONSTANT(AVATAR_LARGE);
}

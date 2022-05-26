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

  galaxy::api::Friends()->RequestUserInformation(galaxy::api::GalaxyID(userID), (galaxy::api::AvatarType) AVATAR_LARGE, this);
  if (galaxy::api::GetError()) {
    printf("GOG Galaxy: Failed to request user informations: %s\n", galaxy::api::GetError()->GetMsg());

    return false;
  }

  return true;
}

Image GodotGOGFriends::drawAvatar(int iSize, uint8_t *iBuffer) {

  // Apply buffer to Image
  Image avatar(iSize, iSize, false, Image::FORMAT_RGBA);
  for (int y = 0; y < iSize; y++) {
    for (int x = 0; x < iSize; x++) {
      int index = 4 * (x + y * iSize);

      float r = float(iBuffer[index]) / 255;
      float g = float(iBuffer[index + 1]) / 255;
      float b = float(iBuffer[index + 2]) / 255;
      float a = float(iBuffer[index + 3]) / 255;

      avatar.put_pixel(x, y, Color(r, g, b, a));
    }
  }

  return avatar;
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
  int size = 184;
  int buffSize = 4 * size * size;
  uint8_t *iBuffer = new uint8_t[buffSize];

  galaxy::api::Friends()->GetFriendAvatarImageRGBA(userID, (galaxy::api::AvatarType) AVATAR_LARGE, iBuffer, buffSize);
  Image avatar = drawAvatar(size, iBuffer);

  call_deferred("emit_signal", "avatar_loaded", AVATAR_LARGE, userID.GetRealID(), avatar);
}

void GodotGOGFriends::OnUserInformationRetrieveFailure(galaxy::api::GalaxyID userID, FailureReason failureReason) {

  call_deferred("emit_signal", "avatar_failed", userID.GetRealID(), failureReason);
}

void GodotGOGFriends::_bind_methods() {
  //Friends
  ObjectTypeDB::bind_method(_MD("get_persona_name"), &GodotGOGFriends::get_persona_name);
  ObjectTypeDB::bind_method(_MD("get_persona_state"), &GodotGOGFriends::get_persona_state);
  ObjectTypeDB::bind_method(_MD("get_friend_persona_name", "user_id"), &GodotGOGFriends::get_friend_persona_name);
  ObjectTypeDB::bind_method(_MD("get_friend_avatar", "user_id"), &GodotGOGFriends::get_friend_avatar);

  ADD_SIGNAL(MethodInfo("avatar_loaded", PropertyInfo(Variant::INT, "userID"), PropertyInfo(Variant::INT, "type"), PropertyInfo(Variant::IMAGE, "avatar")));
  ADD_SIGNAL(MethodInfo("avatar_failed", PropertyInfo(Variant::INT, "userID"), PropertyInfo(Variant::INT, "failureReason")));

  BIND_CONSTANT(AVATAR_NONE);
  BIND_CONSTANT(AVATAR_SMALL);
  BIND_CONSTANT(AVATAR_MEDIUM);
  BIND_CONSTANT(AVATAR_LARGE);
}

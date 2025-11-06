#ifndef GODOTGOGFRIENDS_H
#define GODOTGOGFRIENDS_H

#include "godotgog.h"

class GodotGOGFriends : public Object,
  galaxy::api::GlobalUserInformationRetrieveListener
{
public:
  enum
  {
    AVATAR_NONE = 0x0000,
    AVATAR_SMALL = 0x0001,
    AVATAR_MEDIUM = 0x0002,
    AVATAR_LARGE = 0x0004
  };

  static GodotGOGFriends *get_singleton();
  static void reset_singleton();

  GodotGOGFriends();
  ~GodotGOGFriends();

  bool isFriendsReady();

  String get_friend_persona_name(uint64_t user_id);
  bool get_friend_avatar(uint64_t user_id);
  String get_persona_name();
  int get_persona_state();

protected:
  static void _bind_methods();
  static GodotGOGFriends *singleton;

private:

  GDCLASS(GodotGOGFriends, Object);

  void OnUserInformationRetrieveSuccess(galaxy::api::GalaxyID userID) override;
  void OnUserInformationRetrieveFailure(galaxy::api::GalaxyID userID, FailureReason failureReason) override;
};

#endif // GODOTGOGFRIENDS_H

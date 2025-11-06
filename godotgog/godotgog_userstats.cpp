#include "godotgog_userstats.h"

GodotGOGUserstats *GodotGOGUserstats::singleton = NULL;

GodotGOGUserstats::GodotGOGUserstats() {
  singleton = this;
}

GodotGOGUserstats *GodotGOGUserstats::get_singleton() {
  if (GodotGOGUserstats::singleton == NULL) {
    GodotGOGUserstats::singleton = new GodotGOGUserstats();
  }

  return GodotGOGUserstats::singleton;
}

GodotGOGUserstats::~GodotGOGUserstats() { singleton = NULL; }

void GodotGOGUserstats::reset_singleton() {
  delete GodotGOGUserstats::singleton;

  GodotGOGUserstats::singleton = NULL;
}

bool GodotGOGUserstats::isUserStatsReady() { return galaxy::api::Stats() != NULL; }

uint32_t GodotGOGUserstats::getUserTimePlayed() {
  GOG_FAIL_COND_V(!isUserStatsReady(), 0);

  return galaxy::api::Stats()->GetUserTimePlayed();
}

bool GodotGOGUserstats::requestLeaderboards() {
  GOG_FAIL_COND_V(!isUserStatsReady(), false);

  galaxy::api::Stats()->RequestLeaderboards(this);
  if (galaxy::api::GetError()) {
    printf("GOG Galaxy: Failed to request leaderboards: %s\n", galaxy::api::GetError()->GetMsg());

    return false;
  }

  return true;
}

bool GodotGOGUserstats::findLeaderboard(const String &name) {
  GOG_FAIL_COND_V(!isUserStatsReady(), false);

  galaxy::api::Stats()->FindLeaderboard(name.utf8().get_data(), this);
  if (galaxy::api::GetError()) {
    printf("GOG Galaxy: Failed to request leaderboard: %s\n", galaxy::api::GetError()->GetMsg());

    return false;
  }

  return true;
}

bool GodotGOGUserstats::findOrCreateLeaderboard(const String &name, const String &displayName, int sortMethod, int displayType) {
  GOG_FAIL_COND_V(!isUserStatsReady(), false);

  galaxy::api::Stats()->FindOrCreateLeaderboard(name.utf8().get_data(), displayName.utf8().get_data(), (galaxy::api::LeaderboardSortMethod) sortMethod, (galaxy::api::LeaderboardDisplayType) displayType, this);
  if (galaxy::api::GetError()) {
    printf("GOG Galaxy: Failed to request leaderboard: %s\n", galaxy::api::GetError()->GetMsg());

    return false;
  }

  return true;
}

int GodotGOGUserstats::getLeaderboardEntryCount(const String &name) {
  GOG_FAIL_COND_V(!isUserStatsReady(), 0);

  return galaxy::api::Stats()->GetLeaderboardEntryCount(name.utf8().get_data());
}

bool GodotGOGUserstats::downloadLeaderboardEntries(const String &name, int countBefore, int countAfter) {
  GOG_FAIL_COND_V(!isUserStatsReady(), false);

  galaxy::api::Stats()->RequestLeaderboardEntriesAroundUser(name.utf8().get_data(), countBefore, countAfter, galaxy::api::User()->GetGalaxyID(), this);
  if (galaxy::api::GetError()) {
    printf("GOG Galaxy: Failed to set stat (float): %s\n", galaxy::api::GetError()->GetMsg());

    return false;
  }

  return true;
}

bool GodotGOGUserstats::setLeaderboardScore(const String &name, int score, bool forceUpdate) {
  GOG_FAIL_COND_V(!isUserStatsReady(), false);

  galaxy::api::Stats()->SetLeaderboardScore(name.utf8().get_data(), score, forceUpdate, this);
  if (galaxy::api::GetError()) {
    printf("GOG Galaxy: Failed to update leaderboard score: %s\n", galaxy::api::GetError()->GetMsg());

    return false;
  }

  return true;
}

Array GodotGOGUserstats::getLeaderboardEntries() {
  return leaderboard_entries;
}

bool GodotGOGUserstats::isAchievementVisible(const String &name) {
  GOG_FAIL_COND_V(!isUserStatsReady(), false);

  return galaxy::api::Stats()->IsAchievementVisible(name.utf8().get_data());
}

bool GodotGOGUserstats::setStatFloat(const String &name, float value) {
  GOG_FAIL_COND_V(!isUserStatsReady(), false);

  galaxy::api::Stats()->SetStatFloat(name.utf8().get_data(), value);

  if (galaxy::api::GetError()) {
    printf("GOG Galaxy: Failed to set stat (float): %s\n", galaxy::api::GetError()->GetMsg());

    return false;
  }

  return true;
}

float GodotGOGUserstats::getStatFloat(const String &name) {
  GOG_FAIL_COND_V(!isUserStatsReady(), 0.0);

  return galaxy::api::Stats()->GetStatFloat(name.utf8().get_data());
}

bool GodotGOGUserstats::setStatInt(const String &name, int value) {
  GOG_FAIL_COND_V(!isUserStatsReady(), false);

  galaxy::api::Stats()->SetStatInt(name.utf8().get_data(), value);

  if (galaxy::api::GetError()) {
    printf("GOG Galaxy: Failed to set stat (int): %s\n", galaxy::api::GetError()->GetMsg());

    return false;
  }

  return true;
}

int GodotGOGUserstats::getStatInt(const String &name) {
  GOG_FAIL_COND_V(!isUserStatsReady(), 0);

  return galaxy::api::Stats()->GetStatInt(name.utf8().get_data());
}

bool GodotGOGUserstats::resetAllStats() {
  GOG_FAIL_COND_V(!isUserStatsReady(), false);

  galaxy::api::Stats()->ResetStatsAndAchievements(this);

  if (galaxy::api::GetError()) {
    printf("GOG Galaxy: Failed to reset stats and achievements: %s\n", galaxy::api::GetError()->GetMsg());

    return false;
  }

  return true;
}

bool GodotGOGUserstats::queryAchievements() {
  GOG_FAIL_COND_V(!isUserStatsReady(), false);

  galaxy::api::Stats()->RequestUserStatsAndAchievements(galaxy::api::User()->GetGalaxyID(), this);

  if (galaxy::api::GetError()) {
    printf("GOG Galaxy: Failed to query achievements: %s\n", galaxy::api::GetError()->GetMsg());

    return false;
  }

  return true;
}

bool GodotGOGUserstats::storeAchievements() {
  GOG_FAIL_COND_V(!isUserStatsReady(), false);
  galaxy::api::Stats()->StoreStatsAndAchievements(this);

  if (galaxy::api::GetError()) {
    printf("GOG Galaxy: Failed to store achievements: %s\n", galaxy::api::GetError()->GetMsg());

    return false;
  }

  return true;
}

bool GodotGOGUserstats::getAchievement(const String &name) {
  // This shortcut is safe, since everything is checked by the 
  // getAchievementAndUnlockTime() function.
  return getAchievementAndUnlockTime(name)["achieved"];
}

Dictionary GodotGOGUserstats::getAchievementAndUnlockTime(const String &name) {
  Dictionary achieve;

  achieve["achieved"] = false;

  GOG_FAIL_COND_V(!isUserStatsReady(), achieve);

  bool unlocked = false;
  uint32_t unlockTime = 0;

  galaxy::api::Stats()->GetAchievement(name.utf8().get_data(), unlocked, unlockTime, galaxy::api::User()->GetGalaxyID());

  if (galaxy::api::GetError()) {
    printf("GOG Galaxy: Failed to get achievement %s: %s\n", name.utf8().get_data(), galaxy::api::GetError()->GetMsg());

  } else {
    achieve["achieved"] = unlocked;
    achieve["unlocked"] = unlockTime;
  }

  return achieve;
}

bool GodotGOGUserstats::clearAchievement(const String &name) {
  GOG_FAIL_COND_V(!isUserStatsReady(), false);

  galaxy::api::Stats()->ClearAchievement(name.utf8().get_data());

  if (galaxy::api::GetError()) {
    printf("GOG Galaxy: Failed to clear achievement: %s\n", galaxy::api::GetError()->GetMsg());

    return false;
  }

  return true;
}

String GodotGOGUserstats::getAchievementDescription(const String &name) {
  GOG_FAIL_COND_V(!isUserStatsReady(), "");

  return galaxy::api::Stats()->GetAchievementDescription(name.utf8().get_data());
}

String GodotGOGUserstats::getAchievementName(const String &name) {
  GOG_FAIL_COND_V(!isUserStatsReady(), "");

  return galaxy::api::Stats()->GetAchievementDisplayName(name.utf8().get_data());
}

bool GodotGOGUserstats::setAchievement(const String &name) {
  GOG_FAIL_COND_V(!isUserStatsReady(), false);

  galaxy::api::Stats()->SetAchievement(name.utf8().get_data());

  if (galaxy::api::GetError()) {
    printf("GOG Galaxy: Failed to set achievement %s: %s\n", name.utf8().get_data(), galaxy::api::GetError()->GetMsg());

    return false;
  }

  return true;
}

// Listeners
void GodotGOGUserstats::OnUserStatsAndAchievementsRetrieveSuccess(galaxy::api::GalaxyID userID) {
  
  emit_signal("stats_and_achievement_retrieve_success", userID.GetRealID());
}

void GodotGOGUserstats::OnUserStatsAndAchievementsRetrieveFailure(galaxy::api::GalaxyID userID, galaxy::api::IUserStatsAndAchievementsRetrieveListener::FailureReason failureReason) {
  
  emit_signal("stats_and_achievement_retrieve_failure", userID.GetRealID(), failureReason);
}

void GodotGOGUserstats::OnUserStatsAndAchievementsStoreSuccess() {
  
  emit_signal("stats_and_achievement_store_success");
}

void GodotGOGUserstats::OnUserStatsAndAchievementsStoreFailure(galaxy::api::IStatsAndAchievementsStoreListener::FailureReason failureReason) {
  
  emit_signal("stats_and_achievement_store_failure", failureReason);
}

void GodotGOGUserstats::OnAchievementUnlocked(const char* name) {
  
  emit_signal("achievement_unlocked", name);
}

void GodotGOGUserstats::OnLeaderboardsRetrieveSuccess() {
  
  emit_signal("leaderboards_retrieve_success");
}

void GodotGOGUserstats::OnLeaderboardsRetrieveFailure(galaxy::api::ILeaderboardsRetrieveListener::FailureReason failureReason) {
  
  emit_signal("leaderboards_retrieve_failure", failureReason);
}

void GodotGOGUserstats::OnLeaderboardEntriesRetrieveSuccess(const char* name, uint32_t entryCount) {
  
  leaderboard_entries.clear();

  for (uint32_t index = 0; index < entryCount; index++) {
    uint32_t rank;
    int32_t score;
    galaxy::api::GalaxyID user_id;

    galaxy::api::Stats()->GetRequestedLeaderboardEntry(index, rank, score, user_id);

    Dictionary entryDict;
    entryDict["score"] = score;
    entryDict["name"] = galaxy::api::Friends()->GetFriendPersonaName(user_id);
    entryDict["user_id"] = user_id.GetRealID();
    entryDict["global_rank"] = rank;

    leaderboard_entries.append(entryDict);
  }

  emit_signal("leaderboard_entries_retrieve_success", name, leaderboard_entries);
}

void GodotGOGUserstats::OnLeaderboardEntriesRetrieveFailure(const char* name, galaxy::api::ILeaderboardEntriesRetrieveListener::FailureReason failureReason) {
  
  emit_signal("leaderboard_entries_retrieve_failure", name, failureReason);
}

void GodotGOGUserstats::OnLeaderboardScoreUpdateSuccess(const char* name, int32_t score, uint32_t oldRank, uint32_t newRank) {
  
  emit_signal("leaderboard_score_update_success", name, score, oldRank, newRank);
}

void GodotGOGUserstats::OnLeaderboardScoreUpdateFailure(const char* name, int32_t score, galaxy::api::ILeaderboardScoreUpdateListener::FailureReason failureReason) {
  
  emit_signal("leaderboard_score_update_failure", name, score, failureReason);
}

void GodotGOGUserstats::OnLeaderboardRetrieveSuccess(const char* name) {
  
  emit_signal("leaderboard_retrieve_success", name);
}

void GodotGOGUserstats::OnLeaderboardRetrieveFailure(const char* name, galaxy::api::ILeaderboardRetrieveListener::FailureReason failureReason) {
  
  emit_signal("leaderboard_retrieve_failure", name, failureReason);
}

void GodotGOGUserstats::_bind_methods() {

  //Stats & Achievements
  ClassDB::bind_method(D_METHOD("queryAchievements"), &GodotGOGUserstats::queryAchievements);
  ClassDB::bind_method(D_METHOD("storeAchievements"), &GodotGOGUserstats::storeAchievements);

  ClassDB::bind_method(D_METHOD("clearAchievement", "name"), &GodotGOGUserstats::clearAchievement);
  ClassDB::bind_method(D_METHOD("getUserTimePlayed"), &GodotGOGUserstats::getUserTimePlayed);
  ClassDB::bind_method(D_METHOD("isAchievementVisible", "name"), &GodotGOGUserstats::isAchievementVisible);
  ClassDB::bind_method(D_METHOD("getAchievement", "name"), &GodotGOGUserstats::getAchievement);
  ClassDB::bind_method(D_METHOD("getAchievementAndUnlockTime", "name"), &GodotGOGUserstats::getAchievementAndUnlockTime);
  ClassDB::bind_method(D_METHOD("setAchievement", "name"), &GodotGOGUserstats::setAchievement);
  ClassDB::bind_method(D_METHOD("getAchievementName", "name"), &GodotGOGUserstats::getAchievementName);
  ClassDB::bind_method(D_METHOD("getAchievementDescription", "name"), &GodotGOGUserstats::getAchievementDescription);
  ClassDB::bind_method(D_METHOD("setStatFloat", "name", "value"), &GodotGOGUserstats::setStatFloat);
  ClassDB::bind_method(D_METHOD("getStatFloat", "name"), &GodotGOGUserstats::getStatFloat);
  ClassDB::bind_method(D_METHOD("setStatInt", "name", "value"), &GodotGOGUserstats::setStatInt);
  ClassDB::bind_method(D_METHOD("getStatInt", "name"), &GodotGOGUserstats::getStatInt);
  ClassDB::bind_method(D_METHOD("resetAllStats"), &GodotGOGUserstats::resetAllStats);
  ClassDB::bind_method(D_METHOD("requestLeaderboards"), &GodotGOGUserstats::requestLeaderboards);
  ClassDB::bind_method(D_METHOD("findLeaderboard", "name"), &GodotGOGUserstats::findLeaderboard);
  ClassDB::bind_method(D_METHOD("findOrCreateLeaderboard", "name", "displayName", "sortMethod", "displayType"), &GodotGOGUserstats::findOrCreateLeaderboard);
  ClassDB::bind_method(D_METHOD("getLeaderboardEntryCount", "name"), &GodotGOGUserstats::getLeaderboardEntryCount);
  ClassDB::bind_method(D_METHOD("downloadLeaderboardEntries", "rStart", "rEnd", "type"), &GodotGOGUserstats::downloadLeaderboardEntries);
  ClassDB::bind_method(D_METHOD("setLeaderboardScore", "name", "score", "forceUpdate"), &GodotGOGUserstats::setLeaderboardScore);
  ClassDB::bind_method(D_METHOD("getLeaderboardEntries"), &GodotGOGUserstats::getLeaderboardEntries);

  //Signals
  ADD_SIGNAL(MethodInfo("stats_and_achievement_retrieve_success", PropertyInfo(Variant::INT, "userID")));
  ADD_SIGNAL(MethodInfo("stats_and_achievement_retrieve_failure", PropertyInfo(Variant::INT, "userID"), PropertyInfo(Variant::INT, "failureReason")));
  ADD_SIGNAL(MethodInfo("stats_and_achievement_store_success"));
  ADD_SIGNAL(MethodInfo("stats_and_achievement_store_failure", PropertyInfo(Variant::INT, "failureReason")));
  ADD_SIGNAL(MethodInfo("achievement_unlocked", PropertyInfo(Variant::STRING, "name")));

  ADD_SIGNAL(MethodInfo("leaderboards_retrieve_success"));
  ADD_SIGNAL(MethodInfo("leaderboards_retrieve_failure", PropertyInfo(Variant::INT, "failureReason")));
  ADD_SIGNAL(MethodInfo("leaderboard_entries_retrieve_success", PropertyInfo(Variant::STRING, "name"), PropertyInfo(Variant::INT, "entries")));
  ADD_SIGNAL(MethodInfo("leaderboard_entries_retrieve_failure", PropertyInfo(Variant::STRING, "name"), PropertyInfo(Variant::INT, "failureReason")));
  ADD_SIGNAL(MethodInfo("leaderboard_score_update_success", PropertyInfo(Variant::STRING, "name"), PropertyInfo(Variant::INT, "score"), PropertyInfo(Variant::INT, "oldRank"), PropertyInfo(Variant::INT, "newRank")));
  ADD_SIGNAL(MethodInfo("leaderboard_score_update_failure", PropertyInfo(Variant::STRING, "name"), PropertyInfo(Variant::INT, "score"), PropertyInfo(Variant::INT, "failureReason")));
  ADD_SIGNAL(MethodInfo("leaderboard_retrieve_success", PropertyInfo(Variant::STRING, "name")));
  ADD_SIGNAL(MethodInfo("leaderboard_retrieve_failure", PropertyInfo(Variant::STRING, "name"), PropertyInfo(Variant::INT, "failureReason")));

  BIND_CONSTANT(LEADERBOARD_SORT_METHOD_NONE);
  BIND_CONSTANT(LEADERBOARD_SORT_METHOD_ASCENDING);
  BIND_CONSTANT(LEADERBOARD_SORT_METHOD_DESCENDING);
  BIND_CONSTANT(LEADERBOARD_DISPLAY_TYPE_NONE);
  BIND_CONSTANT(LEADERBOARD_DISPLAY_TYPE_NUMBER);
  BIND_CONSTANT(LEADERBOARD_DISPLAY_TYPE_TIME_SECONDS);
  BIND_CONSTANT(LEADERBOARD_DISPLAY_TYPE_TIME_MILLISECONDS);

}

#ifndef GODOTGOGUSERSTATS_H
#define GODOTGOGUSERSTATS_H

#include "godotgog.h"
#include "godotgog_friends.h"

class GodotGOGUserstats : public Object,
  galaxy::api::GlobalUserStatsAndAchievementsRetrieveListener,
  galaxy::api::GlobalStatsAndAchievementsStoreListener,
  galaxy::api::GlobalAchievementChangeListener,
  galaxy::api::GlobalLeaderboardsRetrieveListener,
  galaxy::api::GlobalLeaderboardEntriesRetrieveListener,
  galaxy::api::GlobalLeaderboardScoreUpdateListener,
  galaxy::api::GlobalLeaderboardRetrieveListener
{
public:
  static GodotGOGUserstats *get_singleton();
  static void reset_singleton();

  enum SortMethod
  {
    LEADERBOARD_SORT_METHOD_NONE,
    LEADERBOARD_SORT_METHOD_ASCENDING,
    LEADERBOARD_SORT_METHOD_DESCENDING
  };

  enum DisplayType
  {
    LEADERBOARD_DISPLAY_TYPE_NONE,
    LEADERBOARD_DISPLAY_TYPE_NUMBER,
    LEADERBOARD_DISPLAY_TYPE_TIME_SECONDS,
    LEADERBOARD_DISPLAY_TYPE_TIME_MILLISECONDS
  };

  GodotGOGUserstats();
  ~GodotGOGUserstats();

  bool isUserStatsReady();

  bool clearAchievement(const String &name);

  bool queryAchievements();
  bool storeAchievements();

  uint32_t getUserTimePlayed();
  bool isAchievementVisible(const String &name);

  bool getAchievement(const String &name);
  Dictionary getAchievementAndUnlockTime(const String &name);
  bool setAchievement(const String &name);

  String getAchievementName(const String &name);
  String getAchievementDescription(const String &name);

  bool setStatFloat(const String &name, float value);
  float getStatFloat(const String &name);

  bool setStatInt(const String &name, int value);
  int getStatInt(const String &name);

  bool resetAllStats();
  
  bool requestLeaderboards();
  bool findLeaderboard(const String &name);
  bool findOrCreateLeaderboard(const String &name, const String &displayName, int sortMethod, int displayType);

  int getLeaderboardEntryCount(const String &name);
  bool downloadLeaderboardEntries(const String &name, int countBefore, int countAfter);
  bool setLeaderboardScore(const String &name, int score, bool forceUpdate = false);

  Array getLeaderboardEntries();

protected:
  static void _bind_methods();
  static GodotGOGUserstats *singleton;

private:
  Array leaderboard_entries;

  // Achievement listeners
  void OnUserStatsAndAchievementsRetrieveSuccess(galaxy::api::GalaxyID userID) override;
  void OnUserStatsAndAchievementsRetrieveFailure(galaxy::api::GalaxyID userID, galaxy::api::IUserStatsAndAchievementsRetrieveListener::FailureReason failureReason) override;

  void OnUserStatsAndAchievementsStoreSuccess() override;
  void OnUserStatsAndAchievementsStoreFailure(galaxy::api::IStatsAndAchievementsStoreListener::FailureReason failureReason) override;

  void OnAchievementUnlocked(const char* name) override;

  // Leaderboard listeners
  void OnLeaderboardsRetrieveSuccess() override;
  void OnLeaderboardsRetrieveFailure(galaxy::api::ILeaderboardsRetrieveListener::FailureReason failureReason) override;

  void OnLeaderboardEntriesRetrieveSuccess(const char* name, uint32_t entryCount) override;
  void OnLeaderboardEntriesRetrieveFailure(const char* name, galaxy::api::ILeaderboardEntriesRetrieveListener::FailureReason failureReason) override;

  void OnLeaderboardScoreUpdateSuccess(const char* name, int32_t score, uint32_t oldRank, uint32_t newRank) override;
  void OnLeaderboardScoreUpdateFailure(const char* name, int32_t score, galaxy::api::ILeaderboardScoreUpdateListener::FailureReason failureReason) override;

  void OnLeaderboardRetrieveSuccess(const char* name) override;
  void OnLeaderboardRetrieveFailure(const char* name, galaxy::api::ILeaderboardRetrieveListener::FailureReason failureReason) override;

  GDCLASS(GodotGOGUserstats, Object);
};
#endif // GODOTGOGUSERSTATS_H

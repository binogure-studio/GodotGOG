#ifndef GODOTGOG_H
#define GODOTGOG_H

#include <iostream>
#include <inttypes.h>
#include <galaxy/GalaxyID.h>
#include <galaxy/GalaxyApi.h>

#include "core/object/object.h"
#include "core/object/ref_counted.h"
#include "core/variant/dictionary.h"

#define GOG_FAIL_COND(m_cond) \
  {                             \
    if (m_cond) {               \
      return;                   \
    }                           \
  }

#define GOG_FAIL_COND_V(m_cond, m_retval) \
  {                                         \
    if (m_cond) {                           \
      return m_retval;                      \
    }                                       \
  }


class GodotGOG : public Object, galaxy::api::GlobalAuthListener
{
public:
  enum {
    GOG_INIT_IN_PROGRESS = 1,
    GOG_INIT_PRODUCT_NOT_OWNED = 2,
    GOG_INIT_NOT_INSTALLED = 3,
    GOG_INIT_OFFLINE = 4,
    GOG_INIT_OK = 5
  };

  static GodotGOG *get_singleton();
  static void reset_singleton();
  GodotGOG();
  ~GodotGOG();

  //Initialization
  int GOGInit(String clientId, String clientSecret, bool online = true);
  void run_callbacks();
  void shutdown();

  //User
  uint64_t get_galaxy_id();
  void sign_in(bool online = false);
  void sign_out();
  bool signed_in();
  bool is_logged_out();

protected:
  static void _bind_methods();
  static GodotGOG *singleton;

private:
  bool isInitSuccess = false;
  int GOGStatus = GOG_INIT_NOT_INSTALLED;

  //callbacks
  void OnAuthSuccess() override;
  void OnAuthFailure(galaxy::api::IAuthListener::FailureReason failureReason) override;
  void OnAuthLost() override;

  GDCLASS(GodotGOG, Object);
};

#endif // GODOTGOG_H

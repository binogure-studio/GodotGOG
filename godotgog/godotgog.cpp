#include "godotgog.h"

GodotGOG *GodotGOG::singleton = NULL;

GodotGOG::GodotGOG() {
  isInitSuccess = false;
  singleton = this;
}

GodotGOG *GodotGOG::get_singleton() {
  if (GodotGOG::singleton == NULL) {
    GodotGOG::singleton = new GodotGOG();
  }

  return GodotGOG::singleton;
}

GodotGOG::~GodotGOG() { singleton = NULL; }

void GodotGOG::reset_singleton() {
  galaxy::api::Shutdown();

  delete GodotGOG::singleton;

  GodotGOG::singleton = NULL;
}

int GodotGOG::GOGInit(String clientId, String clientSecret) {
  galaxy::api::Init({
    clientId.utf8().get_data(),
    clientSecret.utf8().get_data()
  });

  if (galaxy::api::GetError()) {
    printf("GOG Not installed: %s\n", galaxy::api::GetError()->GetMsg());
    GOGStatus = GOG_INIT_NOT_INSTALLED;

  } else {
    GOGStatus = GOG_INIT_IN_PROGRESS;
    // Trigger sign in right away
    sign_in();
  }

  return GOGStatus;
}

void GodotGOG::sign_in() {
  // If GOG hasn't been initialized, we'd better not try to sign in again
  // It's better to call GOGInit insteads
  GOG_FAIL_COND(GOGStatus == GOG_INIT_NOT_INSTALLED);

  printf("GOG Initialization\n");
  galaxy::api::User()->SignInGalaxy(false, this);
  if (galaxy::api::GetError()) {
    printf("GOG Signing in error: %s\n", galaxy::api::GetError()->GetMsg());
    GOGStatus = GOG_INIT_PRODUCT_NOT_OWNED;

  } else {
    GOGStatus = GOG_INIT_IN_PROGRESS;
  }
}

void GodotGOG::run_callbacks() {
  GOG_FAIL_COND(GOGStatus == GOG_INIT_NOT_INSTALLED);

  galaxy::api::ProcessData();
}

void GodotGOG::shutdown() {
  GOG_FAIL_COND(GOGStatus == GOG_INIT_NOT_INSTALLED);

  galaxy::api::Shutdown();
}

//User
uint64_t GodotGOG::get_galaxy_id() {
  GOG_FAIL_COND_V(GOGStatus < GOG_INIT_OFFLINE, 0);

  galaxy::api::GalaxyID galaxyID = galaxy::api::User()->GetGalaxyID();
  return galaxyID.GetRealID();
}

void GodotGOG::sign_out() {
  GOG_FAIL_COND(GOGStatus < GOG_INIT_OK);

  galaxy::api::User()->SignOut();
}

bool GodotGOG::signed_in() {
  GOG_FAIL_COND_V(GOGStatus < GOG_INIT_OFFLINE, false);

  return galaxy::api::User()->SignedIn();
}

bool GodotGOG::is_logged_out() {
  GOG_FAIL_COND_V(GOGStatus < GOG_INIT_OFFLINE, true);

  return !galaxy::api::User()->IsLoggedOn();
}

//Signals
void GodotGOG::OnAuthSuccess() {
  galaxy::api::Stats()->RequestUserStatsAndAchievements(galaxy::api::User()->GetGalaxyID());
  GOGStatus = GOG_INIT_OK;

  emit_signal("auth_success");
}

void GodotGOG::OnAuthFailure(galaxy::api::IAuthListener::FailureReason failureReason) {
  switch (failureReason) {
    case galaxy::api::IAuthListener::FailureReason::FAILURE_REASON_GALAXY_NOT_INITIALIZED:
      printf("GOG Galaxy not initialized, please call GOGInit() first!\n");

    case galaxy::api::IAuthListener::FailureReason::FAILURE_REASON_UNDEFINED:
    case galaxy::api::IAuthListener::FailureReason::FAILURE_REASON_GALAXY_SERVICE_NOT_AVAILABLE:
      GOGStatus = GOG_INIT_NOT_INSTALLED;
    break;

    case galaxy::api::IAuthListener::FailureReason::FAILURE_REASON_GALAXY_SERVICE_NOT_SIGNED_IN:
    case galaxy::api::IAuthListener::FailureReason::FAILURE_REASON_CONNECTION_FAILURE:
    case galaxy::api::IAuthListener::FailureReason::FAILURE_REASON_INVALID_CREDENTIALS:
    case galaxy::api::IAuthListener::FailureReason::FAILURE_REASON_EXTERNAL_SERVICE_FAILURE:
      GOGStatus = GOG_INIT_OFFLINE;
    break;

    case galaxy::api::IAuthListener::FailureReason::FAILURE_REASON_NO_LICENSE:
      GOGStatus = GOG_INIT_PRODUCT_NOT_OWNED;
    break;
  }

  emit_signal("auth_failure", failureReason);
}

void GodotGOG::OnAuthLost() {
  // Try to log out to be sure that the session is not broken
  galaxy::api::User()->SignOut();
  GOGStatus = GOG_INIT_OFFLINE;

  emit_signal("auth_lost");
}

void GodotGOG::_bind_methods() {
  //Initialization
  ObjectTypeDB::bind_method(_MD("GOGInit", "clientId", "clientSecret"), &GodotGOG::GOGInit);
  ObjectTypeDB::bind_method(_MD("run_callbacks"), &GodotGOG::run_callbacks);
  ObjectTypeDB::bind_method(_MD("shutdown"), &GodotGOG::shutdown);

  //User
  ObjectTypeDB::bind_method(_MD("get_galaxy_id"), &GodotGOG::get_galaxy_id);
  ObjectTypeDB::bind_method(_MD("sign_in"), &GodotGOG::signed_in);
  ObjectTypeDB::bind_method(_MD("sign_out"), &GodotGOG::sign_out);
  ObjectTypeDB::bind_method(_MD("signed_in"), &GodotGOG::signed_in);
  ObjectTypeDB::bind_method(_MD("is_logged_out"), &GodotGOG::is_logged_out);

  //Signals
  ADD_SIGNAL(MethodInfo("auth_success"));
  ADD_SIGNAL(MethodInfo("auth_failure", PropertyInfo(Variant::INT, "reason")));
  ADD_SIGNAL(MethodInfo("auth_lost"));

  BIND_CONSTANT(GOG_INIT_OK);
  BIND_CONSTANT(GOG_INIT_OFFLINE);
  BIND_CONSTANT(GOG_INIT_NOT_INSTALLED);
  BIND_CONSTANT(GOG_INIT_IN_PROGRESS);
  BIND_CONSTANT(GOG_INIT_PRODUCT_NOT_OWNED);
}

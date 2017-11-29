/**
 * @brief A header for the homepage class which represents the mission control for the hue lights system
 * @author Gurkiran Tatla
 * @author Jake Schindler
 * @author Justine Kim
 * @author Paul Salvatore
 * @author Timal Peramune
 */

#ifndef HOME_PAGE_H
#define HOME_PAGE_H


#include "Light.hpp"
#include "LightGroup.hpp"
#include "Predict.hpp"
#include "ScheduleItem.hpp"
#include "User.h"
#include "Persistence.h"
#include "Bridge.hpp"
#include <Wt/WApplication>
#include <Wt/WIOService>
#include <Wt/WInPlaceEdit>
#include <Wt/WSlider>
#include <vector>
#include <ctime>
#include <chrono>
#include <future>
#include <iterator>
#include <unistd.h>
#include <pthread.h>

#define SLEEP_PERIOD 60*60

class Light;
class LightGroup;

class HomePage : public Wt::WApplication {

  std::string buildCurrentSchedule;
  std::string deleteMessage;

  std::vector<LightGroup *> groups;
  std::vector<Wt::WSlider *> groupHueSliders;
  std::vector<Wt::WSlider *> groupBriSliders;

  std::vector<Wt::WLineEdit *> groupEdits;

  std::vector<Light *> lights;
  std::string name;

  std::vector<Wt::WLineEdit *> edits;
  std::vector<Wt::WSlider *> hueSliders;
  std::vector<Wt::WSlider *> brightnessSliders;

  Wt::WLineEdit *addLightEdit;
  Wt::WLineEdit *removeLightEdit;
  Wt::WLineEdit * removeGroupEdit;

  Wt::WLineEdit *deleteScheduleId;
  Wt::WLineEdit *createScheduleId;

  Wt::WLineEdit *scheduleYear;
  Wt::WLineEdit *scheduleMonth;
  Wt::WLineEdit *scheduleDay;
  Wt::WLineEdit *scheduleHour;
  Wt::WLineEdit *scheduleMin;
  Wt::WLineEdit *scheduleOn;
  Wt::WLineEdit *scheduleTransition;
  Wt::WSlider *scheduleHue;
  Wt::WSlider *scheduleBrightness;

  Wt::WLineEdit *bridgeName;
  Wt::WLineEdit *bridgePlace;
  Wt::WLineEdit *bridgeIPAddress;
  Wt::WLineEdit *bridgePort; 
  Wt::WLineEdit *bridgeSelect;

  Wt::WText *scheduleText;
  Wt::WText *scheduleErrorText;
  Wt::WText *predictionText;
  Wt::WPushButton *predictionRecording;
  
  pthread_t worker_thread;
  bool threadRunning;

  Wt::WContainerWidget *containerLogin;
  Wt::WContainerWidget *containerSettings;
  Wt::WContainerWidget *containerHomePage;

  Wt::WPushButton *loginContainerBtn;
  Wt::WPushButton *SettingsContainerBtn;
  Wt::WPushButton *HomePageContainerBtn;
  Wt::WPushButton *DeleteAccountButton;

  //login/create user vars
  Wt::WLineEdit *emailEdit_;
  Wt::WLineEdit *firstNameEdit_;
  Wt::WLineEdit *lastNameEdit_;
  Wt::WLineEdit *passNew_;
  Wt::WLineEdit *passOld_;
  Wt::WText *changeOutput_;
  Wt::WText *changeOutputPass_;
  Wt::WText *header_;
  Wt::WText *loginText_;
  Wt::WText *emailLoginText_;
  Wt::WText *passwordLoginText_;
  Wt::WText *createText_;
  Wt::WText *firstname_;
  Wt::WText *lastname_;
  Wt::WText *createUpdateText_;
  Wt::WText *bridgeOutput_;

  Wt::WLineEdit *emailLogin_;
  Wt::WLineEdit *passwordLogin_;
  Wt::WLineEdit *emailCreate_;
  Wt::WLineEdit *passwordCreate_;
  Wt::WLineEdit *firstCreate_;
  Wt::WLineEdit *lastCreate_;


  Wt::WPushButton *loginBtn;
  Wt::WPushButton *createBtn;

  User *userTemp;
  User *currentUser;

  bool bridgeFlag;
  Wt::WText *bridgeStatus;

  std::vector<Bridge *> bridges;
  Bridge *currentBridge;

  void createUser();
  void validateLogin();
  void update();
  void updatePassword();
  void loadBridge();
  void deleteAccount();

  void handleHttpResponseLights(boost::system::error_code err, const Wt::Http::Message &response);
  void handleHttpResponsePrediction(boost::system::error_code err, const Wt::Http::Message &response);
  void handleHttpResponsePredictionOff(boost::system::error_code err, const Wt::Http::Message &response);
  void handleHttpResponseGetSchedule(boost::system::error_code err, const Wt::Http::Message &response);
  void handleHttpResponseDeleteSchedule(boost::system::error_code err, const Wt::Http::Message &response);

  void handleLightsInGroups(boost::system::error_code err, const Wt::Http::Message &response,int group_index);
  void handleHttpResponseGroups(boost::system::error_code err, const Wt::Http::Message &response);

  void handleHttpResponseAddBridge(boost::system::error_code err, const Wt::Http::Message &response);

  void toggleLogin();
  void toggleSettings();
  void toggleHomePage();

  void updateBridgeText();
  void handleCreateBridge(); 

  void handlePrediction();
  void handlePredictionOff();
  void handleScheduleDelete();
  void getAndDisplaySchedule();
  void handleScheduleCreateLight();
  void handleScheduleCreateGroup();
  void handleScheduleCreate(int group);
  void toggleThread();

public:
  HomePage(const Wt::WEnvironment &env);

  int getNumLights();
  int getNumGroups();

  void changeLightName(std::string,int);
  void changeLightHue(std::string,int);
  void changeLightBrightness(std::string,int);
  void changeGroupName(std::string,int);
  void changeGroupHue(std::string, int);
  void changeGroupBrightness(std::string, int);

  void addLightToGroup(std::string str);
  void removeLightFromGroup(std::string str);
  void storeLightData();

  void addGroup();
  void removeGroup();

  Bridge *getCurrentBridge();

  Light* getLightWithID(std::string id);

};

#endif

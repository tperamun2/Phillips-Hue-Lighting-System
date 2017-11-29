/**
 * @brief the homepage class represents the mission control for the hue lights system
 * @author Gurkiran Tatla
 * @author Jake Schindler
 * @author Justine Kim
 * @author Paul Salvatore
 * @author Timal Peramune
 */

#include "HomePage.hpp"

/**
 * HomePage
 * @brief constructor for controlling bridges and user settings
 *
 * @param env: an envirnmental variable
 * @return no return value
 */
HomePage::HomePage(const Wt::WEnvironment &env) : Wt::WApplication(env) {

  currentBridge = new Bridge("a", "b", "http://localhost", "8001");

  Wt::WApplication::setTitle("hUe");
  Wt::WIOService *ioservice = new Wt::WIOService();
  Wt::WIOService *ioservice2 = new Wt::WIOService();

  ioservice->start();
  ioservice2->start();

  Wt::Http::Client *client = new Wt::Http::Client(*ioservice);
  Wt::Http::Client *client2 = new Wt::Http::Client(*ioservice2);


  client->done().connect(
      boost::bind(&HomePage::handleHttpResponseLights, this, _1, _2));

  client->get(currentBridge->getIP() + ":" + currentBridge->getPort() + "/api/newdeveloper/lights");


  client2->done().connect(
      boost::bind(&HomePage::handleHttpResponseGroups, this, _1, _2));

  client2->get(currentBridge->getIP() + ":" + currentBridge->getPort() + "/api/newdeveloper/groups");


  ioservice->stop();
  ioservice2->stop();



  //TOGGLE BUTTONS

  //create containers
  containerLogin = new Wt::WContainerWidget(root());
  containerSettings = new Wt::WContainerWidget(root());
  containerHomePage = new Wt::WContainerWidget(root());
  containerHomePage->hide();
  containerSettings->hide();


  HomePageContainerBtn = new Wt::WPushButton("Go to HomePage", containerSettings);
  HomePageContainerBtn->clicked().connect(this, &HomePage::toggleHomePage);

  loginContainerBtn = new Wt::WPushButton("Logout", containerSettings);
  loginContainerBtn->clicked().connect(this, &HomePage::toggleLogin);

  //create temp (blank) user to compile
  userTemp = new User();
  currentUser=userTemp;
  
  //////////////////////////CONTAINERLOGIN
  //CREATE USER
  createText_ = new Wt::WText(root());
  createText_->setText("CREATE USER");
  containerLogin->addWidget(createText_);
  containerLogin->addWidget(new Wt::WBreak());


  firstCreate_ = new Wt::WLineEdit(root());
  firstname_ = new Wt::WText(root());
  firstname_->setText("firstname");
  lastCreate_ = new Wt::WLineEdit(root());
  lastname_ = new Wt::WText(root());
  lastname_->setText("lastname");

  containerLogin->addWidget(firstCreate_);
  containerLogin->addWidget(firstname_);
  containerLogin->addWidget(lastCreate_);
  containerLogin->addWidget(lastname_);

  emailCreate_ = new Wt::WLineEdit(root());
  emailLoginText_ = new Wt::WText(root());
  emailLoginText_->setText("email");
  containerLogin->addWidget(emailCreate_);
  containerLogin->addWidget(emailLoginText_);
  containerLogin->addWidget(new Wt::WBreak());

  passwordCreate_ = new Wt::WLineEdit(root());
  passwordLoginText_ = new Wt::WText(root());
  passwordLoginText_->setText("password");
  containerLogin->addWidget(passwordCreate_);
  containerLogin->addWidget(passwordLoginText_);
  containerLogin->addWidget(new Wt::WBreak());

  createBtn = new Wt::WPushButton("Create User", root());
  createBtn->clicked().connect(this, &HomePage::createUser);
  containerLogin->addWidget(createBtn);

  containerLogin->addWidget(new Wt::WBreak());
  createUpdateText_ = new Wt::WText(root());
  containerLogin->addWidget(createUpdateText_);
  containerLogin->addWidget(new Wt::WBreak());


  //LOGIN USER
  loginText_ = new Wt::WText(root());
  loginText_->setText("LOGIN");
  containerLogin->addWidget(loginText_);
  containerLogin->addWidget(new Wt::WBreak());
  emailLogin_ = new Wt::WLineEdit(root());
  emailLoginText_ = new Wt::WText(root());
  emailLoginText_->setText("email");
  containerLogin->addWidget(emailLogin_);
  containerLogin->addWidget(emailLoginText_);
  containerLogin->addWidget(new Wt::WBreak());
  passwordLogin_ = new Wt::WLineEdit(root());
  passwordLoginText_ = new Wt::WText(root());
  passwordLoginText_->setText("password");
  containerLogin->addWidget(passwordLogin_);
  containerLogin->addWidget(passwordLoginText_);
  containerLogin->addWidget(new Wt::WBreak());

  loginBtn = new Wt::WPushButton("Log in", root());
  loginBtn->clicked().connect(this, &HomePage::validateLogin);
  containerLogin->addWidget(loginBtn);
  //CONTAINERLOGIN//////////////////////////  

  //////////////////////////CONTAINERSETTINGS
  containerSettings->addWidget(new Wt::WBreak());
  header_ = new Wt::WText("Hello " + currentUser->getFirstName());
  containerSettings->addWidget(header_);
  containerSettings->addWidget(new Wt::WBreak());

  emailEdit_ = new Wt::WLineEdit(root());
  containerSettings->addWidget(emailEdit_);
  containerSettings->addWidget(new Wt::WText("  new email"));
  emailEdit_ -> setFocus();

  containerSettings->addWidget(new Wt::WBreak());

  firstNameEdit_ = new Wt::WLineEdit(root());
  containerSettings->addWidget(firstNameEdit_);
  containerSettings->addWidget(new Wt::WText("  new first name"));

  containerSettings->addWidget(new Wt::WBreak());

  lastNameEdit_ = new Wt::WLineEdit(root());
  containerSettings->addWidget(lastNameEdit_);
  containerSettings->addWidget(new Wt::WText("  new last name"));

  containerSettings->addWidget(new Wt::WBreak());

  Wt::WPushButton *btnSubmit = new Wt::WPushButton("Submit", root());     //Submit Button
  btnSubmit->setMargin(5, Wt::Left);
  containerSettings->addWidget(btnSubmit);

  containerSettings->addWidget(new Wt::WBreak());                               //Line Break

  changeOutput_ = new Wt::WText(root());
  containerSettings->addWidget(changeOutput_);

  containerSettings->addWidget(new Wt::WBreak());


  btnSubmit->clicked().connect(this, &HomePage::update);


  emailEdit_->enterPressed().connect(boost::bind(&HomePage::update, this));

  containerSettings->addWidget(new Wt::WBreak());

  /*
   * CHANGE PASSWORD
   */
  Wt::WText *passHeader_ = new Wt::WText("Change Password");
  containerSettings->addWidget(passHeader_);
  containerSettings->addWidget(new Wt::WBreak());


  passOld_ = new Wt::WLineEdit(root());
  passOld_->setEchoMode(Wt::WLineEdit::Password);
  containerSettings->addWidget(passOld_);
  containerSettings->addWidget(new Wt::WText("  enter old password"));
  containerSettings->addWidget(new Wt::WBreak());
  passNew_ = new Wt::WLineEdit(root());
  passNew_->setEchoMode(Wt::WLineEdit::Password);
  containerSettings->addWidget(passNew_);
  containerSettings->addWidget(new Wt::WText("  enter new password"));
  containerSettings->addWidget(new Wt::WBreak());

  Wt::WPushButton *btnPassSubmit = new Wt::WPushButton("Change Password", root());
  btnPassSubmit->setMargin(5, Wt::Left);
  containerSettings->addWidget(btnPassSubmit);


  containerSettings->addWidget(new Wt::WBreak());                                //Line Break
  changeOutputPass_ = new Wt::WText(root());
  containerSettings->addWidget(changeOutputPass_);

  //password button click
  btnPassSubmit->clicked().connect(this, &HomePage::updatePassword);
  containerSettings->addWidget(btnPassSubmit);

  containerSettings->addWidget(new Wt::WBreak());

  DeleteAccountButton = new Wt::WPushButton("DELETE ACCOUNT", containerSettings);
  DeleteAccountButton->clicked().connect(this, &HomePage::deleteAccount);

  containerSettings->addWidget(new Wt::WBreak());



  // BRIDGE ADDING
  bridgeName = new Wt::WLineEdit();
  bridgePlace = new Wt::WLineEdit();
  bridgeIPAddress = new Wt::WLineEdit();
  bridgePort = new Wt::WLineEdit();


  containerSettings->addWidget(new Wt::WBreak());
  containerSettings->addWidget(new Wt::WBreak());
  containerSettings->addWidget(new Wt::WBreak());
  containerSettings->addWidget(bridgeName);
  containerSettings->addWidget(new Wt::WText("  Bridge Name"));
  containerSettings->addWidget(new Wt::WBreak());
  containerSettings->addWidget(bridgePlace);
  containerSettings->addWidget(new Wt::WText("  Bridge Place"));
  containerSettings->addWidget(new Wt::WBreak());
  containerSettings->addWidget(bridgeIPAddress);
  containerSettings->addWidget(new Wt::WText("  Bridge IP Address"));
  containerSettings->addWidget(new Wt::WBreak());
  containerSettings->addWidget(bridgePort);
  containerSettings->addWidget(new Wt::WText("  Bridge Port"));
  containerSettings->addWidget(new Wt::WBreak());
  containerSettings->addWidget(new Wt::WBreak());

  
  Wt::WPushButton *createBridge = new Wt::WPushButton("Add Bridge", containerSettings);
  createBridge->clicked().connect(this, &HomePage::handleCreateBridge);
  containerSettings->addWidget(createBridge);
  containerSettings->addWidget(new Wt::WBreak());
  containerSettings->addWidget(new Wt::WBreak());
  bridgeStatus = new Wt::WText();
  containerSettings->addWidget(bridgeStatus);
  containerSettings->addWidget(new Wt::WBreak());

  bridgeOutput_ = new Wt::WText(containerSettings);
  containerSettings->addWidget(bridgeOutput_);








  containerSettings->addWidget(new Wt::WBreak());

  bridgeSelect = new Wt::WLineEdit();
  containerSettings->addWidget(bridgeSelect);
  containerSettings->addWidget(new Wt::WText(" Please select a bridge number (if you just added your first bridge, select 1)"));





}



/**
 * @brief a function that is controlling the UI of the homepage allowing for all Hue light functionality
 */
void HomePage::loadBridge(){
    //////HOMEPAGE
  containerHomePage = new Wt::WContainerWidget(root());
  containerHomePage->addWidget(new Wt::WText("<br/><br/>Hue Mission Control.<br/><br/><br/><br/>"));
  containerHomePage->show();

  SettingsContainerBtn = new Wt::WPushButton("User Settings", containerHomePage);
  SettingsContainerBtn->clicked().connect(this, &HomePage::toggleSettings);
    loginContainerBtn = new Wt::WPushButton("Logout", containerHomePage);
  loginContainerBtn->clicked().connect(this, &HomePage::toggleLogin);
  containerHomePage->addWidget(new Wt::WBreak());
  containerHomePage->addWidget(new Wt::WBreak());

  // SCHEDULE BUTTON + DISPLAY
  deleteMessage = "";

  Wt::WPushButton *createScheduleLight = new Wt::WPushButton("Create Schedule (Light).", containerHomePage);
  createScheduleLight->clicked().connect(this, &HomePage::handleScheduleCreateLight);

  Wt::WPushButton *createScheduleGroup = new Wt::WPushButton("Create Schedule (Group).", containerHomePage);
  createScheduleGroup->clicked().connect(this, &HomePage::handleScheduleCreateGroup);

  createScheduleId = new Wt::WLineEdit();

  containerHomePage->addWidget(createScheduleId);
  containerHomePage->addWidget(new Wt::WBreak());
  containerHomePage->addWidget(createScheduleLight);
  containerHomePage->addWidget(createScheduleGroup);
  containerHomePage->addWidget(new Wt::WBreak());


  // INFO FOR SCHEDULING
  scheduleErrorText = new Wt::WText();
  scheduleYear = new Wt::WLineEdit();
  scheduleMonth = new Wt::WLineEdit();
  scheduleDay = new Wt::WLineEdit();
  scheduleHour = new Wt::WLineEdit();
  scheduleMin = new Wt::WLineEdit();
  scheduleOn = new Wt::WLineEdit();
  scheduleTransition = new Wt::WLineEdit();

  containerHomePage->addWidget(scheduleYear);
  containerHomePage->addWidget(new Wt::WText("  Schedule Year"));
  containerHomePage->addWidget(new Wt::WBreak());
  containerHomePage->addWidget(scheduleMonth);
  containerHomePage->addWidget(new Wt::WText("  Schedule Month"));
  containerHomePage->addWidget(new Wt::WBreak());
  containerHomePage->addWidget(scheduleDay);
  containerHomePage->addWidget(new Wt::WText("  Schedule Day"));
  containerHomePage->addWidget(new Wt::WBreak());
  containerHomePage->addWidget(scheduleHour);
  containerHomePage->addWidget(new Wt::WText("  Schedule Hour"));
  containerHomePage->addWidget(new Wt::WBreak());
  containerHomePage->addWidget(scheduleMin);
  containerHomePage->addWidget(new Wt::WText("  Schedule Minute"));
  containerHomePage->addWidget(new Wt::WBreak());
  containerHomePage->addWidget(scheduleOn);
  containerHomePage->addWidget(new Wt::WText("  Schedule State (1 = On, 0 = Off)"));
  containerHomePage->addWidget(new Wt::WBreak());
  containerHomePage->addWidget(scheduleTransition);
  containerHomePage->addWidget(new Wt::WText("  Schedule Transition (seconds)"));
  containerHomePage->addWidget(new Wt::WBreak());


  scheduleHue = new Wt::WSlider(Wt::Horizontal);
  scheduleBrightness = new Wt::WSlider(Wt::Horizontal);

  scheduleHue->setMinimum(0);
  scheduleHue->setMaximum(46920);
  scheduleHue->setValue(20000);
  scheduleHue->setTickInterval(10000);
  scheduleHue->setTickPosition(Wt::WSlider::TicksAbove);
  scheduleHue->resize(500, 50);

  scheduleBrightness->setMinimum(0);
  scheduleBrightness->setMaximum(254);
  scheduleBrightness->setValue(127);
  scheduleBrightness->setTickInterval(20);
  scheduleBrightness->setTickPosition(Wt::WSlider::TicksAbove);
  scheduleBrightness->resize(500, 50);
  containerHomePage->addWidget(new Wt::WText("Adjust Schedule Hue:<br/>"));
  containerHomePage->addWidget(scheduleHue);
  containerHomePage->addWidget(new Wt::WBreak());
  containerHomePage->addWidget(new Wt::WText("Adjust Schedule Brightness:<br/>"));
  containerHomePage->addWidget(scheduleBrightness);
  containerHomePage->addWidget(new Wt::WBreak());


  containerHomePage->addWidget(new Wt::WBreak());
  containerHomePage->addWidget(new Wt::WBreak());
  scheduleErrorText = new Wt::WText();
  containerHomePage->addWidget(scheduleErrorText);
  containerHomePage->addWidget(new Wt::WBreak());
  containerHomePage->addWidget(new Wt::WBreak());


  Wt::WPushButton *deleteSchedule = new Wt::WPushButton("Delete Schedule.", containerHomePage);
  deleteSchedule->clicked().connect(this, &HomePage::handleScheduleDelete);

  deleteScheduleId = new Wt::WLineEdit();

  containerHomePage->addWidget(deleteScheduleId);
  containerHomePage->addWidget(new Wt::WBreak());

  scheduleText = new Wt::WText();
  HomePage::getAndDisplaySchedule();

  containerHomePage->addWidget(new Wt::WBreak());
  containerHomePage->addWidget(scheduleText);


  // PREDICTION BUTTON + DISPLAY



  containerHomePage->addWidget(new Wt::WBreak());
  predictionRecording = new Wt::WPushButton("Toggle predictive recording : ON", containerHomePage);
  predictionRecording->clicked().connect(this, &HomePage::toggleThread);
  containerHomePage->addWidget(predictionRecording);
  containerHomePage->addWidget(new Wt::WBreak());

  Wt::WPushButton *prediction = new Wt::WPushButton("Schedule 48-hour predictions.", containerHomePage);

  prediction->clicked().connect(this, &HomePage::handlePrediction);

  Wt::WPushButton *predictionOff = new Wt::WPushButton("Remove predicted schedules.", containerHomePage);
  predictionOff->clicked().connect(this, &HomePage::handlePredictionOff);

  predictionText = new Wt::WText();

  containerHomePage->addWidget(new Wt::WBreak());
  containerHomePage->addWidget(predictionText);
  containerHomePage->addWidget(new Wt::WBreak());
  containerHomePage->addWidget(new Wt::WBreak());


  containerHomePage->addWidget(new Wt::WText("<br /><br /><br /><h3><b>Your groups:</b></h3><br /><br />"));

  for (auto itr = this->groups.begin(); itr != this->groups.end(); ++itr) {

    std::cout << (*itr)->getName() << std::endl;

    std::string str = "<h3><b><i>" + (*itr)->getName() + "</i></b></h3><br />";
    containerHomePage->addWidget(new Wt::WText(str));

    std::string *light_names = (*itr)->getAllLightNames();


    this->groupHueSliders.push_back(new Wt::WSlider(Wt::Horizontal));
    this->groupBriSliders.push_back(new Wt::WSlider(Wt::Horizontal));

    containerHomePage->addWidget(new Wt::WText(*light_names));
    containerHomePage->addWidget(new Wt::WBreak());
    containerHomePage->addWidget(new Wt::WBreak());


  }




for (int i = 0; i < this->lights.size(); i++) {

    containerHomePage->addWidget(this->lights[i]);
    this->lights[i]->setMargin(5, Wt::Right);

    this->edits.push_back(new Wt::WLineEdit(
        "Edit light name and press 'enter'. Refresh to see changes"));
    this->edits[i]->setTextSize(50);
    containerHomePage->addWidget(this->edits[i]);
    this->edits[i]->setMargin(5, Wt::Right);

    this->hueSliders.push_back(new Wt::WSlider(Wt::Horizontal));
    this->hueSliders[i]->setMinimum(0);
    this->hueSliders[i]->setMaximum(46920);
    this->hueSliders[i]->setValue(20000);
    this->hueSliders[i]->setTickInterval(10000);
    this->hueSliders[i]->setTickPosition(Wt::WSlider::TicksAbove);
    this->hueSliders[i]->resize(500, 50);
    this->brightnessSliders.push_back(new Wt::WSlider(Wt::Horizontal));
    this->brightnessSliders[i]->setMinimum(0);
    this->brightnessSliders[i]->setMaximum(254);
    this->brightnessSliders[i]->setValue(127);

    this->brightnessSliders[i]->setTickInterval(20);
    this->brightnessSliders[i]->setTickPosition(Wt::WSlider::TicksAbove);
    this->brightnessSliders[i]->resize(500, 50);
    containerHomePage->addWidget(new Wt::WBreak());
    containerHomePage->addWidget(new Wt::WText("Adjust Hue: "));
    containerHomePage->addWidget(new Wt::WBreak());

    containerHomePage->addWidget(new Wt::WBreak());
    containerHomePage->addWidget(this->hueSliders[i]);
    this->hueSliders[i]->setMargin(10, Wt::Right);

    containerHomePage->addWidget(new Wt::WBreak());
    containerHomePage->addWidget(new Wt::WBreak());

    containerHomePage->addWidget(new Wt::WText("Adjust Brightness: "));
    containerHomePage->addWidget(new Wt::WBreak());

    containerHomePage->addWidget(this->brightnessSliders[i]);

    containerHomePage->addWidget(new Wt::WBreak());
    containerHomePage->addWidget(new Wt::WBreak());
    containerHomePage->addWidget(new Wt::WBreak());
  }

  Wt::WPushButton *addGroupButton = new Wt::WPushButton("Add Group", containerHomePage);
  this->removeGroupEdit = new Wt::WLineEdit(
      "Enter group name to delete --(Refresh to see changes)");
  this->removeGroupEdit->setTextSize(50);
  addGroupButton->setMargin(40, Wt::Right);
  containerHomePage->addWidget(this->removeGroupEdit);
  containerHomePage->addWidget(new Wt::WBreak());
  containerHomePage->addWidget(new Wt::WBreak());
  addGroupButton->clicked().connect(this, &HomePage::addGroup);

  this->removeGroupEdit->enterPressed().connect(this, &HomePage::removeGroup);

  Wt::WText *addLightText =
      new Wt::WText("Add Light (Enter name of light to add followed by the | "
                    "character. Enter Group name after the |)");
  addLightText->setMargin(60, Wt::Right);
  containerHomePage->addWidget(addLightText);

  containerHomePage->addWidget(new Wt::WBreak());

  this->addLightEdit = new Wt::WLineEdit("Enter light name | Enter Group name");
  this->addLightEdit->setMargin(20, Wt::Right);

  this->addLightEdit->enterPressed().connect(boost::bind(
      &HomePage::addLightToGroup, this, addLightEdit->text().toUTF8()));

  this->addLightEdit->setTextSize(50);
  containerHomePage->addWidget(addLightEdit);
  containerHomePage->addWidget(new Wt::WBreak());
  containerHomePage->addWidget(new Wt::WBreak());

  Wt::WText *removeLightText =
      new Wt::WText("Remove Light (Enter name of light to add followed by the "
                    "| character. Enter Group name after the |)");
  removeLightText->setMargin(60, Wt::Right);
  containerHomePage->addWidget(removeLightText);

  containerHomePage->addWidget(new Wt::WBreak());

  this->removeLightEdit =
      new Wt::WLineEdit("Enter light name | Enter Group name");
  this->removeLightEdit->setMargin(20, Wt::Right);

  this->addLightEdit->enterPressed().connect(boost::bind(&HomePage::removeLightFromGroup, this, removeLightEdit->text().toUTF8()));

  this->removeLightEdit->setTextSize(50);

  containerHomePage->addWidget(removeLightEdit);

  containerHomePage->addWidget(new Wt::WBreak());
  containerHomePage->addWidget(new Wt::WBreak());

  for (int i = 0; i < this->groups.size(); i++) {
    containerHomePage->addWidget(this->groups[i]);

    this->groupHueSliders[i]->setMinimum(0);
    this->groupHueSliders[i]->setMaximum(46920);
    this->groupHueSliders[i]->setValue(20000);
    this->groupHueSliders[i]->setTickInterval(10000);
    this->groupHueSliders[i]->setTickPosition(Wt::WSlider::TicksAbove);
    this->groupHueSliders[i]->resize(500, 50);

    this->groupBriSliders[i]->setMinimum(0);
    this->groupBriSliders[i]->setMaximum(254);
    this->groupBriSliders[i]->setValue(127);

    this->groupBriSliders[i]->setTickInterval(20);
    this->groupBriSliders[i]->setTickPosition(Wt::WSlider::TicksAbove);
    this->groupBriSliders[i]->resize(500, 50);

    this->groupEdits.push_back(new Wt::WLineEdit(
        "Edit group name and press 'enter'. Refresh to see changes"));

    this->groupEdits[i]->setTextSize(50);
    this->groupEdits[i]->setMargin(5, Wt::Left);

    containerHomePage->addWidget(this->groupEdits[i]);


    containerHomePage->addWidget(new Wt::WBreak());

    containerHomePage->addWidget(new Wt::WText("Adjust Hue for Group: "));
    containerHomePage->addWidget(new Wt::WBreak());
    containerHomePage->addWidget(this->groupHueSliders[i]);

    containerHomePage->addWidget(new Wt::WBreak());

    containerHomePage->addWidget(new Wt::WText("Adjust Brightness for Group: "));
    containerHomePage->addWidget(new Wt::WBreak());
    containerHomePage->addWidget(this->groupBriSliders[i]);
    containerHomePage->addWidget(new Wt::WBreak());

  }

  // For editing the name of a group
  for (int i = 0; i < this->groupEdits.size(); i++) {
    this->groupEdits[i]->enterPressed().connect(
        boost::bind(&HomePage::changeGroupName, this,
                    this->groupEdits[i]->text().toUTF8(), i));
  }

  //For group Hue slider
  for(int i = 0; i < this->groupHueSliders.size() ; i++){

    this->groupHueSliders[i]->valueChanged().connect(boost::bind(&HomePage::changeGroupHue, this,
                this->groupHueSliders[i]->valueText().toUTF8(), i));

  }

  //for group Brightness Slider
  for(int i =0; i < this->groupBriSliders.size(); i++){
    this->groupBriSliders[i]->valueChanged().connect(boost::bind(&HomePage::changeGroupBrightness, this,
                this->groupBriSliders[i]->valueText().toUTF8(), i));
  }

  // For Hue Slider
  for (int i = 0; i < this->hueSliders.size(); i++) {
    this->hueSliders[i]->valueChanged().connect(
        boost::bind(&HomePage::changeLightHue, this,
                    this->hueSliders[i]->valueText().toUTF8(), i));
  }
  // for Brightness slider
  for (int i = 0; i < this->brightnessSliders.size(); i++) {
    this->brightnessSliders[i]->valueChanged().connect(
        boost::bind(&HomePage::changeLightBrightness, this,
                    this->brightnessSliders[i]->valueText().toUTF8(), i));
  }

  // For editing the name of a light
  for (int i = 0; i < this->edits.size(); i++) {
    this->edits[i]->enterPressed().connect(boost::bind(
        &HomePage::changeLightName, this, this->edits[i]->text().toUTF8(), i));
  }

  // for turning a light on/off
  for (int i = 0; i < this->lights.size(); i++) {
    this->lights[i]->clicked().connect(this->lights[i], &Light::turnONorOFF);
  }

  storeLightData();
}

/**
 * HomePage::handleHttpResponseAddBridge
 * @brief method for handling the adding of a bridge
 *
 * @param args: boost::system::error_code err, const Wt::Http::Message &response
 * @return void*: returns void
 */
void HomePage::handleHttpResponseAddBridge(boost::system::error_code err, const Wt::Http::Message &response){
  std::cout << response.body() << std::endl;
  if(response.body() != ""){
    bridgeFlag = true;
  }
}

Bridge *HomePage::getCurrentBridge(){
  return this->currentBridge;
}

/**
 * HomePage::handleHttpResponseAddBridge
 * @brief method for handling the creation of a bridge
 *
 * @param args: none
 * @return void*: returns void
 */
void HomePage::handleCreateBridge(){

  // set initially to false incase of malformed destination
  bridgeFlag = false;

  Wt::WIOService *ioservice = new Wt::WIOService();
  ioservice->start();
  Wt::Http::Client *client = new Wt::Http::Client(*ioservice);
  client->done().connect(boost::bind(&HomePage::handleHttpResponseAddBridge, this, _1, _2));

  std::string address = (bridgeIPAddress->text()).toUTF8() + ":" + (bridgePort->text()).toUTF8() + "/api/newdeveloper/lights";
  std::cout << address << std::endl;

  client->get(address);

  
  std::future<int> future = std::async(std::launch::async, [](){
  std::this_thread::sleep_for(std::chrono::seconds(2));
    return 8;
  });

  std::cout << "loading, please wait\n";
  std::chrono::milliseconds span (100);
  while (future.wait_for(span) == std::future_status::timeout){
    std::cout << '.';
  }

  ioservice->stop();
  delete client;
  delete ioservice;


  std::string bName = (bridgeName->text()).toUTF8();
  std::string bPlace = (bridgePlace->text()).toUTF8();
  if(!bridgeFlag || bName == "" || bPlace == ""){
    bridgeStatus->setText("Invalid bridge, please ensure all information is correct. (ips must be preceeded by https://)");
  } else {
    bridgeStatus->setText("Bridge added");

    std::string writeFile = currentUser->getEmail() + "Bridges.txt";
    std::ofstream out;
    out.open(writeFile, std::ios::app);


    std::string toWrite = "";
    toWrite += bName + " ";
    toWrite += bPlace + " ";
    toWrite += (bridgeIPAddress->text()).toUTF8() + " ";
    toWrite += (bridgePort->text()).toUTF8() + "\n";

    out << toWrite;
    HomePage::updateBridgeText();
    out.close();
  }

}



/**
 * @brief a struct to pass args to a new thread
 */
struct args{
  std::vector<Light *> lights;
};

/**
 * do_work
 * @brief method for creating a thread to store light data
 *
 * @param args: void * representing the arguments passed to the thread
 * @return void*: returns void
 */
void *do_work(void *args){

  struct args *arguments = (struct args *)args;
  std::vector<Light *> lights = arguments->lights;

  // infinite loop
  while(true){

    // sleep for an hour
    std::future<int> future = std::async(std::launch::async, [](){
    std::this_thread::sleep_for(std::chrono::seconds(SLEEP_PERIOD));
      return 8;
    });

    std::chrono::milliseconds span (SLEEP_PERIOD);
    while (future.wait_for(span) == std::future_status::timeout){
    }

    // iterate through the lights
    for (auto itr = lights.begin(); itr != lights.end(); ++itr) {

      std::string light_id = (*itr)->getID();
      light_id = "lightData" + light_id + ".txt";

      if (!std::ifstream(light_id)) {
        std::ofstream file(light_id);
      }

      time_t rawtime;
      tm * timeinfo;
      time(&rawtime);
      timeinfo = localtime(&rawtime);
      int wday = timeinfo->tm_wday;
      int whour = timeinfo->tm_hour;

      int on = (*itr)->getState()->isON() ? 1 : 0;

      std::string toWrite = std::to_string(wday + 1) + " " + std::to_string(whour) + " " + std::to_string(on) + "\n";

      std::ofstream out;
      out.open(light_id, std::ios::app);
      out << toWrite;

      std::cout<< "Writing: " + toWrite <<std::endl;

      out.close();
    }
  }
}

/**
 * storeLightData
 * @brief method for creating a thread to store light data
 *
 * @param none
 * @return none
 */
void HomePage::storeLightData(){

  std::future<int> future = std::async(std::launch::async, [](){
    std::this_thread::sleep_for(std::chrono::seconds(5));
    return 8;
  });

  std::chrono::milliseconds span (SLEEP_PERIOD);
    while (future.wait_for(span) == std::future_status::timeout){
  }

  struct args arguments;
  arguments.lights = lights;

  threadRunning = true;
  if(pthread_create(&worker_thread, NULL, do_work, (void *)&arguments)){
    threadRunning = false;
    perror("pthread_create()");   
  }
}

/**
 * toggleThread
 * @brief turns on/off recording of light states for predictive mode
 *
 * @param none
 * @return none
 */
void HomePage::toggleThread(){
  if(threadRunning){
    pthread_cancel(worker_thread);
    threadRunning = false;
    predictionRecording->setText("Toggle predictive recording : OFF");
  } else {
    storeLightData();
    predictionRecording->setText("Toggle predictive recording : ON");
  }
}

/**
 * HomePage::toggleLogin, HomePage::toggleSettings, HomePage::toggleHomePage
 * @brief method for moving between pages
 *
 * @param args: none
 * @return void*: returns void
 */
void HomePage::toggleLogin(){
  containerHomePage->hide();
  containerSettings->hide();
  containerLogin->show();
}

void HomePage::toggleSettings(){
  containerHomePage->hide();
  containerSettings->show();
  HomePage::updateBridgeText();
}

void HomePage::toggleHomePage(){
  containerSettings->hide();
  HomePage::loadBridge();

}

void HomePage::deleteAccount(){
  std::string userFile = currentUser->getEmail() + ".txt";
  std::string userFilebr = currentUser->getEmail() + "Bridges.txt";

  std::ifstream file(userFile);

  if (file){
    remove(userFile.c_str());
    remove(userFilebr.c_str());
    HomePage::toggleLogin();
  }
}
/**
 * HomePage::updateBridgeText
 * @brief method for updating current bridges connected with a user
 *
 * @param args: none
 * @return void*: returns void
 */
void HomePage::updateBridgeText(){
    bridgeOutput_->setText("");
    std::string userFile = currentUser->getEmail() + "Bridges.txt";
    std::ifstream file(userFile);
    if (file) {    //this file already exists
    std::string currentLine;

    int i = 1;
    while (file>>std::ws && std::getline(file,currentLine)){
        bridgeOutput_->setText(bridgeOutput_->text() +std::to_string(i) + ". " + currentLine+"<br/>");
        
        i++;
    }
  }
}

/**
 * HomePage::updateBridgeText
 * @brief method to create a user, creates a textfile with user's info
 *
 * @param args: none
 * @return void*: returns void
 */
void HomePage::createUser() {
    std::string email = emailCreate_->text().toUTF8();
    std::string password = passwordCreate_->text().toUTF8();
    std::string firstName = firstCreate_->text().toUTF8();
    std::string lastName = lastCreate_->text().toUTF8();

    createUpdateText_->setText("");
    bool err = false;
    //check for filled in fields and change info
    //email
    if (email.compare("") != 0){
        //check if email has @
        std::size_t foundAt = email.find("@");
        if (foundAt!=std::string::npos){
            //check if "." is after the @
            std::size_t foundPer = email.find(".", foundAt+2);
            if (foundPer!=std::string::npos){
                ;
            }else{
                createUpdateText_->setText(createUpdateText_->text() + "Incorrect email format" + ", ");
                err = true;
            }
        }else{
            createUpdateText_->setText(createUpdateText_->text() + "Incorrect email format" + ", ");
            err = true;
        }
    }
    std::string userFile = email + ".txt";
    if (std::ifstream(userFile)){ //if user file exists
      createUpdateText_->setText(createUpdateText_->text() + "User with this email already exists" + ", ");
      err = true;
    }
    //firstname
    if (firstName.compare("") != 0){
        //check to see if less than 50 char
        if (firstName.length() < 50) {
            ;
        }else{
            createUpdateText_->setText(createUpdateText_->text() + "First name not set TOO LONG, ");
            err = true;
        }
    }else{
      err = true;
    }
    //lastname
    if (lastName.compare("") != 0){
        //check to see if less than 50 char
        if (lastName.length() < 50) {
            ;
        }else{
            createUpdateText_->setText(createUpdateText_->text() + "Last name not set TOO LONG, ");
            err = true;
        }
    }else{
      err = true;
    }

    if (err != true){
      User *newuser = new User();
      newuser->update(firstName, lastName, email, password);
      emailCreate_->setText("");
      passwordCreate_->setText("");
      firstCreate_->setText("");
      lastCreate_->setText("");

      createUpdateText_->setText("User Created!");
    }
}
/**
 * HomePage::validateLogin
 * @brief method for validating user's login credentials, sets the currentuser to 
    and object of this user if credentials are correct
 *
 * @param args: none
 * @return void*: returns void
 */
void HomePage::validateLogin() {
    std::string email = emailLogin_->text().toUTF8();
    std::string password = passwordLogin_->text().toUTF8();
    std::string hashPassword = Persistence::encryptDecrypt(password);
    std::string userFile = email + ".txt";
    if (std::ifstream(userFile)){ //if user file exists
      std::ifstream file(userFile);
      std::string currentLine;


      std::string details[] = {"", "", "", ""};
      int i = 0;
      while (file>>std::ws && std::getline(file,currentLine)){
          std::cout << currentLine <<std::endl;
          details[i++] = currentLine;
      }

      if (details[3] == hashPassword){
        User *user = new User();
        user->update(details[0],details[1],details[2], password);
        currentUser = user;
        header_->setText("Hello " + currentUser->getFirstName());
        containerLogin->hide();
        containerSettings->show();
        HomePage::updateBridgeText();
      }
    }
}

/**
 * HomePage::update
 * @brief method for updating a user's personal and account info
 *
 * @param args: none
 * @return void*: returns void
 */
void HomePage::update(){

    std::string email = emailEdit_->text().toUTF8();
    std::string firstName = firstNameEdit_->text().toUTF8();
    std::string lastName = lastNameEdit_->text().toUTF8();

    //reset output
    changeOutput_->setText("");

    //check for filled in fields and change info
    //email
    if (email.compare("") != 0){
        //check if email has @
        std::size_t foundAt = email.find("@");
        if (foundAt!=std::string::npos){
            //check if "." is after the @
            std::size_t foundPer = email.find(".", foundAt+2);
            if (foundPer!=std::string::npos){
                currentUser->setEmail(email);
                changeOutput_->setText(changeOutput_->text() + "Email set to " + currentUser->getEmail() + ", ");
            }else{
                changeOutput_->setText(changeOutput_->text() + "Incorrect email format" + ", ");
            }
        }else{
            changeOutput_->setText(changeOutput_->text() + "Incorrect email format" + ", ");
        }
    }
    //firstname
    if (firstName.compare("") != 0){
        //check to see if less than 50 char
        if (firstName.length() < 50) {
            currentUser->setFirstName(firstName);
            changeOutput_->setText(changeOutput_->text() + "First name set to " + currentUser->getFirstName() + ", ");
            header_->setText("Hello " + currentUser->getFirstName());
        }else{
            changeOutput_->setText(changeOutput_->text() + "First name not set TOO LONG, ");
        }
    }
    //lastname
    if (lastName.compare("") != 0){
        //check to see if less than 50 char
        if (lastName.length() < 50) {
            currentUser->setLastName(lastName);
            changeOutput_->setText(changeOutput_->text() + "Last name set to " + currentUser->getLastName() + ", ");
        }else{
            changeOutput_->setText(changeOutput_->text() + "Last name not set TOO LONG, ");
        }
    }

}

/**
 * HomePage::updatePassword
 * @brief method for updating a user's password
 *
 * @param args: none
 * @return void*: returns void
 */
void HomePage::updatePassword(){
    std::string oldPassword = passOld_->text().toUTF8();
    std::string newPassword = passNew_->text().toUTF8();

    //reset output
    changeOutputPass_->setText("");

    if (oldPassword.compare(currentUser->getPassword()) == 0){
        if (newPassword.length() > 0){
            currentUser->setPassword(newPassword);
            changeOutputPass_->setText("Password changed");
        }
    }else{
        changeOutputPass_->setText("ERROR: old password does not match");
    }
}







/**
 * addLightToGroup
 * @brief adds a light to an existing group
 *
 * @param str the name of the light to add to the group
 * @return none
 */
void HomePage::addLightToGroup(std::string str) {

  str = this->addLightEdit->text().toUTF8();
  std::string light_name = str.substr(0, str.find("|"));
  std::string group_name = str.substr(str.find("|") + 1, str.size() - 1);

  light_name.erase(light_name.begin(),
                   std::find_if(light_name.begin(), light_name.end(),
                                std::bind1st(std::not_equal_to<char>(), ' ')));
  light_name.erase(std::find_if(light_name.rbegin(), light_name.rend(),
                                std::bind1st(std::not_equal_to<char>(), ' '))
                       .base(),
                   light_name.end());

  group_name.erase(group_name.begin(),
                   std::find_if(group_name.begin(), group_name.end(),
                                std::bind1st(std::not_equal_to<char>(), ' ')));
  group_name.erase(std::find_if(group_name.rbegin(), group_name.rend(),
                                std::bind1st(std::not_equal_to<char>(), ' '))
                       .base(),
                   group_name.end());

  Wt::WIOService *ioservice = new Wt::WIOService();
  ioservice->start();
  Wt::Http::Client *client = new Wt::Http::Client(*ioservice);

  Wt::Http::Message message = Wt::Http::Message();
  message.addHeader("Content-type", "application/json");

  std::string light_id;
  std::string group_id;
  for (auto itr = this->groups.begin(); itr != this->groups.end(); ++itr) {
    if ((*itr)->getName() == group_name)
      group_id = (*itr)->getID();
  }

  for (auto itr = this->lights.begin(); itr != this->lights.end(); ++itr) {
    if ((*itr)->getName() == light_name)
      light_id = (*itr)->getID();
  }

  std::string body = "{name: " + group_name + "," + "lights: " + "[" + light_id + "]}";
  message.addBodyText(body);
  client->put(currentBridge->getIP() + ":" + currentBridge->getPort() + "/api/newdeveloper/groups/" + group_id,
              message);
  ioservice->stop();
  for (auto itr = this->lights.begin(); itr != this->lights.end(); ++itr) {

    if ((*itr)->getName() == light_name) {
      for (auto itr2 = this->groups.begin(); itr2 != this->groups.end(); ++itr2) {

        if ((*itr2)->getName() == group_name) {
          (*itr2)->addLight(*itr);
        }

      }
    }
  }
}

/**
 * removeLightFromGroup
 * @brief removes a light to an existing group
 *
 * @param str the name of the light to removes from the group
 * @return none
 */
void HomePage::removeLightFromGroup(std::string str) {

  str = this->addLightEdit->text().toUTF8();
  std::string light_name = str.substr(0, str.find("|"));
  std::string group_name = str.substr(str.find("|") + 1, str.size() - 1);

  light_name.erase(light_name.begin(),
                   std::find_if(light_name.begin(), light_name.end(),
                                std::bind1st(std::not_equal_to<char>(), ' ')));
  light_name.erase(std::find_if(light_name.rbegin(), light_name.rend(),
                                std::bind1st(std::not_equal_to<char>(), ' '))
                       .base(),
                   light_name.end());

  group_name.erase(group_name.begin(),
                   std::find_if(group_name.begin(), group_name.end(),
                                std::bind1st(std::not_equal_to<char>(), ' ')));
  group_name.erase(std::find_if(group_name.rbegin(), group_name.rend(),
                                std::bind1st(std::not_equal_to<char>(), ' '))
                       .base(),
                   group_name.end());

  for (auto itr = this->lights.begin(); itr != this->lights.end(); ++itr) {

    if ((*itr)->getName() == light_name) {
      for (auto itr2 = this->groups.begin(); itr2 != this->groups.end();
           ++itr2) {

        if ((*itr2)->getName() == group_name) {
          (*itr2)->removeLight(*itr);
        }
      }
    }
  }
}

/**
 * removeGroup
 * @brief deletes a specified group based on a text input field
 *
 * @params none
 * @return none
 */
void HomePage::removeGroup(){

  std::string group_name = this->removeGroupEdit->text().toUTF8();

  for(auto itr = this->groups.begin(); itr != this->groups.end(); ++itr){

    if((*itr)->getName() == group_name){

      Wt::WIOService *ioservice = new Wt::WIOService();
      ioservice->start();
      Wt::Http::Client *client = new Wt::Http::Client(*ioservice);

      Wt::Http::Message message = Wt::Http::Message();
      message.addHeader("Content-type", "application/json");
      // message.addBodyText("{\"name\": " + name + "}");

      client->deleteRequest(currentBridge->getIP() + ":" + currentBridge->getPort() + "/api/newdeveloper/groups/" + (*itr)->getID(),
                  message);

      ioservice->stop();
      // this->name = name;
    }
  }
}

/**
 * addGroup
 * @brief adds a specified group based on a text input field
 *
 * @params none
 * @return none
 */
void HomePage::addGroup() {

  std::string group_name = "Group " + std::to_string(this->groups.size() + 1);
  Wt::WIOService *ioservice = new Wt::WIOService();
  ioservice->start();
  Wt::Http::Client *client = new Wt::Http::Client(*ioservice);

  Wt::Http::Message message = Wt::Http::Message();
  message.addHeader("Content-type", "application/json");
  message.addBodyText("{\"name\": " + group_name + "}");

  client->post(currentBridge->getIP() + ":" + currentBridge->getPort() + "/api/newdeveloper/groups/", message);

  ioservice->stop();

  this->groups.push_back(new LightGroup(std::to_string(this->groups.size() + 1),
                                        group_name, this, currentBridge->getIP(), currentBridge->getPort()));
  this->groupEdits.push_back(new Wt::WLineEdit(
      "Edit group name and press 'enter'. Refresh to see changes"));
  this->groupEdits[this->groupEdits.size() - 1]->setTextSize(50);
  this->groupEdits[this->groupEdits.size() - 1]->setMargin(5, Wt::Right);
  containerHomePage->addWidget(this->groups[this->groups.size() - 1]);
  containerHomePage->addWidget(
      this->groupEdits[this->groupEdits.size() - 1]);
  containerHomePage->addWidget(new Wt::WBreak());

}


/**
 * changeGroupName
 * @brief changes the name of an existing group
 *
 * @param name the name we want to change the group to
 * @param i the index of the group we are renaming
 */
void HomePage::changeGroupName(std::string name, int i) {
  this->groups[i]->setName(this->groupEdits[i]->text().toUTF8(), currentBridge->getIP(), currentBridge->getPort());
}

void HomePage::handleHttpResponseGroups(boost::system::error_code err,
                                        const Wt::Http::Message &response) {

  Wt::Json::Object result;
  Wt::Json::parse(response.body(), result);

  for (int i = 0; i < result.size(); i++) {
    Wt::Json::Object jsonObj1 = result.get(std::to_string(i + 1));
    Wt::WString groupName = jsonObj1.get("name");

    LightGroup *group =
        new LightGroup(std::to_string(i + 1), groupName.toUTF8(), this, currentBridge->getIP(), currentBridge->getPort());
    this->groups.push_back(group);

    Wt::WIOService *ioservice = new Wt::WIOService();
    ioservice->start();
    Wt::Http::Client *client = new Wt::Http::Client(*ioservice);

    std::string group_id = this->groups[i]->getID();
    client->done().connect(
        boost::bind(&HomePage::handleLightsInGroups, this, _1, _2, i));

    client->get(currentBridge->getIP() + ":" + currentBridge->getPort() + "/api/newdeveloper/groups/" + group_id);
    ioservice->start();
  }
}

/**
 * getLightWithID
 * @brief get a light with a specific id
 *
 * @param id a string representing the id of a light to get
 * @return null if the light does not exist, otherwise returns the light pointer object
 */
Light * HomePage::getLightWithID(std::string id){

  for(auto itr = this->lights.begin() ; itr != this->lights.end(); ++itr){

    if((*itr)->getID() == id){
      return *itr;
    }

  }

  return nullptr;


}

/**
 * handleLightsInGroups
 * @brief adds all lights to their groups
 *
 * @param err a boost err variable
 * @param response a Wt http message response variable
 * @param group_index the integer index of the group we are adding to
 * @return none
 */
void HomePage::handleLightsInGroups(boost::system::error_code err, const Wt::Http::Message &response, int group_index) {

  Wt::Json::Object result;
  Wt::Json::parse(response.body(), result);

  for (int k = 0; k < result.size(); k++) {


    try{
      Wt::Json::Array light_array = result.get("lights");

      for (int i = 0; i < this->lights.size(); i++) {

        for (int j = 0; j < light_array.size(); j++) {
          std::string my_str = light_array[i].toString();
          if (this->lights[i]->getID() == my_str) {
            this->groups[group_index]->addLight(this->lights[i]);
          }
        }
      }
    } catch(Wt::Json::TypeException){
      continue;
    }
  }
}

/**
 * getNumGroups
 * @brief returns the number of group
 *
 * @param none
 * @return an int representing the number of groups
 */
int HomePage::getNumGroups() {
  return this->groups.size();
}

/**
 * handleHttpResponseDeleteSchedule
 * @brief handles the http response after deleting a specific schedule
 *
 * @param err a boost err variable
 * @param response a Wt http message response variable
 * @return none
 */
void HomePage::handleHttpResponseDeleteSchedule(boost::system::error_code err, const Wt::Http::Message &response) {

  deleteMessage = "Delete was successful.<br /><br />";
  if(response.body()[0] == '['){
    deleteMessage = "Delete was not successful, ensure you are using a valid schedule id.<br /><br />";
  }

  HomePage::getAndDisplaySchedule();
}

/**
 * handleScheduleDelete
 * @brief a method handling button clicks to delete a particular schedule
 *
 * @param none
 * @return none
 */
void HomePage::handleScheduleDelete(){

  try {
    int num = std::stoi(deleteScheduleId->text());


    Wt::Http::Message message = Wt::Http::Message();
    message.addHeader("Content-type", "application/json");

    Wt::WIOService *ioservice = new Wt::WIOService();
    ioservice->start();
    Wt::Http::Client *client = new Wt::Http::Client(*ioservice);
    client->done().connect(boost::bind(&HomePage::handleHttpResponseDeleteSchedule, this, _1, _2));

    client->deleteRequest(currentBridge->getIP() + ":" + currentBridge->getPort() + "/api/newdeveloper/schedules/" + std::to_string(num), message);


    std::future<int> future = std::async(std::launch::async, [](){
      std::this_thread::sleep_for(std::chrono::seconds(2));
      return 8;
    });

    std::cout << "loading, please wait\n";
    std::chrono::milliseconds span (100);
    while (future.wait_for(span) == std::future_status::timeout){
      std::cout << '.';
    }

    getAndDisplaySchedule();

  } catch(std::invalid_argument){
    deleteMessage = "Delete was not successful, all ids should be numeric.<br /><br />";
    getAndDisplaySchedule();
  }

}

/**
 * handleHttpResponseGetSchedule
 * @brief handles the http response after getting all of the current schedule
 *
 * @param err a boost err variable
 * @param response a Wt http message response variable
 * @return none
 */
void HomePage::handleHttpResponseGetSchedule(boost::system::error_code err, const Wt::Http::Message &response) {

  if(response.body() == ""){
    return;
  }

  Wt::Json::Object result;
  Wt::Json::parse(response.body(), result);

  Wt::Http::Message message = Wt::Http::Message();
  message.addHeader("Content-type", "application/json");

  std::set<std::string> names = result.names();
  std::set<std::string>::iterator it;

  buildCurrentSchedule = "Schedules: <br /><br />";
  bool first = true;
  for (it = names.begin(); it != names.end(); it++) {
      first = false;

      Wt::Json::Object scheduleObject = result.get(*it);
      std::string currentName = scheduleObject.get("name");

      buildCurrentSchedule += *it + " - ";
      buildCurrentSchedule += currentName + "<br /><br />";
  }
  if(first){
    buildCurrentSchedule += "None.<br/><br/>";
  }
}

/**
 * getAndDisplaySchedule
 * @brief makes an api call to get all of the current schedule and prints it to the UI
 *
 * @param none
 * @return none
 */
void HomePage::getAndDisplaySchedule(){

  Wt::WIOService *ioservice = new Wt::WIOService();
  ioservice->start();
  Wt::Http::Client *client = new Wt::Http::Client(*ioservice);
  client->done().connect(boost::bind(&HomePage::handleHttpResponseGetSchedule, this, _1, _2));

  client->get(currentBridge->getIP() + ":" + currentBridge->getPort() + "/api/newdeveloper/schedules");


  std::future<int> future = std::async(std::launch::async, [](){
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return 8;
  });

  std::cout << "loading, please wait\n";
  std::chrono::milliseconds span (100);
  while (future.wait_for(span) == std::future_status::timeout){
    std::cout << '.';
  }

  scheduleText->setText(deleteMessage + "<br /><br />" +buildCurrentSchedule);
  deleteMessage = "";
}

/**
 * handleScheduleCreateLight
 * @brief calls the handleScheduleCreate for only a light
 *
 * @param none
 * @return none
 */
void HomePage::handleScheduleCreateLight(){
  handleScheduleCreate(0);
}

/**
 * handleScheduleCreateLight
 * @brief calls the handleScheduleCreate for an entire group
 *
 * @param none
 * @return none
 */
void HomePage::handleScheduleCreateGroup(){
  handleScheduleCreate(1);
}

/**
 * handleScheduleCreate
 * @brief makes an API to schedule a change dependent on several text fields
 *
 * @param group an int (0 or 1) representing whether to schedule a light, or group, respectively
 * @return none
 */
void HomePage::handleScheduleCreate(int group){

  ScheduleItem *toSchedule = new ScheduleItem();

  try {
    time_t rawtime;
    time(&rawtime);

    tm * schedulingTime;
    schedulingTime = localtime(&rawtime);
    schedulingTime->tm_year = std::stoi(scheduleYear->text());
    schedulingTime->tm_mon = std::stoi(scheduleMonth->text());
    schedulingTime->tm_mday = std::stoi(scheduleDay->text());
    schedulingTime->tm_hour = std::stoi(scheduleHour->text());
    schedulingTime->tm_min = std::stoi(scheduleMin->text());

    int on = std::stoi(scheduleOn->text());
    int transitionTime = std::stoi(scheduleTransition->text());

    if((on != 0 && on != 1) || (transitionTime < 0)){
      std::cout << "error" << std::endl;
      scheduleErrorText->setText("Please ensure all schedule parameters are valid.");
      delete toSchedule;
      return;
    }

    int hue = std::stoi(scheduleHue->valueText().toUTF8());
    int brightness = std::stoi(scheduleBrightness->valueText().toUTF8());

    State *schedulingState = new State(on, brightness, hue, transitionTime);

    toSchedule->setTime(schedulingTime);
    toSchedule->setState(schedulingState);

    toSchedule->sendSchedule(std::stoi(createScheduleId->text()), group, currentBridge->getIP(), currentBridge->getPort());

    std::future<int> future = std::async(std::launch::async, [](){
      std::this_thread::sleep_for(std::chrono::seconds(2));
      return 8;
    });

    std::cout << "loading, please wait\n";
    std::chrono::milliseconds span (100);
    while (future.wait_for(span) == std::future_status::timeout){
      std::cout << '.';
    }

    getAndDisplaySchedule();
    scheduleErrorText->setText("");
    delete toSchedule;
  } catch(std::invalid_argument){

    std::cout << "error" << std::endl;

    scheduleErrorText->setText("Please ensure all schedule parameters are valid.");
    delete toSchedule;
  }
}



/**
 * changeLightBrightness
 * @brief takes in string bri and int i (place in array that light is in) and it changes the brightness. It has no return value.
 *
 * @param bri: string representing brightness
 * @param i: int representing place in array
 * @return nothing
*/
void HomePage::changeLightBrightness(std::string bri, int i){
  this->lights[i]->setBrightness(std::stoi(this->brightnessSliders[i]->valueText().toUTF8()));
}

/**
 * changeLightHue
 * @brief takes in string curVal and int i (place in array that light is in) and it changes the hue. It has no return value.
 *
 * @param curVal: string representing hue
 * @param i: int representing place in array
 * @return nothing
 */
void HomePage::changeLightHue(std::string curVal, int i) {

  this->lights[i]->setHue(
      std::stoll(this->hueSliders[i]->valueText().toUTF8()));
}

/* &&& */
void HomePage::changeGroupHue(std::string curVal, int i){

  this->groups[i]->setHue(std::stoll(this->groupHueSliders[i]->valueText().toUTF8()), currentBridge->getIP(), currentBridge->getPort());

}

/* &&& */
void HomePage::changeGroupBrightness(std::string curVal, int i){
  this->groups[i]->setBrightness(std::stoi(this->groupBriSliders[i]->valueText().toUTF8()), currentBridge->getIP(), currentBridge->getPort());
}

/**
 * changeLightName
 * @brief takes in the new name String and an integer i ( place in array that light is in) and it changes the light name to that. It had no return value.
 *
 * @param name: string for the new light name
 * @param i: int representing place in array
 * @return nothing
 */
void HomePage::changeLightName(std::string name, int i) {
  this->lights[i]->setName(this->edits[i]->text().toUTF8(), currentBridge->getIP(), currentBridge->getPort());
}

/**
 * handleHttpResponseLights
 * @brief takes an error code err and a message response as parameters and handles the Http Response Lights it. It does not have a return value.
 *
 * @param err: error code
 * @param response: message response
 * @return nothing
 */
void HomePage::handleHttpResponseLights(boost::system::error_code err,
                                        const Wt::Http::Message &response) {

  Wt::Json::Object result;

  Wt::Json::parse(response.body(), result);

  for (int i = 0; i < result.size(); i++) {

    Wt::Json::Object jsonObj1 = result.get(std::to_string(i + 1));
    Wt::WString bulbName = jsonObj1.get("name");

    Wt::Json::Object jsonStateObj = jsonObj1.get("state");
    int saturation = jsonStateObj.get("sat").toNumber().orIfNull(0);

    int brightness = jsonStateObj.get("bri").toNumber().orIfNull(0);
    long long hue = jsonStateObj.get("hue").toNumber().orIfNull(0);
    bool isOn = jsonStateObj.get("on");

    Light *l = new Light(std::to_string(i + 1), bulbName.toUTF8(), this);
    Light *light =
        new Light(std::to_string(i + 1), bulbName.toUTF8(),
                  new State(isOn, brightness, hue, saturation, l), this);

    this->lights.push_back(light);
  }
}

/**
 * getNumLights
 * @brief gets the number of the lights and return it
 * takes no parameters returns the size of lights (how many there are)
 *
 * @param none
 * @return an int representing the number of lights
 */
int HomePage::getNumLights() {
  return this->lights.size();
}

/**
 * handlePredictionOff
 * @brief deletes all events scheduled through predictive mode
 *
 * @param none
 * @return none
 */
void HomePage::handlePredictionOff(){

  predictionText->setText("Removed predicted schedule events.");

  Wt::WIOService *ioservice = new Wt::WIOService();
  ioservice->start();
  Wt::Http::Client *client = new Wt::Http::Client(*ioservice);
  client->done().connect(boost::bind(&HomePage::handleHttpResponsePredictionOff, this, _1, _2));

  client->get(currentBridge->getIP() + ":" + currentBridge->getPort() + "/api/newdeveloper/schedules");

  ioservice->stop();
  delete ioservice;

  std::future<int> future = std::async(std::launch::async, [](){
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return 8;
  });

  std::cout << "loading, please wait\n";
  std::chrono::milliseconds span (100);
  while (future.wait_for(span) == std::future_status::timeout){
    std::cout << '.';
  }

  getAndDisplaySchedule();
}

/**
 * handleHttpResponsePredictionOff
 * @brief response from api call to delete a specific scheduled event
 *
 * @param err a boost err variable
 * @param response a Wt http message response variable
 * @return none
 */
void HomePage::handleHttpResponsePredictionOff(boost::system::error_code err, const Wt::Http::Message &response) {

  Wt::Json::Object result;
  Wt::Json::parse(response.body(), result);

  Wt::Http::Message message = Wt::Http::Message();
  message.addHeader("Content-type", "application/json");

  std::set<std::string> names = result.names();
  std::set<std::string>::iterator it;
  for (it = names.begin(); it != names.end(); it++) {
      if(*it == "1"){
        continue;
      }

      Wt::Json::Object scheduleObject = result.get(*it);
      std::string currentName = scheduleObject.get("name");

      if(currentName.at(0) == 'P'){

        Wt::WIOService *ioservice = new Wt::WIOService();
        ioservice->start();
        Wt::Http::Client *client = new Wt::Http::Client(*ioservice);
        client->done().connect(boost::bind(&HomePage::handleHttpResponsePrediction, this, _1, _2));

        std::cout << "Deleting schedule with id: " +  *it << std::endl;

        client->deleteRequest(currentBridge->getIP() + ":" + currentBridge->getPort() + "/api/newdeveloper/schedules/" + *it, message);

        ioservice->stop();
        delete ioservice;
      }
  }
}

/**
 * handleHttpResponsePrediction
 * @brief response from api call to creating a specific scheduled event
 *
 * @param err a boost err variable
 * @param response a Wt http message response variable
 * @return none
 */
void HomePage::handleHttpResponsePrediction(boost::system::error_code err, const Wt::Http::Message &response) {
    std::cout << "\nstatus: " +  std::to_string(response.status()) << std::endl;
    std::cout << "body: " +  response.body() << std::endl;
}

/**
 * handlePrediction
 * @brief creates predicted events using the Predict class and logistic regression
 *
 * @param none
 * @return none
 */
void HomePage::handlePrediction(){

  int logisticRegression; // flag for whether of not we are using machine learning
  int id;

  std::string buildResponse = "";
  arma::mat predicted;
  std::string days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


  // Getting the current time to know which period needs to be scheduled
  time_t rawtime;
  tm * timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  int wday = timeinfo->tm_wday;
  int whour = timeinfo->tm_hour;

  std::string currentDay;
  std::string currentHour;
  std::string currentState;

  std::string previous;

  // We traina nd predict for each light
  for (int i = 0; i < this->lights.size(); i++) {

    // Get the light id
    previous = "";
    id = stoi(this->lights[i]->getID());

    buildResponse += "<br/><br/>--- 48 hour schedule for light with ID: " + std::to_string(id) + " ---<br/><br/>";

    // If first we chek how confident we are
    if(i == 0){
      logisticRegression = Predict::checkConfidence(id);
    }

    // Get vector of predictions
    predicted = Predict::trainAndPredict(id, logisticRegression);

    // Get the current time to know where to start in the list
    int currentPeriod = wday == 0 ? whour : (wday - 1) * 24 + whour;

    // Iterate for 48 hours in the list
    for(int j = currentPeriod + 1; j < currentPeriod + 48; j++){

      currentDay = days[(int(std::floor(j / 24))) % 7];
      currentHour = std::to_string(j % 24);

      // Regularize to 24:00 notation
      if(currentHour.size() == 1){
        currentHour = "0" + currentHour;
      }
      currentHour += ":00";

      // Check prediction to determine state
      if(predicted.at(j % arma::size(predicted, 0), 0) == 1){
        currentState = "On";
      } else {
        currentState = "Off";
      }

      // Only schedule on state changes
      if(currentState != previous){

        // The time to use for scheduling
        tm * schedulingTime;
        schedulingTime = localtime(&rawtime);
        schedulingTime->tm_min = 0;
        schedulingTime->tm_sec = 0;
        schedulingTime->tm_hour += j - currentPeriod;
        mktime(schedulingTime);

        std::string scheduleTime = "";
        scheduleTime += std::to_string(1900 + schedulingTime->tm_year) + "-";
        scheduleTime += std::to_string(schedulingTime->tm_mon + 1) + "-";
        scheduleTime += std::to_string(schedulingTime->tm_mday) + "T";
        if(std::to_string(schedulingTime->tm_hour).size() == 1){
          scheduleTime += "0";
        }
        scheduleTime += std::to_string(schedulingTime->tm_hour) + ":00:00";

        // Building the response to display to the user.
        previous = currentState;
        buildResponse += currentHour;
        buildResponse += "   " + currentDay;
        buildResponse += "<br/>State: " +  currentState + "<br/><br/>";

        std::string onFlag = "false";
        if(currentState == "On"){
          onFlag = "true";
        }

        Wt::WIOService *ioservice = new Wt::WIOService();
        ioservice->start();
        Wt::Http::Client *client = new Wt::Http::Client(*ioservice);
        client->done().connect(boost::bind(&HomePage::handleHttpResponsePrediction, this, _1, _2));

        // Build the http message
        Wt::Http::Message message = Wt::Http::Message();
        message.addHeader("Content-type", "application/json");
        message.addBodyText(
          "{\"name\": \"P" + std::to_string(id) + " Day:" + std::to_string(schedulingTime->tm_mday) + " Hr:" + std::to_string(schedulingTime->tm_hour) + " On:" + onFlag + "\", " +
           "\"command\": {" +
              "\"address\": \"/api/newdeveloper/lights/" + std::to_string(id)+ "/action\", " +
              "\"method\": \"PUT\", " +
              "\"body\": {" +
                "\"on\": "  + onFlag +
              "}" +
            "}, " +
            "\"time\": \"" + scheduleTime + "\"" +
          "}");

        /*
            {
              "name": "P1",
              "command": {
                "address": "/api/newdeveloper/lights/1/action",
                "method": "PUT",
                "body": {
                  "on": true
                }
              },
              "localtime": "2017-10-12T13:00:00"
            }
        */

        client->post(currentBridge->getIP() + ":" + currentBridge->getPort() + "/api/newdeveloper/schedules", message);
        ioservice->stop();
        delete ioservice;
      }
    }
  }

  if(buildResponse == ""){
    buildResponse = "There are currently no available lights.";
  } else {
    buildResponse = "";
  }

  predictionText->setText(buildResponse);

  std::future<int> future = std::async(std::launch::async, [](){
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return 8;
  });

  std::cout << "loading, please wait\n";
  std::chrono::milliseconds span (100);
  while (future.wait_for(span) == std::future_status::timeout){
    std::cout << '.';
  }

  getAndDisplaySchedule();

};

/**
 * createProfile
 * @brief create a new profile, takes in parameter env, and returns a new HomePage
 *
 * @param env: an environment variable
 * @return a wt application
 */
Wt::WApplication *createProfile(const Wt::WEnvironment &env) {

  return new HomePage(env);
}

/**
 * main
 * @brief takes in int argc and char ** argv, it passes these oarameters to Wt::WRun with &createprofile and returns that
 *
 * @param argc: number of arguments
 * @param argv: array of arguments
 * @return error code
 */
int main(int argc, char **argv) { return Wt::WRun(argc, argv, &createProfile); }

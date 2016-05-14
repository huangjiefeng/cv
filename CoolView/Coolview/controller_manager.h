#ifndef CONTROLLER_MANAGER_H
#define CONTROLLER_MANAGER_H

#include "controller_manager_interface.h"

class ISessionController;
class IConferenceController;
class ILayoutController;
class IDisplayController;
class ISystemController;
class IMonitorController;
class IDaemonController;
class IFloorController;

class MediaManager;
class DeviceManager;
class RunningConfig;
class RunningProfile;
class IMsgManager;
class HttpMsgManager;

class ControllerManager : public IControllerManager {
  Q_OBJECT

public:
  ControllerManager(QObject *parent);
  ~ControllerManager();

  void Initialize(RunningProfile *profile,
                  RunningConfig *config,
                  DeviceManager *device_manager,
                  IMsgManager *msg_manager,
                  MediaManager *media_manager,
                  IDaemonController *daemon_controller,
                  HttpMsgManager *http_msg_manager);

  const ISessionController* GetSessionController() const override {
    return session_controller_;
  }

  ISessionController* GetSessionController() override {
    return session_controller_;
  }

  IConferenceController* GetConferenceController() override {
    return conference_controller_;
  }

  const IConferenceController* GetConferenceController() const override {
    return conference_controller_;
  }

  const IDisplayController* GetDisplayController() const override {
    return display_controller_;
  }

  IDisplayController* GetDisplayController() override {
    return display_controller_;
  }

  const ITXController* GetTXController() const override {
    return tx_controller_;
  }

  ITXController* GetTXController() override {
    return tx_controller_;
  }

  const ITXClientController* GetTXClientController() const override {
    return tx_client_controller_;
  }

  ITXClientController* GetTXClientController() override {
    return tx_client_controller_;
  }

  const IFilePlayController* GetFilePlayController() const override {
    return fileplay_controller_;
  }

  IFilePlayController* GetFilePlayController() override {
    return fileplay_controller_;
  }

  ISystemController * GetSystemController() override {
    return system_controller_;
  }

  const ISystemController * GetSystemController() const override {
    return system_controller_;
  }

  IMonitorController *GetMonitorController() override {
    return monitor_controller_;
  }

  const IMonitorController *GetMonitorController() const override {
    return monitor_controller_;
  }

  IDaemonController *GetDaemonController() override {
    return daemon_controller_;
  }

  const IDaemonController *GetDaemonController() const override {
    return daemon_controller_;
  }

  IFloorController* GetFloorController() override {
    return floor_controller_;
  }

  const IFloorController* GetFloorController() const override {
    return floor_controller_;
  }

private:
  void InitSenssionAndConferenceController();
  void InitDisplayController();
  void InitTXController();
  void InitFilePlayController();
  void InitSystemController();
  void InitMonitorController();

private:
  RunningProfile *profile_;
  RunningConfig *config_;

  DeviceManager *device_manager_;
  MediaManager *media_manager_;
  IMsgManager *msg_manager_;
  HttpMsgManager *http_msg_manager_;

  ISessionController *session_controller_;
  IConferenceController *conference_controller_;
  IDisplayController *display_controller_;
  ITXController *tx_controller_;
  ITXClientController *tx_client_controller_;
  IFilePlayController *fileplay_controller_;
  ISystemController *system_controller_;
  IMonitorController *monitor_controller_;
  IDaemonController *daemon_controller_;
  IFloorController *floor_controller_;
};

#endif // CONTROLLER_MANAGER_H

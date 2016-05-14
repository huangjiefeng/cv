#ifndef TERMINAL_MANAGER_H
#define TERMINAL_MANAGER_H

#include "terminal_description.h"

class TerminalManager {
public:
  enum ActionType {
    kNoAction = 0,     // 不执行动作
    kInsertAction = 1, // 执行插入动作
    kUpdateAction = 2, // 执行更新动作
  };

public:
  TerminalManager();
  ~TerminalManager();
  
  // 插入或更新终端信息
  // 返回执行的动作
  // note: 当已有相同的终端，且信息相同，则返回kNoAction
  ActionType AddTerminal(TerminalPointer terminal, bool use_FCS);

  // 若输入的是uri，则将返回virtual index为0的虚拟终端信息
  // 若没有找到该终端的信息，则返回nullptr
  TerminalPointer FindTerminal(const QString &uri_or_vuri) const;

  // 输入user_id，即uri中@的前部分。
  // 若没有该终端的信息，则返回nullptr
  TerminalPointer FindTerminalByUserId(const QString &user_id) const;

  TerminalPointer FindTerminalById(uint16_t id) const;

  // 通过虚拟终端的视频端口查找虚拟终端信息
  // 若没有找到该终端的信息，则返回nullptr
  TerminalPointer FindTerminalByVideoPort(int video_port) const;

  // 通过虚拟终端的视频端口查找虚拟终端信息
  // 若没有找到该终端的信息，则返回nullptr
  TerminalPointer FindTerminalByAudioPort(int audio_port) const;

  // 获取Tx终端
  TerminalPointer FindAvailableTxTerminal() const;

  // 是否包含uri对应的终端
  bool ContainTerminal(const QString &uri) const {
    return terminals_.find(uri) != terminals_.end();
  }

  // 获取uri对应的所有虚拟终端
  TerminalList& operator[](const QString &uri) {
    return terminals_[uri];
  }

  // 获取uri对应的所有虚拟终端
  TerminalList operator[](const QString &uri) const {
    return terminals_[uri];
  }
  
  // 按uri的不同，计算终端数量
  int GetTerminalCount() const;

  // 返回虚拟终端总数
  int GetVirtualTerminalCount() const;

  // 按uri，返回对应的虚拟终端总数
  int GetVirtualTerminalCount(const QString &uri) const;

  // 对于目前的uri，是否所有的虚拟终端都已经记录了
  bool IsAllVirtualTerminalReceived() const;
  
  // 情况终端列表
  void Clear() {
    terminals_.clear();
  }

private:
  TerminalListDict terminals_;
};

#endif // TERMINAL_MANAGER_H

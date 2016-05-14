#include "terminal_manager.h"

#include <cassert>

#include <QDebug>

#include "sip_msg_parser.h"

TerminalManager::TerminalManager() {

}

TerminalManager::~TerminalManager() {

}

TerminalManager::ActionType
TerminalManager::AddTerminal(TerminalPointer terminal, bool use_FCS) {
  auto it = terminals_.find(terminal->uri);
  if (it == terminals_.end()) {
    // 若保存的终端列表当前没有uri的虚拟终端，则直接插入新的终端到列表中
    terminals_[terminal->uri].push_back(terminal);
    return kInsertAction;

  } else {  // 若已有uri的虚拟终端
    TerminalList &virtual_terminals = it.value();
    assert(!virtual_terminals.isEmpty());

    // 检查重复性，更新终端或插入新终端
    bool is_new_terminal = true;
    for (auto &temp : virtual_terminals) {
      if (temp->virtual_uri == terminal->virtual_uri) {
        is_new_terminal = false;
        // 只有当虚拟终端的信息不同时，才发出更新信号
        if (!temp->IsSameWithoutTotalNumCompared(*terminal)) {
          if (use_FCS) {
            terminal->is_speaking = temp->is_speaking;
            terminal->is_handup = temp->is_handup;
          }
          temp = terminal;
          return kUpdateAction;
        }
        break;
      }
    }

    if (is_new_terminal) {
      virtual_terminals.push_back(terminal);
      return kInsertAction;
    }
  }
  return kNoAction;
}

TerminalPointer
TerminalManager::FindTerminal( const QString &uri_or_vuri ) const {
  QString terminal_uri = TerminalHelper::GetTerminalURI(uri_or_vuri);
  QString terminal_vuri = TerminalHelper::IsVirtualURI(uri_or_vuri) ? 
    uri_or_vuri : TerminalHelper::ConstructDefaultVirtualURI(uri_or_vuri);

  auto it = terminals_.find(terminal_uri);
  if (it != terminals_.end()) {
    auto &virtual_terminals = it.value();
    for (auto &terminal : virtual_terminals) {
      if (terminal_vuri == terminal->virtual_uri) {
        return terminal;
      }
    }
  }
  return nullptr;
}

TerminalPointer 
TerminalManager::FindTerminalByUserId( const QString &user_id ) const {
  QString terminal_user_id = user_id.mid(user_id.indexOf('#') + 1);

  auto it = terminals_.begin();
  for (; it != terminals_.end(); ++it) {
    if (it.key().startsWith(terminal_user_id)) {
      break;
    }
  }
  if (it != terminals_.end()) {
    const QString &terminal_uri = it.key();
    int virtual_index = TerminalHelper::GetVirtualIndex(user_id);
    QString terminal_vuri = TerminalHelper::ConstructDefaultVirtualURI(terminal_uri, virtual_index);
    for (auto terminal : it.value()) {
      if (terminal_vuri == terminal->virtual_uri) {
        return terminal;
      }
    }
  }
  return nullptr;
}


TerminalPointer
TerminalManager::FindTerminalByVideoPort( int video_port ) const {
  for (auto &virtual_terminals : terminals_) {
    for (auto &terminal : virtual_terminals) {
      if (terminal->video_port == video_port) {
        return terminal;
      }
    }
  }
  return nullptr;
}

TerminalPointer 
TerminalManager::FindTerminalByAudioPort( int audio_port ) const {
  for (auto &virtual_terminals : terminals_) {
    for (auto &terminal : virtual_terminals) {
      if (terminal->audio_port == audio_port) {
        return terminal;
      }
    }
  }
  return nullptr;
}

int TerminalManager::GetTerminalCount() const {
  int count = 0;
  for (auto &virtual_terminals : terminals_) {
    if (!virtual_terminals.isEmpty()) {
      ++count;
    }
  }
  return count;
}

int TerminalManager::GetVirtualTerminalCount() const {
  int count = 0;
  for (auto &virtual_terminals : terminals_) {
    count += virtual_terminals.size();
  }
  return count;
}

int TerminalManager::GetVirtualTerminalCount( const QString &uri ) const {
  auto it = terminals_.find(uri);
  if (it != terminals_.end()) {
    return it.value().size();
  }
  return 0;
}

bool TerminalManager::IsAllVirtualTerminalReceived() const {
  for (auto &virtual_terminal_list : terminals_) {
    if (!virtual_terminal_list.isEmpty() && 
        virtual_terminal_list[0]->is_available &&
        virtual_terminal_list.size() < virtual_terminal_list[0]->virtual_count) {
      qDebug() << "Virtual terminal insufficient:"
        << virtual_terminal_list[0]->uri << "Recv"
        << virtual_terminal_list.size() << "/" 
        << virtual_terminal_list[0]->virtual_count;
      return false;
    }
  }
  return true;
}

TerminalPointer TerminalManager::FindAvailableTxTerminal() const {
  for (auto &virtual_terminal_list : terminals_) {
    if (!virtual_terminal_list.isEmpty() && 
        virtual_terminal_list[0]->is_available &&
        TerminalHelper::IsModelTX(virtual_terminal_list[0]->model)) {
      return virtual_terminal_list[0];
    }
  }
  return nullptr;
}

TerminalPointer TerminalManager::FindTerminalById( uint16_t id ) const {
  for (auto &virtual_terminal_list : terminals_) {
    if (!virtual_terminal_list.isEmpty() && 
        virtual_terminal_list[0]->id == id) {
      return virtual_terminal_list[0];
    }
  }
  return nullptr;
}

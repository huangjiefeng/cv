#include "stream_receiver_info_manager.h"

void StreamReceiverInfoManager::AddRemoteReceiver( const StreamReceiverInfo &info ) {
  auto receiver = 
    FindRemoteReceiverByURI(info.recv_uri, info.local_virtual_index);
  if (receiver) {
    *receiver = info;
  } else {
    receiver = new StreamReceiverInfo(info);
    dict_[receiver->recv_uri][info.local_virtual_index] = receiver;
  }
}

void StreamReceiverInfoManager::RemoveReceiverByURI( 
    const QString &recv_uri, 
    int local_virtual_index ) {
  auto it = dict_.find(recv_uri);
  if (it == dict_.end()) {
    return;
  }
  VIndexReceiversDict &vindex_dict = it.value();
  auto index_it = vindex_dict.find(local_virtual_index);
  if (index_it != vindex_dict.end()) {
    delete index_it.value();
    vindex_dict.erase(index_it);
  }
}

void StreamReceiverInfoManager::RemoveReceiverByIP(
    const QString &recv_ip, 
    int local_virtual_index) {
  for (auto it = dict_.begin(); it != dict_.end(); ++it) {
    VIndexReceiversDict &vindex_dict = it.value();
    auto vindex_it = vindex_dict.find(local_virtual_index);
    if (vindex_it != vindex_dict.end() && 
        vindex_it.value()->recv_ip == recv_ip) {
      delete vindex_it.value();
      vindex_dict.erase(vindex_it);
      return;
    }
  }
}

StreamReceiverInfo*
StreamReceiverInfoManager::FindRemoteReceiverByURI(
    const QString &recv_uri, 
    int local_virtual_index) {
  auto it = dict_.find(recv_uri);
  if (it != dict_.end()) {
    VIndexReceiversDict &vindex_dict = it.value();
    auto index_it = vindex_dict.find(local_virtual_index);
    if (index_it != vindex_dict.end()) {
      return index_it.value();
    } else {
      return nullptr;
    }
  }
  return nullptr;
}

StreamReceiverInfo* 
StreamReceiverInfoManager::FindRemoteReceiverByIP( 
    const QString &ip,
    int local_virtual_index ) {
  for (auto it = dict_.begin(); it != dict_.end(); ++it) {
    VIndexReceiversDict &vindex_dict = it.value();
    auto vindex_it = vindex_dict.find(local_virtual_index);
    if (vindex_it != vindex_dict.end() && 
        vindex_it.value()->recv_ip == ip) {
      return vindex_it.value();
    }
  }
  return nullptr;
}

int StreamReceiverInfoManager::GetReceiverCount() const {
  int count = 0;
  for (auto it = dict_.begin(); it != dict_.end(); ++it) {
    count += it.value().size();
  }
  return count;
}

void StreamReceiverInfoManager::Clear() {
  for (auto it = dict_.begin(); it != dict_.end(); ++it) {
    VIndexReceiversDict &vindex_dict = it.value();
    for (auto value : vindex_dict) {
      delete value;
    }
  }
  dict_.clear();
}

StreamReceiverInfoManager::ReceiverList 
StreamReceiverInfoManager::ToList() const {
  ReceiverList receivers;
  for (auto it = dict_.begin(); it != dict_.end(); ++it) {
    const VIndexReceiversDict &vindex_dict = it.value();
    for (auto receiver : vindex_dict) {
      receivers.push_back(*receiver);
    }
  }
  return receivers;
}


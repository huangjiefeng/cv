#ifndef GRAPH_CONTAINER_H
#define GRAPH_CONTAINER_H

#include "streams.h"
#include <atlbase.h>
#include "DxDefinitions.h"

namespace msdx
{

class GraphContainer
{
public:
  GraphContainer(void);
  ~GraphContainer(void);

  dxStatus InitializeGraph();

  IGraphBuilder *GetGraphBuilder() { return graph_builder_; }
  ICaptureGraphBuilder2 *GetCaptureGraphBuilder2() { return capture_graph_builder2_; }
  IMediaControl *GetMediaControl() { return media_control_; }
  IMediaEvent *GetMediaEvent() { return media_event_; }

#ifdef _DEBUG
  void AddToRot(const wchar_t *prefix);
  void RemoveFromRot();
#else
  void AddToRot(const wchar_t *prefix) {}
  void RemoveFromRot() {}
#endif // _DEBUG

protected:
  CComPtr<IGraphBuilder> graph_builder_;
  CComQIPtr<IMediaControl> media_control_;
  CComQIPtr<IMediaEvent> media_event_;
  CComPtr<ICaptureGraphBuilder2> capture_graph_builder2_;

#ifdef _DEBUG
  DWORD debug_register_;
#endif // _DEBUG

};

}


#endif

#include "GraphContainer.h"

#include "GraphUtil.h"
#include "LogUtil.h"

using namespace msdx;

GraphContainer::GraphContainer(void)
{
#ifdef _DEBUG
    debug_register_ = 0;
#endif // _DEBUG

}


GraphContainer::~GraphContainer(void)
{
    RemoveFromRot();
}

dxStatus GraphContainer::InitializeGraph()
{
    if (graph_builder_)
    {
        return DX_ERR_ABORT;
    }
    HRESULT hr;

    hr = graph_builder_.CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, "Create GraphBuilder failed.");

    media_control_ = graph_builder_;
    CHK_FAIL_MSG_RETURN(!media_control_, DX_ERR_ABORT, "Query MediaControl interface failed.");

    media_event_ = graph_builder_;
    CHK_FAIL_MSG_RETURN(!media_event_, DX_ERR_ABORT, "Query MediaEvent interface failed.");

    hr = capture_graph_builder2_.CoCreateInstance(CLSID_CaptureGraphBuilder2 , NULL, CLSCTX_INPROC);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, "Create CaptureGraphBuilder2 failed.");

    hr = capture_graph_builder2_->SetFiltergraph(graph_builder_);
    CHK_FAIL_MSG_RETURN(FAILED(hr), DX_ERR_ABORT, "Set GraphBuilder to CaptureGraphBuilder2 failed.");

    return DX_ERR_NONE;
}

#ifdef _DEBUG
void GraphContainer::AddToRot(const wchar_t *prefix)
{
    RemoveFromRot();
    HRESULT hr = GraphUtil::AddToRot(prefix, graph_builder_, &debug_register_);
    if (FAILED(hr))
    {
        LOG_WARN("Add graph to debug rot failed: 0x%08x", hr);
    }
}

void GraphContainer::RemoveFromRot()
{
    if (debug_register_)
    {
        GraphUtil::RemoveFromRot(debug_register_);
    }
    debug_register_ = 0;
}
#endif // _DEBUG

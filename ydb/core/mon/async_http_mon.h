#pragma once

#include <library/cpp/monlib/service/monservice.h>
#include <library/cpp/monlib/dynamic_counters/counters.h>
#include <library/cpp/monlib/service/pages/index_mon_page.h>
#include <library/cpp/monlib/service/pages/tablesorter/css_mon_page.h>
#include <library/cpp/monlib/service/pages/tablesorter/js_mon_page.h>

#include <library/cpp/actors/core/mon.h>
#include <library/cpp/actors/http/http.h>

#include "mon.h"

namespace NActors {

class TAsyncHttpMon : public TMon {
public:
    TAsyncHttpMon(TConfig config);

    void Start(TActorSystem* actorSystem) override;
    void Stop() override;

    void Register(NMonitoring::IMonPage* page) override;
    NMonitoring::TIndexMonPage* RegisterIndexPage(const TString& path, const TString& title) override;
    NMonitoring::IMonPage* RegisterActorPage(TRegisterActorPageFields fields) override;
    NMonitoring::IMonPage* RegisterCountersPage(const TString& path, const TString& title, TIntrusivePtr<NMonitoring::TDynamicCounters> counters) override;
    NMonitoring::IMonPage* FindPage(const TString& relPath) override;

protected:
    TConfig Config;
    TIntrusivePtr<NMonitoring::TIndexMonPage> IndexMonPage;
    TActorSystem* ActorSystem = {};
    TActorId HttpProxyActorId;
    TActorId HttpMonServiceActorId;
    TActorId NodeProxyServiceActorId;

    struct TActorMonPageInfo {
        NMonitoring::IMonPage* Page;
        TString Path;
    };

    std::vector<TActorMonPageInfo> ActorMonPages;
    std::vector<TActorId> ActorServices;

    void RegisterActorMonPage(const TActorMonPageInfo& pageInfo);
};

} // NActors

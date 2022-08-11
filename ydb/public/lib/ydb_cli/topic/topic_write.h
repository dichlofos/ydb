#pragma once

#include "ydb/public/lib/ydb_cli/commands/ydb_command.h"
#include <ydb/public/sdk/cpp/client/ydb_persqueue_public/persqueue.h>
#include <ydb/public/lib/ydb_cli/common/format.h>
#include <ydb/public/lib/ydb_cli/common/interruptible.h>

namespace NYdb::NConsoleClient {
#define GETTER(TYPE, NAME) \
    TYPE NAME() const {    \
        return NAME##_;    \
    }

    class TTopicWriterParams {
    public:
        TTopicWriterParams();
        TTopicWriterParams(EOutputFormat inputFormat, TMaybe<TString> delimiter,
                           ui64 messageSizeLimit, TMaybe<TDuration> batchDuration,
                           TMaybe<ui64> batchSize, TMaybe<ui64> batchMessagesCount);
        TTopicWriterParams(const TTopicWriterParams&) = default;
        TTopicWriterParams(TTopicWriterParams&&) = default;

        GETTER(TMaybe<char>, Delimiter);
        GETTER(TMaybe<TDuration>, BatchDuration);
        GETTER(TMaybe<ui64>, BatchSize);
        GETTER(TMaybe<ui64>, BatchMessagesCount);
        GETTER(ui64, MessageSizeLimit);
        GETTER(EOutputFormat, InputFormat);

    private:
        TMaybe<TString> File_;
        TMaybe<char> Delimiter_;
        EOutputFormat InputFormat_ = EOutputFormat::Default;

        // TODO(shmel1k@): move to 'TWithBatchingCommand' or something like that.
        TMaybe<TDuration> BatchDuration_;
        TMaybe<ui64> BatchSize_;
        TMaybe<ui64> BatchMessagesCount_;

        ui64 MessageSizeLimit_ = 0;
    };

    class TTopicWriterTests;

    // TODO(shmel1k@): think about interruption here.
    class TTopicWriter {
    public:
        TTopicWriter();
        TTopicWriter(const TTopicWriter&) = default;
        TTopicWriter(TTopicWriter&&) = default;
        TTopicWriter(std::shared_ptr<NPersQueue::IWriteSession>, TTopicWriterParams);

        bool Close(TDuration closeTimeout = TDuration::Max());
        int Init();
        int Run(IInputStream&);

    private:
        struct TSendMessageData {
            TString Data = "";
            bool NeedSend = false;
            bool ContinueSending = false;
        };

        int HandleEvent(NPersQueue::TWriteSessionEvent::TEvent&);
        int HandleAcksEvent(const NPersQueue::TWriteSessionEvent::TAcksEvent&);
        int HandleReadyToAcceptEvent(NPersQueue::TWriteSessionEvent::TReadyToAcceptEvent&);
        int HandleSessionClosedEvent(const NPersQueue::TSessionClosedEvent&);
        TTopicWriter::TSendMessageData EnterMessage(IInputStream&); // TODO(shmel1k@): make static or like a helper function

        std::shared_ptr<NPersQueue::IWriteSession> WriteSession_;
        const TTopicWriterParams WriterParams_;

        TMaybe<NPersQueue::TContinuationToken> ContinuationToken_ = Nothing();

        ui64 CurrentSeqNo_ = 0;

        friend class TTopicWriterTests;
    };
} // namespace NYdb::NConsoleClient
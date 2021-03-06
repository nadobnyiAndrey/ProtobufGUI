#ifndef ROOTPROTOITEM_H
#define ROOTPROTOITEM_H

#include "messageprotoitem.h"

class RootProtoItem : public MessageProtoItem
{
public:
    RootProtoItem(ProtobufModel& model, const proto::Descriptor *protoclass);
    std::string getStringMessage();
    google::protobuf::Message* getMessage();
    void initMessage(const std::string& fp);

    // ProtoTreeItem interface
protected:
    void initFieldValue(const google::protobuf::Message *) override;
};

#endif // ROOTPROTOITEM_H

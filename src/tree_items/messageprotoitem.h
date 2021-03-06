#ifndef MESSAGEPROTOITEM_H
#define MESSAGEPROTOITEM_H

#include "prototreeitem.h"
class MessageProtoItem : public virtual ProtoTreeItem
{
public:
    MessageProtoItem(ProtobufModel& model, const proto::FieldDescriptor * field, ProtoTreeItem *parentItem = nullptr);
    MessageProtoItem(ProtobufModel& model, const proto::Descriptor *protoclass, ProtoTreeItem *parentItem = nullptr);

    ~MessageProtoItem() override;

    QItemDelegate *getDelegate() const override;
    void clearValue() override;
    ItemState state() const;

protected:
    void fillFieldValue(google::protobuf::Message *message) override;
    void addFieldValue(google::protobuf::Message *, const google::protobuf::FieldDescriptor *) override;
    void initRepeatedFieldValue(const google::protobuf::Message *, int idx) override;
    void initFieldValue(const google::protobuf::Message *) override;

    proto::Message *getMessage();

    void printHex(const proto::Message *) const;
    void printHex(const std::string&, size_t line_width = 80 ) const;

protected:
    proto::DynamicMessageFactory mFactory;

private:
    void initChildValues(const proto::Message&);
};

#endif // MESSAGEPROTOITEM_H

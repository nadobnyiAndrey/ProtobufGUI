#ifndef PROTOMANAGER_H
#define PROTOMANAGER_H

#include <QObject>
#include <QHash>

QStringList removeEmptyOrDupl(const QStringList& src);

namespace google
{
    namespace protobuf
    {
        class Descriptor;
    }
}

namespace proto = google::protobuf;

class ProtoContext;

class ProtoManager : public QObject
{
    Q_OBJECT
public:
    explicit ProtoManager(QObject *parent = nullptr);
    ~ProtoManager();
    void load(const QUrl &path);

    QMultiHash <QString, QString> getProtoClasses() const;
    const proto::Descriptor * getClassDescriptor(const QString& pPackage, const QString& pClass) const;

public slots:
    void setPackage(const QString& pPackage);
    void setClass(const QString& pClass);

signals:
    void onProtoChange(const QStringList& packages);
    void onPackageChange(const QStringList& classes);
    void onClassChange(const proto::Descriptor* desc);

private:
    QHash< QString, QHash<QString, const proto::Descriptor* > > mProtoPackages;

    QString mCurPackage;
    QString mCurClass;

    ProtoContext* context;
};

#endif // PROTOMANAGER_H

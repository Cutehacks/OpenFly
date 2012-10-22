/*
 *  Copyright (C) 2010 Cutehacks AS. All rights reserved.
 *  info@cutehacks.com
 *  http://cutehacks.com
 *
 */

#ifndef AIRLINENAMES_H
#define AIRLINENAMES_H

#include "xmldatasource.h"
#include <QHash>

class AirlineNames : public XmlDataSource
{
    Q_OBJECT
public:
    AirlineNames(QObject *parent = 0);
    ~AirlineNames();

    QString airlineName(const QString &code) const;

protected Q_SLOTS:
    void createMapping(Node *root);

private:
    QHash<QString, QString> m_mapping;
};

#endif

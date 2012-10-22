/*
 *  Copyright (C) 2012 Cutehacks AS. All rights reserved.
 *  info@cutehacks.com
 *  http://cutehacks.com
 *
 *  This file is part of Fly.
 *
 *  Fly is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Fly is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Fly.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "airportnames.h"
#include "node.h"
#include <QStack>

static const QString ch_avinorAirports[] = {
    "Alta (ALF)",
    "Andøya (ANX)",
    "Bardufoss (BDU)",
    "Bergen (BGO)",
    "Berlevåg (BVG)",
    "Bodø (BOO)",
    "Brønnøysunn (BNN)",
    "Båtsfjord (BJF)",
    "Fagernes (VDB)",
    "Florø (FRO)",
    "Førde (FDE)",
    "Hammerfest (HFT)",
    "Harstad/Narvik (EVE)",
    "Hasvik (HAA)",
    "Haugesund (HAU)",
    "Honningsvåg (HVG)",
    "Kirkenes (KKN)",
    "Kristiansand (KRS)",
    "Kristiansund (KSU)",
    "Lakselv (LKL)",
    "Leknes (LKN)",
    "Mehamn (MEH)",
    "Mo i Rana (MQN)",
    "Molde (MOL)",
    "Mosjøen (MJF)",
    "Namsos (OSY)",
    "Narvik (NVK)",
    "Oslo (OSL)",
    "Røros (RRS)",
    "Rørvik (RVK)",
    "Røst (RET)",
    "Sandane (SDN)",
    "Sandnessjøen (SSJ)",
    "Sogndal (SOG)",
    "Stavanger (SVG)",
    "Stokmarknes (SKN)",
    "Svalbard (LYR)",
    "Svolvær (SVJ)",
    "Sørkjosen (SOJ)",
    "Tromsø (TOS)",
    "Trondheim (TRD)",
    "Vadsø (VDS)",
    "Vardø (VAW)",
    "Værøy (VRY)",
    "Ørsta-Volda (HOV)",
    "Ålesund (AES)"
};

AirportNames::AirportNames(QObject *parent)
    : XmlDataSource(parent)
{
    for (int i = 0; i < 46; ++i)
        m_airports.append(ch_avinorAirports[i]);
    setUrl(QUrl("http://flydata.avinor.no/airportNames.asp"));
    setRefreshRate(86400000); //msecs == 24h
    QObject::connect(this, SIGNAL(dataUpdated(Node*)), this, SLOT(createMapping(Node*)));
}

AirportNames::~AirportNames()
{
}

QStringList AirportNames::avinorAirports() const
{
    return m_airports;
}

QString AirportNames::avinorAirportCodeAt(int index) const
{
    QString airport = m_airports.value(index);
    return airport.mid(airport.length() - 4, 3);
}

QString AirportNames::airportName(const QString &code) const
{
    return m_mapping.value(code, code);
}

void AirportNames::createMapping(Node *root)
{
    m_mapping.clear();
    QStack<Node*> pending;
    pending.push(root);
    while (!pending.isEmpty()) {
        Node *node = pending.pop();
        if (node->type() == "airportName")
            m_mapping.insert(node->attribute("code"), node->attribute("name"));
        for (int i = 0; i < node->childCount(); ++i)
            pending.push(node->child(i));
    }
}
